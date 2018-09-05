  #include <Wire.h>
#include "RTClib.h"
#include <Servo.h>
//-------------------------------------------
RTC_DS1307 rtc;
unsigned int hh;
unsigned int mm;
int first_init = 1;


Servo X_Servo;
Servo Y_Servo;
Servo Z_Servo;

int x = 6;                  // min=6 max=180
int y = 178;                  // min=0 max=178
//int z = 100;                // Z_Pos = 140 for write , Z_Pos = 100 for space
//-------------------------------------------
void setup()
{
  Serial.begin(9600);
    if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  //rtc.adjust(DateTime(2016, 9, 24, 22, 59, 27));
  //rtc.adjust(DateTime(F(__DATE__),F(__TIME__)));
  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
     //rtc.adjust(DateTime(2016, 9, 24, 13, 51, 10));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
  
  X_Servo.attach(8);
  Y_Servo.attach(9);
  Z_Servo.attach(10);
  
  NotWrite();
}
//-------------------------------------------
void loop()
{
  DateTime now = rtc.now();
  unsigned int new_hh = now.hour();
  unsigned int new_mm = now.minute();
  
  if (first_init)
        Erase_board();
  
  if (mm != new_mm)
  {
    unsigned int Fhh_old = hh / 10;
    unsigned int Shh_old = hh % 10;
    unsigned int Fmm_old = mm / 10;
    unsigned int Smm_old = mm % 10;
    int board_erased = 0;
    hh = new_hh;
    mm = new_mm;
    unsigned int Fhh_new = hh / 10;
    unsigned int Shh_new = hh % 10;
    unsigned int Fmm_new = mm / 10;
    unsigned int Smm_new = mm % 10;
    
    x = 28;
    if ((Fhh_old != Fhh_new) || (first_init))
    {
      if (!first_init)
      {
        Erase_digit(x);
        board_erased = 1;
      }
      PenWrite(hh / 10);
    }
    x += 29;
    if ((Shh_old != Shh_new) || (first_init))
    {
      if ((!first_init) && (!board_erased))
      {
        Erase_digit(x-2);
        board_erased = 1;
      }
      PenWrite(hh % 10);
    }
    x += 28;
    if ((first_init) || (board_erased))
      PenWrite(10);
    x += 17;
    if ((Fmm_old != Fmm_new) || (first_init))
    {
      if ((!first_init) && (!board_erased))
      {
        Erase_digit(x);
        board_erased = 1;
      }
      PenWrite(mm / 10);
    }
      
    x += 38;
    if ((Smm_old != Smm_new) || (first_init))
    {
      if ((!first_init) && (!board_erased))
      {
        Erase_digit(x-9);
        board_erased = 1;
      }
      PenWrite(mm % 10);
    }
    board_erased = 0;
  }
  
  first_init = 0;
  Serial.write(" hour=");
  Serial.print(new_hh);
  Serial.write(" minute=");
  Serial.print(new_mm);
  Serial.write(" second=");
  Serial.print(now.second());
  Serial.print("\n");

  Go_to_with_delay(6,160, 15);
  delay(1000);

}

