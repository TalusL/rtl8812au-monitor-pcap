#include <cassert>
#include <iostream>
#include <pcap.h>
#include <memory>

#include <libusb.h>

#include "FrameParser.h"
#include "WiFiDriver.h"
#include "logger.h"



#ifdef _MSC_VER
int gettimeofday(struct timeval *tp, void *tzp)
{
  time_t clock;
  struct tm tm;
  SYSTEMTIME wtm;

  GetLocalTime(&wtm);
  tm.tm_year   = wtm.wYear  - 1900;
  tm.tm_mon    = wtm.wMonth - 1;
  tm.tm_mday   = wtm.wDay;
  tm.tm_hour   = wtm.wHour;
  tm.tm_min    = wtm.wMinute;
  tm.tm_sec    = wtm.wSecond;
  tm.tm_isdst  = -1;

  clock = mktime(&tm);
  tp->tv_sec   = clock;
  tp->tv_usec  = wtm.wMilliseconds * 1000;
  return (0);
}
#endif

#define USB_VENDOR_ID 0x0b05
#define USB_PRODUCT_ID 0x17d2

#define MAX_PACKET_SIZE 65535

pcap_t *handle;
pcap_dumper_t *dumper = nullptr;

static void packetProcessor(const Packet &packet) {
  pcap_pkthdr hdr{};
  hdr.caplen = packet.Data.size() - 4;
  hdr.len = hdr.caplen;
  timeval currentTime{};
  gettimeofday(&currentTime, nullptr);
  hdr.ts = currentTime;
  pcap_dump((u_char *) dumper, &hdr,reinterpret_cast<const u_char *>(packet.Data.data()));
}

int main() {


  handle = pcap_open_dead(DLT_IEEE802_11, MAX_PACKET_SIZE);
  dumper = pcap_dump_open_append(handle, "80211.pcap");
  if (dumper == nullptr) {
    fprintf(stderr, "pcap_dump_open_append() failed: %s\n", pcap_geterr(handle));
    pcap_close(handle);
    return -1;
  }

  libusb_context *ctx;
  libusb_device **devs;
  int rc;
  ssize_t cnt;


  auto logger = std::make_shared<Logger>();

  rc = libusb_init(&ctx);
  if (rc < 0)
    return rc;

// #ifndef NDEBUG
#if 0
  libusb_set_option(ctx, LIBUSB_OPTION_LOG_LEVEL, LIBUSB_LOG_LEVEL_DEBUG);
#endif

  libusb_device_handle *dev_handle =
      libusb_open_device_with_vid_pid(ctx, USB_VENDOR_ID, USB_PRODUCT_ID);
  if (dev_handle == NULL) {
    logger->error("Cannot find device {:04x}:{:04x}", USB_VENDOR_ID,
                  USB_PRODUCT_ID);
    libusb_exit(ctx);
    return 1;
  }

  /*Check if kenel driver attached*/
  if (libusb_kernel_driver_active(dev_handle, 0)) {
    rc = libusb_detach_kernel_driver(dev_handle, 0); // detach driver
  }
  rc = libusb_claim_interface(dev_handle, 0);
  assert(rc == 0);

  WiFiDriver wifi_driver{logger};
  auto rtlDevice = wifi_driver.CreateRtlDevice(dev_handle);
  rtlDevice->Init(packetProcessor, SelectedChannel{
                                       .Channel = 36,
                                       .ChannelOffset = 0,
                                       .ChannelWidth = CHANNEL_WIDTH_80,
                                   });

  rc = libusb_release_interface(dev_handle, 0);
  assert(rc == 0);

  libusb_close(dev_handle);

  libusb_exit(ctx);

  pcap_dump_close(dumper);
  pcap_close(handle);
  return 0;
}
