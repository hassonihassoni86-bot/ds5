/*
 * Minimal libusb 1.0 type definitions (standard USB descriptor layouts).
 *
 * The OpenOrbis libusb.h / Usbd.h differ between toolchain releases (some only
 * forward-declare these structures). The plugin resolves every sceUsbd*
 * function dynamically, so it only needs the data types: they are defined here
 * and <orbis/Usbd.h> is intentionally NOT included anywhere.
 */

#ifndef LIBUSB_ENDPOINT_PATCH_H
#define LIBUSB_ENDPOINT_PATCH_H

#include <stdint.h>

typedef struct libusb_device        libusb_device;          // opaque
typedef struct libusb_device_handle libusb_device_handle;   // opaque

struct libusb_device_descriptor {
    uint8_t  bLength;
    uint8_t  bDescriptorType;
    uint16_t bcdUSB;
    uint8_t  bDeviceClass;
    uint8_t  bDeviceSubClass;
    uint8_t  bDeviceProtocol;
    uint8_t  bMaxPacketSize0;
    uint16_t idVendor;
    uint16_t idProduct;
    uint16_t bcdDevice;
    uint8_t  iManufacturer;
    uint8_t  iProduct;
    uint8_t  iSerialNumber;
    uint8_t  bNumConfigurations;
};

struct libusb_endpoint_descriptor {
    uint8_t  bLength;
    uint8_t  bDescriptorType;
    uint8_t  bEndpointAddress;
    uint8_t  bmAttributes;
    uint16_t wMaxPacketSize;
    uint8_t  bInterval;
    uint8_t  bRefresh;        /* audio only */
    uint8_t  bSynchAddress;   /* audio only */
    const unsigned char *extra;
    int32_t  extra_length;
};

struct libusb_interface_descriptor {
    uint8_t  bLength;
    uint8_t  bDescriptorType;
    uint8_t  bInterfaceNumber;
    uint8_t  bAlternateSetting;
    uint8_t  bNumEndpoints;
    uint8_t  bInterfaceClass;
    uint8_t  bInterfaceSubClass;
    uint8_t  bInterfaceProtocol;
    uint8_t  iInterface;
    const struct libusb_endpoint_descriptor *endpoint;
    const unsigned char *extra;
    int32_t  extra_length;
};

struct libusb_interface {
    const struct libusb_interface_descriptor *altsetting;
    int32_t  num_altsetting;
};

struct libusb_config_descriptor {
    uint8_t  bLength;
    uint8_t  bDescriptorType;
    uint16_t wTotalLength;
    uint8_t  bNumInterfaces;
    uint8_t  bConfigurationValue;
    uint8_t  iConfiguration;
    uint8_t  bmAttributes;
    uint8_t  MaxPower;
    const struct libusb_interface *interface;
    const unsigned char *extra;
    int32_t  extra_length;
};

#endif /* LIBUSB_ENDPOINT_PATCH_H */
