#include <18F26K80.h>
#device ADC=12

#FUSES NOMCLR
#FUSES NOWDT                    //No Watch Dog Timer
//#FUSES WDT128                   //Watch Dog Timer uses 1:128 Postscale
#FUSES NOBROWNOUT               //No brownout reset
//#FUSES NOLVP                    //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O
#FUSES NOXINST                  //Extended set extension and Indexed Addressing mode disabled (Legacy mode)

#use delay(internal=16MHz)
#use rs232 (baud=9600, xmit=PIN_C6, rcv=PIN_C7, stream=PC)

//#use STANDARD_IO( A )

#define ONEWIRE PIN_A0