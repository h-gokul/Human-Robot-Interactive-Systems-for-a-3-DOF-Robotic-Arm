
#include <MPU6050_tockn.h>
#include <Wire.h>
#include <Servo.h>
MPU6050 mpu6050(Wire);
Servo myservo,myservo1;  // create servo object to control a servo

//float x[20];
//float y[20];
//float z[20];
int flag = 0;
int state, n = 25;
float x, y, z;
long prevtime;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);
  myservo.attach(9);
  myservo1.attach(8);
}

void loop()
{ 
//
//  if (Serial.available() == 1)
//  {
//    state = Serial.parseInt();
//    Serial.flush();
//  }

//  if (state == 1) 
//  { prevtime = millis();
//    while (millis() - prevtime <= 3000) { 
      Read(); 
//      }}
//    state=0;
  //}
}

void Read()
  {mpu6050.update();
//    for (int i = 0; i <= n; i++)
//    {
//      x += mpu6050.getAccX();
//      y += mpu6050.getAccY();
//      z += mpu6050.getAccZ();
//    }
//    x = x / n; y = y / n; z = z / n;
 
    if (abs((mpu6050.getAngleX())&&(mpu6050.getAngleY()))||(mpu6050.getAngleZ())<180)
    x=(mpu6050.getAngleX());  y= (mpu6050.getAngleY()); z=(mpu6050.getAngleZ());  
Serial.print(x); Serial.print("\t"); Serial.print(y); Serial.println("\t");  Serial.print(z); Serial.print("\t");
//  myservo.write(y);  delay(15);
//  myservo1.write(x);  delay(15);
  }


  //int dtw_distance()
  //{
  //int cost[50][50];
  //
  //cost[0][0] = abs(a[0] - b[0]);
  //
  //for (int i = 1; i <= 49 ; i++)
  //{
  //  cost[i][0] = cost[i - 1][0] + abs(a[i] - b[0]);
  //  Serial.print(flag);Serial.print(i);   Serial.print(" "); Serial.println(cost[i][0]);
  //}
  //    for (int j = 1; j <= 99; j++)
  //    { cost[0][j] = cost[0][j - 1] + abs(a[0] - b[j]);
  //      //Serial.print(cost[0][j]);
  //    }
  //
  //    for (int i = 1; i <=99; i++)
  //    {
  //      for (int j = 1; j <=99; j++)
  //      {
  //        x = min(cost[i - 1][j - 1], cost[i][j - 1]);
  //        cost[i][j] = min(x, cost[i - 1][j]) + abs(a[i] - b[j]);
  //
  //       }
  //    }
  //
  //    for (int i = 1; i < 99; i++)
  //    {
  //      for (int j = 1; j < 99; j++)
  //      {
  //        Serial.print(cost[i][j]); Serial.print("  ");
  //
  //      }
  //       Serial.println(" ");
  //    }
  //  return cost[10][0];
  //  } 
