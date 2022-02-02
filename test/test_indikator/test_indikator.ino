#define led_green   53
#define led_red     51
#define led_blue    49
#define led_yellow  47

#define buttom1     3
#define buttom2     2
#define buttom3     18
#define buttom4     19

#define encoder_s1  52
#define encoder_s2  50
#define encoder_key 48

#define pin_A       44
#define pin_B       42
#define pin_C       40
#define pin_D       38
#define pin_E       36
#define pin_F       34
#define pin_G       33
#define pin_DT      31

#define D1          29
#define D2          27
#define D3          25
#define D4          23

#define GET_MM        0
#define CHEK_NAME     1
#define CHEK_CATEGORY 2
#define LOAD_DATA     3

#define BERS 3210
#define OCIN 7654
#define ELKA 9781
#define LIPA 9268
#define COCN 7545

#include <Wire.h>
#include <VL53L0X.h>

VL53L0X sensor;

int list_trees[5] = {BERS, OCIN, ELKA, LIPA, COCN};

uint32_t dividers[8] = {
  1000,
  100,
  10,
  1
};

int digits[10][8] = {{  44,  42, 40, 36, 33, 31, 0},      //0
  { 42,  36, 0},                   //1
  { 42,  40, 38, 33, 31, 0},         //2
  { 42,  40, 38, 36, 33, 0},         //3
  {44,  42, 38, 36, 0},             //4
  { 44,  40, 38, 36, 33, 0},         //5
  { 44,  40, 38, 36, 33, 31, 0},     //6
  { 42,  40, 36, 0},            //7
  { 44,  42, 40, 38, 36, 33, 31, 0},  //8
  { 44,  42, 40, 38, 36, 33, 0}
};     //9

int positions[4] = {
  23, 25, 27, 29
};

int trees[19][4][8] =  {{{0},                          {40, 38, 44, 33, 36, 0},          {44, 42, 40, 36, 33, 31, 0},       {40, 38, 44, 33, 36, 0}},     //SOS - сосна
                        {{0},                          {0},                              {44, 40, 38, 31, 33, 0},           {44, 31, 33,  0}},            //EL - ель
                        {{44, 38, 42, 31, 40, 0},      {44, 31, 0},                      {44, 40, 31, 33, 0},               {42, 36, 44, 31, 38, 0}},     //PICH - пихта
                        {{44, 31, 33, 0},              {44, 31, 0},                      {40, 38, 44, 33, 36, 0},           {44, 31, 40, 0}},             //LIST - лиственница
                        {{0},                          {0},                              {0},                               {42, 36, 44, 31, 38, 0}},     //K - кедр
                        {{0},                          {44,  42, 40, 38, 36, 33, 31, 0}, {44, 40, 38, 31, 33, 0},           {44, 38, 42, 31, 40, 36, 0}}, //BER - берёза
                        {{0},                          {0},                              {44, 42, 40, 36, 33, 31, 0},       {40, 38, 44, 33, 36, 0}},     //OS - осина
                        {{0},                          {44, 42, 40, 36, 33, 31, 0},      {44, 31, 33,  0},                  {40, 38, 44, 33, 36, 0}},     //OLS - ольха серая
                        {{0},                          {0},                              {44, 31, 33,  0},                  {44, 38, 42, 31, 40, 0}},     //LP - липа
                        {{0},                          {44, 31, 0},                      {44, 42, 36, 33, 31, 0},           {44, 42, 40, 38, 36, 31, 0}}, //IVA - ива дреновидная
                        {{0},                          {44, 42, 36, 33, 31, 0},          {44, 42, 40, 38, 36, 31, 0},       {42, 40, 38, 33, 31, 0}},     //YAS - ясень
                        {{0},                          {0},                              {42, 36, 44, 31, 38, 0},           {44, 31, 33,  0}},            //KL - клён
                        {{0},                          {0},                              {0},                               {44, 42, 36, 33, 31, 0}},     //V - вяз
                        {{0},                          {44, 31, 0},                      {44, 31, 33,  0},                  {42, 36, 44, 31, 38, 0}},     //ILM - ильм
                        {{0},                          {0},                              {0},                               {44, 42, 40, 36, 33, 31, 0}}, //D - дуб
                        {{0},                          {0},                              {44,  42, 40, 38, 36, 33, 31, 0},  {42, 36, 44, 31, 38, 0}},     //BK - бук
                        {{0},                          {0},                              {44, 40, 31, 33, 0},               {44, 38, 42, 31, 40, 36, 0}}, //GR - граб
                        {{44, 42, 40, 36, 33, 31, 0},  {44, 31, 33,  0},                 {44, 40, 31, 33, 0},               {42, 36, 44, 31, 38, 0}},     //OLCH - ольха чёрная
                        {{0},                          {44, 31, 40, 0},                  {44, 42, 40, 36, 33, 31, 0},       {44, 38, 42, 31, 40, 0}}};    //TOP - тополь

