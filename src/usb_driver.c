/*
 * usb_driver.c
 *
 *  Created on: Jun 18, 2015
 *      Author: yliu
 */

#include <stdint.h>
#include <string.h>

#ifdef __USE_CMSIS
#include "LPC11Uxx.h"
#endif


#include "usb_driver.h"

#define PDRUNCFGUSEMASK 0x0000E800
#define PDRUNCFGMASKTMP 0x000005FF

USBD_HANDLE_T g_usb_hnd;
const  USBD_API_T *g_pUsbApi;


/* Find the address of interface descriptor for given class type. */
USB_INTERFACE_DESCRIPTOR *find_IntfDesc(const uint8_t *pDesc, uint32_t intfClass)
{
	USB_COMMON_DESCRIPTOR *pD;
	USB_INTERFACE_DESCRIPTOR *pIntfDesc = 0;
	uint32_t next_desc_adr;

	pD = (USB_COMMON_DESCRIPTOR *) pDesc;
	next_desc_adr = (uint32_t) pDesc;

	while (pD->bLength) {
		/* is it interface descriptor */
		if (pD->bDescriptorType == USB_INTERFACE_DESCRIPTOR_TYPE) {

			pIntfDesc = (USB_INTERFACE_DESCRIPTOR *) pD;
			/* did we find the right interface descriptor */
			if (pIntfDesc->bInterfaceClass == intfClass) {
				break;
			}
		}
		pIntfDesc = 0;
		next_desc_adr = (uint32_t) pD + pD->bLength;
		pD = (USB_COMMON_DESCRIPTOR *) next_desc_adr;
	}

	return pIntfDesc;
}




void init_usb_clock () {
	// no need to do this: LPC_SYSCON->PDRUNCFG     &= ~(1 <<  8);
	// system_LPC11Uxx.c done that already.
	// configure SYSAHBCLKCTRL

	// no need to do this: enabled in ResetISR()
	//LPC_SYSCON->SYSAHBCLKCTRL |= 1 << 26; // ram1 clock source enable
	//LPC_SYSCON->SYSAHBCLKCTRL |= 1 << 27; // usbram clock source enable
	// conf usb main clock
	LPC_SYSCON->USBCLKSEL = 0;
	LPC_SYSCON->USBCLKUEN = 0;
	LPC_SYSCON->USBCLKUEN = 1;
	LPC_SYSCON->USBCLKDIV = 1;

	LPC_SYSCON->SYSAHBCLKCTRL |= 1 << 14; // usb clock source
	//LPC_SYSCON->SYSAHBCLKCTRL |= 1 << 26;
	LPC_SYSCON->SYSAHBCLKCTRL |= 1 << 27;
}

void init_usb_power () {
	uint32_t pdrun;
	pdrun = LPC_SYSCON->PDRUNCFG & PDRUNCFGMASKTMP;
	pdrun &= ~((1 << 10) & PDRUNCFGMASKTMP);
	LPC_SYSCON->PDRUNCFG = (pdrun | PDRUNCFGUSEMASK);
}

void USB_IRQHandler(void) {
	uint32_t *addr = (uint32_t *) LPC_USB->EPLISTSTART;

	/*	WORKAROUND for artf32289 ROM driver BUG:
	    As part of USB specification the device should respond
	    with STALL condition for any unsupported setup packet. The host will send
	    new setup packet/request on seeing STALL condition for EP0 instead of sending
	    a clear STALL request. Current driver in ROM doesn't clear the STALL
	    condition on new setup packet which should be fixed.
	 */
	if ( LPC_USB->DEVCMDSTAT & (1 << 8) ) {	/* if setup packet is received */
		addr[0] &= ~(1 << 29);	/* clear EP0_OUT stall */
		addr[2] &= ~(1 << 29);	/* clear EP0_IN stall */
	}
	USBD_API->hw->ISR(g_usb_hnd);
}

