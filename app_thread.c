  
/*
 * app_thread.c
 * Version: v19.11
 *
 * This file contains the code that gets launched on threads by the app
 *
 * Usage:
 *    1) This file sould not be interacted with by the user
 * 
 * Copyright (c) 2020 Conor Walsh
 * qos_sched_custom is licensed under an MIT license (see included license file)
 *
 * This project was based on the DPDK qos_sched app and as per it's original
 *    BSD-3 licence the original copyright and licence are maintained below
 * ------------------------------------------
 *  SPDX-License-Identifier: BSD-3-Clause
 *  Copyright(c) 2010-2014 Intel Corporation
 * ------------------------------------------
 */

#include <stdint.h>

#include <rte_log.h>
#include <rte_mbuf.h>
#include <rte_malloc.h>
#include <rte_cycles.h>
#include <rte_ethdev.h>
#include <rte_memcpy.h>
#include <rte_byteorder.h>
#include <rte_branch_prediction.h>
#include <rte_sched.h>

#include "main.h"
#include "cycle.h"

#define SUBPORT_OFFSET	7
#define PIPE_OFFSET		9
#define QUEUE_OFFSET	20
#define COLOR_OFFSET	19

#define QOSSTATS

static inline int
get_pkt_sched(struct rte_mbuf *m, uint32_t *subport, uint32_t *pipe,
			uint32_t *traffic_class, uint32_t *queue, uint32_t *color)
{
	// Read the Ethernet Header of the packet
	struct rte_ether_hdr *eth;
	eth = rte_pktmbuf_mtod(m, struct rte_ether_hdr *);

	// Use Destinsation MAC address to set how packet will be classified by QoS
	// XX:XX:XX:XX:XX:XX
	// COLOR:XX:SUBPORT:PIPE:TRAFFICCLASS:QUEUE
	*subport = eth->d_addr.addr_bytes[2];
	*pipe = eth->d_addr.addr_bytes[3];
	*traffic_class = eth->d_addr.addr_bytes[4];
	*queue = eth->d_addr.addr_bytes[5];
	*color = eth->d_addr.addr_bytes[0];

	return 0;
}

void
app_rx_thread(struct thread_conf **confs)
{
	uint32_t i, nb_rx;
	struct rte_mbuf *rx_mbufs[burst_conf.rx_burst] __rte_cache_aligned;
	struct thread_conf *conf;
	int conf_idx = 0;

	uint32_t subport;
	uint32_t pipe;
	uint32_t traffic_class;
	uint32_t queue;
	uint32_t color;

	while ((conf = confs[conf_idx])) {
		
		#ifdef QOSSTATS	
		uint64_t cycStartTotal = rte_get_tsc_cycles();
		#endif

		nb_rx = rte_eth_rx_burst(conf->rx_port, conf->rx_queue, rx_mbufs,
				burst_conf.rx_burst);

		if (likely(nb_rx != 0)) {
			APP_STATS_ADD(conf->stat.nb_rx, nb_rx);
						
			#ifdef QOSSTATS
			uint64_t cycStart = rte_get_tsc_cycles();
			#endif

			for(i = 0; i < nb_rx; i++) {
				get_pkt_sched(rx_mbufs[i],
						&subport, &pipe, &traffic_class, &queue, &color);
				rte_sched_port_pkt_write(conf->sched_port,
						rx_mbufs[i],
						subport, pipe,
						traffic_class, queue,
						(enum rte_color) color);
			}
			
			#ifdef QOSSTATS
			uint64_t cycStop = rte_get_tsc_cycles();
			if(nb_rx>0){
				uint64_t cycFinal = cycStop-cycStart;
				CYCLE_PUT_RX(cycFinal,nb_rx);
			}
			#endif

			if (unlikely(rte_ring_sp_enqueue_bulk(conf->rx_ring,
					(void **)rx_mbufs, nb_rx, NULL) == 0)) {
				for(i = 0; i < nb_rx; i++) {
					rte_pktmbuf_free(rx_mbufs[i]);

					APP_STATS_ADD(conf->stat.nb_drop, 1);
				}
			}
		}
		conf_idx++;
		if (confs[conf_idx] == NULL)
			conf_idx = 0;
		
		#ifdef QOSSTATS
		uint64_t cycStopTotal = rte_get_tsc_cycles();
		uint64_t cycFinalTotal = cycStopTotal-cycStartTotal;
		if(nb_rx<=0){
			CYCLE_PUT_RX_IDLE(cycFinalTotal);
		}
		else{
			CYCLE_PUT_RX_USED(cycFinalTotal);
		}
		#endif
	}
}



