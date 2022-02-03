#include <SPI.h>
#include <SD.h>

const int PIN_CHIP_SELECT = 53;

File tree;

void setup() {
  Serial.begin(9600);
  Serial.print("Initializing SD card...");
 // Этот пин обязательно должен быть определен как OUTPUT
  pinMode(10, OUTPUT);

  // Пытаемся проинициализировать модуль
  if (!SD.begin(PIN_CHIP_SELECT)) {
    Serial.println("Card failed, or not present");
    // Если что-то пошло не так, завершаем работу:
    while(1);
  }
  Serial.println("card initialized.");
}

void loop() {
  // Строка с данными, которые мы поместим в файл:
  
  tree = SD.open("tree.csv", FILE_WRITE);

  tree.println("Arduino, file, write");
  tree.println("tree, tree, tree");
  tree.println("metr");
  
  tree.close();
  
  Serial.println("files write");
  while(1);
}
