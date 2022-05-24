#define led_green   11
#define led_red     9
#define led_blue    8
#define led_yellow  10

#define buttom2     A9
#define buttom3     A8
#define buttom4     A7
#define buttom5     A6

#define inter_buttom_pin 2

#define pin1  44
#define pin2  42
#define pin3  41
#define pin4  40
#define pin5  39
#define pin6  38
#define pin7  37
#define pin8  36
#define pin9  35
#define pin10 34
#define pin11 33
#define pin12 32
#define pin13 A11
#define pin14 A12
#define pin15 A13
#define pin16 A14
#define pin17 A15

#define d1          45
#define d2          43
#define d3          46
#define d4          49

#define GET_MM        0
#define CHEK_NAME     1
#define CHEK_CATEGORY 2
#define LOAD_DATA     3

const int trigPin = 6;
const int echoPin = 7;

//#include <NewPing.h>

//NewPing sonar(trigPin, echoPin, 65);

#include <SPI.h>
#include <SD.h>
File tree;
const int PIN_CHIP_SELECT = 53;

uint32_t dividers[8] = {
  1000,
  100,
  10,
  1
};
//  A15, 38, 37, A14, A12, 32, 0 - reserve "zero"
int digits [10][17] = {{A15,  38, 42, 40, 36, 33, A12, 32, 0},      //0
  { 44,  40, 36, 0},                   //1
  { 38,  42, 40, 41, A11, 32, A12, 33, 0},         //2
  { 38,  42, 40, 36, 33, A12, 41, A11, 0},         //3
  {A15,  A11, 41, 40, 36, 0},             //4
  { 42,  38, A15, A11, 41, 36, 33, A12, 0},         //5
  { 42,  38, A15, 32, A12, 33, 36, 41, A11, 0},     //6
  { 38,  42, 44, A13, 0},            //7
  {A15, 38, 37, A14, A12, 32, A11, 0},  //8
  { 41,  A11, A15, 38, 42, 40, 36, 33, A12, 0}};     //9

int positions[4] = {
  45, 43, 46, 49
};
int count_tree = 0;
const char* trees_names[19] = {"Сосна", "Ель", "Пихта", "Лиственница", "Кедр", "Береза", "Осина", "Ольха Серая", "Липа", "Ива древовидная", "Ясень", "Клён", "Вяз", "Ильм", "Дуб", "Бук", "Граб", "Ольха черная", "Тополь"};
const char* category_names[3] = {"Деловые", "Полуделовые", "Дровяные"};
int num_tree, num_category, next, state, value_mm, data_mm = 0;

