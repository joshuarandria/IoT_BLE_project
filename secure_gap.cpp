#include "secure_gap.hpp"
#include "ble/BLE.h"
#include "ble/Gap.h"
// #include "gap.hpp"
#include "mbed.h"

// void CSecureGap::onBleStackInit(
void CSecureGap::onBleStackInit(
    BLE::InitializationCompleteCallbackContext *context) {
  ble_error_t error;
  if (context->error) {
    printf("Error during the initialisation\n");
    return;
  }
  // initialize security manager
  error = ble::BLE::Instance().securityManager().init(
      false /* Enable bonding*/, true /*Require MITM protection*/,
      SecurityManager::IO_CAPS_DISPLAY_ONLY /*IO capabilities*/,
      NULL /*Passkey*/, false /*Support data signing*/);
  ble_utils::printError(error, "_ble.securityManager().init() ");
  if (error != BLE_ERROR_NONE) {
    return;
  }
  ble::BLE::Instance().securityManager().allowLegacyPairing(true);
  ble::BLE::Instance().securityManager().setSecurityManagerEventHandler(this);
  ble::BLE::Instance().securityManager().setPairingRequestAuthorisation(true);

  // start advertisement using CGap instance of this class
  //   _gap.onBleStackInit(context); // _gap is the CGap type member of this
  //   class
  CGAP::onBleStackInit(context);

  error = ble::BLE::Instance().gap().enablePrivacy(true);
  ble_utils::printError(error, "_ble.gap().enablePrivacy() ");

  // Configure privacy settings
  privacyConfiguration.use_non_resolvable_random_address = false;
  privacyConfiguration.resolution_strategy =
      ble::peripheral_privacy_configuration_t::REJECT_NON_RESOLVED_ADDRESS;
  ble::BLE::Instance().gap().setPeripheralPrivacyConfiguration(
      &privacyConfiguration);
}

void CSecureGap::onConnectionComplete(
    const ble::ConnectionCompleteEvent &event) {
  CGAP::onConnectionComplete(event);
  ble_utils::printError(event.getStatus(), "onConnectionComplete() ");
  ble::connection_handle_t handle = event.getConnectionHandle();
  ble_error_t error = ble::BLE::Instance().securityManager().setLinkSecurity(
      handle, SecurityManager::SECURITY_MODE_ENCRYPTION_WITH_MITM);
  ble_utils::printError(error, "_ble.securityManager().setLinkSecurity() ");
}

void CSecureGap::pairingRequest(ble::connection_handle_t connectionHandle) {
  printf("Pairing requested - authorising\n");
  _ble.securityManager().acceptPairingRequest(connectionHandle);
}

void CSecureGap::linkEncryptionResult(ble::connection_handle_t connectionHandle,
                                      ble::link_encryption_t result) {
  if (result == ble::link_encryption_t::ENCRYPTED) {
    printf("Link ENCRYPTED\n");
  } else if (result == ble::link_encryption_t::ENCRYPTED_WITH_MITM) {
    printf("Link ENCRYPTED_WITH_MITM\n");
  } else if (result == ble::link_encryption_t::NOT_ENCRYPTED) {
    printf("Link NOT_ENCRYPTED\n");
  }
}

void CSecureGap::passkeyDisplay(ble::connection_handle_t connectionHandle,
                                const SecurityManager::Passkey_t passkey) {
  printf("Input passKey: \n");
  for (unsigned i = 0; i < ble::SecurityManager::PASSKEY_LEN; i++) {
    printf("%c ", passkey[ble::SecurityManager::PASSKEY_LEN - 1 - i]);
  }
  printf("\n");
}

void CSecureGap::confirmationRequest(
    ble::connection_handle_t connectionHandle) {
  printf("Conf required!\n");
}

void CSecureGap::passkeyRequest(ble::connection_handle_t connectionHandle) {
  printf("passkeyRequest\n");
}
void CSecureGap::keypressNotification(ble::connection_handle_t connectionHandle,
                                      SecurityManager::Keypress_t keypress) {
  printf("keypressNotification\n");
}

void CSecureGap::signingKey(ble::connection_handle_t connectionHandle,
                            const ble::csrk_t *csrk, bool authenticated) {
  printf("signingKey\n");
}

void CSecureGap::pairingResult(
    ble::connection_handle_t connectionHandle,
    SecurityManager::SecurityCompletionStatus_t result) {
  printf("Security status 0x%02x\r\n", result);
  if (result == SecurityManager::SEC_STATUS_SUCCESS) {
    printf("Security success\n");
  } else {
    printf("Security failed\n");
  }
}