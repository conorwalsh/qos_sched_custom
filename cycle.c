#include <unistd.h>
#include <stdint.h>
#include "cycle.h"


uint64_t enQos = 0;
uint64_t enQosCall = 0;
uint64_t enQosPkt = 0;
uint64_t deQos = 0;
uint64_t deQosCall = 0;
uint64_t deQosPkt = 0;
uint64_t ringQos = 0;
uint64_t ringQosCall = 0;
uint64_t ringQosPkt = 0;
uint64_t sendQos = 0;
uint64_t sendQosCall = 0;
uint64_t sendQosPkt = 0;
uint64_t rxCyc = 0;
uint64_t rxPkt = 0;
uint64_t txCyc = 0;
uint64_t txPkt = 0;
uint64_t rxThreadIdleCalls = 0;
uint64_t rxThreadUsedCalls = 0;
uint64_t txThreadIdleCalls = 0;
uint64_t txThreadUsedCalls = 0;
uint64_t rxThreadIdleCyc = 0;
uint64_t rxThreadUsedCyc = 0;
uint64_t txThreadIdleCyc = 0;
uint64_t txThreadUsedCyc = 0;
uint64_t rxThreadIdleCPC = 0;
uint64_t rxThreadUsedCPC = 0;
uint64_t txThreadIdleCPC = 0;
uint64_t txThreadUsedCPC = 0;

uint64_t delta_total = 0;
uint64_t delta_count = 0;

void cycle_put_en_qos(uint64_t cycEnQos, uint64_t pktEnQos)
{
	enQos += cycEnQos;
	enQosCall++;
	enQosPkt += pktEnQos;
}

uint64_t cycle_get_en_qos(void)
{
	uint64_t cycQosPkt = 0;
	if(enQosPkt!=0){
		cycQosPkt = (int)(enQos/enQosPkt);
	}
	return cycQosPkt;
}

uint64_t cycle_get_en_qos_call(void)
{
        return enQosCall;
}


void cycle_put_de_qos(uint64_t cycDeQos, uint64_t pktDeQos)
{
	deQos += cycDeQos;
	deQosCall++;
	deQosPkt += pktDeQos;
}

uint64_t cycle_get_de_qos(void)
{
	uint64_t cycQosPkt = 0;
	if(deQosPkt!=0){
		cycQosPkt = (int)(deQos/deQosPkt);
	}
	return cycQosPkt;
}

void cycle_put_ring_qos(uint64_t cycRingQos, uint64_t pktRingQos)
{
        ringQos += cycRingQos;
	ringQosCall++;
	ringQosPkt += pktRingQos;
}

uint64_t cycle_get_ring_qos(void)
{
	uint64_t cycRingPkt = 0;
	if(ringQosPkt!=0){
		cycRingPkt = (int)(ringQos/ringQosPkt);
	}
	return cycRingPkt;
}

void cycle_put_send_qos(uint64_t cycSendQos, uint64_t pktSendQos)
{
	sendQos += cycSendQos;
	sendQosCall++;
	sendQosPkt += pktSendQos;
}

uint64_t cycle_get_send_qos(void)
{
	uint64_t cycSendPkt = 0;
	if(sendQosPkt!=0){
		cycSendPkt = (int)(sendQos/sendQosPkt);
	}
	return cycSendPkt;
}

uint64_t cycle_get_de_qos_call(void)
{
        return deQosCall;
}

void cycle_put_rx(uint64_t cycRx, uint64_t pktRx)
{
	rxCyc += cycRx;
	rxPkt += pktRx;
}

uint64_t cycle_get_rx(void)
{
	uint64_t cycQosPkt = 0;
	if(rxPkt!=0){
		cycQosPkt = (int)(rxCyc/rxPkt);
	}
	return cycQosPkt;
}

uint64_t cycle_get_rx_total(void)
{
	uint64_t cycRxTot = 0;
	if(rxPkt!=0){
		cycRxTot = (int)(rxThreadUsedCyc/rxPkt);
	}
	return cycRxTot;
}

