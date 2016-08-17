#define RS485_USE_EXT_INT                 TRUE

#define MODBUS_TYPE                       MODBUS_TYPE_MASTER
#define MODBUS_SERIAL_TYPE                MODBUS_RTU     //use MODBUS_ASCII for ASCII mode
#define MODBUS_SERIAL_RX_BUFFER_SIZE      64
#define MODBUS_SERIAL_BAUD                9600

#define MODBUS_SERIAL_INT_SOURCE          MODBUS_INT_EXT

#define MODBUS_SERIAL_RX_PIN              PIN_B0   // Data receive pin
#define MODBUS_SERIAL_TX_PIN              PIN_B3   // Data transmit pin

#define MODBUS_SERIAL_ENABLE_PIN          PIN_B4   // Controls DE pin for RS485
#define MODBUS_SERIAL_RX_ENABLE           PIN_B5   // Controls RE pin for RS485

