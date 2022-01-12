#include <NewPing.h>

#define pin_A 2
#define pin_B 3
#define pin_C 4
#define pin_D 5
#define pin_E 6
#define pin_F 7
#define pin_G 8
#define pin_DT 9

#define D1 10
#define D2 11
#define D3 12
#define D4 13

NewPing sonar(24, 22, 1300);

uint32_t dividers[8] = {
       1000,
        100,
         10,
          1
};


int digits[10][8] = {{  2,  3, 4, 6, 8, 9, 0},      //0
                      { 3,  6, 0},                   //1
                      { 3,  4, 5, 8, 9, 0},         //2
                      { 3,  4, 5, 6, 8, 0},         //3
                      { 2,  3, 5, 6, 0},             //4
                      { 2,  4, 5, 6, 8, 0},         //5
                      { 2,  4, 5, 6, 8, 9, 0},     //6
                      { 3,  4, 6, 0},            //7
                      { 2,  3, 4, 5, 6, 8, 9, 0},  //8
                      { 2,  3, 4, 5, 6, 8, 0}};     //9

int positions[4] = {
   13, 12, 11, 10
};     

void displayAnyValue(uint32_t x)
{
  bool y = 0;
  
   for(int i = 0; i < 4; i++)
   {
    //if(x >= dividers[i])
    //{
      int digit = x / dividers[i];
      digitalWrite(positions[i], 0);
      displayDigit(digit);
      x = x % dividers[i];
      delay(4);
      digitalWrite(positions[i], 1);
    //}
    //else
    //{
      //digitalWrite(positions[i], 1);   
    //}
   }  
}

void displayDigit(int digit)
{
   for(int i = 2; i <= 9; i++) digitalWrite(i, 0); // гасим всё
   for(int i = 0; i < 7; i++)
   {
      if(digits[digit][i] != 0)
      {
        digitalWrite(digits[digit][i], 1);
      }
      else
      {
        break;
      }
   }
   //delay(1);
}

uint32_t v = 0;
uint32_t t = 0;                 

void setup() 
{
  pinMode(pin_A, OUTPUT);
  pinMode(pin_B, OUTPUT);
  pinMode(pin_C, OUTPUT);
  pinMode(pin_D, OUTPUT);
  pinMode(pin_E, OUTPUT);
  pinMode(pin_F, OUTPUT);
  pinMode(pin_G, OUTPUT);
  pinMode(pin_DT, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);

  Serial.begin(9600);
  t = millis();
}

void loop() 
{
  
  for(int i = 0; i <= 9999; i++)
  {
    while(millis() - t < 500)
    {
      displayAnyValue(i);
    }
    t = millis();
  }
  
  //displayAnyValue(77);
  //displayAnyValue(8888);
  //displayAnyValue(sonar.ping_cm());
  //Serial.println(sonar.ping_cm());
  //delayMicroseconds(1);
  //digitalWrite(10, 1);
  //digitalWrite(11, 1);
  //digitalWrite(12, 0);
  //digitalWrite(13, 1);
  //displayDigit(2);
  //while(1);
}
