/*
 * Ecommodbus.cpp
 *
 *  Created on: 05.09.2018
 *      Author: johannes
 */

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN

#include "Ecommodbus.h"
#define __LINUX__
#ifdef __WINDOWS
#include <winsock.h>
#endif
#ifdef __LINUX
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
void closesocket(int socket) { close(socket); }
#endif

//#include "stdafx.h"
#include <thread>
#include "suppl.h"

#include <stdio.h>
#ifndef _MSC_VER
#include <unistd.h>
#endif
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include </usr/include/modbus/modbus.h>
#include <assert.h>
#include <sstream>
#include <iomanip>
#include <random>
#include <stdint.h>
#include <iomanip>      // std::setw
#include "ecom_modbus.h"




#endif


#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")

#ifdef __WINDOWS__
WSADATA wsaData;
#endif



namespace mod {






Ecommodbus::Ecommodbus(std::string comport,std::string ipv4) {

	// TODO Auto-generated constructor stub
	  std::cout << ecomModbusMasterVersionString << std::endl ;
	    //int numberOfWrites = 1;
	    std::string comPort = comport;
	    std::string ipv4Address = ipv4;
		//modbus_t *ctx=NULL;

		#ifdef __WINDOWS__

			if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0){
			printf("WSAStartup() failed miserably! With error code %ld\n", WSAGetLastError());
			//return 1;

			}
			else{

				printf("WSAStartup() looks fine!\n");

			}

		#endif

	    if (comPort == "TCP") {

	            PRINT_OUT("Connect to %s TCP Port 502\n", ipv4Address.c_str() );
	            this->ctx = modbus_new_tcp( ipv4Address.c_str(), 502);

	        }
	        else {

	             std::string ComPortStr = "\\\\.\\";
	            ComPortStr += comPort;
	            PRINT_OUT("Open Comport: %s\n", ComPortStr.c_str());
	            this->ctx = modbus_new_rtu(ComPortStr.c_str(), 9600, 'N', 8, 1);
	            modbus_set_slave(this->ctx, 0x0002);

	        }

	        if (this->ctx == NULL) {

	            fprintf(stderr, "Unable to allocate libmodbus context\n");
	            //return -1;

	        }

	    #ifdef __WINDOWS__

	    	WSACleanup();

		#endif


}


const char * Ecommodbus::MeasurementIntervalToString(const MeasurementInterval measurementInterval)
{
    switch (measurementInterval) {
        /* Modbus function codes */
    case MeasurementInterval_Disabled:   return "Measurement disabled";
    case MeasurementInterval_10_Minutes: return "Measurement Interval 10 Minutes";
    case MeasurementInterval_20_Minutes: return "Measurement Interval 20 Minutes";
    case MeasurementInterval_30_Minutes: return "Measurement Interval 30 Minutes";
    case MeasurementInterval_40_Minutes: return "Measurement Interval 40 Minutes";
    case MeasurementInterval_50_Minutes: return "Measurement Interval 50 Minutes";
    case MeasurementInterval_60_Minutes: return "Measurement Interval 60 Minutes";
    case MeasurementInterval_70_Minutes: return "Measurement Interval 70 Minutes";
    case MeasurementInterval_80_Minutes: return "Measurement Interval 80 Minutes";

    default:
    {
        return "Not supported measurement interval!";
    }
    }
}

