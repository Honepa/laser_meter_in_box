// Укажем, что к каким пинам подключено
int trigPin = 2;
int echoPin = 3;
unsigned long t;
void setup() 
{
  Serial.begin (115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  t = millis();
}
unsigned long  sen_dur;
int sen_dir[100];
int gradat[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int mm, tmp, min_dur, max_dur, max_gradat, max_gradat_index;
void loop()
{
  unsigned long duration, distance;
  sen_dur = 0;
  
  for(int i = 0; i < 10; i++)
  {
    gradat[i] = 0;
  }
  for (int i = 0; i < 100; i++)
  {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    // Теперь установим высокий уровень на пине Trig
    digitalWrite(trigPin, HIGH);
    // Подождем 10 μs
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH, 5000);
    sen_dur += duration;
    sen_dir[i] = duration;
  }
  min_dur = sen_dir[0];
  max_dur = sen_dir[0];
  for(int i = 0; i < 100; i++)
  {
    if(min_dur > sen_dir[i])
    {
      min_dur = sen_dir[i];
    }
    if(max_dur < sen_dir[i])
    {
      max_dur = sen_dir[i];
    }
  }
 
  for(int i = 0; i < 100; i++)
  {
    gradat[int(map(sen_dir[i], min_dur, max_dur, 0, 10))]++;
   
  }
  max_gradat = gradat[0];
  max_gradat_index = 0;
  
  for(int i = 0; i < 10; i++)
  {
    if(max_gradat < gradat[i])
    {
      max_gradat = gradat[i];
      max_gradat_index = i;
    }
  }
  sen_dur = sen_dur / 100;
  if(sen_dur > 300)
  {
    //mm = 0.00028671 * sen_dur * sen_dur + 5.44540188 * sen_dur + 117.17137;
    mm = (-5.44540188 + sqrt(5.44540188 * 5.4450188 - 4 * 0.00028671 * (117.17137 - sen_dur))) / (2 * 0.00028671);
  }
  else
  {
    mm = sen_dur / 5.8;
  }
  Serial.print(mm);
  Serial.print("  ");

  for(int i = 0; i < 100; i++)
  {
    if(map(sen_dir[i], min_dur, max_dur, 0, 10) == max_gradat_index)
    {
      sen_dur = sen_dir[i];
    }
  }
  
  if(sen_dur > 300)
  {
    //mm = 0.00028671 * sen_dur * sen_dur + 5.44540188 * sen_dur + 117.17137;
    mm = (-5.44540188 + sqrt(5.44540188 * 5.4450188 - 4 * 0.00028671 * (117.17137 - sen_dur))) / (2 * 0.00028671);
  }
  else
  {
    mm = sen_dur / 5.8;
  }
  Serial.print(mm);
  Serial.print('\n');
  delay(1000);
}
