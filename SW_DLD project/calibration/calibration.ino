#include <Servo.h>
//-------------------------------------------
Servo X_Servo;
Servo Y_Servo;
Servo Z_Servo;

int x = 0;                  // min=6 max=180
int y = 180;                  // min=0 max=178
int z = 0;                // Z_Pos = 140 for write , Z_Pos = 100 for space
//-------------------------------------------
void setup()
{
Serial.begin(9600);
  
  X_Servo.attach(8);
  Y_Servo.attach(9);
  Z_Servo.attach(10);
  
  NotWrite();
  Go_to(x, y);
}
//-------------------------------------------
void loop()
{
//Z_Servo.write(60);
  //delay(1000);
  //Z_Servo.write(90);
  //delay(1000);
  //Z_Servo.write(120);
  //delay(1000);
}
//-------------------------------------------
void Write()
{
  Z_Servo.write(140);
  delay(200);
}
//-------------------------------------------
void NotWrite()
{
  Z_Servo.write(90);
  delay(200);
}
//-------------------------------------------
void Go_to(int x,int y)
{
  Y_Servo.write(y);
  //delay(20);
  X_Servo.write(x);
  delay(400);
}
//-------------------------------------------
void Go_to_with_delay(int x,int y, int delayed)
{
  Y_Servo.write(y);
  X_Servo.write(x);
  delay(delayed);
}
//-------------------------------------------
void Linear_Right(double x_start, double y_start, double x_end, double y_end, double delayed)
{
  double m = (y_end - y_start) / (x_end - x_start);
  double yy = 0;
  for (double xx = x_start ; xx <= x_end ; xx++ )
  {
    yy = m*(xx - x_start) + y_start;
    Go_to_with_delay(xx,yy,delayed);
  }
  delay(100);
}
//-------------------------------------------
void Linear_Left(double x_start, double y_start, double x_end, double y_end, double delayed)
{
  double m = (y_end - y_start) / (x_end - x_start);
  double yy = 0;
  for (double xx = x_start ; xx >= x_end ; xx-- )
  {
    yy = m*(xx - x_start) + y_start;
    Go_to_with_delay(xx,yy,delayed);
  }
  delay(100);
}
