//Пины управления шаговиком
#define STEP_PIN     8
#define DIR_PIN      9

const int   STEPS_PER_REV = 200;   
const int   REVS_TEST     = 40;    
float       DISTANCE_CM   = 4.5;   

float cmPerRev;

void setup() {
  pinMode(STEP_PIN   , OUTPUT);        
  pinMode(DIR_PIN    , OUTPUT);
  pinMode(6    , INPUT);               
  Serial.begin(9600);

  digitalWrite(DIR_PIN, LOW);          
  cmPerRev = DISTANCE_CM / REVS_TEST;  // см / обороты = см на 1 оборот
  Serial.print("1 оборот = ");
  Serial.print(cmPerRev, 4);
  Serial.println(" см");
 
}

void loop() {
//  Serial.println(digitalRead(6));
  if (digitalRead(6) == 1){
 
    delay(30);                         
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
     

      while (digitalRead(6) == 1);     
      delay(50);                       
    }
  }

}
