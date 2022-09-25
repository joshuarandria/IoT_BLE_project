#include "gap.hpp"
// #include "InterruptIn.h"
// #include "ble/BLE.h"
// #include "example_gatt_server.hpp"
// #include "mbed-trace/mbed_trace.h"
// #include "mbed.h"
// #include <Gap.h>
// #include <cstdint>
// #include <cstdio>
// #include <events/mbed_events.h>

void CGAP::run() {
  _event_queue.call(printf, "run\n");
  _gap.setEventHandler(this);
  ble_error_t error = _ble.init(this, &CGAP::onBleStackInit);
  _event_queue.dispatch_forever();
}

void CGAP::onBleStackInit(BLE::InitializationCompleteCallbackContext *context) {
  _event_queue.call(printf, "on_ble_init_complete\n");
  BLE &ble_interface = context->ble;
  ble_error_t initialization_error = context->error;
  if (_on_ble_init_callback != nullptr) {
    _on_ble_init_callback();
  }
  if (initialization_error) {
    return;
  }
  ble_utils::print_mac_address();
  CGAP::start_advertising();
//   _event_queue.call(printf, "boolBlink = %d\n", _boolBlink);
  _event_queue.call_every(2s, this, &CGAP::blink);
}

void CGAP::start_advertising() {
  _event_queue.call(printf, "start_advertising\n");
  uint8_t advBuffer[ble::LEGACY_ADVERTISING_MAX_SIZE];
  ble::AdvertisingParameters advParameters(
      ble::advertising_type_t::CONNECTABLE_UNDIRECTED,
      ble::adv_interval_t(ble::millisecond_t(100)));
  ble::AdvertisingDataBuilder advDataBuilder(advBuffer);
  advDataBuilder.setFlags();
  advDataBuilder.setName(_device_name);
  //   advDataBuilder.addData(ble::adv_data_type_t::MANUFACTURER_SPECIFIC_DATA,
  //                          mbed::make_Span(&button_count, 1));
  //   advDataBuilder.replaceData(ble::adv_data_type_t::MANUFACTURER_SPECIFIC_DATA,
  //                              mbed::make_Span(&button_count, 1));

  ble_error_t error = _ble.gap().setAdvertisingParameters(
      ble::LEGACY_ADVERTISING_HANDLE, advParameters);
  if (error != 0U) {
    printf("%s\n", ble::BLE::errorToString(error));
    return;
  }
  error = _ble.gap().setAdvertisingPayload(ble::LEGACY_ADVERTISING_HANDLE,
                                           advDataBuilder.getAdvertisingData());
  if (error != 0U) {
    printf("%s\n", ble::BLE::errorToString(error));
    return;
  }
  error = _ble.gap().startAdvertising(ble::LEGACY_ADVERTISING_HANDLE);
  if (error != 0U) {
    printf("%s\n", ble::BLE::errorToString(error));
    return;
  }
}

void CGAP::blink(void) {
  if (_boolBlink == 0) {
    _led = !_led;
  } else if (_boolBlink == 1) {
    _led = false;
  } else {
    _event_queue.call(printf, "error in blink\n");
  }
}


void CGAP::onAdvertisingReport(const ble::AdvertisingReportEvent &event) {
  _event_queue.call(printf, "onAdvertisingReport\n");
  if (event.getRssi() < -65) {
    return;
  }
  ble::AdvertisingDataParser advParser(event.getPayload());
  while (advParser.hasNext()) {
    ble::AdvertisingDataParser::element_t field = advParser.next();
    if (field.type != ble::adv_data_type_t::FLAGS || field.value.size() != 1 ||
        !ble::adv_data_flags_t(field.value[0]).getGeneralDiscoverable()) {
      continue;
    }
    printf("%s\n", "We found a connectable device");
    ble_error_t error = _ble.gap().connect(
        event.getPeerAddressType(), event.getPeerAddress(),
        ble::ConnectionParameters() // use the default connection parameters
    );
    if (error != 0U) {
      printf("%s\n", ble::BLE::errorToString(error));
      return;
    }
  }
}
void CGAP::onConnectionComplete(const ble::ConnectionCompleteEvent &event) {
  _event_queue.call(printf, "onConnectionComplete\n");
  _boolBlink = 1;
//   _event_queue.call(printf, "boolBlink=%d\n", _boolBlink);
}

void CGAP::onDisconnectionComplete(
    const ble::DisconnectionCompleteEvent &event) {
  _event_queue.call(printf, "onDisconnectionComplete\n \n \n");
  start_advertising();
  _boolBlink = 0;
//   _event_queue.call(printf, "boolBlink=%d\n", _boolBlink);
}
