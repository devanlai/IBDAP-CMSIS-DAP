/*
===============================================================================
 Name        : IBDAP.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC11Uxx.h"
#endif

#include "DAP_config.h"

#include "DAP.h"

#include "usb_driver.h"



#define TICKRATE_100msec (10)
#define TICKRATE_10msec  (100)
#define TICKRATE_1msec 	 (1000)

#define TICKRATE 		 TICKRATE_1msec

// Connected LED RED          PIN 21: PIO0_11
#define LED_ERROR_PORT         0
#define LED_ERROR_BIT          11
#define IOCON_ERROR_REG        (LPC_IOCON->TDI_PIO0_11)

void indicator_code (int code);

void init_error_led () {
	IOCON_ERROR_REG = 1 | 1 << 4 | 1 << 7;
	LPC_GPIO->DIR[LED_ERROR_PORT] |= 1 << LED_ERROR_BIT;
	LPC_GPIO->SET[LED_ERROR_PORT] = 1 << LED_ERROR_BIT;
}

INLINE void LED_ERROR_ON () {
	LPC_GPIO->CLR[LED_ERROR_PORT] = 1 << LED_ERROR_BIT;
}

INLINE void LED_ERROR_OFF () {
	LPC_GPIO->SET[LED_ERROR_PORT] = 1 << LED_ERROR_BIT;
}


static volatile uint32_t sys_time = 0;


void SysTick_Handler () {
	sys_time++;
}

void sleep_ms (int ms) {

	int e = sys_time * (1000/TICKRATE) + ms;
	while ((sys_time * (1000/TICKRATE)) <= e) {
		__WFI ();
	}
}

// HID callbacks and buffer

typedef struct _IBDAP_HND_T {
	volatile uint8_t usb_outs_end_idx;
	volatile uint8_t usb_outs_start_idx;
	uint8_t usb_outs[DAP_PACKET_COUNT][DAP_PACKET_SIZE];
	volatile uint8_t usb_ins_end_idx;
	volatile uint8_t usb_ins_start_idx;
	volatile uint8_t usb_ins_busy;
	uint8_t usb_ins[DAP_PACKET_COUNT][DAP_PACKET_SIZE];
}IBDAP_HND_T;

IBDAP_HND_T* ibdap = 0;


ErrorCode_t DAP_GetReport_Callback( USBD_HANDLE_T hHid, USB_SETUP_PACKET* pSetup, uint8_t** pBuffer, uint16_t* length) {
	return LPC_OK;
}

ErrorCode_t DAP_SetReport_Callback( USBD_HANDLE_T hHid, USB_SETUP_PACKET* pSetup, uint8_t** pBuffer, uint16_t length) {
	return LPC_OK;
}

ErrorCode_t DAP_EpInOut_Hdlr_Callback (USBD_HANDLE_T hUsb, void* data, uint32_t event) {
	USB_HID_CTRL_T *pHidCtrl = (USB_HID_CTRL_T *) data;
	switch (event) {
	case USB_EVT_IN:
		if (ibdap->usb_ins_start_idx == ibdap->usb_ins_end_idx) {
			ibdap->usb_ins_busy = 0;
			break;
		}
		USBD_API->hw->WriteEP(hUsb, pHidCtrl->epin_adr, ibdap->usb_ins[ibdap->usb_ins_start_idx], DAP_PACKET_SIZE);
		ibdap->usb_ins_start_idx = (ibdap->usb_ins_start_idx+1) % DAP_PACKET_COUNT;
		//LED_RUNNING_OUT (1);
		//LED_CONNECTED_OUT (0);
		break;
	case USB_EVT_OUT:
		USBD_API->hw->ReadEP(hUsb, pHidCtrl->epout_adr, ibdap->usb_outs[ibdap->usb_outs_end_idx]);
		ibdap->usb_outs_end_idx = (ibdap->usb_outs_end_idx+1) % DAP_PACKET_COUNT;
		//LED_CONNECTED_OUT (1);
		//LED_RUNNING_OUT (0);
		break;
	}
	return LPC_OK;
}

void suspend () {
	while (1) {
		LED_ERROR_ON ();
		sleep_ms (1000);
		LED_ERROR_OFF ();
		sleep_ms (1000);
	}
}

void error_code (int code) {
	LED_ERROR_ON ();
	while (1) {
		if (code & 0x01 && !(code & 0x02 )) { // code == 1
			LED_CONNECTED_OUT (1);
			LED_RUNNING_OUT (0);
			sleep_ms (1000);
			LED_CONNECTED_OUT (0);
		} else if (!(code & 0x01) && (code & 0x02)) { // code == 2
			LED_CONNECTED_OUT (0);
			LED_RUNNING_OUT (1);
			sleep_ms (1000);
			LED_RUNNING_OUT (0);
		} else if ((code & 0x01) == 3) {
			LED_CONNECTED_OUT (1);
			LED_RUNNING_OUT (1);
			sleep_ms (1000);
			LED_CONNECTED_OUT (0);
			LED_RUNNING_OUT (0);
		} else {
			LED_CONNECTED_OUT (1);
			LED_RUNNING_OUT (0);
			sleep_ms (1000);
			LED_CONNECTED_OUT (0);
			LED_RUNNING_OUT (1);
		}

		sleep_ms (1000);
	}
}

void indicator_code (int code) {
	LED_ERROR_OFF ();
	while (1) {
		if (code & 0x01 && !(code & 0x02 )) { // code == 1
			LED_CONNECTED_OUT (1);
			LED_RUNNING_OUT (0);
			sleep_ms (1000);
			LED_CONNECTED_OUT (0);
		} else if (!(code & 0x01) && (code & 0x02)) { // code == 2
			LED_CONNECTED_OUT (0);
			LED_RUNNING_OUT (1);
			sleep_ms (1000);
			LED_RUNNING_OUT (0);
		} else if ((code & 0x01) == 3) {
			LED_CONNECTED_OUT (1);
			LED_RUNNING_OUT (1);
			sleep_ms (1000);
			LED_CONNECTED_OUT (0);
			LED_RUNNING_OUT (0);
		} else {
			LED_CONNECTED_OUT (1);
			LED_RUNNING_OUT (0);
			sleep_ms (1000);
			LED_CONNECTED_OUT (0);
			LED_RUNNING_OUT (1);
		}

		sleep_ms (1000);
	}
}
// TODO: insert other definitions and declarations here

void device_boot() {
	LPC_SYSCON->SYSAHBCLKCTRL |= 1 << 6; // enable gpio clock
}

int main(void) {

	device_boot();

	USBD_API_INIT_PARAM_T usb_param;

	SysTick_Config(SystemCoreClock / TICKRATE);

	init_error_led ();

    DAP_Setup ();


    init_usb_clock ();
    init_usb_power ();
    if (init_usb_driver (&usb_param)) {
    	// printf error;
    	// LED error;
    	suspend ();
    	return -1;
    }

    if (init_usb_hid (&usb_param,
    		&DAP_GetReport_Callback, &DAP_SetReport_Callback,
			&DAP_EpInOut_Hdlr_Callback, &DAP_EpInOut_Hdlr_Callback,
			(uint8_t**)&ibdap, sizeof (IBDAP_HND_T))) {
    	// printf error;
    	// LED error;
    	error_code (1);
    	return -1;
    }

    connect_to_usb_bus ();
    //LED_ERROR_ON ();
    while (1) {
    	/*
    	LED_ERROR_ON ();
    	sleep_ms (500);
    	LED_ERROR_OFF ();
    	sleep_ms (1000);
    	*/
    	if (ibdap->usb_outs_start_idx == ibdap->usb_outs_end_idx) {
    		//LED_ERROR_ON ();
    		__WFI ();
    		continue;
    	} else {
    		LED_CONNECTED_OUT (1);
    	}
    	//LED_ERROR_OFF ();
    	DAP_ProcessCommand (ibdap->usb_outs[ibdap->usb_outs_start_idx], ibdap->usb_ins[ibdap->usb_ins_end_idx]);
    	ibdap->usb_outs_start_idx = (ibdap->usb_outs_start_idx+1) % DAP_PACKET_COUNT;
    	ibdap->usb_ins_end_idx = (ibdap->usb_ins_end_idx+1) % DAP_PACKET_COUNT;
    	if (!ibdap->usb_ins_busy) { // kickstart
    		ibdap->usb_ins_busy = 1;
    		uint8_t idx = ibdap->usb_ins_start_idx;
    		ibdap->usb_ins_start_idx = (ibdap->usb_ins_start_idx+1) % DAP_PACKET_COUNT;
    		USBD_API->hw->WriteEP(g_usb_hnd, HID_EP_IN, ibdap->usb_ins[idx], DAP_PACKET_SIZE);

    		//LED_RUNNING_OUT (1);
    		//LED_CONNECTED_OUT (0);
    	}

    }
}
