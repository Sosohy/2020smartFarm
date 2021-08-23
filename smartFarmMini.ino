#include <dht.h>
#include <Servo.h> 
#include <SoftwareSerial.h>

#define LED 7
#define DHT11_PIN 8
#define BIA 9
#define BIB 10
#define MY_SERVO 11

dht DHT;
int CDS = 0;
int value=0;
String thisString;

Servo myservo;
SoftwareSerial mySerial(2,3);

void setup(){
  pinMode(LED, OUTPUT);
  pinMode(BIA, OUTPUT);  
  pinMode(BIB, OUTPUT); 
  myservo.attach(MY_SERVO);
  myservo.write(90); 
  mySerial.begin(9600);
}

void loop() {
  //습도&조도
  int chk = DHT.read11(DHT11_PIN);
  value = analogRead(CDS);
  value = map(value,0,1023,0,100);

  sendData(DHT.humidity, (byte)value);

 if(mySerial.available()){
    thisString=mySerial.readString();

   //물주기
   if(thisString=="water") 
    {
      digitalWrite(BIA, LOW);  
      digitalWrite(BIB, HIGH);  
      delay(3000);
      digitalWrite(BIB, LOW);     
    }
   
   //뚜껑 열기
   if(thisString=="open"){    
       myservo.write(180);
    }
    else if(thisString=="close")
        myservo.write(0); 

   //무드등
   if(thisString=="LightU")
     digitalWrite(LED,HIGH);
   else if(thisString=="LightD")
     digitalWrite(LED,LOW);   
   }   
  delay(1000);
}

void sendData(byte humidity, byte sunlight)
{
    byte data[2] = {0,};
 
    data[0] = humidity;
    data[1] = sunlight;
    
    mySerial.write(data, 2);
}
