/**
 * \file
 *
 * \brief USB Device Stack HID Mouse Function Descriptor Setting.
 *
 * Copyright (c) 2015-2018 Microchip Technology Inc. and its subsidiaries.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * \asf_license_stop
 */

#ifndef USBDF_HID_MOUSE_DESC_H_
#define USBDF_HID_MOUSE_DESC_H_

#include "usb_protocol.h"
#include "usb_protocol_hid.h"
#include "usbd_config.h"

#define HID_MOUSE_DEV_DESC                                                                                             \
	USB_DEV_DESC_BYTES(CONF_USB_HID_MOUSE_BCDUSB,                                                                      \
	                   0x00,                                                                                           \
	                   0x00,                                                                                           \
	                   0x00,                                                                                           \
	                   CONF_USB_HID_MOUSE_BMAXPKSZ0,                                                                   \
	                   CONF_USB_HID_MOUSE_IDVENDER,                                                                    \
	                   CONF_USB_HID_MOUSE_IDPRODUCT,                                                                   \
	                   CONF_USB_HID_MOUSE_BCDDEVICE,                                                                   \
	                   CONF_USB_HID_MOUSE_IMANUFACT,                                                                   \
	                   CONF_USB_HID_MOUSE_IPRODUCT,                                                                    \
	                   CONF_USB_HID_MOUSE_ISERIALNUM,                                                                  \
	                   0x01)

#define HID_MOUSE_CFG_DESC                                                                                             \
	USB_CONFIG_DESC_BYTES(                                                                                             \
	    34, 1, 0x01, CONF_USB_HID_MOUSE_ICONFIG, CONF_USB_HID_MOUSE_BMATTRI, CONF_USB_HID_MOUSE_BMAXPOWER)

#define HID_MOUSE_IFC_DESC                                                                                             \
	USB_IFACE_DESC_BYTES(CONF_USB_HID_MOUSE_BIFCNUM, 0x00, 1, 0x03, 0x01, 0x02, CONF_USB_HID_MOUSE_IIFC),              \
	    USB_HID_DESC_BYTES(0x09, 0x21, 0x01, 0x22, 52),                                                                \
	    USB_ENDP_DESC_BYTES(CONF_USB_HID_MOUSE_INTIN_EPADDR, 0x03, CONF_USB_HID_MOUSE_INTIN_MAXPKSZ, 10)

#define HID_MOUSE_STR_DESCES                                                                                           \
	CONF_USB_HID_MOUSE_LANGID_DESC                                                                                     \
	CONF_USB_HID_MOUSE_IMANUFACT_STR_DESC                                                                              \
	CONF_USB_HID_MOUSE_IPRODUCT_STR_DESC                                                                               \
	CONF_USB_HID_MOUSE_ISERIALNUM_STR_DESC                                                                             \
	CONF_USB_HID_MOUSE_ICONFIG_STR_DESC

/** USB Device descriptors and configuration descriptors */
#define HID_MOUSE_DESCES_LS_FS HID_MOUSE_DEV_DESC, HID_MOUSE_CFG_DESC, HID_MOUSE_IFC_DESC, HID_MOUSE_STR_DESCES

#endif /* USBDF_HID_MOUSE_DESC_H_ */