int init_usb_driver (USBD_API_INIT_PARAM_T *usb_param) {
	USB_CORE_DESCS_T desc;
	ErrorCode_t ret = LPC_OK;

	g_pUsbApi = (const USBD_API_T *) LPC_ROM_API->usbdApiBase;
	memset((void *) usb_param, 0, sizeof(USBD_API_INIT_PARAM_T));
	usb_param->usb_reg_base = LPC_USB_BASE;
	/*	WORKAROUND for artf44835 ROM driver BUG:
		    Code clearing STALL bits in endpoint reset routine corrupts memory area
		    next to the endpoint control data. For example When EP0, EP1_IN, EP1_OUT,
		    EP2_IN are used we need to specify 3 here. But as a workaround for this
		    issue specify 4. So that extra EPs control structure acts as padding buffer
		    to avoid data corruption. Corruption of padding memory doesn’t affect the
		    stack/program behaviour.
	*/
	usb_param->max_num_ep = 2 + 1;
	usb_param->mem_base = USB_STACK_MEM_BASE;
	usb_param->mem_size = USB_STACK_MEM_SIZE;

	desc.device_desc = (uint8_t *) USB_DeviceDescriptor;
	desc.string_desc = (uint8_t *) USB_StringDescriptor;

	desc.high_speed_desc = USB_FsConfigDescriptor;
	desc.full_speed_desc = USB_FsConfigDescriptor;
	desc.device_qualifier = 0;

	ret = USBD_API->hw->Init(&g_usb_hnd, &desc, usb_param);
	if (ret != LPC_OK) return -1;

	usb_param->mem_base = USB_STACK_MEM_BASE + (USB_STACK_MEM_SIZE - usb_param->mem_size);
	return 0;
}



int init_usb_hid (USBD_API_INIT_PARAM_T *usb_param,
		HID_GetReport_Func_T getreport_fun, HID_SetReport_Func_T setreport_fun,
		HID_EpIn_Hdlr_Func_T epin_hdlr_fun, HID_EpOut_Hdlr_Func_T epout_hdlr_fun,
		uint8_t** report_saddr, int report_size) {
	USBD_HID_INIT_PARAM_T hid_param;
	USB_HID_REPORT_T reports_data[1];
	ErrorCode_t ret = LPC_OK;

	memset((void *) &hid_param, 0, sizeof(USBD_HID_INIT_PARAM_T));
	hid_param.max_reports = 1;

	/* Init reports_data */
	reports_data[0].len = hid_report_size;
	reports_data[0].idle_time = 0;
	reports_data[0].desc = (uint8_t *) &HID_ReportDescriptor[0];

	USB_INTERFACE_DESCRIPTOR *pIntfDesc = (USB_INTERFACE_DESCRIPTOR *) &USB_FsConfigDescriptor[sizeof(USB_CONFIGURATION_DESCRIPTOR)];

	if ((pIntfDesc == 0) || (pIntfDesc->bInterfaceClass != USB_DEVICE_CLASS_HUMAN_INTERFACE)) {
		return -1;
	}

	hid_param.mem_base = usb_param->mem_base;
	hid_param.mem_size = usb_param->mem_size;
	hid_param.intf_desc = (uint8_t *) pIntfDesc;
	/* user defined functions */
	hid_param.HID_GetReport = getreport_fun;
	hid_param.HID_SetReport = setreport_fun;
	hid_param.HID_EpIn_Hdlr  = epin_hdlr_fun;
	hid_param.HID_EpOut_Hdlr = epout_hdlr_fun;
	hid_param.report_data  = reports_data;

	ret = USBD_API->hid->init(g_usb_hnd, &hid_param);
	if (ret != LPC_OK) return -2;
	/* allocate USB accessable memory space for report data */
	*report_saddr =  (uint8_t *) hid_param.mem_base;
	memset (hid_param.mem_base, 0, report_size);
	hid_param.mem_base += report_size;
	hid_param.mem_size -= report_size;
	/* update memory variables */
	usb_param->mem_base = hid_param.mem_base;
	usb_param->mem_size = hid_param.mem_size;
	return 0;
}

void connect_to_usb_bus () {
	NVIC_EnableIRQ (USB_IRQn);
	USBD_API->hw->Connect (g_usb_hnd, 1);
}

void disconnect_to_usb_bus () {
	USBD_API->hw->Connect (g_usb_hnd, 0);
	NVIC_DisableIRQ (USB_IRQn);
}
