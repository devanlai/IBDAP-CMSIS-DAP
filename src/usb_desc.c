/*
 * usb_desc.c
 *
 *  Created on: Jun 18, 2015
 *      Author: yliu
 */

#include "compiler.h"
#include "app_usbd_cfg.h"
#include "DAP_config.h"

ALIGNED(4) const uint8_t HID_ReportDescriptor[] = {
	HID_UsagePageVendor(0x00),
	HID_Usage(0x01),
	HID_Collection(HID_Application),
	HID_LogicalMin(0),	/* value range: 0 - 0xFF */
	HID_LogicalMaxS(0xFF),
	HID_ReportSize(8),	/* 8 bits */
	HID_ReportCount(DAP_PACKET_SIZE),
	HID_Usage(0x01),
	HID_Input(HID_Data | HID_Variable | HID_Absolute),
	HID_ReportCount(DAP_PACKET_SIZE),
	HID_Usage(0x01),
	HID_Output(HID_Data | HID_Variable | HID_Absolute),
	HID_ReportCount(1),
	HID_Usage(0x01),
	HID_Feature(HID_Data | HID_Variable | HID_Absolute),
	HID_EndCollection,
};

const uint8_t hid_report_size = sizeof (HID_ReportDescriptor);


/**
 * USB Standard Device Descriptor
 */
ALIGNED(4) const uint8_t USB_DeviceDescriptor[] = {
	USB_DEVICE_DESC_SIZE,			/* bLength */
	USB_DEVICE_DESCRIPTOR_TYPE,		/* bDescriptorType */
	WBVAL(0x0200),					/* bcdUSB 2.0 */
	0x00,							/* bDeviceClass */
	0x00,							/* bDeviceSubClass */
	0x00,							/* bDeviceProtocol */
	USB_MAX_PACKET0,				/* bMaxPacketSize0 */
	WBVAL(0x1FC9),					/* idVendor */
	WBVAL(0x0081),					/* idProduct */
	WBVAL(0x0100),					/* bcdDevice */
	0x01,							/* iManufacturer */
	0x02,							/* iProduct */
	0x03,							/* iSerialNumber */
	0x01							/* bNumConfigurations */
};

/**
 * USB FSConfiguration Descriptor
 * All Descriptors (Configuration, Interface, Endpoint, Class, Vendor)
 */
ALIGNED(4) uint8_t USB_FsConfigDescriptor[] = {
	/* Configuration 1 */
	USB_CONFIGURATION_DESC_SIZE,			/* bLength */
	USB_CONFIGURATION_DESCRIPTOR_TYPE,		/* bDescriptorType */
	WBVAL(									/* wTotalLength */
		USB_CONFIGURATION_DESC_SIZE   +
		USB_INTERFACE_DESC_SIZE       +
		HID_DESC_SIZE                 +
		USB_ENDPOINT_DESC_SIZE        +
		USB_ENDPOINT_DESC_SIZE
		),
	0x01,							/* bNumInterfaces */
	0x01,							/* bConfigurationValue */
	0x00,							/* iConfiguration */
	USB_CONFIG_SELF_POWERED,		/* bmAttributes */
	USB_CONFIG_POWER_MA(100),		/* bMaxPower */

	/* Interface 0, Alternate Setting 0, HID Class */
	USB_INTERFACE_DESC_SIZE,		/* bLength */
	USB_INTERFACE_DESCRIPTOR_TYPE,	/* bDescriptorType */
	0x00,							/* bInterfaceNumber */
	0x00,							/* bAlternateSetting */
	0x02,							/* bNumEndpoints */
	USB_DEVICE_CLASS_HUMAN_INTERFACE,	/* bInterfaceClass */
	HID_SUBCLASS_NONE,				/* bInterfaceSubClass */
	HID_PROTOCOL_NONE,				/* bInterfaceProtocol */
	0x04,							/* iInterface */
	/* HID Class Descriptor */
	/* HID_DESC_OFFSET = 0x0012 */
	HID_DESC_SIZE,					/* bLength */
	HID_HID_DESCRIPTOR_TYPE,		/* bDescriptorType */
	WBVAL(0x0111),					/* bcdHID : 1.11*/
	0x00,							/* bCountryCode */
	0x01,							/* bNumDescriptors */
	HID_REPORT_DESCRIPTOR_TYPE,		/* bDescriptorType */
	WBVAL(sizeof(HID_ReportDescriptor)),	/* wDescriptorLength */
	/* Endpoint, HID Interrupt In */
	USB_ENDPOINT_DESC_SIZE,			/* bLength */
	USB_ENDPOINT_DESCRIPTOR_TYPE,	/* bDescriptorType */
	HID_EP_IN,						/* bEndpointAddress */
	USB_ENDPOINT_TYPE_INTERRUPT,	/* bmAttributes */
	WBVAL(64),					/* wMaxPacketSize */
	0x01,		/* 1ms */           /* bInterval */
	/* Endpoint, HID Interrupt Out */
	USB_ENDPOINT_DESC_SIZE,			/* bLength */
	USB_ENDPOINT_DESCRIPTOR_TYPE,	/* bDescriptorType */
	HID_EP_OUT,						/* bEndpointAddress */
	USB_ENDPOINT_TYPE_INTERRUPT,	/* bmAttributes */
	WBVAL(64),					/* wMaxPacketSize */
	0x01,							/* bInterval: 1ms */
	/* Terminator */
	0								/* bLength */
};

/**
 * USB String Descriptor (optional)
 */
const uint8_t USB_StringDescriptor[] = {
	/* Index 0x00: LANGID Codes */
	0x04,							/* bLength */
	USB_STRING_DESCRIPTOR_TYPE,		/* bDescriptorType */
	WBVAL(0x0409),					/* wLANGID : US English*/
	/* Index 0x01: Manufacturer */
	(8 * 2 + 2),					/* bLength (8 Char + Type + lenght) */
	USB_STRING_DESCRIPTOR_TYPE,		/* bDescriptorType */
	'A', 0,
	'R', 0,
	'M', 0,
	'S', 0,
	'T', 0,
	'A', 0,
	'R', 0,
	'T', 0,
	/* Index 0x02: Product */
	(24 * 2 + 2),					/* bLength (24 Char + Type + lenght) */
	USB_STRING_DESCRIPTOR_TYPE,		/* bDescriptorType */
	'I', 0,
	'B', 0,
	'D', 0,
	'A', 0,
	'P', 0,
	'-', 0,
	'L', 0,
	'P', 0,
	'C', 0,
	'1', 0,
	'1', 0,
	'U', 0,
	'3', 0,
	'5', 0,
	' ', 0,
	'C', 0,
	'M', 0,
	'S', 0,
	'I', 0,
	'S', 0,
	'-', 0,
	'D', 0,
	'A', 0,
	'P', 0,
	/* Index 0x03: Serial Number */
	(11 * 2 + 2),					/* bLength (13 Char + Type + lenght) */
	USB_STRING_DESCRIPTOR_TYPE,		/* bDescriptorType */
	'I', 0,
	'B', 0,
	'D', 0,
	'A', 0,
	'P', 0,
	'2', 0,
	'0', 0,
	'1', 0,
	'5', 0,
	'0', 0,
	'7', 0,
	/* Index 0x04: Interface 0, Alternate Setting 0 */
	(3 * 2 + 2),					/* bLength (3 Char + Type + lenght) */
	USB_STRING_DESCRIPTOR_TYPE,		/* bDescriptorType */
	'H', 0,
	'I', 0,
	'D', 0,
};