const char * Ecommodbus::FunctionCodeToString(const uint8_t functionCode)
{
    switch (functionCode) {
        /* Modbus function codes */
    case MODBUS_FC_READ_COILS:               return "READ_COILS";
    case MODBUS_FC_READ_DISCRETE_INPUTS:     return "READ_DISCRETE_INPUTS";
    case MODBUS_FC_READ_HOLDING_REGISTERS:   return "READ_HOLDING_REGISTERS";
    case MODBUS_FC_READ_INPUT_REGISTERS:     return "READ_INPUT_REGISTERS";
    case MODBUS_FC_WRITE_SINGLE_COIL:        return "WRITE_SINGLE_COIL";
    case MODBUS_FC_WRITE_SINGLE_REGISTER:    return "WRITE_SINGLE_REGISTER";
    case MODBUS_FC_READ_EXCEPTION_STATUS:    return "READ_EXCEPTION_STATUS";
    case MODBUS_FC_WRITE_MULTIPLE_COILS:     return "WRITE_MULTIPLE_COILS";
    case MODBUS_FC_WRITE_MULTIPLE_REGISTERS: return "WRITE_MULTIPLE_REGISTERS";
    case MODBUS_FC_REPORT_SLAVE_ID:          return "REPORT_SLAVE_ID";
    case MODBUS_FC_MASK_WRITE_REGISTER:      return "MASK_WRITE_REGISTER";
    case MODBUS_FC_WRITE_AND_READ_REGISTERS: return "WRITE_AND_READ_REGISTERS";
    case 0x2B: return "Read_Device_Identification";
    default:
    {
        return "Not supported functionCode";
    }
    }
}
void Ecommodbus::ecom_modbus_master_parse_request(modbus_t *ctx, const uint8_t *inputBuffer)
{
    const uint8_t headerLength = modbus_get_header_length(ctx);

    const uint8_t functionCode = inputBuffer[headerLength];

    MASTER_TRACE("MASTER Received: FunctionCode: %s (%d) ", FunctionCodeToString(functionCode), functionCode);

    if (MODBUS_FC_READ_HOLDING_REGISTERS == functionCode) {
        /* Read holding registers */
    }
    else if (MODBUS_FC_WRITE_SINGLE_REGISTER == functionCode) {
        const int addr = MODBUS_GET_INT16_FROM_INT8(inputBuffer, headerLength + 1);

        MASTER_TRACE("Addr: 0x%.2X ", addr);
        if (ECOM_MODBUS_DATA_ADDRESS_SET_MEASUREMENT_INTERVAL == addr) {
            const int value = MODBUS_GET_INT16_FROM_INT8(inputBuffer, headerLength + 3);
            MASTER_TRACE("Value: 0x%.2X ", value);
            const int crc = MODBUS_GET_INT16_FROM_INT8(inputBuffer, headerLength + 5);
            MASTER_TRACE("CRC16: 0x%.2X \n", crc);
            MASTER_TRACE("SLAVE  Meaning : Set %s\n", MeasurementIntervalToString(value));
        }
    }
    else {
        fprintf(stderr, "Not supported functionCode: %s (%d)\n", FunctionCodeToString(functionCode), functionCode);
    }
}

/*---------------------------------------------------------------------------------------*/

void Ecommodbus::ecom_modbus_master_set_measurment_interval(modbus_t *ctx, /*MeasurementInterval*/ int interval)
{
    int addr = ECOM_MODBUS_DATA_ADDRESS_SET_MEASUREMENT_INTERVAL;

    MASTER_TRACE("MASTER Requests:");
    int rc = modbus_write_register(ctx, addr, interval);
    if (rc != 1) {
        fprintf(stderr, "ERROR modbus_write_registers (%d) %s\n", rc, modbus_strerror(errno));
        fprintf(stderr, "Address = %d\n", addr);
    }
}

uint16_t Ecommodbus::ecom_modbus_master_read_holding_register(modbus_t *ctx, int dataAddress, int * error)
{
    MASTER_TRACE("MASTER Requests:");
    uint16_t Register[1] = { 0 };

    int rc = modbus_read_registers(ctx, dataAddress, 1, Register);

    if (rc == -1) {
        *error = EMBXGTAR;
        fprintf(stderr, "ERROR: Reading Address = 0x%x ", dataAddress);
        fprintf(stderr, "modbus_read_registers failed.\n");
        return Register[0];
    }
    MASTER_TRACE("Received Register value = %d (0x%02X)\n", Register[0], Register[0]);
    return  Register[0];
}

void Ecommodbus::SetupConsole(const std::string &title) {
    std::wstring wsTmp(title.begin(), title.end());

    LPWSTR lpszString = (WCHAR*)wsTmp.c_str();

// TODO Opfermann
//    SetConsoleTitle(lpszString);
}

double Ecommodbus::CalulcateCorrectionFactorToStandardExcessAir(const double referenceO2InPermill,const double measuredO2InPermill){

    double calucaltedCorrectionFactorToStandardExcessAir = 1.0;
    calucaltedCorrectionFactorToStandardExcessAir = 1 / ((referenceO2InPermill - measuredO2InPermill) / (21 - referenceO2InPermill) + 1);
    return calucaltedCorrectionFactorToStandardExcessAir;
}

