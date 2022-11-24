/*
OpenTherm.h - OpenTherm Library for the ESP8266/Arduino platform
https://github.com/ihormelnyk/OpenTherm
http://ihormelnyk.com/pages/OpenTherm
Licensed under MIT license
Copyright 2018, Ihor Melnyk

Frame Structure:
P MGS-TYPE SPARE DATA-ID  DATA-VALUE
0 000      0000  00000000 00000000 00000000
*/

#ifndef OPENTHERM_HPP
#define OPENTHERM_HPP

#include <stdint.h>
#include <Arduino.h>

namespace OpenTherm {
  enum OpenThermResponseStatus {
    NONE = 0x0,
    SUCCESS = 0x1,
    INVALID = 0x2,
    TIMEOUT = 0x3
  };


  enum OpenThermMessageType {
    /*  Master to Slave */
    READ_DATA = B000,
    READ = READ_DATA, // for backwared compatibility
    WRITE_DATA = B001,
    WRITE = WRITE_DATA, // for backwared compatibility
    INVALID_DATA = B010,
    RESERVED = B011,
    /* Slave to Master */
    READ_ACK = B100,
    WRITE_ACK = B101,
    DATA_INVALID = B110,
    UNKNOWN_DATA_ID = B111
  };

  typedef OpenThermMessageType OpenThermRequestType; // for backwared compatibility

