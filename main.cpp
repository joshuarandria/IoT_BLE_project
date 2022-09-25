#include "InterruptIn.h"
#include "ble/BLE.h"
#include "gatt_server.hpp"
// #include "gap.hpp"
#include "mbed-trace/mbed_trace.h"
#include "mbed.h"
#include "secure_gap.hpp"
#include <Gap.h>
#include <cstdint>
#include <cstdio>
#include <events/mbed_events.h>
// #include <ble_utils.h>


static EventQueue event_queue(20 * EVENTS_EVENT_SIZE);
DigitalOut led(LED1, true);
int boolBlink = 0;

void schedule_ble_processing(BLE::OnEventsToProcessCallbackContext *context) {
  event_queue.call(Callback<void()>(&context->ble, &BLE::processEvents));
}
int main() {
//   using namespace ble_utils;

  event_queue.call(printf, "\n\n\n - - - - -    New test      - - - \n\n");
  BLE &ble_interface = BLE::Instance();

  ble_interface.onEventsToProcess(schedule_ble_processing);

  CSecureGap demoSecure(ble_interface, event_queue, "MTC-BLE", led,
                        boolBlink);
  CGattService demoService(ble_interface);
  demoSecure.setterForCallback(&CGattService::run, &demoService);
  demoSecure.run();

  return 0;
}
