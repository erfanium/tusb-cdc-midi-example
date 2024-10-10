#include "tusb.h"

#ifndef USBD_VID
#define USBD_VID (0x2E8A)  // Raspberry Pi
#endif

#define USBD_PID (0x000a)  // Raspberry Pi Pico SDK CDC for RP2040

#ifndef USBD_MANUFACTURER
#define USBD_MANUFACTURER "Raspberry Pi"
#endif

#ifndef USBD_PRODUCT
#define USBD_PRODUCT "Pico"
#endif

#define USBD_DESC_LEN \
  (TUD_CONFIG_DESC_LEN + TUD_CDC_DESC_LEN + TUD_MIDI_DESC_LEN)
#define TUD_RPI_RESET_DESC_LEN 9
#if !PICO_STDIO_USB_DEVICE_SELF_POWERED
#define USBD_CONFIGURATION_DESCRIPTOR_ATTRIBUTE (0)
#define USBD_MAX_POWER_MA (250)
#else
#define USBD_CONFIGURATION_DESCRIPTOR_ATTRIBUTE \
  TUSB_DESC_CONFIG_ATT_SELF_POWERED
#define USBD_MAX_POWER_MA (1)
#endif

#define USBD_ITF_CDC (0)   // needs 2 interfaces
#define USBD_ITF_MIDI (2)  // needs 2 interfaces
#define USBD_ITF_MAX (4)

#define USBD_CDC_EP_CMD (0x81)
#define USBD_CDC_EP_OUT (0x02)
#define USBD_CDC_EP_IN (0x82)
#define USBD_CDC_CMD_MAX_SIZE (8)
#define USBD_CDC_IN_OUT_MAX_SIZE (64)

#define USBD_MIDI_EP_OUT (0x03)
#define USBD_MIDI_EP_IN (0x83)
#define USBD_MIDI_IN_OUT_MAX_SIZE (64)

#define USBD_STR_0 (0x00)
#define USBD_STR_MANUF (0x01)
#define USBD_STR_PRODUCT (0x02)
#define USBD_STR_SERIAL (0x03)
#define USBD_STR_CDC (0x04)
#define USBD_STR_MIDI (0x05)
#define USBD_STR_RPI_RESET (0x06)

static const tusb_desc_device_t usbd_desc_device = {
    .bLength = sizeof(tusb_desc_device_t),
    .bDescriptorType = TUSB_DESC_DEVICE,
    .bcdUSB = 0x0210,
    .bDeviceClass = TUSB_CLASS_MISC,
    .bDeviceSubClass = MISC_SUBCLASS_COMMON,
    .bDeviceProtocol = MISC_PROTOCOL_IAD,
    .bMaxPacketSize0 = CFG_TUD_ENDPOINT0_SIZE,
    .idVendor = USBD_VID,
    .idProduct = USBD_PID,
    .bcdDevice = 0x0100,
    .iManufacturer = USBD_STR_MANUF,
    .iProduct = USBD_STR_PRODUCT,
    .iSerialNumber = USBD_STR_SERIAL,
    .bNumConfigurations = 1,
};

static const uint8_t usbd_desc_cfg[USBD_DESC_LEN] = {
    TUD_CONFIG_DESCRIPTOR(1, USBD_ITF_MAX, USBD_STR_0, USBD_DESC_LEN,
                          USBD_CONFIGURATION_DESCRIPTOR_ATTRIBUTE,
                          USBD_MAX_POWER_MA),

    TUD_CDC_DESCRIPTOR(USBD_ITF_CDC, USBD_STR_CDC, USBD_CDC_EP_CMD,
                       USBD_CDC_CMD_MAX_SIZE, USBD_CDC_EP_OUT, USBD_CDC_EP_IN,
                       USBD_CDC_IN_OUT_MAX_SIZE),

    TUD_MIDI_DESCRIPTOR(USBD_ITF_MIDI, USBD_STR_MIDI, USBD_MIDI_EP_OUT,
                        USBD_MIDI_EP_IN, USBD_MIDI_IN_OUT_MAX_SIZE)

};

static char usbd_serial_str[8 * 2 + 1];

static const char *const usbd_desc_str[] = {
    [USBD_STR_MANUF] = USBD_MANUFACTURER, [USBD_STR_PRODUCT] = USBD_PRODUCT,
    [USBD_STR_SERIAL] = usbd_serial_str,  [USBD_STR_CDC] = "Board CDC",
#if PICO_STDIO_USB_ENABLE_RESET_VIA_VENDOR_INTERFACE
    [USBD_STR_RPI_RESET] = "Reset",
#endif
};

const uint8_t *tud_descriptor_device_cb(void) {
  return (const uint8_t *)&usbd_desc_device;
}

uint8_t const *tud_descriptor_configuration_cb(uint8_t index) {
  (void)index;  // for multiple configurations

  return usbd_desc_cfg;
}

#define USBD_DESC_STR_MAX 20

uint16_t const *tud_descriptor_string_cb(uint8_t index,
                                         __unused uint16_t langid) {
  static uint16_t desc_str[USBD_DESC_STR_MAX];

  // Assign the SN using the unique flash id
  if (!usbd_serial_str[0]) {
    pico_get_unique_board_id_string(usbd_serial_str, sizeof(usbd_serial_str));
  }

  uint8_t len;
  if (index == 0) {
    desc_str[1] = 0x0409;  // supported language is English
    len = 1;
  } else {
    if (index >= sizeof(usbd_desc_str) / sizeof(usbd_desc_str[0])) {
      return NULL;
    }
    const char *str = usbd_desc_str[index];
    for (len = 0; len < USBD_DESC_STR_MAX - 1 && str[len]; ++len) {
      desc_str[1 + len] = str[len];
    }
  }

  // first byte is length (including header), second byte is string type
  desc_str[0] = (uint16_t)((TUSB_DESC_STRING << 8) | (2 * len + 2));

  return desc_str;
}

// BOS configuration
#define BOS_TOTAL_LEN (TUD_BOS_DESC_LEN + TUD_BOS_MICROSOFT_OS_DESC_LEN)

#define MS_OS_20_DESC_LEN 166

uint8_t const desc_bos[] = {
    // total length, number of device caps
    TUD_BOS_DESCRIPTOR(BOS_TOTAL_LEN, 1),

    // Microsoft OS 2.0 descriptor
    TUD_BOS_MS_OS_20_DESCRIPTOR(MS_OS_20_DESC_LEN, 1)};

TU_VERIFY_STATIC(sizeof(desc_bos) == BOS_TOTAL_LEN, "Incorrect size");

uint8_t const *tud_descriptor_bos_cb(void) { return desc_bos; }
