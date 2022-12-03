//se utilizó este programa para probar los sensores de pitot en etapa de pruebas.
//Routine for calculating the velocity from 
//a pitot tube and MPXV7002DP pressure differential sensor

float V_0 = 4.98; // supply voltage to the pressure sensor
float rho = 1.204; // density of air 

// parameters for averaging and offset
int offset1 = 0;
int offset2 = 0;
int offset3 = 0;
int offset_size = 10;
int veloc_mean_size = 20;
int zero_span = 2;

// setup and calculate offset
void setup() {
  Serial.begin(9600);
  for (int ii=0;ii<offset_size;ii++){
    offset1 += analogRead(A15)-(1023/2);
  }
   for (int ii=0;ii<offset_size;ii++){
    offset2 += analogRead(A1)-(1023/2);
  }
  for (int ii=0;ii<offset_size;ii++){
    offset3 += analogRead(A7)-(1023/2);
  }
  offset1 /= offset_size;
  offset2 /= offset_size;
  offset3 /= offset_size;
}

void loop() {
  float adc_avg1 = 0; float veloc1 = 0.0;
  float adc_avg2 = 0; float veloc2 = 0.0;
  float adc_avg3 = 0; float veloc3 = 0.0;
  
// average a few ADC readings for stability
  for (int ii=0;ii<veloc_mean_size;ii++){
    adc_avg1+= analogRead(A15)-offset1;
    adc_avg2+= analogRead(A1)-offset2;
    adc_avg2+= analogRead(A7)-offset3;
  }
  adc_avg1/=veloc_mean_size;
  adc_avg2/=veloc_mean_size;
  adc_avg3/=veloc_mean_size; 
 
  // make sure if the ADC reads below 512, then we equate it to a negative velocity
  if (adc_avg1>512-zero_span and adc_avg1<512+zero_span){
  } else{
    if (adc_avg1<512){
      veloc1 = -sqrt((-10000.0*((adc_avg1/1023.0)-0.5))/rho);
    } else{
      veloc1 = sqrt((10000.0*((adc_avg1/1023.0)-0.5))/rho);
    }
  }
  /*
  // make sure if the ADC reads below 512, then we equate it to a negative velocity
  if (adc_avg2>512-zero_span and adc_avg2<512+zero_span){
  } else{
    if (adc_avg2<512){
      veloc2 = -sqrt((-10000.0*((adc_avg2/1023.0)-0.5))/rho);
    } else{
      veloc2 = sqrt((10000.0*((adc_avg2/1023.0)-0.5))/rho);
    }
  }
  */
  // make sure if the ADC reads below 512, then we equate it to a negative velocity
 /*
  if (adc_avg3>512-zero_span and adc_avg3<512+zero_span){
  } else{
    if (adc_avg3<512){
      veloc3 = -sqrt((-10000.0*((adc_avg3/1023.0)-0.5))/rho);
    } else{
      veloc3 = sqrt((10000.0*((adc_avg3/1023.0)-0.5))/rho);
    }
  }
  */
//Serial.println((String) "La velocidad 1 es " + veloc1 + "m/s la velocidad 2 es:" + veloc2 + "m/s");
  Serial.println((String) "La velocidad 1 es " + veloc1 + "m/s");
  delay(1000); // delay for stability
}
