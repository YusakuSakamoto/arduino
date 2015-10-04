#include <iostream>
#ifndef __CONFIG_H__
#define __CONFIG_H__
#include "/home/sakamoto/lib/imcs01_driver/driver/urbtc.h"
#include "/home/sakamoto/lib/imcs01_driver/driver/urobotc.h"

const std::string arduino_serial   = "/dev/ttyUSB0";
const std::string imcs01_port_name = "/dev/urbtc2";
const std::string connect_device   = "/dev/ttyACM0";
const std::string gps_serial       = "/dev/ttyUSB0";

#endif