int trees[19][4][17] =  {{{0},                              {42, 38, A15, A11, 41, 36, 33, A12, 0},      {A15, 38, 37, A14, A12, 32, 0}, {42, 38, A15, A11, 41, 36, 33, A12, 0}},     //SOS - сосна
                        {{0},                               {0},                                         {42, 38, A15, A11, 41, 32, A12, 33, 0}, {A15, 32, A12, 33, 0}},            //EL - ель
                        {{A11, 41, 40, 42, 38, A15, 32, 0}, {38, 42, 37, A14, A12, 33, 0},               {42, 38, A15, 32, A12, 33, 0},          {A15, 32, A11, 41, 40, 36, 0}},     //PICH - пихта
                        {{A15, 32, A12, 33, 0},             {38, 42, 37, A14, A12, 33, 0},               {42, 38, A15, A11, 41, 36, 33, A12, 0}, {38, 42, 37, A14, 0}},             //LIST - лиственница
                        {{0},                               {0},                                         {0},                                    {A15, 32, 44, 35, A11, 37, 0}},     //K - кедр
                        {{0},                               {44,  42, 38, A15, A11, 32, A12, 33, 35, 0}, {42, 38, A15, A11, 41, 32, A12, 33, 0}, {44, 42, 38, A15, 32, A11, 35, 0}}, //BER - берёза
                        {{0},                               {0},                                         {A15, 38, 37, A14, A12, 32, 0},         {42, 38, A15, A11, 41, 36, 33, A12, 0}},     //OS - осина
                        {{0},                               {A15, 38, 37, A14, A12, 32, 0},      {A15, 32, A12, 33, 0},                  {42, 38, A15, A11, 41, 36, 33, A12, 0}},     //OLS - ольха серая
                        {{0},                           {0},                          {A15, 32, A12, 33, 0},                      {A11, 41, 40, 42, 38, A15, 32, 0}},     //LP - липа
                        {{0},                           {38, 42, 37, A14, A12, 33, 0},{A15, 32, A12, 33, 36, 40, 0},              {32, A15, 38, 43, 40, 36, 41, A11, 0}}, //IVA - ива дреновидная
                        {{0},                           {39, 44, A14, 37, 0},             {32, A15, 38, 43, 40, 36, 41, A11, 0},      {42, 38, A15, A11, 41, 36, 33, A12, 0}},     //YAS - ясень
                        {{0},                           {0},                          {A15, 32, 44, 35, A11, 37, 0},                  {A15, 32, A12, 33, 0}},            //KL - клён
                        {{0},                           {0},                          {0},                                        {A15, 32, A12, 33, 36, 40, 0}},     //V - вяз
                        {{0},                           {38, 42, 37, A14, A12, 33, 0},{A15, 32, A12, 33, 0},                      {32, A15, 39, 44, 40, 36, 0}},     //ILM - ильм
                        {{0},                           {0},                          {0},                                        {A15,  38, 42, 40, 36, 33, A12, 32, 0}}, //D - дуб
                        {{0},                           {0},                          {44,  42, 38, A15, A11, 32, A12, 33, 35, 0},{A15, 32, 44, 35, A11, 37, 0}},     //BK - бук
                        {{0},                           {0},                          {44, 42, 38, A15, 32, A12, 33, 35, 0},      {44, 42, 38, A15, 32, A11, 35, 0}}, //GR - граб
                        {{A15, 38, 37, A14, A12, 32, 0},{A15, 32, A12, 33, 0},        {42, 38, A15, 32, A12, 33, 0},              {A15, 32, A11, 41, 40, 36, 0}},     //OLCH - ольха чёрная
                        {{0},                           {38, 42, 37, A14, 0},         {A15, 38, 37, A14, A12, 32, 0},             {A11, 41, 40, 42, 38, A15, 32, 0}}};    //TOP - тополь

int category_tech[3][4][11] = {{{0}, {0}, {0},                              {A15,  38, 42, 40, 36, 33, A12, 32, 0}}, //D  - Деловые
                              {{0}, {0}, {A11, 41, 40, 42, 38, A15, 32, 0},    {A15,  38, 42, 40, 36, 33, A12, 32, 0}}, //PD - Полуделовые
                              {{0}, {0}, {A15,  38, 42, 40, 36, 33, A12, 32, 0}, {42, 38, A15, 32, A11, 35, 40, 41,0}}   //DR - Дрова
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

void clean_digits()
{
  digitalWrite(32,  0);
  digitalWrite(33,  0);
  digitalWrite(34,  0);
  digitalWrite(35,  0);
  digitalWrite(36,  0);
  digitalWrite(37,  0);
  digitalWrite(38,  0);
  digitalWrite(39,  0);
  digitalWrite(40,  0);
  digitalWrite(41,  0);
  digitalWrite(42,  0);
  
  digitalWrite(44,  0);
  digitalWrite(A11, 0);
  digitalWrite(A12, 0);
  digitalWrite(A13, 0);
  digitalWrite(A14, 0);
  digitalWrite(A15, 0);
}


void displayTree(uint32_t pos)
{
  for (int i = 0; i < 4; i++)
  {
    digitalWrite(positions[i], 0);
    clean_digits();
    for (int j = 0; j < 17; j++)
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
    delay(5);
    digitalWrite(positions[i], 1);
  }
}

void displayCategory(uint32_t pos)
{
  for (int i = 0; i < 4; i++)
  {
    digitalWrite(positions[i], 0);
    clean_digits();
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
      //delay(4);
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
      //delay(8);
      digitalWrite(positions[i], 1);
    }
  }
}