int category_tech[3][4][8] = {{{0}, {0}, {0},                          {44,  42, 40, 36, 33, 31, 0}}, //D  - Деловые
                              {{0}, {0}, {44,  38, 42,  31, 40, 0},    {44,  42, 40, 36, 33, 31, 0}}, //PD - Полуделовые
                              {{0}, {0}, {44,  42, 40, 36, 33, 31, 0}, {44, 38, 42, 31, 40, 36, 0}}   //DR - Дрова
};

int old_trees[10][8] = {  {  44, 40, 38, 36, 33, 31, 0},      //Б/1
  { 44,  40, 38, 31, 33, 0},                   //Е/2
  { 44,  38, 42,  31, 40, 0},         //Р/3
  { 42,  40, 38, 36, 33, 0},         //З/4
  { 44,  42, 40, 36, 33, 31, 0},             //O/5
  { 44,  40, 31, 33, 0},         //C/6
  { 44,  31, 0},     //I/7
  { 42,   36, 44, 31, 38, 0},            //H/8
  { 44,  31, 33,  0},  //L/9
  { 44,  42, 40, 38, 36, 31, 0}
};     //A/10

int arr_pin_indicator[8] = {44, 42, 40, 38, 36, 34, 33, 31};

void displayTree(uint32_t pos)
{
  for (int i = 0; i < 4; i++)
  {
    digitalWrite(positions[i], 0);
    for (int j = 31; j <= 44; j++) digitalWrite(j, 0);
    for (int j = 0; j < 8; j++)
    {
      if (trees[pos][i][j] != 0)
      {
        digitalWrite(trees[pos][i][j], 1);
      }
      else
      {
        break;
      }
    }
    delay(4);
    digitalWrite(positions[i], 1);
  }
}

void displayCategory(uint32_t pos)
{
  for (int i = 0; i < 4; i++)
  {
    digitalWrite(positions[i], 0);
    for (int j = 31; j <= 44; j++) digitalWrite(j, 0);
    for (int j = 0; j < 8; j++)
    {
      if (category_tech[pos][i][j] != 0)
      {
        digitalWrite(category_tech[pos][i][j], 1);
      }
      else
      {
        break;
      }
    }
    delay(4);
    digitalWrite(positions[i], 1);
  }
}

void displayAnyValue(uint32_t x)
{
  int digit = 0;
  if (x <= 99)
  {
    for (int i = 2; i < 4; i++)
    {
      digit = x / dividers[i];
      digitalWrite(positions[i], 0);
      displayDigit(digit, i);
      x = x % dividers[i];
      delay(4);
      digitalWrite(positions[i], 1);
    }
  }
  else if (x <= 999)
  {
    for (int i = 1; i < 4; i++)
    {
      digit = x / dividers[i];
      digitalWrite(positions[i], 0);
      displayDigit(digit, i);
      x = x % dividers[i];
      delay(4);
      digitalWrite(positions[i], 1);
    }
  }
  else if (x <= 9999)
  {
    for (int i = 0; i < 4; i++)
    {
      digit = x / dividers[i];
      digitalWrite(positions[i], 0);
      displayDigit(digit, i);
      x = x % dividers[i];
      delay(4);
      digitalWrite(positions[i], 1);
    }
  }
}

