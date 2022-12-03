  
  // Este programa se utiliza para programar el modulo esp8266 usando arduino Mega.

 //Diagrama de conexion en : https://www.prometec.net/esp8266/
 
void setup()
{ Serial.begin(9600);
  Serial1.begin(9600); //Se envia mediante el monitor serial a 115200 baudios el comando "AT"
                    //El Monitor serial tiene que estar en modo de enviar y recibir (AMBOS)
                    // Si se responde con un OK se envia el siguiente codigo
                    //AT+UART_DEF=9600,8,1,0,0
  //Posteriormente se envia "AT+RST" para reiniciar el modulo 
  //Despues de reiniciar el modulo se sustituye la lina de arriba por la de abajo
  //Serial1.begin(9600);//       
  //Para cambiar la frecuencia de comunicación del modulo y se vuelve a compilar el programa
  //Se vuelve a enviar "AT" en la frecuencia de 9600, en caso de recibir un OK
  //Todo esta listo
      
}
 
void loop()
{ 
  if (Serial1.available())
  { char c = Serial1.read();
    Serial.print(c);
  }
  if (Serial.available())
  { char c = Serial.read();
    Serial1.print(c);
  }
}
