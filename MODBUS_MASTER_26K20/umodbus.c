void read_all_coils(int8 MODBUS_SLAVE_ADDRESS)
{  
   PC_MSG("Coils:\r\n");
   if(!(modbus_read_coils(MODBUS_SLAVE_ADDRESS,0,8)))
   {
      PC_MSG("Data: ");
      /*Started at 1 since 0 is quantity of coils*/
      for(int i=1; i < (modbus_rx.len); ++i)
         fprintf(PC,"GET_ALL_COILS=%u:%X*", MODBUS_SLAVE_ADDRESS, modbus_rx.data[i]);
      PC_MSG("\r\n\r\n");
   }
   else
   {
      fprintf(PC,"ERR_ALL_COILS=%u:%X*",MODBUS_SLAVE_ADDRESS,modbus_rx.error);
   }
}

void read_all_inputs(int8 MODBUS_SLAVE_ADDRESS)
{
   PC_MSG("Inputs:\r\n");
   if(!(modbus_read_discrete_input(MODBUS_SLAVE_ADDRESS,0,8)))
   {
      PC_MSG("Data: ");
      /*Started at 1 since 0 is quantity of coils*/
      for(int i=1; i < (modbus_rx.len); ++i)
         PC_DATA("%X ", modbus_rx.data[i]);
      PC_MSG("\r\n\r\n");
   }
   else
   {
      //PC_DATA("GET_ALL_COILSERRO", modbus_rx.error);
   }
}


void read_temperatura(int8 MODBUS_SLAVE_ADDRESS)
{  
   int8 cont = 0;
   
   while (cont < 5)
   {
      if(!(modbus_read_holding_registers(MODBUS_SLAVE_ADDRESS,0,1)))
      {  
         if (modbus_rx.len > 2){
            unsigned char sign;
            
            int16 sensData = make16(modbus_rx.data[1],modbus_rx.data[2]);
            
            // check negative 
            if (bit_test(sensData, 15))
            { 
               sign = '-'; 
               sensData = ~sensData + 1;                
            } else 
                  sign = '+'; 
   
            int16 fract = 0; 
            int8 tmp = sensData&0xF;        // obtain the fractional part nibble                
            float celsius = sensData >> 4 ;  // calculate the whole number part 
             
            if (tmp == 0xFF) 
                celsius = celsius + 1;    // Calculate the fractional part            
            else 
               for (int i=0; i<tmp; i++)    
                  fract = fract + 0625;    
     
            // Testa se o valor da temperatura n�o � negativo
            if (celsius > 1)
            {  
               float temp = celsius + (fract * 0.0001);
               
               fprintf(PC,"GET_TEMP=%u:%f*\r\n",MODBUS_SLAVE_ADDRESS,temp);
               return;
            }
         }
      }
      
      cont += 1;
      delay_ms(100);
   }
   
   fprintf(PC,"GET_TEMP=%u:ERRO_%X*\r\n", MODBUS_SLAVE_ADDRESS, modbus_rx.error);
}

void read_vitais(int8 MODBUS_SLAVE_ADDRESS)
{  
   int8 cont = 0;
   
   while (cont < 5)
   {
      if(!(modbus_read_holding_registers(MODBUS_SLAVE_ADDRESS,1,1)))
      {
         if (modbus_rx.len > 2){
            int16 BPM = make16(modbus_rx.data[1],modbus_rx.data[2]);
            
            fprintf(PC,"GET_BPM=%u:%lu*\r\n",MODBUS_SLAVE_ADDRESS,BPM);
            return;
         }
      }
      
      cont += 1;
      delay_ms(100);
   }
   
   fprintf(PC,"GET_BPM=%u:ERRO_%X*\r\n", MODBUS_SLAVE_ADDRESS, modbus_rx.error);
}

