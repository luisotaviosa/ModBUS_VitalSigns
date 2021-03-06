int8 coils = 0b00000101;
int8 inputs = 0b00001001;
int16 hold_regs[] = {0x0000,0x0000,0x0000,0x0000};
int16 input_regs[] = {0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000};
int16 event_count = 0;

#define RS485_USE_EXT_INT                 TRUE

#define MODBUS_TYPE                       MODBUS_TYPE_SLAVE
#define MODBUS_SERIAL_TYPE                MODBUS_RTU     //use MODBUS_ASCII for ASCII mode
#define MODBUS_SERIAL_RX_BUFFER_SIZE      64
#define MODBUS_SERIAL_BAUD                9600

#define MODBUS_SERIAL_INT_SOURCE          MODBUS_INT_EXT

#define MODBUS_SERIAL_TX_PIN              PIN_B3   // Data transmit pin
#define MODBUS_SERIAL_RX_PIN              PIN_B0   // Data receive pin

#define MODBUS_SERIAL_ENABLE_PIN          PIN_B4   // Controls DE pin for RS485
#define MODBUS_SERIAL_RX_ENABLE           PIN_B5   // Controls RE pin for RS485

#define MODBUS_ADDRESS                    1

//  VARIABLES
int pulsePin = 1;                         // Pulse Sensor purple wire connected to analog pin 0
#define blinkPin PIN_C4;                  // pin to blink led at each beat
#define fadePin PIN_C5;                   // pin to do fancy classy fading blink at each beat
int fadeRate = 1;                         // used to fade LED on with PWM on fadePin
// these variables are volatile because they are used during the interrupt service routine!
int16 BPM;                   // used to hold the pulse rate
int8 Signal;                // holds the incoming raw data
int8 IBI = 600;             // holds the time between beats, the Inter-Beat Interval
boolean Pulse = false;     // true when pulse wave is high, false when it's low
boolean QS = false;        // becomes true when Arduoino finds a beat.
