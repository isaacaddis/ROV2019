#include "Servo.h"
#include <Wire.h>


//Gyro Variables
float elapsedTime, time, timePrev;        //Variables for time control
int gyro_error=0;                         //We use this variable to only calculate once the gyro data error
float Gyr_rawX, Gyr_rawY, Gyr_rawZ;     //Here we store the raw data read 
float Gyro_angle_x, Gyro_angle_y;         //Here we store the angle value obtained with Gyro data
float Gyro_raw_error_x, Gyro_raw_error_y; //Here we store the initial gyro data error

//Acc Variables
int acc_error=0;                         //We use this variable to only calculate once the Acc data error
float rad_to_deg = 180/3.141592654;      //This value is for pasing from radians to degrees values
float Acc_rawX, Acc_rawY, Acc_rawZ;    //Here we store the raw data read 
float Acc_angle_x, Acc_angle_y;          //Here we store the angle value obtained with Acc data
float Acc_angle_error_x, Acc_angle_error_y; //Here we store the initial Acc data error

float Total_angle_x, Total_angle_y;
String Gyro, gyro_X, gyro_Y;
char gyro[20];

 
char joy[25];
  
String  FBSpeed;
String SideSpeed;
String VertSpeed; 
String TurnSpeed; 
String TiltSpeed; 

int  fbSpeed;
int sideSpeed;
int vertSpeed; 
int turnSpeed; 
int tiltSpeed; 


String Joystick;
String message; 

Servo OutputM1;
Servo OutputM2;
Servo OutputM3;
Servo OutputM4;
Servo OutputM5;
Servo OutputM6;


int speed01;
int speed02;

int speed03;
int speed04;


int speed05;
int speed06;

int speed07;
int speed08;


int speed09;
int speed10;



String speedOne;
String speedTwo;

String speedThree;
String speedFour;


String speedFive;
String speedSix;

String speedSeven;
String speedEight;


String speedNine;
String speedTen;



void setup() {
  Serial.begin(57600);
  Serial1.begin(57600);
  pinMode(LED_BUILTIN, OUTPUT);

  OutputM1.attach(10);
  OutputM2.attach(5);
  OutputM3.attach(3);
  OutputM4.attach(4);
  OutputM5.attach(11);
  OutputM6.attach(12);
  
  OutputM1.write(90);
  OutputM2.write(90);
  OutputM3.write(90);
  OutputM4.write(90);  
  OutputM5.write(90);
  OutputM6.write(90);

    speed01 = 90;
    speed02 = 90;
    speed03 = 90;
    speed04 = 90;
    speed05 = 90;
    speed06 = 90;
    speed07 = 90;
    speed08 = 90;
    speed09 = 90;
    speed10 = 90;
  

  // put your setup code here, to run once:
 Wire.begin();                           //begin the wire comunication
  
  Wire.beginTransmission(0x68);           //begin, Send the slave adress (in this case 68)              
  Wire.write(0x6B);                       //make the reset (place a 0 into the 6B register)
  Wire.write(0x00);
  Wire.endTransmission(true);             //end the transmission
  //Gyro config
  Wire.beginTransmission(0x68);           //begin, Send the slave adress (in this case 68) 
  Wire.write(0x1B);                       //We want to write to the GYRO_CONFIG register (1B hex)
  Wire.write(0x10);                       //Set the register bits as 00010000 (1000dps full scale)
  Wire.endTransmission(true);             //End the transmission with the gyro
  //Acc config
  Wire.beginTransmission(0x68);           //Start communication with the address found during search.
  Wire.write(0x1C);                       //We want to write to the ACCEL_CONFIG register
  Wire.write(0x10);                       //Set the register bits as 00010000 (+/- 8g full scale range)
  Wire.endTransmission(true); 
  delay(1000);
    time = millis();
      if(acc_error==0)
  {
    for(int a=0; a<200; a++)
    {
      Wire.beginTransmission(0x68);
      Wire.write(0x3B);                       //Ask for the 0x3B register- correspond to AcX
      Wire.endTransmission(false);
      Wire.requestFrom(0x68,6,true); 
      
      Acc_rawX=(Wire.read()<<8|Wire.read())/4096.0 ; //each value needs two registres
      Acc_rawY=(Wire.read()<<8|Wire.read())/4096.0 ;
      Acc_rawZ=(Wire.read()<<8|Wire.read())/4096.0 ;

      
      /*---X---*/
      Acc_angle_error_x = Acc_angle_error_x + ((atan((Acc_rawY)/sqrt(pow((Acc_rawX),2) + pow((Acc_rawZ),2)))*rad_to_deg));
      /*---Y---*/
      Acc_angle_error_y = Acc_angle_error_y + ((atan(-1*(Acc_rawX)/sqrt(pow((Acc_rawY),2) + pow((Acc_rawZ),2)))*rad_to_deg)); 
      
      if(a==199)
      {
        Acc_angle_error_x = Acc_angle_error_x/200;
        Acc_angle_error_y = Acc_angle_error_y/200;
        acc_error=1;
      }
    }
  }//end of acc error calculation   


/*Here we calculate the gyro data error before we start the loop
 * I make the mean of 200 values, that should be enough*/
  if(gyro_error==0)
  {
    for(int i=0; i<200; i++)
    {
      Wire.beginTransmission(0x68);            //begin, Send the slave adress (in this case 68) 
      Wire.write(0x43);                        //First adress of the Gyro data
      Wire.endTransmission(false);
      Wire.requestFrom(0x68,4,true);           //We ask for just 4 registers 
         
      Gyr_rawX=Wire.read()<<8|Wire.read();     //Once again we shif and sum
      Gyr_rawY=Wire.read()<<8|Wire.read();
   
      /*---X---*/
      Gyro_raw_error_x = Gyro_raw_error_x + (Gyr_rawX/32.8); 
      /*---Y---*/
      Gyro_raw_error_y = Gyro_raw_error_y + (Gyr_rawY/32.8);
      if(i==199)
      {
        Gyro_raw_error_x = Gyro_raw_error_x/200;
        Gyro_raw_error_y = Gyro_raw_error_y/200;
        gyro_error=1;
      }
    }
  }//end of gyro error calculation   
}