  enum OpenThermMessageID {
    Status = 0,                       // R - / flag8 / flag8 Master and Slave Status flags.
    TSet = 1,                         // - W / f8.8 /      Control setpoint  ie CH  water temperature setpoint (°C)
    MConfigMMemberIDcode = 2,          // - W / flag8 / u8   Master Configuration Flags /  Master MemberID Code
    SConfigSMemberIDcode = 3,          // R - / flag8 / u8   Slave Configuration Flags /  Slave MemberID Code
    Command = 4,                      // - W / u8 / u8     Remote Command
    ASFflags = 5,                      // R - / OEM-fault-code  flag8 / u8  Application-specific fault flags and OEM fault code
    RBPflags = 6,                      // R - / flag8 / flag8 Remote boiler parameter transfer-enable & read/write flags
    CoolingControl = 7,               // - W / f8.8        Cooling control signal (%)
    TsetCH2 = 8,                      // - W / f8.8        Control setpoint for 2e CH circuit (°C)
    TrOverride = 9,                   // R - / f8.8        Remote override room setpoint
    TSP = 10,                         // R - / u8 / u8     Number of Transparent-Slave-Parameters supported by slave
    TSPindexTSPvalue = 11,            // R W / u8 / u8     Index number / Value of referred-to transparent slave parameter.
    FHBsize = 12,                     // R - / u8 / u8     Size of Fault-History-Buffer supported by slave
    FHBindexFHBvalue = 12,            // R - / u8 / u8     Index number / Value of referred-to fault-history buffer entry.
    MaxRelModLevelSetting = 13,       // - W / 8.8         Maximum relative modulation level setting (%)
    MaxCapacityMinModLevel = 14,      // R - / u8 / u8     Maximum boiler capacity (kW) / Minimum boiler modulation level(%)
    TrSet = 16,                       // - W / f8.8        Room Setpoint (°C)
    RelModLevel = 17,                 // R - / f8.8        Relative Modulation Level (%)
    CHPressure = 18,                  // R - / f8.8        Water pressure in CH circuit  (bar)
    DHWFlowRate = 19,                 // R - / f8.8        Water flow rate in DHW circuit. (litres/minute)
    DayTime = 20,                     // R W / special / u8  Day of Week and Time of Day
    Date = 21,                        // R W /  u8 / u8    Calendar date
    Year = 22,                        // R W / u16         Calendar year
    TrSetCH2 = 23,                    // - W / f8.8        Room Setpoint for 2nd CH circuit (°C)
// ----------------------------------------------------------------------------------------
    Tr = 24,                          // - W / f8.8        Room temperature (°C)
    Tboiler = 25,                     // R - / f8.8        Boiler flow water temperature (°C)
    Tdhw = 26,                        // R - / f8.8        DHW temperature (°C)
    Toutside = 27,                    // R - / f8.8        Outside temperature (°C)
    Tret = 28,                        // R - / 8.8         Return water temperature (°C)
    Tstorage = 29,                    // R - / f8.8        Solar storage temperature (°C)
    Tcollector = 30,                  // R - / f8.8        Solar collector temperature (°C)
    TflowCH2 = 31,                     // R - / f8.8        Flow water temperature CH2 circuit (°C)
    Tdhw2 = 32,                       // R - / f8.8        Domestic hot water temperature 2 (°C)
    Texhaust = 33,                    // R - / s16         Boiler exhaust temperature (°C)
// ----------------------------------------------------------------------------------------
    TdhwSetUBTdhwSetLB = 48,          // R - / s8 / s8    DHW setpoint upper & lower bounds for adjustment  (°C)
    MaxTSetUBMaxTSetLB = 49,          // R - / s8 / s8    Max CH water setpoint upper & lower bounds for adjustment  (°C)
    HcratioUBHcratioLB = 50,          // R - / s8 / s8    OTC heat curve ratio upper & lower bounds for adjustment
// ----------------------------------------------------------------------------------------
    TdhwSet = 56,                     // R W / f8.8       DHW setpoint (°C)    (Remote parameter 1)
    MaxTSet = 57,                     // R W / f8.8       Max CH water setpoint (°C)  (Remote parameters 2)
    Hcratio = 58,                     // R W / f8.8       OTC heat curve ratio (°C)  (Remote parameter 3)
// ----------------------------------------------------------------------------------------
    // Ventilation Specific Message IDs
    VentStatus = 70,                  // flag8 / flag8      Master and Slave Status flags specific to Vent systems
    VentNomVentSet = 71,              // _ / u8           Set the relative ventilation 0-100%
    VentFault = 72,                   // flag8 / flag8      Application specific fault flags
    VentOEMFault = 73,                // flag8 / flag8      Oem specific fault flags
    VentSlaveVentConfig = 74,          // Slave Configuration Flags /  Slave MemberID Code
    VentNomVent = 77,                 // _ / u8           Read the relative ventilation 0-100%
    VentRelHumid = 78,                // _ / u8           Read the relative humidity 0-100%
    VentCO2level = 79,                // u16              CO2 level in PPM (0-2000)
    VentTsupplyin = 80,               // f8.8             Supply Outlet temperature
    VentTsupplyout = 81,              // f8.8             Supply Outlet temperature
    VentTexhaustin = 82,              // f8.8             Exhaust Inlet temperature
    VentTexhaustout = 83,             // f8.8             Exhaust Outlet temperature
    VentRPMexhaust = 84,              // u16              Actual RPM for inlet fan
    VentRPMsupply = 85,               // u16              Actual RPM for supply fan
    VentNomRelVent = 87,              // _ / u8           Get the current relative ventilation
// ----------------------------------------------------------------------------------------
    RemoteOverrideFunction = 100,     // R - / = 100, // flag8 / -  Function of manual and program changes in master and remote room setpoint.
    OEMDiagnosticCode = 115,          // R - / u16         OEM-specific diagnostic/service code
    BurnerStarts = 116,               // R W / u16         Number of starts burner
    CHPumpStarts = 117,               // R W / u16         Number of starts CH pump
    DHWPumpValveStarts = 118,         // R W / u16         Number of starts DHW pump/valve
    DHWBurnerStarts = 119,            // R W / u16         Number of starts burner during DHW mode
    BurnerOperationHours = 120,       // R W / u16         Number of hours that burner is in operation (i.e. flame on)
    CHPumpOperationHours = 121,       // R W / u16         Number of hours that CH pump has been running
    DHWPumpValveOperationHours = 122, // R W / u16         Number of hours that DHW pump has been running or DHW valve has been opened
    DHWBurnerOperationHours = 123,    // R W / u16         Number of hours that burner is in operation during DHW mode
    OpenThermVersionMaster = 124,     // - W / f8.8        The implemented version of the OpenTherm Protocol Specification in the master.
    OpenThermVersionSlave = 125,      // R - / f8.8        The implemented version of the OpenTherm Protocol Specification in the slave.
    MasterVersion = 126,              // - W / u8 / u8     Master product version number and type
    SlaveVersion = 127,               // R - / u8 / u8     Slave product version number and type
  };

