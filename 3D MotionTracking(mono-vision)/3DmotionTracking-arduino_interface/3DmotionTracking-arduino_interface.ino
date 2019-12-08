int data;

//Servo Write the incoming serial data through arduino  

int anglev;
int angleh;
int anglec;
int x=0;
int y=0;
int z=0;
#include<Servo.h>
Servo horizontal;
Servo vertical;
Servo c;
int incoming[2];
void setup() {
  Serial.begin(9600);
  horizontal.attach(3);
  vertical.attach(5);
  c.attach(9);
  horizontal.write(90);
  vertical.write(90);
  c.write(90);
  

}

void loop() {
  while(Serial.available()>=3)
  {
    for(int i=0; i<3; i++)
    {
      incoming[i]=Serial.read();
      
    }
    
    x= incoming[0];
    y=incoming[1];
    z=incoming[2];

   horizontal.write(x);
   vertical.write(z);
   c.write(y); 
  }

}
