char received[20];         // Armazena os dados recebidos na serial
char tmp[20];              // Armazena dados tempor�rios 

char comando[4];           // Armazena o tipo do comando
char param[18];            // Armazena o valor do parametro
char endereco[4];          // Armazena o numero do parametro
char valor[10];            // Armazena o valor a ser tratado

// Bloco respons�veis por modificar dados
void tratar_SET()
{  
   tmp = "RESET";
   if (!strcmp(tmp,param))
   {         
      fprintf(PC,"SET_RESET=1:OK*");
      delay_ms(200);
      reset_cpu();
   }
}

// Bloco respons�veis pela leitura dos dados
void tratar_GET()
{
   tmp = "ALL_COILS";
   if (!strcmp(tmp,param))
   {         
      int add = atoi(endereco);
      read_all_coils(add);
   }
   
   tmp = "TEMP";
   if (!strcmp(tmp,param))
   {         
      int add = atoi(endereco);
      read_temperatura(add);
   }
   
   tmp = "BPM";
   if (!strcmp(tmp,param))
   {         
      int add = atoi(endereco);
      read_vitais(add);
   }
   
   tmp = "ALL";
   if (!strcmp(tmp,param))
   {
      int add = atoi(endereco);
      read_all_holding(add);
   }
   
   tmp = "INPUTS";
   if (!strcmp(tmp,param))
   {         
      int add = atoi(endereco);
      read_all_input_reg(add);
   }
   
}

// Procedimento realiza o tratamento de Inser��es
void comando_set()
{  
   int i = 0;
   int inicio = 4;
   while (!(received[inicio] == '='))
   {
      param[i] = received[inicio];
      i++;
      inicio++;
   }   param[i] = '\0';

   i = 0;
   inicio = inicio+1;
   while (!(received[inicio] == ':'))
   {
      endereco[i] = received[inicio];
      i++;
      inicio++;
   } endereco[i] = '\0';

   i = 0;
   inicio = inicio+1;
   while (!(received[inicio] == '*'))
   {
      valor[i] = received[inicio];
      i++;
      inicio++;
   } valor[i] = '\0';
   
   tratar_SET();
}

// Procedimento realiza o tratamento de Solicita��es
void comando_get()
{
   int i = 0;
   int inicio = 4;
   while (!(received[inicio] == '='))
   {
      param[i] = received[inicio];
      i++;
      inicio++;
   }   param[i] = '\0';

   i = 0;
   inicio = inicio+1;
   while (!(received[inicio] == ':'))
   {
      endereco[i] = received[inicio];
      i++;
      inicio++;
   } endereco[i] = '\0';
   
   tratar_GET();   
}

//   Usado para decodificar o comando enviado
void decodifica()
{
   char comando[4];     // Armazena o tipo do comando
   char tmp[20];        // Vari�vel armazena temporariamente os dados a serem comparados com o da serial;
   
   comando[0] = received[0];
   comando[1] = received[1];
   comando[2] = received[2];
   comando[3] = '\0';
   
   tmp = "SET"; // Se for comando SET
   if (!strcmp(comando,tmp)){ comando_set(); }
   
   tmp = "GET"; // Se for comando GET
   if (!strcmp(comando,tmp)){ comando_get(); }
}

/// Evento disparado ao receber dados na serial
void evt_serial()
{  
   float timeout = 0;            // Conta o tempo de timeout
   int cont = 0;                 // Conta quantidade de caracteres recebidos
   char tmp = '\1';              // Armazena o caractere recuperado
   
   // Desabilita as interrup�es
   //disable_interrupts(int_rda);
   
   // Enquanto n�o estourar o timeout ou n�o vir o caractere de finaliza��o de mensagem '\r'
   while((timeout<200) && (tmp != '*'))
   {
      if (kbhit())
      {
         tmp  = getc(PC);         
         received[cont] = tmp;         
         cont++;
         timeout = 0;
      }else{
         timeout = timeout + 0.01;
      }
   }
   
   // Finaliza a string
   received[cont] = '\0';
   
   // Verifica se possui o tamanho m�nimo de um comando
   if (cont > 6){ decodifica(); }
   
   // Habilita as interrup��es
   //enable_interrupts(int_rda);  
}
