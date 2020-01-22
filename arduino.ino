#define SENSOR 2 // порт, к которому подклюн датчик движения
bool isMoving = false; // переменная, которая будет хранить информацию о наличии движения
void setup() {
   pinMode(SENSOR, INPUT); // установка INPUT режима для порта, к которому подключен датчик
   Serial.begin(115200);
}

void loop() {
  if (digitalRead(SENSOR) == HIGH)  // Проверка движения и его изменения
  {
    Serial.println('1'); // пердача данных о движении через Serial порт
  } 
  else if(digitalRead(SENSOR) == LOW)
  {
   Serial.println('0'); // пердача данных о движении через Serial порт

   }   

}
