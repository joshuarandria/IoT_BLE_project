#include "InterruptIn.h"
#include "ble/BLE.h"
#include "ble/Gap.h"
#include "mbed-trace/mbed_trace.h"
#include "mbed.h"
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <events/mbed_events.h>
#include <string>
#include <ble_utils.h>
using namespace ble_utils;

class CGAP : private mbed::NonCopyable<CGAP>, public ble::Gap::EventHandler {
public:
  CGAP(BLE &ble, events::EventQueue &event_queue, const char *device_name,
       DigitalOut &led, int &boolBlink)
      : _ble(ble), _gap(ble.gap()), _event_queue(event_queue),
        _device_name(device_name), _led(led), _boolBlink(boolBlink){};

  void run();
  template <typename Function, typename Object>
  void setterForCallback(Function function, Object object) {
    _on_ble_init_callback = mbed::Callback<void(void)>(function, object);
  }
  virtual void
  onBleStackInit(BLE::InitializationCompleteCallbackContext *context);
  void start_advertising();
  void blink(void);
  void onAdvertisingReport(const ble::AdvertisingReportEvent &event) override;
  void onConnectionComplete(const ble::ConnectionCompleteEvent &event) override;
  void onDisconnectionComplete(
      const ble::DisconnectionCompleteEvent &event) override;

protected:
  BLE &_ble;
  ble::Gap &_gap;
  events::EventQueue &_event_queue;
  const char *_device_name;
  DigitalOut &_led;
  int &_boolBlink;
  mbed::Callback<void(void)> _on_ble_init_callback;
};