void Ecommodbus::RequestGasValues(std::ostringstream &outputStream, modbus_t *ctx) {
    int error = 0;

    uint16_t O2 = ecom_modbus_master_read_holding_register(ctx, ECOM_MODBUS_DATA_ADDRESS_GET_O2_PERMILL, &error);

    if (error) {
        fprintf(stderr, "Reading O2 failed!\n");
    }

    uint16_t CO = ecom_modbus_master_read_holding_register(ctx, ECOM_MODBUS_DATA_ADDRESS_GET_CO_PPM, &error);
    uint16_t CO2 = ecom_modbus_master_read_holding_register(ctx, ECOM_MODBUS_DATA_ADDRESS_GET_CO2_PERMILL, &error);
    uint16_t NO = ecom_modbus_master_read_holding_register(ctx, ECOM_MODBUS_DATA_ADDRESS_GET_NO_PPM, &error);
    uint16_t NO2 = ecom_modbus_master_read_holding_register(ctx, ECOM_MODBUS_DATA_ADDRESS_GET_NO2_PPM, &error);
    uint16_t SO2 = ecom_modbus_master_read_holding_register(ctx, ECOM_MODBUS_DATA_ADDRESS_GET_SO2_PPM, &error);

    //            outputStream << std::fixed << std::setprecision(2);
    //            outputStream << std::setw(5) << std::setfill('0');
    outputStream << "O2 : " << float(O2) / 100.0 << " %" << std::endl;
    outputStream << "CO : " << CO << " ppm" << std::endl;
    outputStream << "CO2: " << float(CO2) / 100.0 << " %" << std::endl;
#if 0
    outputStream << "NO : " << std::setw(5) << float(NO) / 10.0 << " ppm | " << std::setw(5) << ((float(NO) / 10.0) * 30) / 22.4 << " mg/qm" << std::endl;
    outputStream << "NO2: " << std::setw(5) << float(NO2) / 10.0 << " ppm | " << std::setw(5) << ((float(NO2) / 10.0) * 46) / 22.4 << " mg/qm" << std::endl;
    outputStream << "SO2: " << std::setw(5) << float(SO2) / 10.0 << " ppm" << std::endl;
#else
    outputStream << "NO : " << float(NO) / 10.0 << " ppm | " << ((float(NO) / 10.0) * 30) / 22.4 << " mg/qm" << std::endl;
    outputStream << "NO2: " << float(NO2) / 10.0 << " ppm | " << ((float(NO2) / 10.0) * 46) / 22.4 << " mg/qm" << std::endl;
    outputStream << "SO2: " << float(SO2) / 10.0 << " ppm" << std::endl;
#endif

    outputStream << "-------------------------------------------------------" << std::endl;

    double NOX = (float(NO) / 10.0) + (float(NO2) / 10.0);
    double cF = CalulcateCorrectionFactorToStandardExcessAir(0, (float(CO2) / 100.0));
    outputStream << "Calulcated NOX: " << NOX << " ppm | ";
    outputStream << NOX*2.05 << " mg/qm" << std::endl;
    outputStream << "With correction ";
    outputStream << "factor " << cF << " with O2 0% Ref: " << NOX*2.05 * cF << " mg/qm" << std::endl;;

    outputStream << "-------------------------------------------------------" << std::endl;

    uint16_t eta = ecom_modbus_master_read_holding_register(ctx, ECOM_MODBUS_DATA_ADDRESS_GET_ETA_PERMILL, &error);
    uint16_t lambda = ecom_modbus_master_read_holding_register(ctx, ECOM_MODBUS_DATA_ADDRESS_GET_LAMBDA, &error);
    uint16_t losses = ecom_modbus_master_read_holding_register(ctx, ECOM_MODBUS_DATA_ADDRESS_GET_LOSSES_PERMILL, &error);
    outputStream << "ETA    : " << float(eta) / 10 << " %" << std::endl;
    outputStream << "Lamda  : " << float(lambda) / 100 << std::endl;
    outputStream << "Losses : " << float(losses) / 10 << " %" << std::endl;
}

