//Пины управления шаговиком
#define STEP_PIN     8
#define DIR_PIN      9

const int   STEPS_PER_REV = 200;     // шагов на оборот (полный шаг)
const float CM_PER_REV    = 0.1125;  // результат калибровки: 4.5 см / 40 оборотов

void setup() {
  pinMode(STEP_PIN   , OUTPUT);        //Настраиваем пины управления
  pinMode(DIR_PIN    , OUTPUT);
  pinMode(4    , INPUT);               //контакт щупа (HIGH при касании пластины)
  pinMode(6    , INPUT);               //кнопка старта
  Serial.begin(9600);
}

void loop() {

  if (digitalRead(6) == 1){

    int  touches   = 0;                //сколько касаний уже было
    long counted   = 0;                //шаги между первым и вторым касанием
    int  prevProbe = 0;                //что было на пине 4 в прошлый шаг

    digitalWrite(DIR_PIN, LOW);        //начальное направление

    while (touches < 2){
      digitalWrite(STEP_PIN  , HIGH);
      delay(1);
      digitalWrite(STEP_PIN  , LOW);
      delay(1);

      if (touches == 1) counted++;     //считаем только после первой смены направления

      int probe = digitalRead(4);
      if (probe == 1 && prevProbe == 0){   //момент касания пластины
        touches++;
        if (touches == 1) digitalWrite(DIR_PIN, HIGH);   //первая смена направления
        else              digitalWrite(DIR_PIN, LOW);    //вторая смена направления
      }
      prevProbe = probe;
    }

    float revs       = (float)counted / STEPS_PER_REV;   //обороты
    float distanceCm = revs * CM_PER_REV;                //путь в см
    Serial.print("Пройденное расстояние: ");
    Serial.print(distanceCm, 3);
    Serial.println(" см");

    for(long i = 0; i < counted / 2; i++){               //возврат на середину
      digitalWrite(STEP_PIN  , HIGH);
      delay(1);
      digitalWrite(STEP_PIN  , LOW);
      delay(1);
    }
  }

}