void loop() {
   timePrev = time;                        // the previous time is stored before the actual time read
  time = millis();                        // actual time read
  elapsedTime = (time - timePrev) / 1000; //divide by 1000 in order to obtain seconds

  //////////////////////////////////////Gyro read/////////////////////////////////////

    Wire.beginTransmission(0x68);            //begin, Send the slave adress (in this case 68) 
    Wire.write(0x43);                        //First adress of the Gyro data
    Wire.endTransmission(false);
    Wire.requestFrom(0x68,4,true);           //We ask for just 4 registers
        
    Gyr_rawX=Wire.read()<<8|Wire.read();     //Once again we shif and sum
    Gyr_rawY=Wire.read()<<8|Wire.read();
    /*Now in order to obtain the gyro data in degrees/seconds we have to divide first
    the raw value by 32.8 because that's the value that the datasheet gives us for a 1000dps range*/
    /*---X---*/
    Gyr_rawX = (Gyr_rawX/32.8) - Gyro_raw_error_x; 
    /*---Y---*/
    Gyr_rawY = (Gyr_rawY/32.8) - Gyro_raw_error_y;
    
    /*Now we integrate the raw value in degrees per seconds in order to obtain the angle
    * If you multiply degrees/seconds by seconds you obtain degrees */
    /*---X---*/
    Gyro_angle_x = Gyr_rawX*elapsedTime;
    /*---X---*/
    Gyro_angle_y = Gyr_rawY*elapsedTime;


    
  
  //////////////////////////////////////Acc read/////////////////////////////////////

  Wire.beginTransmission(0x68);     //begin, Send the slave adress (in this case 68) 
  Wire.write(0x3B);                 //Ask for the 0x3B register- correspond to AcX
  Wire.endTransmission(false);      //keep the transmission and next
  Wire.requestFrom(0x68,6,true);    //We ask for next 6 registers starting withj the 3B  
  /*We have asked for the 0x3B register. The IMU will send a brust of register.
  * The amount of register to read is specify in the requestFrom function.
  * In this case we request 6 registers. Each value of acceleration is made out of
  * two 8bits registers, low values and high values. For that we request the 6 of them  
  * and just make then sum of each pair. For that we shift to the left the high values 
  * register (<<) and make an or (|) operation to add the low values.
  If we read the datasheet, for a range of+-8g, we have to divide the raw values by 4096*/    
  Acc_rawX=(Wire.read()<<8|Wire.read())/4096.0 ; //each value needs two registres
  Acc_rawY=(Wire.read()<<8|Wire.read())/4096.0 ;
  Acc_rawZ=(Wire.read()<<8|Wire.read())/4096.0 ; 
 /*Now in order to obtain the Acc angles we use euler formula with acceleration values
 after that we substract the error value found before*/  
 /*---X---*/
 Acc_angle_x = (atan((Acc_rawY)/sqrt(pow((Acc_rawX),2) + pow((Acc_rawZ),2)))*rad_to_deg) - Acc_angle_error_x;
 /*---Y---*/
 Acc_angle_y = (atan(-1*(Acc_rawX)/sqrt(pow((Acc_rawY),2) + pow((Acc_rawZ),2)))*rad_to_deg) - Acc_angle_error_y;    


 //////////////////////////////////////Total angle and filter////U/////////////////////////////////
 /*---X axis angle---*/
 Total_angle_x = 0.98 *(Total_angle_x + Gyro_angle_x) + 0.02*Acc_angle_x;
 /*---Y axis angle---*/
 Total_angle_y = 0.98 *(Total_angle_y + Gyro_angle_y) + 0.02*Acc_angle_y;


  Joystick = String(joy);
  //Serial.println(Joystick);
 // Serial.println(Joystick);
  //digitalWrite(LED_BUILTIN, HIGH);
  
  SideSpeed = Joystick.substring((Joystick.indexOf("R") + 1), Joystick.indexOf("F"));
  FBSpeed = Joystick.substring((Joystick.indexOf("F") + 1), Joystick.indexOf("U"));
  VertSpeed = Joystick.substring((Joystick.indexOf("U") + 1), Joystick.indexOf("T"));
  TurnSpeed = Joystick.substring((Joystick.indexOf("T") + 1), Joystick.indexOf("P"));
  TiltSpeed = Joystick.substring((Joystick.indexOf("P") + 1), Joystick.length());

    fbSpeed = FBSpeed.toInt();
  sideSpeed = SideSpeed.toInt();
  vertSpeed = VertSpeed.toInt();
  turnSpeed = TurnSpeed.toInt();
  tiltSpeed = TiltSpeed.toInt() ;


 //  Serial.println(fbSpeed);
 //  Serial.println(sideSpeed);
 //  Serial.println(vertSpeed);
 //  Serial.println(turnSpeed);
 //  Serial.println(tiltSpeed);


   if (vertSpeed > 0) { //Up    
    speed08= map( vertSpeed, 0, 25, 90, 25);
    OutputM5.write(speed08);
    OutputM6.write(speed08);
//    speedEight = String(speed08);
//    Serial2.println("Up"+ speedEight);
  }
        
  if (vertSpeed < 0) { //Down 
    vertSpeed = abs(vertSpeed);
    speed10 = map(vertSpeed,0, 25, 90, 163);
    OutputM5.write(speed10);
    OutputM6.write(speed10);
//    speedTen = String(speed10);
//    Serial2.println("Vertical"+ speedTen);
    
  }

   if (tiltSpeed > 0) { //Tilt Up
    speed05 = map( tiltSpeed, 0, 25, 90, 163);
    speed06 = map( tiltSpeed, 0, 25, 25, 90);
    OutputM5.write(speed06);
    OutputM6.write(speed05);
//    speedSix = String(speed06);
//    speedFive = String(speed05);
//    Serial2.println("Tilt Up" +speedSix);
//    Serial2.println("Tilt Up" + speedFive);
  }
        
  if (tiltSpeed < 0) { //Tilt Down
    tiltSpeed = abs(tiltSpeed);
    speed05 = map( tiltSpeed, 0, 25, 90, 163);
    speed06 = map( tiltSpeed, 0, 25, 25, 90);
    OutputM5.write(speed05);
    OutputM6.write(speed06);
//    speedSix = String(speed06);
//    speedFive = String(speed05);
//    Serial2.println("Tilt Down" +speedSix);
//    Serial2.println("Tilt Down" + speedFive);
  }



   if (fbSpeed > 0) { //Forwards
    speed07 = map(fbSpeed, 0, 25, 90, 25);
    OutputM1.write(speed07);
    OutputM2.write(speed07);
    OutputM3.write(speed07);
    OutputM4.write(speed07);
//    speedSeven = String(speed07);
//    Serial2.println("Forward"+speedSeven);
  }
  if (fbSpeed < 0) { //Backwards
    fbSpeed = abs(fbSpeed);
    speed09 = map(fbSpeed, 0, 25, 90, 163);
    OutputM1.write(speed09);
    OutputM2.write(speed09);
    OutputM3.write(speed09);
    OutputM4.write(speed09);
//    speedNine = String(speed09);
//    Serial2.println("Forward"+speedNine);
  }
 if (sideSpeed > 0) { //Right 
    speed01 = map(turnSpeed, 0, 25, 90, 163);
    speed02 = map(turnSpeed, 0, 25, 90, 25);
    OutputM1.write(speed02);
    OutputM2.write(speed01);
    OutputM3.write(speed01);
    OutputM4.write(speed02);
//    speedOne = String(speed01);
//    speedTwo = String(speed01);
//    Serial2.println("Right"+ speedOne);
//    Serial2.println("Right" + speedTwo);
    
  }

   if (sideSpeed < 0) { // Left
    sideSpeed = abs(sideSpeed);
    speed01 = map(turnSpeed, 0, 25, 90, 163);
    speed02 = map(turnSpeed, 0, 25, 90, 25);
    OutputM1.write(speed01);
    OutputM2.write(speed02);
    OutputM3.write(speed02);
    OutputM4.write(speed01);
//    speedOne = String(speed01);
//    speedTwo = String(speed01);
//    Serial2.println("Left"+ speedOne);
//    Serial2.println("Left" + speedTwo);
  }
    if (turnSpeed > 0) { //Turn Clock Wise ( turn Right)
    speed03 = map(turnSpeed, 0, 25, 90, 163);
    speed04 = map(turnSpeed, 0, 25, 90, 25);
    OutputM1.write(speed04);
    OutputM2.write(speed03);
    OutputM3.write(speed04);
    OutputM4.write(speed03);
//    speedThree = String(speed04);
//    speedFour = String(speed03);
//    Serial2.println("Turn Right"+ speedThree);
//    Serial2.println("Turn Right" + speedFour);
  }
  if (turnSpeed <0) { //Turn Counter ClockwIse ( Turn Left)
    turnSpeed = abs(turnSpeed);
    speed03 = map(turnSpeed, 0, 25, 90, 163);
    speed04 = map(turnSpeed, 0, 25, 90, 25);
    OutputM1.write(speed03);
    OutputM2.write(speed04);
    OutputM3.write(speed03);
    OutputM4.write(speed04);
//    speedThree = String(speed03);
//    speedFour = String(speed04);
//    Serial2.println("Turn Left"+ speedThree);
//    Serial2.println("Turn Left" + speedFour);
  }
  
  if ((fbSpeed == 0) and (sideSpeed == 0) and (vertSpeed  == 0)  and (turnSpeed == 0)  and (tiltSpeed == 0) ){

    OutputM1.write(90);
    OutputM2.write(90);
    OutputM3.write(90);
    OutputM4.write(90);  
    OutputM5.write(90);
    OutputM6.write(90); 
    speed01 = 90;
    speed02 = 90;
    speed03 = 90;
    speed04 = 90;
    speed05 = 90;
    speed06 = 90;
    speed07 = 90;
    speed08 = 90;
    speed09 = 90;
    speed10 = 90;
   }  

 gyro_X = String(Total_angle_x,2);
 gyro_Y = String(Total_angle_y,2);
 Gyro = "45 :"+ gyro_X + ", " + gyro_Y;
 Gyro.toCharArray(gyro,20);
 Wire.write(gyro,20);
 Serial1.write(gyro,20); 
  Serial1.readBytes(joy,25);



  
    
  }