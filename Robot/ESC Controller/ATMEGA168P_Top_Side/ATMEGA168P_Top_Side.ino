/**
 * @author Jiajer Ho, Alexander Vazquez, and Marcos De La Torre
 * 
 * ROV ESC Controller Top Side Code (Transmitter)  7/29/2018
 * Use ATMEGA168P
 * 
 * Takes data from Joystick and output TX data  
 * 
 */
 
#include "Servo.h"

/////////Port Numbers /////////
const int InputA0 = A0;
const int InputA1 = A1;
///////////////////////////////

///////Initialize values to 0//////
int PotA0 = 0;        // value read from the pot
int SpeedA0 = 0;      // relative speed from pot
int PotA1 = 0;        
int SpeedA1 = 0;
///////////////////////////////////

////////Get relative values for X and Y////////
int outputValueX = 0;
int outputValueY = 0;
///////////////////////////////////////////////

//////Make Values into Vectors//////
int XDirection = 0;
int YDirection = 0;

int SpeedY = 0;
int SpeedX = 0;
///////////////////////////////////

/////Format As Strings To Send Information//////

char sendsig[25];

String str1;
String str2;
String str3;

String strSpeedY;
String strSpeedX;

String strDirectX;
String strDirectY;

String ultimateStr;

const int unitOne = 1;

///////////////////////////////////////////////

//////Init psuedo-Motors//////
Servo f_Left;
Servo f_Right;
Servo b_Left;
Servo b_Right;
//////////////////////////////

/////Create a fake instance of Moving Forward/////
void goForward(int param) {
  int percent = abs(param / 10);
  int velocity = percent * 180;
  if(velocity > 150) {
    velocity = 150;
  }
  f_Left.write(velocity * -1);
  f_Right.write(velocity * -1);
  b_Left.write(velocity * -1);
  b_Right.write(velocity * -1);

//  Serial.println("Current Command: Move Forward");
}
//////////////////////////////////////////////////

/////Create a fake instance of Moving Backward/////
void goBackward(int param) {
  int percent = abs(param / 10);
  int velocity = percent * 180;
  if(velocity > 150) {
    velocity = 150;
  }
  f_Left.write(velocity);
  f_Right.write(velocity);
  b_Left.write(velocity);
  b_Right.write(velocity);

//  Serial.println("Current Command: Move Backward");
}
///////////////////////////////////////////////////

/////Create a fake instance of Moving Left/////
void goLeft(int param) {
  int percent = abs(param/10);
  int velocity = percent * 180;
  if(velocity > 150) {
    velocity = 150;
  }
  f_Left.write(velocity);
  f_Right.write(velocity * -1);
  b_Left.write(velocity * -1);
  b_Right.write(velocity);

// Serial.println("Current Command: Move Left");
}
///////////////////////////////////////////////

/////Create a fake instance of Moving Right/////
void goRight(int param) {
  int percent = abs(param/10);
  int velocity = percent * 180;
  if(velocity > 150) {
    velocity = 150;
  }
  f_Left.write(velocity * -1);
  f_Right.write(velocity);
  b_Left.write(velocity);
  b_Right.write(velocity * -1);

//  Serial.println("Current Command: Move Right");
}
///////////////////////////////////////////////

/////Create a fake instance of Not Moving/////
void noMove() {
  int velocity = 0;
  f_Left.write(velocity);
  f_Right.write(velocity);
  b_Left.write(velocity);
  b_Right.write(velocity );

//  Serial.println("Current Command: Don't Move");
}
//////////////////////////////////////////////

//////Begin and Specify each port for a motor///////
void setup() {
f_Left.attach(8);
f_Right.attach(9);
b_Left.attach(10);
b_Right.attach(11);
delay(500);
Serial.begin(9600);
}
///////////////////////////////////////////////////