  enum OpenThermStatus {
    NOT_INITIALIZED = 0x0,
    READY = 0x1,
    DELAY = 0x2,
    REQUEST_SENDING = 0x3,
    RESPONSE_WAITING = 0x4,
    RESPONSE_START_BIT = 0x5,
    RESPONSE_RECEIVING = 0x6,
    RESPONSE_READY = 0x7,
    RESPONSE_INVALID = 0x8
  };

  class OpenTherm {
  public:
    volatile OpenThermStatus status;

  private:
    const int inPin;
    const int outPin;
    const bool isSlave;

    volatile unsigned long response;
    volatile OpenThermResponseStatus responseStatus;
    volatile unsigned long responseTimestamp;
    volatile byte responseBitIndex;

  public:
    OpenTherm( int inPin = 4, int outPin = 5, bool isSlave = false );

    void begin( void(*handleInterruptCallback)( void ) );

    void begin( void(*handleInterruptCallback)( void ), void(*processResponseCallback)( unsigned long, OpenThermResponseStatus ) );

    bool isReady( );

    unsigned long sendRequest( unsigned long request );

    bool sendResponse( unsigned long request );

    bool sendRequestAync( unsigned long request );

    unsigned long buildRequest( OpenThermMessageType type, OpenThermMessageID id, unsigned int data );

    unsigned long buildResponse( OpenThermMessageType type, OpenThermMessageID id, unsigned int data );

    unsigned long getLastResponse( );

    OpenThermResponseStatus getLastResponseStatus( );

    const char *statusToString( OpenThermResponseStatus status );

    void handleInterrupt( );

    void process( );

    void end( );

    bool parity( unsigned long frame );

    OpenThermMessageType getMessageType( unsigned long message );

    OpenThermMessageID getDataID( unsigned long frame );

    const char *messageTypeToString( OpenThermMessageType message_type );

    bool isValidRequest( unsigned long request );

    bool isValidResponse( unsigned long response );

    //requests
    unsigned long buildSetBoilerStatusRequest( bool enableCentralHeating, bool enableHotWater = false, bool enableCooling = false, bool enableOutsideTemperatureCompensation = false, bool enableCentralHeating2 = false );

    unsigned long buildSetBoilerTemperatureRequest( float temperature );

    unsigned long buildGetBoilerTemperatureRequest( );

    //responses
    bool isFault( unsigned long response );

    bool isCentralHeatingActive( unsigned long response );

    bool isHotWaterActive( unsigned long response );

    bool isFlameOn( unsigned long response );

    bool isCoolingActive( unsigned long response );

    bool isDiagnostic( unsigned long response );

    uint16_t getUInt( const unsigned long response ) const;

    float getFloat( const unsigned long response ) const;

    unsigned int temperatureToData( float temperature );

    //basic requests
    unsigned long setBoilerStatus( bool enableCentralHeating, bool enableHotWater = false, bool enableCooling = false, bool enableOutsideTemperatureCompensation = false, bool enableCentralHeating2 = false );

    bool setBoilerTemperature( float temperature );

    float getBoilerTemperature( );

    float getReturnTemperature( );

    bool setDHWSetpoint( float temperature );

    float getDHWTemperature( );

    float getOutsideTemperature();

    float getDHWFlowrate();

    float getModulation( );

    float getPressure( );

    unsigned char getFault( );

    //Ventilation systems requests
    unsigned int getVentilation();

    unsigned int setVentilation(unsigned int nominal_value);

    float getVentSupplyInTemperature();

    float getVentSupplyOutTemperature();

    float getVentExhaustInTemperature();

    float getVentExhaustOutTemperature();

    bool getFaultIndication();

    bool getVentilationMode();

    bool getBypassStatus();

    bool getBypassAutomaticStatus();

    bool getDiagnosticIndication();

  private:

    int readState( );

    void setActiveState( );

    void setIdleState( );

    void activateBoiler( );

    void sendBit( bool high );

    void (*handleInterruptCallback)( );

    void (*processResponseCallback)( unsigned long, OpenThermResponseStatus );
  };

#ifndef IRAM_ATTR
#define IRAM_ATTR
#endif

#ifndef IRAM_ATTR
#define IRAM_ATTR IRAM_ATTR
#endif

}

#endif // OPENTHERM_HPP
