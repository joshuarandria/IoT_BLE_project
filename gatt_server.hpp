#include "ble/BLE.h"
#include "ble_gatt_immediate_alert_service.hpp"
#include "characteristic.hpp"
#include "mbed.h"


#define USERDESC1 "Alert Charac"

#define UUID_SERV "ed5d0820-118c-49ae-93a7-0eaf03489916"

#define CHARAC_UUID1 "b09ad0d7-67e4-4b7b-be06-4cb50fbf8c46"
// #define CHARAC_UUID2 "23020210-7027-4292-ad19-892bcffbb30a"
// #define CHARAC_UUID3 "aefb514e-82df-4227-b9b1-ae318346420d"

class CGattService : public GattServer::EventHandler {
public:
  CGattService(BLE &ble);

  void run(void);
  virtual void onDataWritten(const GattWriteCallbackParams &e) override;

  virtual void onDataRead(const GattReadCallbackParams &e) override;

  virtual void
  onUpdatesEnabled(const GattUpdatesEnabledCallbackParams &params) override;

  virtual void
  onUpdatesDisabled(const GattUpdatesDisabledCallbackParams &params) override;

  virtual void onConfirmationReceived(
      const GattConfirmationReceivedCallbackParams &params) override;

private:
  BLE &_ble;
  //   DigitalOut _led2;
  PwmOut _led2;

  GattAttribute _userDesc1;
  //   GattAttribute _userDesc2;
  //   GattAttribute _userDesc3;
  //   GattAttribute _empty;

  GattCharacteristic *_characTab[1];

  GattAttribute *_charac1Desc[1];
  //   GattAttribute *_charac2Desc[1];
  //   GattAttribute *_charac3Desc[1];

  // GattCharacteristic _charac1;
  // GattCharacteristic _charac2;
  // GattCharacteristic _charac3;

  //   CCharacteristic<uint8_t> _characteristic1;
  //   CCharacteristic<uint16_t> _characteristic2;
  //   CCharacteristic<uint32_t> _characteristic3;
  AlertCharac<uint8_t> _characteristic1;

  GattService _serv;
};