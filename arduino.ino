#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ThingSpeak.h>

WiFiClient client;

long myChannelNumber = 1619741;
const char myWriteAPIKey[] = "0X9SL9D9UHIOA4YI";

#define RE 2
#define DE 3

//const byte code[]= {0x01, 0x03, 0x00, 0x1e, 0x00, 0x03, 0x65, 0xCD};
const int all_command[]= {0x01, 0x03, 0x00, 0x00, 0x00, 0x09, 0x85, 0xCC};
const int moisture_command[]=    {0x01, 0x03, 0x00, 0x00, 0x00, 0x01, 0x84, 0x0A};
const int temperature_command[]= {0x01, 0x03, 0x00, 0x01, 0x00, 0x01, 0xD5, 0xCA};
const int ec_command[]=          {0x01, 0x03, 0x00, 0x02, 0x00, 0x01, 0x25, 0xCA};
const int ph_command[]=          {0x01, 0x03, 0x00, 0x03, 0x00, 0x01, 0x74, 0x0A};
const int n_command[]=           {0x01, 0x03, 0x00, 0x04, 0x00, 0x01, 0xC5, 0xCB};
const int p_command[]=           {0x01, 0x03, 0x00, 0x05, 0x00, 0x01, 0x94, 0x0B};
const int k_command[]=           {0x01, 0x03, 0x00, 0x06, 0x00, 0x01, 0x64, 0x0B};
int response[10];
float TEMPERATURE=0, MOISTURE=0, EC=0, NITROGEN=0, PHOSPHORUS=0, POTASSIUM=0;
int PH=0;
int count=0;

void setup()
{
  Serial.begin(4800);  //Serial port 1 for PC USB Connection of ESP32
  Serial2.begin(4800, SERIAL_8N1, 16, 17); // Serial port 2 for RS485 of Sensor
  pinMode(RE, OUTPUT); // if you use 4pin RS485 module or else you can uncomment this 
  pinMode(DE, OUTPUT); // if you use 4pin RS485 module or else you can uncomment this 
  delay(500);
  Wire.begin(D1, D2); /* join i2c bus with (Serial Data Pin)SDA=D1 and (Serial Clock Pin)SCL=D2 of NodeMCU */
  WiFi.begin("mafat nu","123456789");
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(200);
    Serial.print("..");
  }
  Serial.println();
  Serial.println("NodeMCU is connected!");
  Serial.println(WiFi.localIP());
  //dht.begin();
  ThingSpeak.begin(client);
}

void loop()
{
 
  read_moisture();
  read_temperature();
  read_ec();
  read_ph();
  read_nitrogen();
  read_phosphorus();
  read_potassium();
  
  Serial.print("Moisture:    ");
  Serial.print(MOISTURE);
  Serial.println("%");
  Serial.print("Temperature: ");
  Serial.print(TEMPERATURE);
  Serial.println("Deg");
  Serial.print("EC:          ");
  Serial.print((int)EC);
  Serial.println("");
  Serial.print("PH:          ");
  Serial.print(PH);
  Serial.println("");
  Serial.print("Nitrogen:    ");
  Serial.print((int)NITROGEN);
  Serial.println(" mg/kg");
  Serial.print("Phosphorus: ");
  Serial.print((int)PHOSPHORUS);
  Serial.println(" mg/kg");
  Serial.print("Potassium:   ");
  Serial.print((int)POTASSIUM);
  Serial.println(" mg/kg");
  Serial.println("\r\n-----------------------\r\n");
 
}

void read_moisture()
{
  int i=0, waittime=20, temp=0;
  digitalWrite(DE,HIGH);
  digitalWrite(RE,HIGH);
  delay(10);
  for (int i = 0; i < 8; i++) 
  {
    Serial2.write(moisture_command[i]);
  }
  digitalWrite(DE,LOW);
  digitalWrite(RE,LOW);
  i=0;
  while(waittime>0 && i<7)  
  {
   if(Serial2.available()>0)
     response[i++] = Serial2.read();
   delay(100);
   waittime--;
  }
  
  //for(int i=0; i<7; i++) 
    //Serial.write(response[i]);

  temp=response[3]<<8;  
  temp=temp|response[4];  
  MOISTURE=(float)temp/10; 
}