void loop() {

////////Read Values from Pot//////////
  PotA0 = analogRead(InputA0);
  PotA1 = analogRead(InputA1);
  
  outputValueY = map(PotA0, 0, 1023, -10, 10);
  outputValueX = map(PotA1, 0, 1023, -10, 10);
  
  int x = outputValueX;
  int y = outputValueY; 
//////////////////////////////////////

/////////////////////Limit Movement to one action at a time///////////////////////////
  if( ((x <= -2 and x >= -9) and abs(x) != unitOne) and ((abs(y) >= x ) and y != 10)){
     goLeft(outputValueX);
     XDirection = -1;
     YDirection = 0;
}
  else if((x == -10) and (abs(y) <= 9)){
    goLeft(outputValueX);
    XDirection = -1;
    YDirection = 0;
}
       
  if( (x >= 2 and x <= 9) and (abs(y) <= x)){
    goRight(outputValueX);
    XDirection = 1;
    YDirection = 0;
  }
  else if((x == 10) and ( abs(y) <= 9)){
    goRight(outputValueX);
    XDirection = 1;
    YDirection = 0;
  }

 
  if( ((x >= -9 and x <= 9) and y != 1) and (y > abs(x))){
     goForward(outputValueY);
     YDirection = 1;
     XDirection = 0;
  }
  if( ( abs(x) == 10 ) and (y == 10 )){
     goForward(outputValueY);
     YDirection = 1;
     XDirection = 0;
  }
  else if((x == 0) and (y >= 2 and abs(y) != unitOne) ){
    goForward(outputValueY);
    YDirection = 1;
    XDirection = 0;
  }
   
  if( ((x >= -9 and x <= 9 ) and x != 1) and (abs(y) < abs(x) * -1 )){
     goBackward(outputValueY);
     YDirection = -1;
     XDirection = 0;
  }
  if( ( abs(x) == 10 ) and (y == -10 )){
     goBackward(outputValueY);
     YDirection = -1;
     XDirection = 0;
  }
  else if((x == 0) and (y <= -2 and abs(y) != unitOne) ){
    goBackward(outputValueY);
    YDirection = -1;
    XDirection = 0;
  }    

  if( (x < 2 and x > -2) and (y < 2 and y > -2) ){
    noMove();
    YDirection = 0;
    XDirection = 0; 
  }

//////////////////////////////////////////////////////////////////////////////

//////////Prepare and Format Strings////////////
  SpeedY = YDirection * outputValueY;
  SpeedX = XDirection * outputValueX;

  str1 = String(outputValueX);
  str2 = String(outputValueY); 
  str3 = String(str1+ " , " + str2);
  
  strDirectY = String(YDirection);
  strDirectX = String(XDirection);
//////////////////////////////////////////////// 

//Index Reading must be constant...Add symbols (ignore when reading//
  if(XDirection == 1){
    strDirectX = "+1";
  }
  if(YDirection == 1){
    strDirectY = "+1";
  }
  if(XDirection == -1){
    strDirectX = "-1";
  }
  if(YDirection == -1){
    strDirectY = "-1";
  }
/////////////////////////////////////////////////////////////////////

//Create Final String//
  strSpeedY = String(SpeedY);
  strSpeedX = String(SpeedX);

  ultimateStr = String(strDirectX + " ," + strDirectY + " ," + strSpeedX + " ," + strSpeedY + "\n");
//////////////////////

////////Trouble Shooting/////////

//  Serial.println("X and Y Coordinates: ");
//  Serial.println(str3);
//  
//  Serial.println("Speed along the Y axis: ");
//  Serial.println(strSpeedY);
//    
//  Serial.println("Speed along the X axis: ");
//  Serial.println(strSpeedX);
//
//  Serial.println("Direction along the Y axis: ");
//  Serial.println(strDirectY);
//  
//  Serial.println("Direction along the X axis: ");
//  Serial.println(strDirectX);
//  
 
//  strDirectY.toCharArray(sendsig, 3);
//  strDirectX.toCharArray(sendsig, 3);
//  strSpeedX.toCharArray(sendsig, 3);
//  strSpeedY.toCharArray(sendsig, 3);

///////////////////////////////

/////Send Signal/////
  ultimateStr.toCharArray(sendsig, 25);
  Serial.write(sendsig, 25); //Write the serial data
/////////////////////

  delay(100);


  

}