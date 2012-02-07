
#include <Servo.h> 
#include <MeetAndroid.h>

MeetAndroid meetAndroid;

Servo myservoR;  // create servo object to control a servo 
Servo myservoL;
// a maximum of eight servo objects can be created 
#define rightZero 88
#define leftZero 87

int velR = rightZero;
int velL = leftZero;
int charrec=0;

boolean useSerial=false;
boolean isDance=false;


void setup() 
{ 
  Serial.begin(9600);
  myservoR.attach(10);  // attaches the servo on pin 9 to the servo object 
  myservoL.attach(9);

  myservoR.write(velR);
  myservoL.write(velL);
  
  meetAndroid.registerFunction(compass, 'A');
  delay(300);
  //useSerial=Serial.available();

} 


void loop() 
{ 

  //readChar();

  //delay(100);

  meetAndroid.receive();
} 


void compass(byte flag, byte numOfValues)
{
  // we use getInt(), since we know only data between 0 and 360 will be sent
 float data[numOfValues];
  meetAndroid.getFloatValues(data);
  //meetAndroid.send("arduino:");
  //meetAndroid.send(data[0]);
  //meetAndroid.send(data[1]);
  //meetAndroid.send(data[2]);
  
  
  
   
  if(data[0] > 2) {
    meetAndroid.send("<--|-- ");
    int valDir = map((int)data[0], 2, 12, 0, 40);
    
    myservoR.write(rightZero-valDir);
    myservoL.write(leftZero-valDir);
    
    meetAndroid.send(rightZero-valDir);
    meetAndroid.send(leftZero-valDir);

  } else if(data[0]  < -2) {
      meetAndroid.send(" --|-->");
        int valDir = map((int)data[0], -12, -2, 0, 40);
    
    myservoR.write(rightZero+valDir);
    myservoL.write(leftZero+valDir);
    
    meetAndroid.send(rightZero+valDir);
    meetAndroid.send(leftZero+valDir);
  } else {
    meetAndroid.send(" --|-- ");
    myservoR.write(rightZero);
    myservoL.write(leftZero);
    
    meetAndroid.send(rightZero);
    meetAndroid.send(leftZero);
  }

  /*
    if(data[1] > 5) {
        meetAndroid.send("Front");
    front();
  } else if(data[1] < -5) {
      meetAndroid.send("Back");
    back();
  } else {
      meetAndroid.send("Stop");
    stopM();
  }
  */
  
}

void readChar() {
  if (Serial.available()) {
    // wait a bit for the entire message to arrive
    //delay(100);
    // clear the screen

    while (Serial.available() > 0) {
      // display each character to the LCD
      charrec=Serial.read();
      if(charrec==97) {
        front();
      } 
      else if(charrec==100) {
        back();
      } 
      else if(charrec==119) {
        left();
      } 
      else if(charrec==115) {
        right();
      } 
      else if(charrec==32) {
        stopM();
      } 
      else {
        Serial.print("char=");
        Serial.println(charrec);
      }
    }
  }
}



void dance() {
  stopM();
  delay(5000);
  front();
  delay(5000);
  front();
  delay(5000);
  front();
  delay(5000);
  stopM();
  delay(5000);
  right();
  right();
  delay(5000);
  stopM();
  delay(5000);
  left();
  left();
  delay(5000);
  stopM();
  back();
  delay(5000);
  back();
  delay(5000);
  stopM();
  delay(5000);
}

void front() {
  velR=velR-1;
  velL=velL+1;
  if (useSerial) {
    Serial.print("Front=");
    Serial.print(velR);
    Serial.print("/");
    Serial.println(velL);
  }
  myservoR.write(velR);
  myservoL.write(velL);
}

void back() {
  velR=velR+1;
  velL=velL-1;
  if (useSerial) {
    Serial.print("Back=");
    Serial.print(velR);
    Serial.print("/");
    Serial.println(velL);
  }
  myservoR.write(velR);
  myservoL.write(velL);
}

void right() {
  velR=velR+1;
  velL=velL+1;
  if (useSerial) {
    Serial.print("Right=");
    Serial.print(velR);
    Serial.print("/");
    Serial.println(velL);
  }
  myservoR.write(velR);
  myservoL.write(velL);
}

void left() {
  velR=velR-1;
  velL=velL-1;
  if (useSerial) {
    Serial.print("Left=");
    Serial.print(velR);
    Serial.print("/");
    Serial.println(velL);
  }
  myservoR.write(velR);
  myservoL.write(velL);
}

void stopM() {
  velR=rightZero;
  velL=leftZero;
  if (useSerial) {
    Serial.print("Stop=");
    Serial.print(velR);
    Serial.print("/");
    Serial.println(velL);
  }
  myservoR.write(velR);
  myservoL.write(velL);
}