//-------------------------------------------
void Write()
{
  Z_Servo.write(120);
  delay(400);
}
//-------------------------------------------
void NotWrite()
{
  Z_Servo.write(90);
  delay(400);
}
//-------------------------------------------
void Erase_board()
{
  Z_Servo.write(25);
  delay(400);
  for (y=60 ; y<=140 ; y=y+10)
  {
    Go_to_with_delay(0, y, 500);
    Go_to_with_delay(150, y, 500);
  }
  NotWrite();
}
//-------------------------------------------
void Erase_digit(int x)
{
  delay(400);
  for (y=60 ; y<=140 ; y=y+10)
  {
    Go_to_with_delay(x-29, y, 500);
    Z_Servo.write(25);
    Go_to_with_delay(150, y, 500);
  }
  NotWrite();
}
//-------------------------------------------
void Go_to(int x,int y)
{
  Y_Servo.write(y);
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
void upper_cycle(double x_start, double x_end , double a , double b, double delayed)
{
  double yy = 0;
  double r = x_end - a;
  for (double xx = x_start ; xx <= x_end ; xx++ )
  {
    yy = sqrt ( sq(r) - sq(xx-a) ) + b;
    Go_to_with_delay(xx,yy,delayed);
    /*
    Serial.write(" x=");
    Serial.print(xx);
    Serial.write(" y=");
    Serial.print(yy);
    Serial.print("\n");
    */
  }
  delay(100);
}
//-------------------------------------------
void lower_cycle(double x_start, double x_end , double a , double b, double delayed)
{
  double yy = 0;
  double r = x_end - a;
  for (double xx = x_end ; xx >= x_start ; xx-- )
  {
    yy = -sqrt ( sq(r) - sq(xx-a) ) + b;
    Go_to_with_delay(xx,yy,delayed);
    /*
    Serial.write(" x=");
    Serial.print(xx);
    Serial.write(" y=");
    Serial.print(yy);
    Serial.print("\n");
    */
  }
  delay(100);
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
    /*
    Serial.write(" x=");
    Serial.print(xx);
    Serial.write(" y=");
    Serial.print(yy);
    Serial.print("\n");
    */
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
    /*
    Serial.write(" x=");
    Serial.print(xx);
    Serial.write(" y=");
    Serial.print(yy);
    Serial.print("\n");
    */
  }
  delay(100);
}    //Linear_Left(x+30, 130, x, 50, 30);
//-------------------------------------------
void Linear_down(double x_start, double y_start, double y_end, double delayed)
{
  for (double yy = y_start ; yy >= y_end ; yy-- )
  {
    Go_to_with_delay(x_start,yy,delayed);
    /*
    Serial.write(" x=");
    Serial.print(x_start);
    Serial.write(" y=");
    Serial.print(yy);
    Serial.print("\n");
    */
  }
  delay(100);
}    //Linear_down(x_start, y_start, y_end, 15);
//-------------------------------------------
void Ellipse(double x_center, double y_center, double x_lenght , double y_lenght,  double delayed)
{
    //upper Ellipse
  double yy = 0;
  for (double xx = -12.5 ; xx <= 12.5 ; xx++ )
  {
    yy = sqrt(1-sq(xx/x_lenght))*y_lenght+y_center;
    Go_to_with_delay(xx+x_center,yy,delayed);
    /*
    Serial.write(" x=");
    Serial.print(xx+x_center);
    Serial.write(" y=");
    Serial.print(yy);
    Serial.print("\n");
    */
  }

  //lower Ellipse
  yy = 0;
  for (double xx = 12.5 ; xx >= -12.5 ; xx-- )
  {
    yy = -sqrt(1-sq(xx/x_lenght))*y_lenght+y_center;
    Go_to_with_delay(xx+x_center,yy,delayed);
    /*
    Serial.write(" x=");
    Serial.print(xx+x_center);
    Serial.write(" y=");
    Serial.print(yy);
    Serial.print("\n");
    */
  }
  delay(100);
}
//-------------------------------------------
void Ellipse_to_left(double x_start, double x_end, double x_center, double x_lenght, double y_center, double y_lenght, int sign, double delayed)
{
  double yy = 0;
  for (double xx = x_start-x_center ; xx >= x_end-x_center ; xx-- )
  {
    yy = sign*sqrt(1-sq(xx/x_lenght))*y_lenght+y_center;
    Go_to_with_delay(xx+x_center,yy,delayed);
    /*
    Serial.write(" x=");
    Serial.print(xx+x_center);
    Serial.write(" y=");
    Serial.print(yy);
    Serial.print("\n");
    */
  }
}
//-------------------------------------------
void Ellipse_to_right(double x_start, double x_end, double x_center, double x_lenght, double y_center, double y_lenght, int sign, double delayed)
{
  
  double yy = 0;
  for (double xx = x_start-x_center ; xx <= x_end-x_center ; xx++ )
  {
    yy = sign*sqrt(1-sq(xx/x_lenght))*y_lenght+y_center;
    Go_to_with_delay(xx+x_center,yy,delayed);
    /*
    Serial.write(" x=");
    Serial.print(xx+x_center);
    Serial.write(" y=");
    Serial.print(yy);
    Serial.print("\n");
    */
  }
}
//-------------------------------------------
void PenWrite(int number)
{
  switch (number)
  {
    case 0:
    Serial.write(" write number=");
    Serial.print(number);
    Serial.print("\n");
    Go_to (x    , 85);
    Write();
    Ellipse(x+12.5, 90 , 12.5 , 34, 30);
    NotWrite();
    break;
    //Ellipse(x_center, y_center, x_lenght , y_lenght, delayed)
    case 1:
    Serial.write(" write number=");
    Serial.print(number);
    Serial.print("\n");
    Go_to (x    , 90);
    Write();
    Linear_Right(x, 90, x+25, 124, 15);
    delay(30);
    Linear_down(x+25, 124, 56, 5);
    Go_to (x+25 , 56);
    NotWrite();
    break;
    // Linear_Right(x_start, y_start, x_end, y_end, delayed)
    // Linear_down(x_start, y_start, y_end, delayed)
    case 2:
    Serial.write(" write number=");
    Serial.print(number);
    Serial.print("\n");
    Go_to (x   , 110);
    Write();
    upper_cycle (x, x+25 , x+12.5 , 110, 30);
    Linear_Left(x+25, 110, x, 56, 20);
    Linear_Right(x, 56, x+25, 56, 20);
    NotWrite();
    break;
    //upper_cycle(x_start, x_end , a , b, delayed)
    // Linear_Left(x_start, y_start, x_end, y_end, delayed)
    // Linear_Right(x_start, y_start, x_end, y_end, delayed)
    case 3:
    Serial.write(" write number=");
    Serial.print(number);
    Serial.print("\n");
    Go_to (x   , 125);
    Write();
    upper_cycle (x, x+22 , x+5 , 107, 25);
    lower_cycle (x, x+22 , x+5 , 107, 25);
    upper_cycle (x, x+22 , x+5 , 73, 25);
    lower_cycle (x, x+22 , x+5 , 73, 25);
    NotWrite();
    break;
    //upper_cycle(x_start, x_end , a , b, delayed)
    //lower_cycle(x_start, x_end , a , b, delayed)
    case 4:
    Serial.write(" write number=");
    Serial.print(number);
    Serial.print("\n");
    Go_to (x, 124);
    Write();
    Linear_down(x, 124, 90, 10);
    Linear_Right(x, 90, x+25, 90, 15);
    NotWrite();
    Go_to (x+25, 124);
    Write();
    Linear_down(x+25, 124, 56, 10);
    NotWrite();
    break;
    // Linear_Left(x_start, y_start, x_end, y_end, delayed)
    // Linear_Right(x_start, y_start, x_end, y_end, delayed)
    // Linear_down(x_start, y_start, y_end, delayed)
    case 5:
    Serial.write(" write number=");
    Serial.print(number);
    Serial.print("\n");
    Go_to (x,124);
    Write();
    Linear_down(x, 124, 100, 10);
    upper_cycle (x, x+25 , x+3.7 , 76, 15);
    lower_cycle (x, x+25 , x+3.7 , 76, 15);
    NotWrite();
    Go_to (x,124);
    Write();
    Linear_Right(x, 124, x+25, 124, 15);
    NotWrite();
    break;
    // Linear_down(x_start, y_start, y_end, delayed)
    // upper_cycle(x_start, x_end , a , b, delayed)
    // lower_cycle(x_start, x_end , a , b, delayed)
    case 6:
    Serial.write(" write number=");
    Serial.print(number);
    Serial.print("\n");
    Go_to (x+25,124);
    Write();
    Ellipse_to_left (x+25, x     , x+25  , 25  , 90, 34,  1, 25);
    Ellipse_to_right(x   , x+12.5, x+12.5, 12.5, 90, 34, -1, 25);
    Ellipse_to_right(x+12.5, x+25, x+12.5, 12.5, 70, 14, -1, 25);
    Ellipse_to_left (x+25, x   , x+12.5, 12.5, 70, 20,  1, 25);
    NotWrite();
    break;
    //Ellipse_to_left(x_start, x_end, x_center, x_lenght, y_center, y_lenght, sign, delayed)
    case 7:
    Serial.write(" write number=");
    Serial.print(number);
    Serial.print("\n");
    Go_to (x,124);
    Write();
    Linear_Right(x, 124, x+25, 124, 15);
    Linear_Left(x+25, 124, x, 56, 15);
    NotWrite();
    Go_to (x+5, 90);
    Write();
    Linear_Right(x+5, 90, x+25, 90, 15);
    NotWrite();
    break;
    
    case 8:
    Serial.write(" write number=");
    Serial.print(number);
    Serial.print("\n");
    Go_to (x+12.5,124);
    Write();
    Ellipse_to_left (x+12.5, x, x+12.5, 12.5, 107, 17, 1, 25);
    Ellipse_to_right(x, x+12.5, x+12.5, 12.5, 107, 17, -1, 25);
    Ellipse_to_right(x+12.5, x+25, x+12.5, 12.5, 73, 17, 1, 25);
    Ellipse_to_left (x+25, x, x+12.5, 12.5, 73, 17, -1, 25);
    Ellipse_to_right(x, x+12.5, x+12.5, 12.5, 73, 17, 1, 25);
    Ellipse_to_right(x+12.5, x+25, x+12.5, 12.5, 107, 17, -1, 25);
    Ellipse_to_left (x+25, x+12.5, x+12.5, 12.5, 107, 17, 1, 25);
    NotWrite();
    break;
    //Ellipse_to_left(x_start, x_end, x_center, x_lenght, y_center, y_lenght, sign, delayed)
    case 9:
    Serial.write(" write number=");
    Serial.print(number);
    Serial.print("\n");
    Go_to (x+25   , 105);
    Write();
    Ellipse_to_left (x+25, x, x+12.5, 12.5, 107, 17, 1, 25);
    Ellipse_to_right(x, x+25, x+12.5, 12.5, 107, 17, -1, 25);
    Linear_down(x+25, 107, 56, 5);
    delay(30);
    NotWrite();
    //Linear_down(x_start, y_start, y_end, delayed)
    break;

    case 10: // 2 dots
    Serial.write(" write :");
    Serial.print("\n");
    Go_to (x+2.5   , 100);
    Write();
    Ellipse_to_right(x+2.5, x+12.5, x+7.5, 5, 105, 5, 1, 25);
    Ellipse_to_left (x+12.5, x+2.5, x+7.5, 5, 105, 5, -1, 25);
    //upper_cycle (x+2.5, x+12.5 , x+7.5 , 105, 50);
    //lower_cycle (x+2.5, x+12.5 , x+7.5 , 105, 50);
    NotWrite();
    Go_to (x+2.5   , 70);
    Write();
    Ellipse_to_right(x+2.5, x+12.5, x+7.5, 5, 75, 5, 1, 25);
    Ellipse_to_left (x+12.5, x+2.5, x+7.5, 5, 75, 5, -1, 25);
    //upper_cycle (x+2.5, x+12.5 , x+7.5 , 75, 50);
    //lower_cycle (x+2.5, x+12.5 , x+7.5 , 75, 50);
    NotWrite();
    //upper_cycle(x_start, x_end , a , b, delayed)
    break;
  }
}
