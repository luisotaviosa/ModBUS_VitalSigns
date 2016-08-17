#include <18F26K20.h>
#device ADC=10

#FUSES MCLR
#FUSES NOWDT                    //No Watch Dog Timer
//#FUSES WDT128                   //Watch Dog Timer uses 1:128 Postscale
#FUSES NOBROWNOUT               //No brownout reset
#FUSES NOLVP                    //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O
#FUSES NOXINST                  //Extended set extension and Indexed Addressing mode disabled (Legacy mode)

#use delay(internal=16MHz)

#use rs232 (baud=9600, UART1, stream=PC, errors)

#define PC_MSG(msg)        fprintf(PC, msg)
#define PC_DATA(msg,data)  fprintf(PC, msg, data)
