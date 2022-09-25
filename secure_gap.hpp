#include "ble/BLE.h"
#include "ble/Gap.h"
#include "gap.hpp"
#include "mbed.h"

class CSecureGap : public ble::SecurityManager::EventHandler, public CGAP {

public:
  CSecureGap(BLE &ble, events::EventQueue &event_queue, const char *device_name,
             DigitalOut &led, int &boolBlink)
      : CGAP(ble, event_queue, device_name, led, boolBlink){};

  virtual void
  onBleStackInit(BLE::InitializationCompleteCallbackContext *context) override;

  virtual void
  onConnectionComplete(const ble::ConnectionCompleteEvent &event) override;

  virtual void
  pairingRequest(ble::connection_handle_t connectionHandle) override;

  virtual void linkEncryptionResult(ble::connection_handle_t connectionHandle,
                                    ble::link_encryption_t result) override;

  virtual void
  passkeyDisplay(ble::connection_handle_t connectionHandle,
                 const SecurityManager::Passkey_t passkey) override;

  virtual void
  confirmationRequest(ble::connection_handle_t connectionHandle) override;

  virtual void
  passkeyRequest(ble::connection_handle_t connectionHandle) override;

  virtual void
  keypressNotification(ble::connection_handle_t connectionHandle,
                       SecurityManager::Keypress_t keypress) override;

  virtual void signingKey(ble::connection_handle_t connectionHandle,
                          const ble::csrk_t *csrk, bool authenticated) override;

  virtual void
  pairingResult(ble::connection_handle_t connectionHandle,
                SecurityManager::SecurityCompletionStatus_t result) override;

private:
  ble::peripheral_privacy_configuration_t privacyConfiguration;
};