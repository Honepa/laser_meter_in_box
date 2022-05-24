#define d1    45 
#define d2    43
#define d3    46
#define d4    49

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

#define buttom1 A10
#define buttom2 A9
#define buttom3 A8
#define buttom4 A7
#define buttom5 A6

#define led_blue 8
#define led_red 9
#define led_yellow 10
#define led_green 11

#define inter_buttom_pin 2

const int trigPin = 6;
const int echoPin = 7;

uint32_t dividers[8] = {
  1000,
  100,
  10,
  1
};

int digits [10][17] = {{  A15, 38, 37, A14, A12, 32, 0},      //0
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
                              {{0}, {0}, {A15,  38, 42, 40, 36, 33, A12, 32, 0}, {44, 42, 38, A15, 32, A11, 35, 0}}   //DR - Дрова
};  

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

int i = 0;
long t = millis();

void setup() 
{
  setup_metr();
 for(int i = 32; i < 47; i++)
 {
  pinMode(i, OUTPUT);
 }
  pinMode(A11, OUTPUT);
  pinMode(A12, OUTPUT);
  pinMode(A13, OUTPUT);
  pinMode(A14, OUTPUT);
  pinMode(pin17, OUTPUT);
  pinMode(49, OUTPUT);
  Serial.begin(9600);
  digitalWrite(d3, 1);
  digitalWrite(d4, 1);
  digitalWrite(d1, 1);
  digitalWrite(d2, 1);

  pinMode(buttom1, INPUT_PULLUP);
  pinMode(buttom2, INPUT_PULLUP);
  pinMode(buttom3, INPUT_PULLUP);
  pinMode(buttom4, INPUT_PULLUP);
  pinMode(buttom5, INPUT_PULLUP);

  pinMode(inter_buttom_pin, INPUT);

  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(inter_buttom_pin), chek_buttom, FALLING);
}

long mm = 0;

void loop() 
{
  //Serial.println(get_mm());

  //digitalWrite(led_red, 1);
  
  analogWrite(8, 10);
  analogWrite(9, 10);
  //analogWrite(10, 10);
  //analogWrite(11, 10);
  //displayAnyValue(9999);
  //displayCategory(2);
  //displayTree(2);
  //clean_digits();
  //digitalWrite(d3, 1);
  //digitalWrite(d4, 1);
  //digitalWrite(d1, 1);
  //digitalWrite(d2, 1);
 // for(int i = 0; i < 11; i++)
 // {
 //   if(digits[8][i] != 0)
 //   {
 //     digitalWrite(digits[8][i], 1);
 //   }
//    else
///{
  //    break;
   // }
 // }
  //digitalWrite(d3, 0);
  //digitalWrite(40, 1);
  //delay(4);
  //while(1);
  /*
  digitalWrite(32,  1);
  digitalWrite(33,  1);
  digitalWrite(34,  1);
  digitalWrite(35,  1);
  digitalWrite(36,  1);
  digitalWrite(37,  1);
  digitalWrite(38,  1);
  digitalWrite(39,  1);
  digitalWrite(40,  1);
  digitalWrite(41,  1);
  digitalWrite(42,  1);
  
  digitalWrite(44,  1);
  digitalWrite(A11, 1);
  digitalWrite(A12, 1);
  digitalWrite(A13, 1);
  digitalWrite(A14, 1);
  digitalWrite(A15, 1);

  digitalWrite(d3, 0);
  digitalWrite(d4, 0);
  digitalWrite(d1, 0);
  digitalWrite(d2, 0);
  */
  //delay(500);
}

void chek_buttom()
{
  if(!digitalRead(buttom5))
  {
    digitalWrite(8, !digitalRead(8));
  }
  else if(!digitalRead(buttom3))
  {
    digitalWrite(9, !digitalRead(9));
  }
  else if(!digitalRead(buttom2))
  {
    digitalWrite(10, !digitalRead(10));
  }
  else if(!digitalRead(buttom4))
  {
    digitalWrite(11, !digitalRead(11));
  }
}