/* Send the packet to an output interface
 * For performance reason function returns number of packets dropped, not sent,
 * so 0 means that all packets were sent successfully
 */

static inline void
app_send_burst(struct thread_conf *qconf)
{
	struct rte_mbuf **mbufs;
	uint32_t n, ret;

	mbufs = (struct rte_mbuf **)qconf->m_table;
	n = qconf->n_mbufs;

	do {
		ret = rte_eth_tx_burst(qconf->tx_port, qconf->tx_queue, mbufs, (uint16_t)n);
		/* we cannot drop the packets, so re-send */
		/* update number of packets to be sent */
		n -= ret;
		mbufs = (struct rte_mbuf **)&mbufs[ret];
	} while (n);
}


/* Send the packet to an output interface */
static void
app_send_packets(struct thread_conf *qconf, struct rte_mbuf **mbufs, uint32_t nb_pkt)
{
	uint32_t i, len;

	len = qconf->n_mbufs;
	for(i = 0; i < nb_pkt; i++) {
		qconf->m_table[len] = mbufs[i];
		len++;
		/* enough pkts to be sent */
		if (unlikely(len == burst_conf.tx_burst)) {
			qconf->n_mbufs = len;
			app_send_burst(qconf);
			len = 0;
		}
	}

	qconf->n_mbufs = len;
}

void
app_tx_thread(struct thread_conf **confs)
{
	struct rte_mbuf *mbufs[burst_conf.qos_dequeue];
	struct thread_conf *conf;
	int conf_idx = 0;
	int retval;
	const uint64_t drain_tsc = (rte_get_tsc_hz() + US_PER_S - 1) / US_PER_S * BURST_TX_DRAIN_US;

	while ((conf = confs[conf_idx])) {
		retval = rte_ring_sc_dequeue_bulk(conf->tx_ring, (void **)mbufs,
					burst_conf.qos_dequeue, NULL);
		if (likely(retval != 0)) {
			app_send_packets(conf, mbufs, burst_conf.qos_dequeue);

			conf->counter = 0; /* reset empty read loop counter */
		}

		conf->counter++;

		/* drain ring and TX queues */
		if (unlikely(conf->counter > drain_tsc)) {
			/* now check is there any packets left to be transmitted */
			if (conf->n_mbufs != 0) {
				app_send_burst(conf);

				conf->n_mbufs = 0;
			}
			conf->counter = 0;
		}

		conf_idx++;
		if (confs[conf_idx] == NULL)
			conf_idx = 0;
	}
}


void
app_worker_thread(struct thread_conf **confs)
{
	struct rte_mbuf *mbufs[burst_conf.ring_burst];
	struct thread_conf *conf;
	int conf_idx = 0;

	while ((conf = confs[conf_idx])) {
		uint32_t nb_pkt;

		/* Read packet from the ring */
		nb_pkt = rte_ring_sc_dequeue_burst(conf->rx_ring, (void **)mbufs,
					burst_conf.ring_burst, NULL);
		if (likely(nb_pkt)) {
			int nb_sent = rte_sched_port_enqueue(conf->sched_port, mbufs,
					nb_pkt);

			APP_STATS_ADD(conf->stat.nb_drop, nb_pkt - nb_sent);
			APP_STATS_ADD(conf->stat.nb_rx, nb_pkt);
		}

		nb_pkt = rte_sched_port_dequeue(conf->sched_port, mbufs,
					burst_conf.qos_dequeue);
		if (likely(nb_pkt > 0))
			while (rte_ring_sp_enqueue_bulk(conf->tx_ring,
					(void **)mbufs, nb_pkt, NULL) == 0)
				; /* empty body */

		conf_idx++;
		if (confs[conf_idx] == NULL)
			conf_idx = 0;
	}
}


