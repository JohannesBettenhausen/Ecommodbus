/*
 * Ecommodbus.h
 *
 *  Created on: 05.09.2018
 *      Author: johannes
 */

#ifndef IMPLEMENT_ECOMMODBUS_H_
#define IMPLEMENT_ECOMMODBUS_H_
#include <string>
#include </usr/include/modbus/modbus.h>

namespace mod {

class Ecommodbus {

private:

	typedef uint8_t MeasurementInterval;

	static const MeasurementInterval MeasurementInterval_Disabled = 0;
	static const MeasurementInterval MeasurementInterval_10_Minutes = 1;
	static const MeasurementInterval MeasurementInterval_20_Minutes = 2;
	static const MeasurementInterval MeasurementInterval_30_Minutes = 3;
	static const MeasurementInterval MeasurementInterval_40_Minutes = 4;
	static const MeasurementInterval MeasurementInterval_50_Minutes = 5;
	static const MeasurementInterval MeasurementInterval_60_Minutes = 6;
	static const MeasurementInterval MeasurementInterval_70_Minutes = 7;
	static const MeasurementInterval MeasurementInterval_80_Minutes = 8;

	const  std::string ecomModbusMasterVersion = "1";

	const std::string timeStamp = __TIMESTAMP__;

	const std::string timeStampString = "(Build " + timeStamp + ").";

	const std::string ecomModbusMasterVersionString = "ecom Modbus Master V" + ecomModbusMasterVersion + timeStampString;

	#define PRINT_OUT( ... ) { fprintf(stdout, __VA_ARGS__ ); }

	#define ENABLE_DEBUG 0

	/*---------------------------------------------------------------------------------------*/
	#define ENABLE_MASTER_TRACE 0

	#if ENABLE_MASTER_TRACE
	#define MASTER_TRACE( ... ) { fprintf(stdout, __VA_ARGS__ ); }
	#else
	#define MASTER_TRACE( ... )
	#endif


	/*-----------------------------------Funktionen----------------------------------------------------*/

	static const char * MeasurementIntervalToString(const MeasurementInterval measurementInterval);

	static const char * FunctionCodeToString(const uint8_t functionCode);

	static void ecom_modbus_master_set_measurment_interval(modbus_t *ctx, /*MeasurementInterval*/ int interval);

	static uint16_t ecom_modbus_master_read_holding_register(modbus_t *ctx, int dataAddress, int * error);

	double CalulcateCorrectionFactorToStandardExcessAir(const double referenceO2InPermill,const double measuredO2InPermill);

	void RequestGasAndAirTemperatureValues(std::ostringstream &outputStream, modbus_t *ctx);

	void RequestLocalSensorTemperatureAndAirPressureValues(std::ostringstream &outputStream, modbus_t *ctx);

	void RequestGasValues(std::ostringstream &outputStream, modbus_t *ctx);


public:

	modbus_t *ctx=NULL;

	Ecommodbus(std::string comport,std::string ipv4);

	std::string ecom_modbus_master(int numberOfRetries);
	std::string ecom_modbus_master_parsemode( int numberOfRetries);
};

} /* namespace mod */

#endif /* IMPLEMENT_ECOMMODBUS_H_ */
