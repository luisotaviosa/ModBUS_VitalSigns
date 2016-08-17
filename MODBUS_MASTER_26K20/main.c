#include <main.h>

#define LOADER_END   0x5FF
#include <bootloader.h>

void main()
{  
   initialize();
   
   // Loop principal
   while(TRUE){     
      // Se houver alguma informação na serial
      if (kbhit()){
         // Função responsável por receber os dados da serial
         evt_serial();
      }
      
      /*read_temperatura(1);
      delay_ms(500);
      read_vitais(2);
      delay_ms(1000);*/
   }
}

