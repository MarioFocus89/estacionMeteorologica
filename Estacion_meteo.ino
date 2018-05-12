#include <Wire.h>
#include <DHT.h>
#include <SFE_BMP180.h>

#define gasSensorPin A0
#define buzzerPin 10
#define tempPin 2
#define loopTime 500
#define gasLimitMed 150
#define gasLimitHigh 500

DHT dht(tempPin, DHT11);
SFE_BMP180 pressure;

void setup()
{
  pinMode(buzzerPin, OUTPUT);
  dht.begin();
  pressure.begin();
  pressure.startPressure(3);
  delay(50);
  Serial.begin(9600);
  //Serial.write("Metano\t\tTemperatura(C)\tHumedad(%)\tPresion(mbar)\n");  
}

void loop() 
{
  while(!Serial.available())
    delay(loopTime);
  Serial.read();
  switch(readMetane()){
    case 0:
      Serial.write("BAJO,");
      break;
    case 1:
      Serial.write("PRECAUCION!,");
      break;
    case 2:
      Serial.write("PELIGRO!,");
      break;
    default:
      Serial.write("DESCONOCIDO,");
      break;
  };
  double temperature = dht.readTemperature();
  Serial.print(temperature); Serial.write(",");
  Serial.print(dht.readHumidity()); Serial.write(",");
  Serial.print(readPressure(temperature));
}

int readMetane(){
  int raw_adc = analogRead(gasSensorPin);
  if(raw_adc < gasLimitMed){
    return 0;
  }else if((raw_adc >= gasLimitMed)&&(raw_adc < gasLimitHigh)){
    tone(buzzerPin, 1000, loopTime*0.3);
    return 1;
  }else if(raw_adc >= gasLimitHigh){
    tone(buzzerPin, 1000, loopTime*0.7);
    return 2;
  }
}


double readPressure(double temp){
  double pression;
  pressure.getPressure(pression, temp);
  return pression;
}
