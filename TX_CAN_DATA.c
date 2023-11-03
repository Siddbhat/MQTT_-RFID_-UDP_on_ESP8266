// SEND blink getting output on this one
#include <SPI.h>
unsigned long  Can_id=0X15EFAABB;
#define CAN_2515
#define pushButton  7
// #define CAN_2518FD

// Set SPI CS Pin according to your hardware

#if defined(SEEED_WIO_TERMINAL) && defined(CAN_2518FD)
// For Wio Terminal w/ MCP2518FD RPi Hat：
// Channel 0 SPI_CS Pin: BCM 8
// Channel 1 SPI_CS Pin: BCM 7
// Interupt Pin: BCM25
const int SPI_CS_PIN  = BCM8;
const int CAN_INT_PIN = BCM25;
#else

// For Arduino MCP2515 Hat:
// the cs pin of the version after v1.1 is default to D9
// v0.9b and v1.0 is default D10
const int SPI_CS_PIN = 53;
const int CAN_INT_PIN = 2;
const int button = 7;
const int button_state = 0;
#endif


#ifdef CAN_2518FD
#include "mcp2518fd_can.h"
mcp2518fd CAN(SPI_CS_PIN); // Set CS pin
#endif

#ifdef CAN_2515
#include "mcp2515_can.h"
mcp2515_can CAN(SPI_CS_PIN); // Set CS pin
#endif

const int ledHIGH    = 1;
const int ledLOW     = 0;
                           


void setup() {
    SERIAL_PORT_MONITOR.begin(115200);

    while (CAN_OK != CAN.begin(CAN_500KBPS)) {             // init can bus : baudrate = 500k
        SERIAL_PORT_MONITOR.println("CAN init fail, retry...");
        delay(100);
    }
    SERIAL_PORT_MONITOR.println("CAN init ok!");
}

unsigned char stmp[8] = {0, 0, 0, 3, 4, 5, 6, 7};

void loop() {
    SERIAL_PORT_MONITOR.println("In loop");
    // send data:  id = 0x70, standard frame, data len = 8, stmp: data buf
    int  buttonState = digitalRead(pushButton);
  int potValue = analogRead(A0);
  SERIAL_PORT_MONITOR.println(potValue);
  stmp[0] = (potValue >> 8)& 0xFF; 
  stmp[1]= potValue & 0xFF;
  SERIAL_PORT_MONITOR.println(stmp[0]);
  SERIAL_PORT_MONITOR.println(stmp[1]);
  int value = ((stmp[0]<<8) | (stmp[1]));
  SERIAL_PORT_MONITOR.println(value);
    stmp[2] = buttonState;
    CAN.MCP_CAN::sendMsgBuf(Can_id, 0, 8, stmp);
    delay(1000);                       // send data once per second
}