void read_temperature()
{
  int i=0, waittime=20, temp=0;
  digitalWrite(DE,HIGH);
  digitalWrite(RE,HIGH);
  delay(10);
  for (int i = 0; i < 8; i++) 
  {
    Serial2.write(temperature_command[i]);
  }
  digitalWrite(DE,LOW);
  digitalWrite(RE,LOW);
  i=0;
  while(waittime>0 && i<7)  
  {
   if(Serial2.available()>0)
     response[i++] = Serial2.read();
   delay(100);
   waittime--;
  }
  /*
  for(int i=0; i<7; i++) 
    Serial.write(response[i]);
  */
  temp=response[3]<<8;  
  temp=temp|response[4];
  TEMPERATURE=(float)temp/10;
}

void read_ec()
{
  int i=0, waittime=20, temp=0;
  digitalWrite(DE,HIGH);
  digitalWrite(RE,HIGH);
  delay(10);
  for (int i = 0; i < 8; i++) 
  {
    Serial2.write(ec_command[i]);
  }
  digitalWrite(DE,LOW);
  digitalWrite(RE,LOW);
  i=0;
  while(waittime>0 && i<7)  
  {
   if(Serial2.available()>0)
     response[i++] = Serial2.read();
   delay(100);
   waittime--;
  }
  
  //for(int i=0; i<7; i++) 
    //Serial.write(response[i]);
  temp=response[3]<<8;  
  temp=temp|response[4];
  EC=temp;  
}

void read_ph()
{
  int i=0, waittime=20, temp=0;
  digitalWrite(DE,HIGH);
  digitalWrite(RE,HIGH);
  delay(10);
  for (int i = 0; i < 8; i++) 
  {
    Serial2.write(ph_command[i]);
  }
  digitalWrite(DE,LOW);
  digitalWrite(RE,LOW);
  i=0;
  while(waittime>0 && i<7)  
  {
   if(Serial2.available()>0)
     response[i++] = Serial2.read();
   delay(100);
   waittime--;
  }
  
  //for(int i=0; i<7; i++) 
    //Serial.write(response[i]);
  
  temp=response[3]<<8;  
  PH=(temp|response[4])/10; 
}

void read_nitrogen()
{
  int i=0, waittime=20, temp=0;
  digitalWrite(DE,HIGH);
  digitalWrite(RE,HIGH);
  delay(10);
  for (int i = 0; i < 8; i++) 
  {
    Serial2.write(n_command[i]);
  }
  digitalWrite(DE,LOW);
  digitalWrite(RE,LOW);
  i=0;
  while(waittime>0 && i<7)  
  {
   if(Serial2.available()>0)
     response[i++] = Serial2.read();
   delay(100);
   waittime--;
  }
  
  //for(int i=0; i<7; i++) 
    //Serial.write(response[i]);
  
  temp=response[3]<<8;  
  temp=temp|response[4];
  NITROGEN=temp;
}


void read_phosphorus()
{
  int i=0, waittime=20, temp=0;
  digitalWrite(DE,HIGH);
  digitalWrite(RE,HIGH);
  delay(10);
  for (int i = 0; i < 8; i++) 
  {
    Serial2.write(p_command[i]);
  }
  digitalWrite(DE,LOW);
  digitalWrite(RE,LOW);
  i=0;
  while(waittime>0 && i<7)  
  {
   if(Serial2.available()>0)
     response[i++] = Serial2.read();
   delay(100);
   waittime--;
  }
  //for (int i = 0; i < 8; i++) 
    //Serial.write(response[i]);
  
  temp=response[3]<<8;  
  temp=temp|response[4];
  PHOSPHORUS=temp;
}


void read_potassium()
{
  int i=0, waittime=20, temp=0;
  digitalWrite(DE,HIGH);
  digitalWrite(RE,HIGH);
  delay(10);
  for (int i = 0; i < 8; i++) 
  {
    Serial2.write(k_command[i]);
  }
  digitalWrite(DE,LOW);
  digitalWrite(RE,LOW);
  i=0;
  while(waittime>0 && i<7)  
  {
   if(Serial2.available()>0)
     response[i++] = Serial2.read();
   delay(100);
   waittime--;
  }
  //for (int i = 0; i < 8; i++) 
    //Serial.write(response[i]);
  
  temp=response[3]<<8;  
  temp=temp|response[4];
  POTASSIUM=temp;
}
