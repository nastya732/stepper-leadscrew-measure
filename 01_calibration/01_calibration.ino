//Пины управления шаговиком
#define STEP_PIN     8
#define DIR_PIN      9

const int   STEPS_PER_REV = 200;   // шагов на оборот (полный шаг, 1.8 градуса)
const int   REVS_TEST     = 40;    // сколько оборотов делаем при калибровке
float       DISTANCE_CM   = 5.0;   // ВПИШИ СЮДА то, что намерил линейкой (см)

float cmPerRev;

void setup() {
  pinMode(STEP_PIN   , OUTPUT);        //Настраиваем пины управления
  pinMode(DIR_PIN    , OUTPUT);
  pinMode(6    , INPUT);               //кнопка старта
  Serial.begin(9600);

  digitalWrite(DIR_PIN, LOW);          //направление задаём один раз (поменяй на HIGH, если едет не в ту сторону)

  cmPerRev = DISTANCE_CM / REVS_TEST;  // см / обороты = см на 1 оборот
  Serial.print("1 оборот = ");
  Serial.print(cmPerRev, 4);
  Serial.println(" см");
  Serial.println("Готов. Нажми кнопку старта.");
}

void loop() {

  if (digitalRead(6) == 1){
    delay(30);                         //антидребезг: проверяем кнопку ещё раз
    if (digitalRead(6) == 1){

      Serial.println("Старт");

      for(long i = 0; i < (long)REVS_TEST * STEPS_PER_REV; i++){
        digitalWrite(STEP_PIN  , HIGH);
        delay(1);
        digitalWrite(STEP_PIN  , LOW);
        delay(1);
      }

      Serial.print("Сделано оборотов: ");
      Serial.println(REVS_TEST);
      Serial.println("Измерь путь гайки и впиши в DISTANCE_CM");

      while (digitalRead(6) == 1);     //ждём отпускания кнопки, чтобы не поехать повторно
      delay(50);                       //пауза на дребезг при отпускании
    }
  }

}