uint64_t cycle_get_tx_total(void)
{
        uint64_t cycTxTot = 0;
        if(txPkt!=0){
	        cycTxTot = (int)(txThreadUsedCyc/txPkt);
	}
	return cycTxTot;
}


void cycle_put_tx(uint64_t cycTx, uint64_t pktTx)
{
	txCyc += cycTx;
	txPkt += pktTx;
}

uint64_t cycle_get_tx(void)
{
	uint64_t cycQosPkt = 0;
	if(txPkt!=0){
		cycQosPkt = (int)(txCyc/txPkt);
	}
	return cycQosPkt;
}

void cycle_put_rx_idle(uint64_t cycRxI)
{
	rxThreadIdleCalls++;
	rxThreadIdleCyc += cycRxI;
}

void cycle_put_rx_used(uint64_t cycRxU)
{
	rxThreadUsedCalls++;
	rxThreadUsedCyc += cycRxU;
}

void cycle_put_tx_idle(uint64_t cycTxI)
{
        txThreadIdleCalls++;
	txThreadIdleCyc += cycTxI;
}

void cycle_put_tx_used(uint64_t cycTxU)
{
	txThreadUsedCalls++;
	txThreadUsedCyc += cycTxU;
}

uint64_t cycle_get_rx_idle_call(void)
{
	return rxThreadIdleCalls;
}

uint64_t cycle_get_rx_used_call(void)
{
	return rxThreadUsedCalls;
}

uint64_t cycle_get_tx_idle_call(void)
{
	return txThreadIdleCalls;
}

uint64_t cycle_get_tx_used_call(void)
{
	return txThreadUsedCalls;
}

uint64_t cycle_get_rx_idle_cyc(void)
{
        return rxThreadIdleCyc;
}

uint64_t cycle_get_rx_used_cyc(void)
{
        return rxThreadUsedCyc;
}

uint64_t cycle_get_tx_idle_cyc(void)
{
        return txThreadIdleCyc;
}

uint64_t cycle_get_tx_used_cyc(void)
{
        return txThreadUsedCyc;
}

uint64_t cycle_get_rx_idle_cpc(void)
{
        uint64_t cpc = 0;
	if(rxThreadIdleCalls!=0){
		cpc = (int)(rxThreadIdleCyc/rxThreadIdleCalls);
	}
	return cpc;
}

uint64_t cycle_get_rx_used_cpc(void)
{
        uint64_t cpc = 0;
	if(rxThreadUsedCalls!=0){
		cpc = (int)(rxThreadUsedCyc/rxThreadUsedCalls);
	}
	return cpc;
}

uint64_t cycle_get_tx_idle_cpc(void)
{
        uint64_t cpc = 0;
	if(txThreadIdleCalls!=0){
		cpc = (int)(txThreadIdleCyc/txThreadIdleCalls);
	}
	return cpc;
}

uint64_t cycle_get_tx_used_cpc(void)
{
        uint64_t cpc = 0;
        if(txThreadUsedCalls!=0){
	        cpc = (int)(txThreadUsedCyc/txThreadUsedCalls);
        }
        return cpc;
}

float cycle_get_rx_ppc(void)
{
	float ppc = 0;
	if(rxThreadUsedCalls!=0){
		ppc = ((float)rxPkt/(float)rxThreadUsedCalls);
	}
	return ppc;
}

float cycle_get_tx_ppc(void)
{
	float ppc = 0;
	if(txThreadUsedCalls!=0){
		ppc = ((float)txPkt/(float)txThreadUsedCalls);
	}
	return ppc;
}

float cycle_get_rx_ipu(void)
{
	float ipu = 0;
	if(rxThreadUsedCalls!=0){
		ipu = ((float)rxThreadIdleCalls/(float)rxThreadUsedCalls);
	}
	return ipu;
}

float cycle_get_tx_ipu(void)
{
        float ipu = 0;
        if(txThreadUsedCalls!=0){
	        ipu = ((float)txThreadIdleCalls/(float)txThreadUsedCalls);
	}
	return ipu;
}

void cycle_put_delta(uint64_t delta){
	delta_total += delta;
	delta_count++;
}

uint64_t cycle_get_delta(void){
	return (uint64_t)(delta_total/delta_count);
}
