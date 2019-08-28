#include "MPU9250.h"
#include <Wire.h>
#include <Servo.h>

MPU9250 mpu;
Servo m1, m2, m3;
const int MPU2 = 0x69;
int p1,p1o, p2o,y2o;
int gyro_x1, gyro_y1, gyro_z1;

long acc_x1, acc_y1, acc_z1, acc_total_vector1, arcal1, apcal1,drifttime;

long x,gyro_x_cal1, gyro_y_cal1, gyro_z_cal1;

float angle_roll_acc1, angle_pitch_acc1, dx;

double del1, del2, del3, interval, preinterval;
float angle_pitch1, angle_roll1;
boolean set_gyro_angles1 = 0 ;
float angle_pitch_output1, angle_roll_output1;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  m1.attach(3);
  m2.attach(5);
  m3.attach(9);     
   setupmpu();
   calib1();
 
}

void loop() {
  // put your main code here, to run repeatedly:
  mpu.update();
  imu1();
   x = millis();
// after 7 secs for yaw to settle
//to print the values for analysis
//Serial.print(mpu.getRoll());Serial.print("  ");Serial.print(mpu.getPitch());Serial.print("  ");Serial.print(mpu.getYaw());Serial.println("  ");
  
  p1 = angle_roll_output1;
  p2o = -mpu.getPitch()+120; // offset clearence to match user arm orientation and that of the robotic arm.
  Serial.print(p2o);Serial.print("  "); // print for analysis
  p1o=abs(p1-p2o-10);Serial.print(p1o);Serial.print("  "); //removing the lower sensor's component from the upper sensor, thereby shifting the reference to upper level. (Read the paper for better understanding)
   
  if((p2o>40)||(p2o>40)){
  y2o = mpu.getYaw()+70;
  }
  Serial.print(y2o);Serial.println("  "); // print for analysis
//  
  if ((x - drifttime) > 7000){  
//    drifttime=millis();
if ((millis()-del1)>50)// drive servo 1 every half a second
    {del1=millis();
      
      m2.write(p2o);}
      
   if ((millis()-del2)>50) //drive servo 2 every half a second
    {del2=millis();
    if (abs(y2o)<190){
      m1.write(abs(y2o));}
      }
      
    if ((millis()-del3)>50) //drive servo 3 every half a second
    {del3=millis();
      m3.write(p1o);}

}
   
}
void setupmpu(){
  Wire.begin();
  delay(2000);
  mpu.setup();
  Wire.begin();
  delay(2000);
  Wire.beginTransmission(MPU2);
  Wire.write(0x6B);
  Wire.write(0b00000000);
  Wire.endTransmission();
  Wire.beginTransmission(MPU2);
  Wire.write(0x1B);
  Wire.write(0x00000000);
  Wire.endTransmission();
  Wire.beginTransmission(MPU2);
  Wire.write(0x1C);
  Wire.write(0b00000000);
  Wire.endTransmission();
}

void calib1() {                                         //Setup the registers of the MPU-6050 (500dfs / +/-8g) and start the gyro
  Serial.print("Calibrating gyro1");
  for (int cal_int = 0; cal_int < 500 ; cal_int ++) {                 //Run this code 1000 times
    if (cal_int % 125 == 0)Serial.print(".");                          //Print a dot  every 125 readings
    read_mpu_6050_data1();
    gyro_x_cal1 += gyro_x1;                                              //Add the gyro x-axis offset to the gyro_x_cal variable
    gyro_y_cal1 += gyro_y1;                                              //Add the gyro y-axis offset to the gyro_y_cal variable
    gyro_z_cal1 += gyro_z1;
    acc_total_vector1 = sqrt((acc_x1 * acc_x1) + (acc_y1 * acc_y1) + (acc_z1 * acc_z1)); //Calculate the total accelerometer vector
    //57.296 = 1 / (3.142 / 180) The Arduino asin function is in radians
    angle_pitch_acc1 = asin((float)acc_y1 / acc_total_vector1) * 57.296;    //Calculate the pitch angle
    angle_roll_acc1 = asin((float)acc_x1 / acc_total_vector1) * 57.296;    //Calculate the roll angle
    apcal1 += angle_pitch_acc1;
    arcal1 += angle_roll_acc1;
    delay(3);                                                          //Delay 3us to simulate the 250Hz program loop

  }
  gyro_x_cal1 /= 500;                                                  //Divide the gyro_x_cal variable by 2000 to get the avarage offset
  gyro_y_cal1 /= 500;                                                  //Divide the gyro_y_cal variable by 2000 to get the avarage offset
  gyro_z_cal1 /= 500;                                                  //Divide the gyro_z_cal variable by 2000 to get the avarage offset
  arcal1 /= 500;
  apcal1 /= 500;
  Serial.print(apcal1); Serial.print(" "); Serial.println(arcal1);
}