void displayDigit(int digit, int pos)
{
  clean_digits();
  for (int i = 0; i < 17; i++)
  {
    if (digits[digit][i] != 0)
    {
      //digitalWrite(42, 1);
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
  delay(5);
}



void clean_d()
{
  digitalWrite(d3, 0);
  digitalWrite(d4, 0);
  digitalWrite(d1, 0);
  digitalWrite(d2, 0);
}

void setup_metr()
{
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void setup_buttom()
{
  pinMode(buttom2,     INPUT_PULLUP);
  pinMode(buttom3,     INPUT_PULLUP);
  pinMode(buttom4,     INPUT_PULLUP);
  pinMode(buttom5,     INPUT_PULLUP);

  pinMode(inter_buttom_pin, INPUT);
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
  for(int i = 32; i < 47; i++)
  {
    pinMode(i, OUTPUT);
  }
  pinMode(A11, OUTPUT);
  pinMode(A12, OUTPUT);
  pinMode(A13, OUTPUT);
  pinMode(A14, OUTPUT);
  pinMode(A15, OUTPUT);
  pinMode(49, OUTPUT);

  clean_digits();

  digitalWrite(45, 1);
  digitalWrite(43, 1);
  digitalWrite(46, 1);
  digitalWrite(49, 1);
}

int get_mm()
{
  
  int mm;
  unsigned long sen_dur = 0;
  sen_dur = 0;
  for(int i = 0; i < 100; i++)
  {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    // Теперь установим высокий уровень на пине Trig
    digitalWrite(trigPin, HIGH);
    // Подождем 10 μs
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    
    sen_dur += pulseIn(echoPin, HIGH, 5000);
  }
  sen_dur = sen_dur / 100;
  
  if(sen_dur > 620)
  {
    //mm = 0.00028671 * sen_dur * sen_dur + 5.44540188 * sen_dur + 117.17137;
    mm = (-5.44540188 + sqrt(5.44540188 * 5.4450188 - 4 * 0.00028671 * (117.17137 - sen_dur))) / (2 * 0.00028671);
  }
  else
  {
    mm = sen_dur / 5.8;
  }
 
  return mm;
}

void file_write(File tree, int mm, int tree_num, int category)
{
  //"NUMBER, MM, TYPE, CATEGORY, TIME"
  uint8_t oldSREG = SREG;
  cli();
  //digitalWrite(led_green,  0);
  //digitalWrite(led_red,    1);
  //digitalWrite(led_blue,   0);
  //digitalWrite(led_yellow, 0);
  String new_line = String(count_tree) + ", " + String(mm) + ", " + trees_names[tree_num] + ", " + category_names[category];
  tree.println(new_line);
  count_tree++;
  state = GET_MM;
  SREG = oldSREG;
  sei();
}

unsigned long t, pre_millis, count = 0;

void card_pannic()
{
  analogWrite(led_red, 10);
  analogWrite(led_green, 10);
  analogWrite(led_blue, 10);
  analogWrite(led_yellow, 10);
  delay(1000);
  digitalWrite(led_red, 0);
  digitalWrite(led_green, 0);
  digitalWrite(led_blue, 0);
  digitalWrite(led_yellow, 0);
  delay(1000);
}

void setup()
{
  setup_led();
  setup_indicator();
  setup_buttom();
  setup_metr();
  
  //Serial.begin(9600);
  t = millis();
  attachInterrupt(digitalPinToInterrupt(inter_buttom_pin), chek_buttom, FALLING);
 

  cli();  // отключить глобальные прерывания
  TCCR5A = 0;   // установить регистры в 0
  TCCR5B = 0;

  OCR5A = 80; // установка регистра совпадения

  TCCR5B |= (1 << WGM12);  // включить CTC режим
  TCCR5B |= (1 << CS10); // Установить биты на коэффициент деления 1024
  TCCR5B |= (1 << CS12);

  TIMSK5 |= (1 << OCIE1A);  // включить прерывание по совпадению таймера
  
  TCCR4A = 0;   // установить регистры в 0
  TCCR4B = 0;

  OCR4A = 40000; // установка регистра совпадения

  TCCR4B |= (1 << WGM12);  // включить CTC режим
  TCCR4B |= (1 << CS10); // Установить биты на коэффициент деления 1024
  TCCR4B |= (1 << CS12);

  TIMSK4 |= (1 << OCIE4A);  // включить прерывание по совпадению таймера

  TCCR3A = 0;   // установить регистры в 0
  TCCR3B = 0;

  OCR3A = 3; // установка регистра совпадения

  TCCR3B |= (1 << WGM12);  // включить CTC режим
  TCCR3B |= (1 << CS10); // Установить биты на коэффициент деления 1024
  TCCR3B |= (1 << CS12);

  TIMSK3 |= (1 << OCIE3A);  // включить прерывание по совпадению таймера
  
  sei();
  
  pinMode(10, OUTPUT);
  // Сделать исключительну. ситуацию когда карта не вставлена
  // Пытаемся проинициализировать модуль
  if (!SD.begin(PIN_CHIP_SELECT)) {
    Serial.println("Card failed, or not present");
    // Если что-то пошло не так, завершаем работу:
    while(1)
    {
      card_pannic();
    }
  }
  Serial.println("card initialized.");
  

  SD.begin(PIN_CHIP_SELECT);
  if(SD.exists("tree.csv"))
  {
    SD.remove("tree.csv");
    tree = SD.open("tree.csv", FILE_WRITE);
  }
  else 
  {
    tree = SD.open("tree.csv", FILE_WRITE);
  }
  tree.println("NUMBER, MM, TYPE, CATEGORY");
  tree.close(); 
  
}

void loop()
{
  switch (state)
  {
    case GET_MM:
      break;
    case CHEK_NAME:
      data_mm = value_mm;
      break;
    case CHEK_CATEGORY:
      break;
    case LOAD_DATA:
      tree = SD.open("tree.csv", FILE_WRITE);
      file_write(tree, data_mm, num_tree, num_category);
      tree.close();
      break;
  }
}

ISR(TIMER5_COMPA_vect)
{
 cli();
 uint8_t oldSREG = SREG;
  if (state == GET_MM)
  {
    //value_mm = get_mm();
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
  SREG = oldSREG;
  sei();
}

ISR(TIMER4_COMPA_vect)
{
  uint8_t oldSREG = SREG;
  if(state == GET_MM)
  {
    value_mm = get_mm();
  }
  SREG = oldSREG;
}

ISR(TIMER3_COMPA_vect)
{
  cli();
  uint8_t oldSREG = SREG;
  switch (state)
  {
    case GET_MM:
      analogWrite(led_green,  10);
      digitalWrite(led_red,    0);
      digitalWrite(led_blue,   0);
      digitalWrite(led_yellow, 0);
      break;
    case CHEK_NAME:
      digitalWrite(led_green,  0);
      digitalWrite(led_red,    0);
      digitalWrite(led_blue,   0);
      analogWrite(led_yellow, 10);
      break;
    case CHEK_CATEGORY:
      digitalWrite(led_green,  0);
      digitalWrite(led_red,    0);
      analogWrite(led_blue,   250);
      digitalWrite(led_yellow, 0);
      break;
    case LOAD_DATA:
      digitalWrite(led_green,  0);
      analogWrite(led_red,    10);
      digitalWrite(led_blue,   0);
      digitalWrite(led_yellow, 0);
      break;
  }
  SREG = oldSREG;
  sei();
}

void chek_buttom()
{
  if(!digitalRead(buttom5))
  {
    count = 0;
    switch(state)
    {
      case GET_MM:
      break;
    case CHEK_NAME:
      if (num_tree < 18)
      {
        num_tree++;
      }
      else
      {
        num_tree = 0;
      }
      break;
    case CHEK_CATEGORY:
      if (num_category < 2)
      {
        num_category++;
      }
      else
      {
        num_category = 0; 
      }
      break;
    case LOAD_DATA:
      break;
    }
  }
  else if(!digitalRead(buttom3))
  {
    count = 0;
    switch(state)
    {
      case GET_MM:
      break;
    case CHEK_NAME:
      if (num_tree > 0)
      {
        num_tree--;
      }
      else
      {
        num_tree = 18;
      }
      break;
    case CHEK_CATEGORY:
      if (num_category > 0)
      {
        num_category--;
      }
      else
      {
        num_category = 2;
      }
      break;
    
    case LOAD_DATA:
      break;
    }
  }
  else if(!digitalRead(buttom2))
  {
    if (state != GET_MM)
    {
      state--;
    }
    else
    { 
      state = 0;
    }
  }
  else if(!digitalRead(buttom4))
  {
    if (state != LOAD_DATA)
    {
      state++;
    }
    else
    {
      state = 0;
    }
  }
}
/*
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
*/
