#include "gatt_server.hpp"
#include "ble/BLE.h"
#include "mbed.h"
#include <cstdio>

CGattService::CGattService(BLE &ble)
    : _ble(ble), _led2(LED2),
      _userDesc1(BLE_UUID_DESCRIPTOR_CHAR_USER_DESC, (uint8_t *)USERDESC1,
                 sizeof(USERDESC1), sizeof(USERDESC1), false),
      _characteristic1(
          GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE_WITHOUT_RESPONSE,
          0,_charac1Desc,1),
      _serv(UUID_SERV, _characTab, 1) {
  _characTab[0] = &_characteristic1;

  _charac1Desc[0] = &_userDesc1;
  // configure read security requirements
  _characteristic1.setReadSecurityRequirement(
      ble::att_security_requirement_t::UNAUTHENTICATED);
  // configure write security requirements
  _characteristic1.setWriteSecurityRequirement(
      ble::att_security_requirement_t::AUTHENTICATED);
  // configure update security requirements
  _characteristic1.setUpdateSecurityRequirement(
      ble::att_security_requirement_t::UNAUTHENTICATED);
}

void CGattService::onDataWritten(const GattWriteCallbackParams &e) {
  printf("onDataWritten()\n");
  if (e.handle == _characteristic1.getValueHandle()) {

    printf("\tCharac 1 (%s) with value %x \n", USERDESC1, (int)e.data[0]);
    if ((int)e.data[0] < 10)
      _led2.write(1.00f);
    else if ((int)e.data[0] < 75 && (int)e.data[0] >= 10)
      _led2.write(0.75f); // 25% brightness
    else
      _led2.write(0.00f);
  } else {
    printf("\tUnknown Characteristic \n");
    printf("\twrite operation: %u", e.writeOp);
    printf("\toffset: %u", e.offset);
    printf("\tlength: %u\n", e.len);
    printf("\t data: ");

    // for (size_t i = 0; i < e.len; ++i) {
    //   printf("%02X ", e.data[i]);
    // }

    printf("\n");
  }
}

void CGattService::onDataRead(const GattReadCallbackParams &e) {
  //   printf("onDataRead() using Conn. Handle %u for Att. Handle  %u",
  //   e.connHandle,
  //          e.handle);
  printf("onDataRead() \n");
  //   if (e.handle == _characteristic3.getValueHandle()) {
  //     uint64_t tickCount = Kernel::Clock::now().time_since_epoch().count();
  //     memcpy(&tickCount, e.data, sizeof(uint32_t));
  //     printf("\tCharac 3 (%s) with tickCount %d \n", USERDESC3,
  //     (int)tickCount);
  //   } else if (e.handle == _characteristic1.getValueHandle()) {
  //     printf("\tCharac 1 (%s) with value %d \n", USERDESC1, (int)e.data[0]);
  //   } else if (e.handle == _characteristic2.getValueHandle()) {
  //     printf("\tCharac 2 (%s) with value %d \n", USERDESC2, (int)e.data[0]);
  //   } else {
  //     printf("\tUnknown Characteristic \n");
  //   }
}

void CGattService::onUpdatesEnabled(
    const GattUpdatesEnabledCallbackParams &params) {
  printf("Updates enabled on handle %u \n", params.attHandle);
}

void CGattService::onUpdatesDisabled(
    const GattUpdatesDisabledCallbackParams &params) {
  printf("Updates disabled on handle %u \n", params.attHandle);
}

void CGattService::onConfirmationReceived(
    const GattConfirmationReceivedCallbackParams &params) {
  printf("Confirmation received on handle %u \n", params.attHandle);
}

void CGattService::run(void) {
  _ble.gattServer().addService(_serv);
  BLE::Instance().gattServer().setEventHandler(this);
}