#pragma once

/*-------------------------------------------------------------------*/

#define ECOM_MODBUS_DATA_ADDRESS_SET_MEASUREMENT_INTERVAL       0x0020

/* Gas Measurement Results */
#define ECOM_MODBUS_DATA_ADDRESS_GET_O2_PERMILL                 0x0000
#define ECOM_MODBUS_DATA_ADDRESS_GET_CO_PPM                     0x0002
#define ECOM_MODBUS_DATA_ADDRESS_GET_CO2_PERMILL                0x0004
#define ECOM_MODBUS_DATA_ADDRESS_GET_NO_PPM                     0x0006
#define ECOM_MODBUS_DATA_ADDRESS_GET_NO2_PPM                    0x0008
#define ECOM_MODBUS_DATA_ADDRESS_GET_SO2_PPM                    0x000A

/* Pressure Measurement Results */
#define ECOM_MODBUS_DATA_ADDRESS_GET_AIR_PRESSURE_HPA           0x0030

/* Temperature Measurement Results */
#define ECOM_MODBUS_DATA_ADDRESS_GET_AIR_TEMPERATURE_CELSIUS    0x0040
#define ECOM_MODBUS_DATA_ADDRESS_GET_GAS_TEMPERATURE_CELSIUS    0x0042
#define ECOM_MODBUS_DATA_ADDRESS_GET_SENSOR_TEMPERATURE_CELSIUS 0x0044

/* Temperature Measurement Results */
#define ECOM_MODBUS_DATA_ADDRESS_GET_AIR_TEMPERATURE_CELSIUS    0x0040
#define ECOM_MODBUS_DATA_ADDRESS_GET_GAS_TEMPERATURE_CELSIUS    0x0042
#define ECOM_MODBUS_DATA_ADDRESS_GET_SENSOR_TEMPERATURE_CELSIUS 0x0044

/* Temperature Measurement Results */
#define ECOM_MODBUS_DATA_ADDRESS_GET_ETA_PERMILL                0x0050
#define ECOM_MODBUS_DATA_ADDRESS_GET_LAMBDA                     0x0052
#define ECOM_MODBUS_DATA_ADDRESS_GET_LOSSES_PERMILL             0x0054

/* System Status and Data */
#define ECOM_MODBUS_GET_SYSTEM_STATUS                           0x1000
#define ECOM_MODBUS_GET_MEASUREMENT_STATUS                      0x1004

/*-------------------------------------------------------------------*/