void imu1() {

  read_mpu_6050_data1();                                                //Read the raw acc and gyro data from the MPU-6050
  gyro_x1 -= gyro_x_cal1;                                                //Subtract the offset calibration value from the raw gyro_x value
  gyro_y1 -= gyro_y_cal1;                                                //Subtract the offset calibration value from the raw gyro_y value
  gyro_z1 -= gyro_z_cal1;                                                //Subtract the offset calibration value from the raw gyro_z value}
  //Gyro angle calculations
  //0.0000611 = 1 / (250Hz / 65.5)
  angle_pitch1 += gyro_x1 * 0.0000611;                                   //Calculate the traveled pitch angle and add this to the angle_pitch variable
  angle_roll1 += gyro_y1 * 0.0000611;                                    //Calculate the traveled roll angle and add this to the angle_roll variable

  //0.000001066 = 0.0000611 * (3.142(PI) / 180degr) The Arduino sin function is in radians
  angle_pitch1 += angle_roll1 * sin(gyro_z1 * 0.000001066);               //If the IMU has yawed transfer the roll angle to the pitch angel
  angle_roll1 += angle_pitch1 * sin(gyro_z1 * 0.000001066);               //If the IMU has yawed transfer the pitch angle to the roll angel
  //  Accelerometer angle calculations

  acc_total_vector1 = sqrt((acc_x1 * acc_x1) + (acc_y1 * acc_y1) + (acc_z1 * acc_z1)); //Calculate the total accelerometer vector
  //57.296 = 1 / (3.142 / 180) The Arduino asin function is in radians
  angle_pitch_acc1 = asin((float)acc_y1 / acc_total_vector1) * 57.296;    //Calculate the pitch angle
  angle_roll_acc1 = asin((float)acc_x1 / acc_total_vector1) * 57.296;    //Calculate the roll angle

  //Place the MPU-6050 spirit level and note the values in the following two lines for calibration
  angle_pitch_acc1 -= apcal1 ;                                             //Accelerometer calibration value for pitch
  angle_roll_acc1 -= arcal1;                                               //Accelerometer calibration value for roll


  // Serial.print(angle_pitch_acc);Serial.print(" ");Serial.println(angle_pitch);
  if (set_gyro_angles1) {                                               //If the IMU is already started
    angle_pitch1 = angle_pitch1 * 0.96 + angle_pitch_acc1 * 0.04;     //Correct the drift of the gyro pitch angle with the accelerometer pitch angle
    angle_roll1 = angle_roll1 * 0.96 + angle_roll_acc1 * 0.04;        //Correct the drift of the gyro roll angle with the accelerometer roll angle
  }
  else {
    //At first start
    angle_pitch1 = angle_pitch_acc1;                                     //Set the gyro pitch angle equal to the accelerometer pitch angle
    angle_roll1 = angle_roll_acc1;                                       //Set the gyro roll angle equal to the accelerometer roll angle
    set_gyro_angles1 = true;                                            //Set the IMU started flag
  }

  //To dampen the pitch and roll angles a complementary filter is used
  angle_pitch_output1 = angle_pitch_output1 * 0.9 + angle_pitch1 * 0.1;   //Take 90% of the output pitch value and add 10% of the raw pitch value
  angle_roll_output1 = angle_roll_output1 * 0.9 + angle_roll1 * 0.1;      //Take 90% of the output roll value and add 10% of the raw roll value
}

void read_mpu_6050_data1() {                                            //Subroutine for reading the raw gyro and accelerometer data
  Wire.beginTransmission(MPU2);
  Wire.write(0x3B);
  Wire.endTransmission();
  Wire.requestFrom(MPU2, 6);
  while (Wire.available() < 6);
  acc_x1 = Wire.read() << 8 | Wire.read();
  acc_y1 = Wire.read() << 8 | Wire.read();
  acc_z1 = Wire.read() << 8 | Wire.read();

  Wire.beginTransmission(MPU2);
  Wire.write(0x43);
  Wire.endTransmission();
  Wire.requestFrom(MPU2, 6);
  while (Wire.available() < 6);
  gyro_x1 = Wire.read() << 8 | Wire.read();
  gyro_y1 = Wire.read() << 8 | Wire.read();
  gyro_z1 = Wire.read() << 8 | Wire.read();
}
