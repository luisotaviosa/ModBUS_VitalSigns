#include <main.h>

#define LOADER_END   0x5FF
#include <bootloader.h>

void main()
{  
   initialize();
   
   // Loop principal
   while(TRUE){     
      // Se houver alguma informa��o na serial
      if (kbhit()){
         // Fun��o respons�vel por receber os dados da serial
         evt_serial();
      }
      
      /*read_temperatura(1);
      delay_ms(500);
      read_vitais(2);
      delay_ms(1000);*/
   }
}

