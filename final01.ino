#include <millisDelay.h>

#define ps2 3
#define ps3 47
#define pout 46

#define ps1_2 10
#define ps1_3 41
#define ps1_pout 44 

const int farba = 400;
const int L_M_minus = 52; //lavy motor -
const int L_M_plus = 53; //lavy motor +
const int P_M_plus = 50; //pravy motor +
const int P_M_minus = 51; //pravy motor -
const int control_motor_L = 8; //analog lavy motor 
const int control_motor_P = 2; //analog pravy motor
const int pTrig = 39; //ultrazvuk
const int pEcho = 7;  //ultrazvuk

int red = 0;  
int green = 0;  
int blue = 0;
int sucet_farba = 0; 
int sucet_farba_2 = 0;
int cas = 0; 

long odozva,vzdialenost;


millisDelay casOtacania;


void setup() {
   Serial.begin(9600);

  pinMode(pTrig, OUTPUT);
  pinMode(pEcho, INPUT);
       
   pinMode(ps2, OUTPUT);  
   pinMode(ps3, OUTPUT);  
   pinMode(pout, INPUT); 

   pinMode (L_M_minus, OUTPUT);
   pinMode (L_M_plus, OUTPUT);
   pinMode (P_M_plus, OUTPUT);
   pinMode (P_M_minus, OUTPUT);
   pinMode (control_motor_L, OUTPUT);
   pinMode (control_motor_P, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  citaj_vzdialenost();
  //Serial.print(vzdialenost);
  if(vzdialenost > 5){
    get_color();
    get_color_2();
    if((sucet_farba>farba)&&(sucet_farba_2>farba)){
      doPredu();
    }
    if(sucet_farba<farba+1){
      doLava();
    }
    if(sucet_farba_2<farba+1){
      doPrava();  
    } 
  }
  else{
    zastav();
   }
  
}
/*
void hladajCestu(){
  bool nasiel = false;
  while(nasiel == false){
     nasiel = doPrava();
     if(nasiel == false){
        nasiel = doLava(); 
     }
  }  
 }
 */

void citaj_vzdialenost(){
    digitalWrite(pTrig, LOW);
    delayMicroseconds(2);
    digitalWrite(pTrig, HIGH);
    delayMicroseconds(5);
    digitalWrite(pTrig, LOW);
    odozva = pulseIn(pEcho, HIGH);
    vzdialenost = odozva / 58.31;
  
 }

 void printSerial(){
   Serial.print(sucet_farba);
   Serial.print("#");
   Serial.print(sucet_farba_2);
   Serial.print("#");
   Serial.print(vzdialenost);
   Serial.println("");
 }
 
void zastav(){
  analogWrite(control_motor_L, 0);
  analogWrite(control_motor_P, 0);
  digitalWrite(L_M_plus, LOW);
  digitalWrite(L_M_minus, LOW); 
  digitalWrite(P_M_plus, LOW);
  digitalWrite(P_M_minus, LOW); 
 }

void doPredu(){
  printSerial();
  analogWrite(control_motor_L, 100);
  analogWrite(control_motor_P, 100);
  digitalWrite(L_M_plus, HIGH);
  digitalWrite(L_M_minus, LOW); 
  digitalWrite(P_M_plus, HIGH);
  digitalWrite(P_M_minus, LOW);
}

void doPrava(){
  //casOtacania.start(1000);
  while(sucet_farba_2<farba+1){
     printSerial();
     analogWrite(control_motor_L, 150);
     analogWrite(control_motor_P, 150);
     digitalWrite(L_M_plus, HIGH);
     digitalWrite(L_M_minus, LOW);
     digitalWrite(P_M_plus, LOW);
     digitalWrite(P_M_minus, LOW); 
     
    // Serial.println(casOtacania.remaining());
     citaj_vzdialenost();
     get_color_2();
     get_color(); 
     //Serial.print(sucet_farba_2);
     //Serial.println("doprava");
     
   }
}

void doLava(){
   //casOtacania.start(1000);
   while(sucet_farba<farba+1){

     printSerial(); 
     analogWrite(control_motor_L, 150);
     analogWrite(control_motor_P, 150);
     digitalWrite(L_M_plus,  LOW);
     digitalWrite(L_M_minus, LOW);
     digitalWrite(P_M_plus, HIGH);
     digitalWrite(P_M_minus, LOW);
     //Serial.print(sucet_farba);
     //Serial.println("dolava");
     citaj_vzdialenost();
     get_color_2();
     get_color();
     //Serial.println(casOtacania.remaining());
     
   }
}

void get_color()  
{    
  // S2 S3  Filter
  //  0  0  red
  //  0  1  blue
  //  1  1  green
  
  digitalWrite(ps2, LOW);      // set filter to red
  digitalWrite(ps3, LOW);      // set filter to red
  while(digitalRead(pout)){};  // wait for output to go LOW
  red = pulseIn(pout, LOW);    // get the value for red
  
  digitalWrite(ps3, HIGH);     // set filter to blue
  while(digitalRead(pout)){};  // wait for output to go LOW
  blue = pulseIn(pout, LOW);   // get the value for blue
  
  digitalWrite(ps2, HIGH);     // set filter to green
  while(digitalRead(pout)){};  // wait for output to go LOW
  green = pulseIn(pout, LOW);  // get the value for green

  
  red=constrain(red,192,5200);     // limit the values of r,g, and b
  green=constrain(green,202,5200);
  blue=constrain(blue,145,3800);
   
  red   = map(red,192,5200,255,0);   // remap the values so they fit into an 8 bit scale (0..255)
  green = map(green,202,5200,255,0);
  blue  = map(blue,145,3800,255,0);

  sucet_farba = 0;
  sucet_farba = red + green + blue;
  
}

void get_color_2()  
{    
  // S2 S3  Filter
  //  0  0  red
  //  0  1  blue
  //  1  1  green
  
  digitalWrite(ps1_2, LOW);      // set filter to red
  digitalWrite(ps1_3, LOW);      // set filter to red
  while(digitalRead(pout)){};  // wait for output to go LOW
  red = pulseIn(pout, LOW);    // get the value for red
  
  digitalWrite(ps1_3, HIGH);     // set filter to blue
  while(digitalRead(ps1_pout)){};  // wait for output to go LOW
  blue = pulseIn(ps1_pout, LOW);   // get the value for blue
  
  digitalWrite(ps1_2, HIGH);     // set filter to green
  while(digitalRead(ps1_pout)){};  // wait for output to go LOW
  green = pulseIn(ps1_pout, LOW);  // get the value for green

  
  red=constrain(red,192,5200);     // limit the values of r,g, and b
  green=constrain(green,202,5200);
  blue=constrain(blue,145,3800);
   
  red   = map(red,192,5200,255,0);   // remap the values so they fit into an 8 bit scale (0..255)
  green = map(green,202,5200,255,0);
  blue  = map(blue,145,3800,255,0);

  sucet_farba_2 = 0;
  sucet_farba_2 = red + green + blue;
  
}