void
app_mixed_thread(struct thread_conf **confs)
{
	struct rte_mbuf *mbufs[burst_conf.ring_burst];
	struct thread_conf *conf;
	int conf_idx = 0;
	const uint64_t drain_tsc = (rte_get_tsc_hz() + US_PER_S - 1) / US_PER_S * BURST_TX_DRAIN_US;

	while ((conf = confs[conf_idx])) {
		uint32_t nb_pkt;
		
		#ifdef QOSSTATS
		uint64_t cycStartTotal = rte_get_tsc_cycles();

		uint64_t cycStartRing = rte_get_tsc_cycles();
		#endif

		/* Read packet from the ring */
		nb_pkt = rte_ring_sc_dequeue_burst(conf->rx_ring, (void **)mbufs,
					burst_conf.ring_burst, NULL);
		
		#ifdef QOSSTATS
		uint64_t cycStopRing = rte_get_tsc_cycles();
		#endif
		
		if (likely(nb_pkt)) {
			#ifdef QOSSTATS
			uint64_t cycStartEn = rte_get_tsc_cycles();
			#endif
			
			int nb_sent = rte_sched_port_enqueue(conf->sched_port, mbufs,
					nb_pkt);
			
			#ifdef QOSSTATS
			uint64_t cycStopEn = rte_get_tsc_cycles();
						
			if(nb_pkt>0){
				uint64_t cycFinal = cycStopEn-cycStartEn;
				CYCLE_PUT_EN_QOS(cycFinal,nb_pkt);
			}
			#endif

			APP_STATS_ADD(conf->stat.nb_drop, nb_pkt - nb_sent);
			APP_STATS_ADD(conf->stat.nb_rx, nb_pkt);
		}

		#ifdef QOSSTATS
		uint64_t cycStartDe = rte_get_tsc_cycles();
		#endif

		nb_pkt = rte_sched_port_dequeue(conf->sched_port, mbufs,
					burst_conf.qos_dequeue);
		
		#ifdef QOSSTATS
		uint64_t cycStopDe = rte_get_tsc_cycles();

		if(nb_pkt>0){
			uint64_t cycFinal = cycStopDe-cycStartDe;
			CYCLE_PUT_DE_QOS(cycFinal,nb_pkt);
		
			uint64_t cycRing = cycStopRing-cycStartRing;
			CYCLE_PUT_RING_QOS(cycRing,nb_pkt);
		}
		#endif
		
		if (likely(nb_pkt > 0)) {
			app_send_packets(conf, mbufs, nb_pkt);

			conf->counter = 0; /* reset empty read loop counter */
		}

		conf->counter++;

		#ifdef QOSSTATS
		uint64_t cycStartTx = rte_get_tsc_cycles();
		#endif

		/* drain ring and TX queues */
		if (unlikely(conf->counter > drain_tsc)) {

			/* now check is there any packets left to be transmitted */
			if (conf->n_mbufs != 0) {
				#ifdef QOSSTATS
				uint64_t cycStartSend = rte_get_tsc_cycles();
				#endif
				
				app_send_burst(conf);

				#ifdef QOSSTATS
				uint64_t cycStopSend = rte_get_tsc_cycles();

				uint64_t cycSend = cycStopSend-cycStartSend;
				CYCLE_PUT_SEND_QOS(cycSend,nb_pkt);
				#endif
				
				conf->n_mbufs = 0;
			}
			conf->counter = 0;
		}

		conf_idx++;
		if (confs[conf_idx] == NULL)
			conf_idx = 0;
		
		#ifdef QOSSTATS	
		uint64_t cycStopTx = rte_get_tsc_cycles();
				
		uint64_t cycStopTotal = rte_get_tsc_cycles();
		uint64_t cycFinalTotal = cycStopTotal-cycStartTotal;
							
		if(nb_pkt>0){
			uint64_t cycFinal = cycStopTx-cycStartTx;
			CYCLE_PUT_TX(cycFinal,nb_pkt);

			CYCLE_PUT_TX_USED(cycFinalTotal);
		}
		else{
			CYCLE_PUT_TX_IDLE(cycFinalTotal);
		}
		#endif
	}
}
