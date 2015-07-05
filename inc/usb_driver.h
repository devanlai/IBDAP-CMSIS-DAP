/*
 * usb_driver.h
 *
 *  Created on: Jun 18, 2015
 *      Author: yliu
 */

#ifndef USB_DRIVER_H_
#define USB_DRIVER_H_


#include "app_usbd_cfg.h"
#include "usbd_rom_api.h"


typedef struct {
	const uint32_t usbdApiBase;				/*!< USBD API function table base address */
	const uint32_t reserved0;				/*!< Reserved */
	const uint32_t candApiBase;				/*!< CAN API function table base address */
	const uint32_t pwrApiBase;				/*!< Power API function table base address */
	const uint32_t reserved1;				/*!< Reserved */
	const uint32_t reserved2;				/*!< Reserved */
	const uint32_t reserved3;				/*!< Reserved */
	const uint32_t reserved4;				/*!< Reserved */
} LPC_ROM_API_T;



#define LPC_ROM_API_BASE_LOC      0x1FFF1FF8
#define LPC_ROM_API               (*((LPC_ROM_API_T        * *) LPC_ROM_API_BASE_LOC))


typedef ErrorCode_t (*HID_GetReport_Func_T)( USBD_HANDLE_T hHid, USB_SETUP_PACKET* pSetup, uint8_t** pBuffer, uint16_t* length);
typedef ErrorCode_t (*HID_SetReport_Func_T)( USBD_HANDLE_T hHid, USB_SETUP_PACKET* pSetup, uint8_t** pBuffer, uint16_t length);
typedef ErrorCode_t (*HID_EpIn_Hdlr_Func_T) (USBD_HANDLE_T hUsb, void* data, uint32_t event);
typedef ErrorCode_t (*HID_EpOut_Hdlr_Func_T) (USBD_HANDLE_T hUsb, void* data, uint32_t event);

void init_usb_clock ();
void init_usb_power ();
int init_usb_driver (USBD_API_INIT_PARAM_T *usb_param);
int init_usb_hid (USBD_API_INIT_PARAM_T *usb_param,
		HID_GetReport_Func_T getreport_fun, HID_SetReport_Func_T setreport_fun,
		HID_EpIn_Hdlr_Func_T epin_hdlr_fun, HID_EpOut_Hdlr_Func_T epout_hdlr_fun,
		uint8_t** report_saddr, int report_size);

void connect_to_usb_bus ();
void disconnect_to_usb_bus ();


extern USBD_HANDLE_T g_usb_hnd;


#endif /* USB_DRIVER_H_ */