void displayDigit(int digit, int pos)
{
  for (int i = 31; i <= 44; i++) digitalWrite(i, 0);
  digitalWrite(34, 0);
  for (int i = 0; i < 8; i++)
  {
    if (digits[digit][i] != 0)
    {
      digitalWrite(digits[digit][i], 1);
    }
    else
    {
      break;
    }
  }
  if (pos == 2)
  {
    digitalWrite(34, 1);
  }
}

void setup_buttom()
{
  pinMode(buttom1,     INPUT_PULLUP);
  pinMode(buttom2,     INPUT_PULLUP);
  pinMode(buttom3,     INPUT_PULLUP);
  pinMode(buttom4,     INPUT_PULLUP);
}

void setup_led()
{
  pinMode(led_green,   OUTPUT);
  pinMode(led_red,     OUTPUT);
  pinMode(led_blue,    OUTPUT);
  pinMode(led_yellow,  OUTPUT);
}

void setup_indicator()
{
  pinMode(pin_A,       OUTPUT);
  pinMode(pin_B,       OUTPUT);
  pinMode(pin_C,       OUTPUT);
  pinMode(pin_D,       OUTPUT);
  pinMode(pin_E,       OUTPUT);
  pinMode(pin_F,       OUTPUT);
  pinMode(pin_G,       OUTPUT);
  pinMode(pin_DT,      OUTPUT);

  pinMode(D1,          OUTPUT);
  pinMode(D2,          OUTPUT);
  pinMode(D3,          OUTPUT);
  pinMode(D4,          OUTPUT);

  for (int i = 0; i < 8; i++) digitalWrite(arr_pin_indicator[i], 0);

  digitalWrite(D1, 1);
  digitalWrite(D2, 1);
  digitalWrite(D3, 1);
  digitalWrite(D4, 1);
}

void setup_encoder()
{
  pinMode(encoder_s1,  INPUT);
  pinMode(encoder_s2,  INPUT);
  pinMode(encoder_key, INPUT);
}

uint32_t t = 0;

void setup()
{
  setup_led();
  setup_indicator();
  setup_buttom();
  Serial.begin(9600);
  t = millis();
  Wire.begin();

  sensor.setTimeout(500);

  


  sensor.setSignalRateLimit(0.1);
  sensor.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
  sensor.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);

  sensor.setMeasurementTimingBudget(20000);
  sensor.setMeasurementTimingBudget(200000);

  attachInterrupt(digitalPinToInterrupt(buttom4), next_state, FALLING);
  attachInterrupt(digitalPinToInterrupt(buttom3), pre_state, FALLING);

  attachInterrupt(digitalPinToInterrupt(buttom1), next_count, FALLING);
  attachInterrupt(digitalPinToInterrupt(buttom2), pre_count, FALLING);

  cli();  // отключить глобальные прерывания
  TCCR1A = 0;   // установить регистры в 0
  TCCR1B = 0;

  OCR1A = 80; // установка регистра совпадения

  TCCR1B |= (1 << WGM12);  // включить CTC режим
  TCCR1B |= (1 << CS10); // Установить биты на коэффициент деления 1024
  TCCR1B |= (1 << CS12);

  TIMSK1 |= (1 << OCIE1A);  // включить прерывание по совпадению таймера

  sei();

}

int num_tree, num_category, next, state, value_mm = 0;

