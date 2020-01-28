/*
 * cycle.h
 * Version: v19.11
 *
 * This file is used define functions to be used by the cycle measurements
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

#define CYCLE_PUT_EN_QOS(cycEnQos,pktEnQos)		cycle_put_en_qos((cycEnQos),(pktEnQos))
#define CYCLE_GET_EN_QOS()         			cycle_get_en_qos()
#define CYCLE_GET_EN_QOS_CALL()                         cycle_get_en_qos_call()
#define CYCLE_PUT_DE_QOS(cycDeQos,pktDeQos)		cycle_put_de_qos((cycDeQos),(pktDeQos))
#define CYCLE_GET_DE_QOS()                              cycle_get_de_qos()
#define CYCLE_GET_DE_QOS_CALL()                         cycle_get_de_qos_call()

#define CYCLE_PUT_RING_QOS(cycRingQos,pktRingQos)       cycle_put_en_qos((cycRingQos),(pktRingQos))
#define CYCLE_GET_RING_QOS()                            cycle_get_en_qos()
#define CYCLE_PUT_SEND_QOS(cycSendQos,pktSendQos)       cycle_put_send_qos((cycSendQos),(pktSendQos))
#define CYCLE_GET_SEND_QOS()                            cycle_get_send_qos()

#define CYCLE_PUT_RX(cycRx,pktRx)                       cycle_put_rx((cycRx),(pktRx))
#define CYCLE_GET_RX()                                  cycle_get_rx()
#define CYCLE_PUT_TX(cycTx,pktTx)                       cycle_put_tx((cycTx),(pktTx))
#define CYCLE_GET_TX()                                  cycle_get_tx()

#define CYCLE_GET_RX_TOTAL()                            cycle_get_rx_total()
#define CYCLE_GET_TX_TOTAL()                            cycle_get_tx_total()

#define CYCLE_PUT_RX_IDLE(cycRxI)			cycle_put_rx_idle((cycRxI))
#define CYCLE_PUT_RX_USED(cycRxU)			cycle_put_rx_used((cycRxU))
#define CYCLE_PUT_TX_IDLE(cycTxI)			cycle_put_tx_idle((cycTxI))
#define CYCLE_PUT_TX_USED(cycTxU)			cycle_put_tx_used((cycTxU))
#define CYCLE_GET_RX_IDLE_CALL()			cycle_get_rx_idle_call()
#define CYCLE_GET_RX_USED_CALL()			cycle_get_rx_used_call()
#define CYCLE_GET_TX_IDLE_CALL()			cycle_get_tx_idle_call()
#define CYCLE_GET_TX_USED_CALL()			cycle_get_tx_used_call()
#define CYCLE_GET_RX_IDLE_CYC()                         cycle_get_rx_idle_cyc()
#define CYCLE_GET_RX_USED_CYC()                         cycle_get_rx_used_cyc()
#define CYCLE_GET_TX_IDLE_CYC()                         cycle_get_tx_idle_cyc()
#define CYCLE_GET_TX_USED_CYC()                         cycle_get_tx_used_cyc()
#define CYCLE_GET_RX_IDLE_CPC()                         cycle_get_rx_idle_cpc()
#define CYCLE_GET_RX_USED_CPC()                         cycle_get_rx_used_cpc()
#define CYCLE_GET_TX_IDLE_CPC()                         cycle_get_tx_idle_cpc()
#define CYCLE_GET_TX_USED_CPC()                         cycle_get_tx_used_cpc()
#define CYCLE_GET_RX_PPC()              		cycle_get_rx_ppc()
#define CYCLE_GET_TX_PPC()              		cycle_get_tx_ppc()
#define CYCLE_GET_RX_IPU()                              cycle_get_rx_ipu()
#define CYCLE_GET_TX_IPU()                              cycle_get_tx_ipu()

#define CYCLE_GET_DELTA()                               cycle_get_delta()
#define CYCLE_PUT_DELTA(delta)                          cycle_put_delta((delta))

void cycle_put_en_qos(uint64_t cycEnQos, uint64_t pktEnQos);
uint64_t cycle_get_en_qos(void);
uint64_t cycle_get_en_qos_call(void);
void cycle_put_de_qos(uint64_t cycDeQos, uint64_t pktDeQos);
uint64_t cycle_get_de_qos(void);
uint64_t cycle_get_de_qos_call(void);

void cycle_put_ring_qos(uint64_t cycRingQos, uint64_t pktRingQos);
uint64_t cycle_get_ring_qos(void);
void cycle_put_send_qos(uint64_t cycSendQos, uint64_t pktSendQos);
uint64_t cycle_get_send_qos(void);

uint64_t cycle_get_rx_total(void);
uint64_t cycle_get_tx_total(void);

void cycle_put_rx(uint64_t cycRx, uint64_t pktRx);
uint64_t cycle_get_rx(void);
void cycle_put_tx(uint64_t cycTx, uint64_t pktTx);
uint64_t cycle_get_tx(void);

void cycle_put_rx_idle(uint64_t cycRxI);
void cycle_put_rx_used(uint64_t cycRxU);
void cycle_put_tx_idle(uint64_t cycTxI);
void cycle_put_tx_used(uint64_t cycTxU);
uint64_t cycle_get_rx_idle_call(void);
uint64_t cycle_get_rx_used_call(void);
uint64_t cycle_get_tx_idle_call(void);
uint64_t cycle_get_tx_used_call(void);
uint64_t cycle_get_rx_idle_cyc(void);
uint64_t cycle_get_rx_used_cyc(void);
uint64_t cycle_get_tx_idle_cyc(void);
uint64_t cycle_get_tx_used_cyc(void);
uint64_t cycle_get_rx_idle_cpc(void);
uint64_t cycle_get_rx_used_cpc(void);
uint64_t cycle_get_tx_idle_cpc(void);
uint64_t cycle_get_tx_used_cpc(void);
float cycle_get_rx_ppc(void);
float cycle_get_tx_ppc(void);
float cycle_get_rx_ipu(void);
float cycle_get_tx_ipu(void);

void cycle_put_delta(uint64_t delta);
uint64_t cycle_get_delta(void);