void read_all_holding(int8 MODBUS_SLAVE_ADDRESS)
{
   PC_MSG("Holding Registers:\r\n");
   if(!(modbus_read_holding_registers(MODBUS_SLAVE_ADDRESS,0,4)))
   {  
      PC_MSG("Data: ");
      //Started at 1 since 0 is quantity of coils
      for(int i=1; i < (modbus_rx.len); ++i){
         PC_DATA("%u ", modbus_rx.data[i]);    
      }
      
      PC_MSG("\r\n\r\n");
   }
   else
   {
      PC_DATA("<-**Exception %X**->\r\n\r\n", modbus_rx.error);
   }
}

void read_all_input_reg(int8 MODBUS_SLAVE_ADDRESS)
{
   //PC_MSG("Input Registers:\r\n");
   if(!(modbus_read_input_registers(MODBUS_SLAVE_ADDRESS,0,2)))
   {
      PC_MSG("Data: ");
      /*Started at 1 since 0 is quantity of coils*/
      for(int i=1; i < (modbus_rx.len); ++i)
         PC_DATA("%X ", modbus_rx.data[i]);
      PC_MSG("\r\n\r\n");
   }
   else
   {
      //PC_DATA("<-**Exception %X**->\r\n\r\n", modbus_rx.error);
   }
}

void write_coil(int8 MODBUS_SLAVE_ADDRESS)
{
   PC_MSG("Writing Single Coil:\r\n");
   if(!(modbus_write_single_coil(MODBUS_SLAVE_ADDRESS,6,TRUE)))
   {
      PC_MSG("Data: ");
      for(int i=0; i < (modbus_rx.len); ++i)
         PC_DATA("%X ", modbus_rx.data[i]);
      PC_MSG("\r\n\r\n");
   }
   else
   {
      PC_DATA("<-**Exception %X**->\r\n\r\n", modbus_rx.error);
   }
}

void write_reg(int8 MODBUS_SLAVE_ADDRESS)
{
   PC_MSG("Writing Single Register:\r\n");
   if(!(modbus_write_single_register(MODBUS_SLAVE_ADDRESS,3,0x4444)))
   {
      PC_MSG("Data: ");
      for(int i=0; i < (modbus_rx.len); ++i)
         PC_DATA("%X ", modbus_rx.data[i]);
      PC_MSG("\r\n\r\n");
   }
   else
   {
      PC_DATA("<-**Exception %X**->\r\n\r\n", modbus_rx.error);
   }
}

void write_coils(int8 MODBUS_SLAVE_ADDRESS)
{
   int8 coils[1] = { 0x50 };
   PC_MSG("Writing Multiple Coils:\r\n");
   if(!(modbus_write_multiple_coils(MODBUS_SLAVE_ADDRESS,0,8,coils)))
   {
      PC_MSG("Data: ");
      for(int i=0; i < (modbus_rx.len); ++i)
         PC_DATA("%X ", modbus_rx.data[i]);
      PC_MSG("\r\n\r\n");
   }
   else
   {
      PC_DATA("<-**Exception %X**->\r\n\r\n", modbus_rx.error);
   }
}

void write_regs(int8 MODBUS_SLAVE_ADDRESS)
{
   int16 reg_array[2] = {0x1111, 0x2222};
   PC_MSG("Writing Multiple Registers:\r\n");
   if(!(modbus_write_multiple_registers(MODBUS_SLAVE_ADDRESS,0,2,reg_array)))
   {
      PC_MSG("Data: ");
      for(int i=0; i < (modbus_rx.len); ++i)
         PC_DATA("%X ", modbus_rx.data[i]);
      PC_MSG("\r\n\r\n");
   }
   else
   {
      PC_DATA("<-**Exception %X**->\r\n\r\n", modbus_rx.error);
   }
}

void unknown_func(int8 MODBUS_SLAVE_ADDRESS)
{
   PC_MSG("Trying unknown function\r\n");
   PC_MSG("Diagnostic:\r\n");
   if(!(modbus_diagnostics(MODBUS_SLAVE_ADDRESS,0,0)))
   {
      PC_MSG("Data:");
      for(int i=0; i < (modbus_rx.len); ++i)
         PC_DATA("%X ", modbus_rx.data[i]);
      PC_MSG("\r\n\r\n");
   }
   else
   {
      PC_DATA("<-**Exception %X**->\r\n\r\n", modbus_rx.error);
   }
}
