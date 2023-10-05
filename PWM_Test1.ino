enum TECStates {cold, hot};
TECStates TecState = hot;
int TecLevel; //0 to 255

int InPin1 = 11;
int InPin2 = 9;

void setup() {
  Serial.begin(9600);
  Serial.println("Start");

  /*inMode(InPin1, OUTPUT); //In+
  pinMode(InPin2, OUTPUT); //In-
  analogWrite(InPin1, 0);
  analogWrite(InPin2, 0);*/
 
  for (int i=494; i>0; i--){

    
    if (i>251){
      TecLevel = i-258;
      TecState = cold;
    } else {
      TecLevel = 251-i;
      TecState = hot;
    }

    Serial.print(i);
    Serial.print(", ");


    switch (TecState){
      case cold:
        Serial.print(0);
        Serial.print(", ");
        Serial.println(TecLevel);
        //analogWrite(InPin1, 0);
        //analogWrite(InPin2, TecLevel);
        break;
      case hot:
        Serial.print(TecLevel);
        Serial.print(", ");
        Serial.println(0);
        //analogWrite(InPin2, 0);
        //analogWrite(InPin1, TecLevel);
        break;
    }

    
    delay(10);

  }
}

void loop() {


}


