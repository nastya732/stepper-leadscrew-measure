//Пины управления шаговиком
#define STEP_PIN     8
#define DIR_PIN      9

const int   STEPS_PER_REV = 200;     // шагов на оборот (полный шаг)
const float CM_PER_REV    = 0.125;   // ВПИШИ результат калибровки из кода №1
const int   LOCKOUT_STEPS = 100;     // после смены направления контакт игнорируем
const long  MAX_STEPS     = 200000L; // защита: если контакт не сработал, остановиться

void setup() {
  pinMode(STEP_PIN   , OUTPUT);        //Настраиваем пины управления
  pinMode(DIR_PIN    , OUTPUT);
  pinMode(4    , INPUT);               //контакт щупа (HIGH при касании пластины)
  pinMode(6    , INPUT);               //кнопка старта
  Serial.begin(9600);
  Serial.println("Готов. Нажми кнопку старта.");
}

void loop() {

  if (digitalRead(6) == 1){
    while (digitalRead(6) == 1);       //ждём отпускания кнопки
    delay(50);

    int  touches    = 0;               //сколько касаний уже было
    long counted    = 0;               //шаги между первым и вторым касанием
    long total      = 0;               //всего шагов (для защиты)
    long sinceTouch = LOCKOUT_STEPS;   //шагов с последней смены направления
    int  prevProbe  = digitalRead(4);

    digitalWrite(DIR_PIN, LOW);        //начальное направление (к первой пластине)
    delay(1);

    while (touches < 2 && total < MAX_STEPS){
      digitalWrite(STEP_PIN  , HIGH);
      delay(1);
      digitalWrite(STEP_PIN  , LOW);
      delay(1);

      total++;
      sinceTouch++;
      if (touches == 1) counted++;     //считаем только после первой смены направления

      int probe = digitalRead(4);
      //реагируем на момент касания (LOW -> HIGH), а не на удержание контакта
      if (probe == 1 && prevProbe == 0 && sinceTouch >= LOCKOUT_STEPS){
        touches++;
        if (touches == 1) digitalWrite(DIR_PIN, HIGH);   //первая смена направления
        else              digitalWrite(DIR_PIN, LOW);    //вторая смена направления
        sinceTouch = 0;
        Serial.print("Касание №");
        Serial.println(touches);
      }
      prevProbe = probe;
    }

    if (touches < 2){
      Serial.println("Ошибка: второе касание не произошло, остановка");
    }
    else {
      float revs       = (float)counted / STEPS_PER_REV;
      float distanceCm = revs * CM_PER_REV;
      Serial.print("Шагов: ");
      Serial.println(counted);
      Serial.print("Оборотов: ");
      Serial.println(revs, 2);
      Serial.print("Пройденное расстояние: ");
      Serial.print(distanceCm, 3);
      Serial.println(" см");

      //возвращаемся на середину: едем половину пути в уже сменённом направлении
      for(long i = 0; i < counted / 2; i++){
        digitalWrite(STEP_PIN  , HIGH);
        delay(1);
        digitalWrite(STEP_PIN  , LOW);
        delay(1);
      }
      Serial.println("Щуп вернулся на середину.");
    }
  }

}