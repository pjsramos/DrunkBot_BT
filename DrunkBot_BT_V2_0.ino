
#include <Servo.h> 
#include <MeetAndroid.h>

MeetAndroid meetAndroid;

Servo myservoR;  // create servo object to control a servo 
Servo myservoL;
// a maximum of eight servo objects can be created 
#define RIGHT_ZERO 88
#define LEFT_ZERO 87

#define PWM_MIN 0
#define PWM_MAX 180

#define COMP_MIN 3.0
#define COMP_MAX 10.0

#define X 0
#define Y 1
#define Z 2


long timeout;
float eixos[3];

int velR = RIGHT_ZERO;
int velL = LEFT_ZERO;
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
  
  andar();
  
  delay(2000);
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
  
  
void andar() {
  
  if(millis() - timeout > 2000) {
    stopM();
  } else {
    
    int intensidadeY = convertToVelocity(Y);
    int intensidadeX = convertToVelocity(X);
    
    meetAndroid.send("Conv X:");
    meetAndroid.send(intensidadeX);
    meetAndroid.send("Conv Y:");
    meetAndroid.send(intensidadeY);
    
    
    if(eixos[X] >= MIN) {
      //Está indo para um dos lados também?
      if (eixos[Y] >= MIN) {
        amarino.send("frente-esquerda");
        //frenteDireita(intensidadeX);
        //frenteEsquerda(intensidadeXY());
      } else if(eixos[Y] < -MIN) {
        amarino.send("frente-direita");
        //frenteDireita(intensidadeXY());
        //frenteEsquerda(intensidadeX);
      } else {
        amarino.send("frente");
        //frenteDireita(intensidadeX);
        //frenteEsquerda(intensidadeX);
      }
    } else if(eixos[X] < -MIN) {
      amarino.send("tras");
      //trasDireita(intensidadeX);
      //trasEsquerda(intensidadeX);
    } else if (eixos[Y] < -MIN) {
      amarino.send("esquerda");
      //frenteDireita(0);
      //frenteEsquerda(intensidadeY);
    } else if(eixos[Y] >= MIN) {
      amarino.send("direita");
      //frenteDireita(intensidadeY);
      //frenteEsquerda(0);
    } else {
      amarino.send("parar");
      //parar();
    }
  }
  
}



int convertToVelocity(int eixo) {
  int intensidade = map(abs(eixos[eixo]), COMP_MIN, COMP_MAX, PWM_MIN, PWM_MAX);
  if(intensidade > PWM_MAX) {
    intensidade = PWM_MAX;
  } else if(intensidade < PWM_MIN) {
    intensidade = 0;
  }
  return intensidade;
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
  velR=RIGHT_ZERO;
  velL=LEFT_ZERO;
  if (useSerial) {
    Serial.print("Stop=");
    Serial.print(velR);
    Serial.print("/");
    Serial.println(velL);
  }
  myservoR.write(velR);
  myservoL.write(velL);
}



