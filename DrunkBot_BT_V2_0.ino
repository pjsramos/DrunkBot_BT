
#include <Servo.h> 
#include <MeetAndroid.h>

MeetAndroid meetAndroid;

Servo myservoR;  // create servo object to control a servo 
Servo myservoL;

#define RIGHT_SERVO_PIN 10
#define LEFT_SERVO_PIN 9

#define RED_LED_PIN 5
#define GREEN_LED_PIN 6
#define BLUE_LED_PIN 11

#define RIGHT_ZERO 88
#define LEFT_ZERO 87

#define PWM_MIN 1
#define PWM_MAX 20

#define COMP_MIN 1.5
#define COMP_MAX 9.0

#define X 1
#define Y 0
#define Z 2


long timeout;
float eixos[3];


void setup() 
{ 
   Serial.begin(9600);
  myservoR.attach(RIGHT_SERVO_PIN);  // attaches the servo on pin 9 to the servo object 
  myservoL.attach(LEFT_SERVO_PIN);

  stopM();

  meetAndroid.registerFunction(compass, 'A');

  meetAndroid.registerFunction(phoneState, 'B');
  
  
    // set all color leds as output pins
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(BLUE_LED_PIN, OUTPUT);
  
  // just set all leds to high so that we see they are working well
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(BLUE_LED_PIN, LOW);
  delay(300);

  front(10);
  delay(300);
  right(10);
  delay(300);
  left(10);
  delay(300);
  back(10);
  delay(300);
  
  digitalWrite(RED_LED_PIN, HIGH);
  digitalWrite(GREEN_LED_PIN, HIGH);
  digitalWrite(BLUE_LED_PIN, HIGH);

} 


void loop() 
{ 

  //readChar();

  //delay(100);

  meetAndroid.receive();

  run();

  delay(20);
} 


void compass(byte flag, byte numOfValues)
{

  timeout = millis();

  meetAndroid.getFloatValues(eixos);

  //  meetAndroid.send("X:");
  //  meetAndroid.send(eixos[X]);
  //  meetAndroid.send("Y:");
  //  meetAndroid.send(eixos[Y]);
  //  meetAndroid.send("Z:");
  //  meetAndroid.send(eixos[Z]);
}



  if(millis() - timeout > 2000) {
    stopM();
  } 
  else {

    int velY = convertToVelocity(Y);
    int velX = convertToVelocity(X);
/*
    meetAndroid.send("Conv X:");
    meetAndroid.send(velX);
    meetAndroid.send("Conv Y:");
    meetAndroid.send(velY);
*/
    //Est\u00e1 indo pra frente?
    if(eixos[X] >= COMP_MIN) {
      //Est\u00e1 indo para um dos lados tamb\u00e9m?
      if (eixos[Y] >= COMP_MIN) {
        meetAndroid.send("back-left");
        //frenteDireita(intensidadeX);
        //frenteEsquerda(intensidadeXY());
      } 
      else if(eixos[Y] < -COMP_MIN) {
        meetAndroid.send("back-right");
        //frenteDireita(intensidadeXY());
        //frenteEsquerda(intensidadeX);
      } 
      else {
        meetAndroid.send("back");
        back(velX);
        //frenteDireita(intensidadeX);
        //frenteEsquerda(intensidadeX);
      }
    } 
    else if(eixos[X] < -COMP_MIN) {
      if (eixos[Y] >= COMP_MIN) {
        meetAndroid.send("forward-left");
        //frenteDireita(intensidadeX);
        //frenteEsquerda(intensidadeXY());
      } 
      else if(eixos[Y] < -COMP_MIN) {
        meetAndroid.send("forward-right");
        //frenteDireita(intensidadeXY());
        //frenteEsquerda(intensidadeX);
      } 
      else {
        meetAndroid.send("forward");
        front(velX);
        //frenteDireita(intensidadeX);
        //frenteEsquerda(intensidadeX);
      }
    } 
    else {
      if (eixos[Y] >= COMP_MIN) {
        meetAndroid.send("left");
        left(velY);
        //frenteDireita(intensidadeX);
        //frenteEsquerda(intensidadeXY());
      } 
      else if(eixos[Y] < -COMP_MIN) {
        meetAndroid.send("right");
        right(velY);
        //frenteDireita(intensidadeXY());
        //frenteEsquerda(intensidadeX);
      }     
      else {
        stopM();
      }
    }

  }

}


int convertToVelocity(int eixo) {
  int vel = map(abs(eixos[eixo]), COMP_MIN, COMP_MAX, PWM_MIN, PWM_MAX);
  
  if(vel > PWM_MAX) {
    vel = PWM_MAX;
  } 
  else if(vel < PWM_MIN) {
    vel = 0;
  }
  return vel;
}

void front(int vel) {
  meetAndroid.send("-->");
  meetAndroid.send((LEFT_ZERO+vel));
  meetAndroid.send((RIGHT_ZERO-vel));
  
  myservoL.write(LEFT_ZERO+vel);
  myservoR.write(RIGHT_ZERO-vel);
}

void back(int vel) {
  meetAndroid.send("-->");
  meetAndroid.send((LEFT_ZERO-vel));
  meetAndroid.send((RIGHT_ZERO+vel));
  
  myservoL.write(LEFT_ZERO-vel);
  myservoR.write(RIGHT_ZERO+vel);
}


void stopM() {
  myservoR.write(RIGHT_ZERO);
  myservoL.write(LEFT_ZERO);
}


void left(int vel) {
  meetAndroid.send("-->");
  meetAndroid.send((LEFT_ZERO+vel));
  meetAndroid.send((RIGHT_ZERO-vel));
  
  myservoL.write(LEFT_ZERO-(vel/2));
  myservoR.write(RIGHT_ZERO-(vel/2));
}

void right(int vel) {
  meetAndroid.send("-->");
  meetAndroid.send((LEFT_ZERO+vel));
  meetAndroid.send((RIGHT_ZERO-vel));
  
  myservoL.write(LEFT_ZERO+(vel/2));
  myservoR.write(RIGHT_ZERO+(vel/2));
}


void phoneState(byte flag, byte numOfValues)
{
  int val = meetAndroid.getInt();
  if (val==0) {
     meetAndroid.send("IDLE");
    analogWrite(RED_LED_PIN, 255 );
    analogWrite(GREEN_LED_PIN, 255);
    analogWrite(BLUE_LED_PIN, 100);
  } else if (val==1){
     meetAndroid.send("RINGING");
    analogWrite(RED_LED_PIN, 100 );
    analogWrite(GREEN_LED_PIN, 255);
    analogWrite(BLUE_LED_PIN, 255);
  } else if (val==2){
     meetAndroid.send("OFFHOOK");
    analogWrite(RED_LED_PIN, 255 );
    analogWrite(GREEN_LED_PIN, 100);
    analogWrite(BLUE_LED_PIN, 255);
  } else {
     meetAndroid.send("OFF");
    analogWrite(RED_LED_PIN, 255 );
    analogWrite(GREEN_LED_PIN, 255);
    analogWrite(BLUE_LED_PIN, 255);
  }
}







