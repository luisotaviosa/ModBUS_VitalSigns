int16 P = 0;                      // used to find peak in pulse wave
int16 T = 4000;                     // used to find trough in pulse wave
int16 amp = 100;                   // used to hold amplitude of pulse waveform
int16 ad_read;
int cont = 0;
int qtd = 0;
int flag = 0;
int tentativas = 0;

void ISR(){

   // Realiza a leitura do AD
   ad_read = read_adc();

   // Verifica o valor m�ximo
   if (ad_read > P){
      P = ad_read;
   }

   // Verifica o valor m�nimo
   if (ad_read < T){
      T = ad_read;
   }
   
   // Verifica a amplitude
   if ((P - T) > amp){
      amp = P - T;
   }

   // Verifica se pulso ascendente
   if ((ad_read > (P - (amp*0.5))) && (flag == 0)){ 
      cont += 1;
      flag = 1;
   }
   
   // Verifica se pulso descendente
   if ((ad_read < (T + (amp*0.5))) && (flag == 1)){ 
      flag = 0;
   }
}

void ler_batidas(){
   P = 0;
   T = 4000;
   amp = 0;
   cont = 0;
   
   for(int i = 0; i < 100; i++){
      ISR();
      delay_ms(2);
   }
   
   // Verifica se a amplitude � a de um dedo no sensor
   if ((amp < 250) && (amp > 50)){
      BPM = 1500/cont;      
      fprintf(PC,"GET_BPM=%u:%lu*\r\n",MODBUS_ADDRESS,BPM);      
      hold_regs[1] = BPM;
   }else{
      if (Tentativas > 5){
         hold_regs[1] = 0;
         Tentativas = 0;
      }else{
         Tentativas += 1;
      }
   }
   
   //fprintf(PC,"P: %lu T: %lu A: %lu\r\n",P,T,amp);
}