void Ecommodbus::RequestGasAndAirTemperatureValues(std::ostringstream &outputStream, modbus_t *ctx) {

    int error = 0;
    uint16_t airTemperature = ecom_modbus_master_read_holding_register(ctx, ECOM_MODBUS_DATA_ADDRESS_GET_AIR_TEMPERATURE_CELSIUS, &error);
    uint16_t gasTemperature = ecom_modbus_master_read_holding_register(ctx, ECOM_MODBUS_DATA_ADDRESS_GET_GAS_TEMPERATURE_CELSIUS, &error);
    outputStream << "Air Temperature:    " << float(airTemperature) / 10 << " Degree Celsius" << std::endl;
    outputStream << "Gas Temperature:    " << float(gasTemperature) / 10 << " Degree Celsius" << std::endl;
}

void Ecommodbus::RequestLocalSensorTemperatureAndAirPressureValues(std::ostringstream &outputStream, modbus_t *ctx) {

    int error = 0;
    uint16_t airPressure = ecom_modbus_master_read_holding_register(ctx, ECOM_MODBUS_DATA_ADDRESS_GET_AIR_PRESSURE_HPA, &error);
    uint16_t sensorTemperature = ecom_modbus_master_read_holding_register(ctx, ECOM_MODBUS_DATA_ADDRESS_GET_SENSOR_TEMPERATURE_CELSIUS, &error);
    outputStream << "-------------------------------------------------------" << std::endl;
    outputStream << "Air Pressure:       " << float(airPressure) / 10.0 << " hPa" << std::endl;
    outputStream << "Sensor Temperature: " << float(sensorTemperature) / 10 << " Degree Celsius" << std::endl;
}

std::string Ecommodbus::ecom_modbus_master_parsemode( int numberOfRetries)
{
#if ENABLE_DEBUG
    modbus_set_debug(this-->ctx, TRUE);
#endif

    const char *out;
    timeval tv;
    uint32_t response_to_sec;
    uint32_t response_to_usec;

    /* Save original timeout */
    modbus_get_response_timeout(this->ctx, &tv);
    response_to_sec = tv.tv_sec ;
    response_to_usec = tv.tv_usec;

    /* Define a new 250ms timeout! */
    tv.tv_sec = 0;
    tv.tv_usec = 250*1000;
    modbus_set_response_timeout(this->ctx, &tv);

    modbus_get_response_timeout(this->ctx, &tv);

    if (modbus_connect(this->ctx) == -1) {
        fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
        modbus_free(this->ctx);
        out="Error";
        //return 0;
    }

    modbus_flush(this->ctx);

    int i = 0;

    bool measurementStopped = true;

    /*---------------------------------------------------------------------------------------*/

    while (true) {

        std::ostringstream outputStream;
        std::string output;

        int error = 0;

        const uint16_t SystemStatus = ecom_modbus_master_read_holding_register(this->ctx, ECOM_MODBUS_GET_SYSTEM_STATUS, &error);

        if ((error > 0)) {
            fprintf(stderr, "Waiting for ecom modbus slave... (%s)\n", modbus_strerror(error));
            Sleep(2000);
            //FillScreen(outputStream.str());
            measurementStopped = true;
            continue;
        }

        uint16_t MeasurementStatus = 0;


        if (0 == SystemStatus) {


            MeasurementStatus = ecom_modbus_master_read_holding_register(this->ctx, ECOM_MODBUS_GET_MEASUREMENT_STATUS, &error);

            if (0 == MeasurementStatus) {
                /* Start Measurement */
                if (measurementStopped) {
                    ecom_modbus_master_set_measurment_interval(this->ctx, MeasurementInterval_10_Minutes);
                    measurementStopped = false;
                }
            }
        }
        else
        {


            Sleep(500);
            //FillScreen(outputStream.str());
            continue;
        }




        if (0 == MeasurementStatus) {


            Sleep(500);
        }
        else
        {

        }

        if (MeasurementStatus > 0) {
            RequestGasValues(outputStream, this->ctx);
            RequestGasAndAirTemperatureValues(outputStream, this->ctx);
            RequestLocalSensorTemperatureAndAirPressureValues(outputStream, this->ctx);
        }
        else
        {

            RequestLocalSensorTemperatureAndAirPressureValues(outputStream, this->ctx);
        }



        numberOfRetries--;

        Sleep(500);

        if (numberOfRetries==0){

        	modbus_close(this->ctx);
        	modbus_free(this->ctx);
        	output=outputStream.str();
        	return output;
        }
    }
}