void loop()
{
  value_mm = 400;
  Serial.println(next);
  
  switch (state)
  {
    case GET_MM:
      digitalWrite(led_green,  1);
      digitalWrite(led_red,    0);
      digitalWrite(led_blue,   0);
      digitalWrite(led_yellow, 0);
      break;
    case CHEK_NAME:
      digitalWrite(led_green,  0);
      digitalWrite(led_red,    0);
      digitalWrite(led_blue,   0);
      digitalWrite(led_yellow, 1);
      break;
    case CHEK_CATEGORY:
      digitalWrite(led_green,  0);
      digitalWrite(led_red,    0);
      digitalWrite(led_blue,   1);
      digitalWrite(led_yellow, 0);
      break;
    case LOAD_DATA:
      digitalWrite(led_green,  0);
      digitalWrite(led_red,    1);
      digitalWrite(led_blue,   0);
      digitalWrite(led_yellow, 0);
      break;
  }

}

ISR(TIMER1_COMPA_vect)
{
 
  if (state == GET_MM)
  {
    displayAnyValue(value_mm);
  }
  else if (state == CHEK_NAME)
  {
    displayTree(num_tree);
  }
  else if (state == CHEK_CATEGORY)
  {
    displayCategory(num_category);
  }
  else if (state == LOAD_DATA)
  {

  }

}

void pre_state()
{
  if (state != GET_MM)
  {
    int count = 0;
    while (count < 1000 and !digitalRead(buttom3))
    {
      count++;
    }
    if (count > 950)
    {
      state--;
    }
  }
  else
  {
    int count = 0;
    while (count < 1000 and !digitalRead(buttom3))
    {
      count++;
    }
    if (count > 950)
    {
      state = 0;
    }
  }
}

void next_state()
{
  if (state != LOAD_DATA)
  {
    int count = 0;
    while (count < 1000 and !digitalRead(buttom4))
    {
      count++;
    }
    if (count > 950)
    {
      state++;
    }
  }
  else
  {
    int count = 0;
    while (count < 1000 and !digitalRead(buttom4))
    {
      count++;
    }
    if (count > 950)
    {
      state = 0;
    }
  }
}

void next_count()
{
  if (state == CHEK_NAME)
  {
    if (num_tree < 18)
    {
      int count = 0;
      while (count < 1000 and !digitalRead(buttom1))
      {
        count++;
      }
      if (count > 950)
      {
        num_tree++;
      }
    }
    else
    {
      int count = 0;
      while (count < 1000 and !digitalRead(buttom1))
      {
        count++;
      }
      if (count > 950)
      {
        num_tree = 0;
      }
    }
  }
  else if (state == CHEK_CATEGORY)
  {
    if (num_category < 2)
    {
      int count = 0;
      while (count < 1000 and !digitalRead(buttom1))
      {
        count++;
      }
      if (count > 950)
      {
        num_category++;
      }
    }
    else
    {
      int count = 0;
      while (count < 1000 and !digitalRead(buttom1))
      {
        count++;
      }
      if (count > 950)
      {
        num_category = 0;
      }
    }
  }
}

void pre_count()
{
  if (state == CHEK_NAME)
  {
    if (num_tree > 0)
    {
      int count = 0;
      while (count < 1000 and !digitalRead(buttom2))
      {
        count++;
      }
      if (count > 950)
      {
        num_tree--;
      }
    }
    else
    {
      int count = 0;
      while (count < 1000 and !digitalRead(buttom2))
      {
        count++;
      }
      if (count > 950)
      {
        num_tree = 18;
      }
    }
  }
  else if (state == CHEK_CATEGORY)
  {
    if (num_category > 0)
    {
      int count = 0;
      while (count < 1000 and !digitalRead(buttom1))
      {
        count++;
      }
      if (count > 950)
      {
        num_category--;
      }
    }
    else
    {
      int count = 0;
      while (count < 1000 and !digitalRead(buttom1))
      {
        count++;
      }
      if (count > 950)
      {
        num_category = 2;
      }
    }
  }
}
