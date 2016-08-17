#include <onewire.c>

void le_DS18B20()
{     
      //int8 numDev = 0;
      //int8 scratch[9];
      
      //int1 RST = 0;              //Utilizado pela one-wire
      int8 i, tmp, numDev = 0;
      float celsius;
      int16 sensData, fract;
      int8 scratch[9];      
      unsigned char sign; 
      
      if (!ow_reset())     // If a device is present and it's time to send string 
      {   
         numDev = FindDevices();  
          
         write_byte(0xCC); // Skip Rom command 
         write_byte(0x44); // Temperature convert command 
         output_float(ONEWIRE); 
         delay_ms(750);    // Max. conv. time is 750mS for 12 bit 
         ow_reset(); 
          
         // read all devices on bus 
         for (numRoms=1; numRoms <= numDev; numRoms++) 
         { 
            if (Send_MatchRom()) 
            { 
               write_byte(0xBE); // Read scratch pad command                
               for(int i=0; i<2; i++) 
               { 
                  scratch[i]= read_byte(); 
               }            
               // raw sensor data (16bit) 
               hold_regs[0] = make16(scratch[1], scratch[0]);
               
               // raw sensor data (16bit) 
               sensData = hold_regs[0]; 
                
               // check negative 
               if (bit_test(sensData, 15)) 
               { 
                  sign = '-'; 
                  sensData = ~sensData + 1;                
               } else 
                     sign = '+'; 
    
               fract = 0; 
               tmp = sensData&0xF;        // obtain the fractional part nibble                
               celsius = sensData >> 4 ;  // calculate the whole number part 
                
               if (tmp == 0xFF) 
                   celsius = celsius + 1;    // Calculate the fractional part            
               else 
                  for (i=0; i<tmp; i++)    
                     fract = fract + 0625;    
        
               // Testa se o valor da temperatura n�o � negativo
               if (celsius > 1)
               {
                  float temperatura = celsius + (fract * 0.0001);
                  fprintf(PC,"TEMP:%f;\r\n",temperatura);
                  //fprintf(PC,"%X\r\n", hold_regs[0]);
               }
            } 
         }
      }
}
