#include "ble/BLE.h"

namespace ble_utils {

static void printDeviceAddress(ble::address_t &address) {
  printf("%02x:%02x:%02x:%02x:%02x:%02x\r\n", address[5], address[4],
         address[3], address[2], address[1], address[0]);
}

static void print_mac_address() {
  ble::own_address_type_t addr_type;
  ble::address_t address;
  BLE::Instance().gap().getAddress(addr_type, address);
  printDeviceAddress(address);
}

static void printError(ble_error_t error, const char *message) {
  printf("%s \n", message);
  ble::BLE::errorToString(error);
}

} // namespace ble_utils