std::string Ecommodbus::ecom_modbus_master( int numberOfRetries)
{
#if ENABLE_DEBUG
    modbus_set_debug(this-->ctx, TRUE);
#endif

    const char *out;
    timeval tv;
    uint32_t response_to_sec;
    uint32_t response_to_usec;

    /* Save original timeout */
    modbus_get_response_timeout(this->ctx, &tv);
    response_to_sec = tv.tv_sec ;
    response_to_usec = tv.tv_usec;

    /* Define a new 250ms timeout! */
    tv.tv_sec = 0;
    tv.tv_usec = 250*1000;
    modbus_set_response_timeout(this->ctx, &tv);

    modbus_get_response_timeout(this->ctx, &tv);

    if (modbus_connect(this->ctx) == -1) {
        fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
        modbus_free(this->ctx);
        out="Error";
        //return 0;
    }

    modbus_flush(this->ctx);

    int i = 0;

    bool measurementStopped = true;

    /*---------------------------------------------------------------------------------------*/

    while (true) {

        std::ostringstream outputStream;
        std::string output;
        outputStream << ecomModbusMasterVersionString << std::endl;
        outputStream << "-------------------------------------------------------" << std::endl;
        outputStream << "Request No. " << i++ << std::endl;
        outputStream << "-------------------------------------------------------" << std::endl;

        int error = 0;

        const uint16_t SystemStatus = ecom_modbus_master_read_holding_register(this->ctx, ECOM_MODBUS_GET_SYSTEM_STATUS, &error);

        if ((error > 0)) {
            fprintf(stderr, "Waiting for ecom modbus slave... (%s)\n", modbus_strerror(error));
            Sleep(2000);
            //FillScreen(outputStream.str());
            measurementStopped = true;
            continue;
        }

        uint16_t MeasurementStatus = 0;

        outputStream << "SystemStatus: ";
        if (0 == SystemStatus) {
            outputStream << "OK" << std::endl;

            MeasurementStatus = ecom_modbus_master_read_holding_register(this->ctx, ECOM_MODBUS_GET_MEASUREMENT_STATUS, &error);

            if (0 == MeasurementStatus) {
                /* Start Measurement */
                if (measurementStopped) {
                    ecom_modbus_master_set_measurment_interval(this->ctx, MeasurementInterval_10_Minutes);
                    measurementStopped = false;
                }
            }
        }
        else
        {
            outputStream << "ERROR Nr. " << SystemStatus << std::endl;
            outputStream << "-------------------------------------------------------" << std::endl;
            Sleep(500);
            //FillScreen(outputStream.str());
            continue;
        }

        outputStream << "-------------------------------------------------------" << std::endl;

        outputStream << "MeasurementStatus:  ";
        if (0 == MeasurementStatus) {
            outputStream << "No Measurement Results available" << std::endl;
            outputStream << "-------------------------------------------------------" << std::endl;
            Sleep(500);
        }
        else
        {
            outputStream << "Measurement results available" << std::endl;
        }
        outputStream << "-------------------------------------------------------" << std::endl;

        if (MeasurementStatus > 0) {
            RequestGasValues(outputStream, this->ctx);
            RequestGasAndAirTemperatureValues(outputStream, this->ctx);
            RequestLocalSensorTemperatureAndAirPressureValues(outputStream, this->ctx);
        }
        else
        {
            outputStream << "Waiting for gas measurement results!" << std::endl;
            RequestLocalSensorTemperatureAndAirPressureValues(outputStream, this->ctx);
        }

        outputStream << "-------------------------------------------------------" << std::endl;

        numberOfRetries--;

        Sleep(500);

        if (numberOfRetries==0){

        	modbus_close(this->ctx);
        	modbus_free(this->ctx);
        	output=outputStream.str();
        	return output;
        }
    }
}

} /* namespace mod */
