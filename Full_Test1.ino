// --------- Thermistor Variables ---------
const int numOfThermistors = 4; // Per TEC**set to correct number
int volt0;
float R1[] = {100315, 100215, 100305,100225}; // Resistance of resistor (ohms)
float R2; // Resistance of thermistor (ohms)
float tempK; // Temperature (kelvin)
float tempC; // Temperature (Celsius)
float tempF; // Temperature (Fahrenheit)
//Steinhart-Hart coefficients, thermistor dependent, calculated
float A = 0.6455327490e-03, B = 2.249456145e-04, C = 0.7778564629e-07;
float avgTemp;


const int numOfTEC = 1;
// [# of TEC][pins] = {in+, in-, Fault0, Fault1, Therm1, Therm2}
const int TEC[5][7] = {{9, 10, 38, 46, A0, A1, A2}, 
                             {11, 12, 42, 40, A1, A2, A1}, 
                             {8, 7, 39, 38, A2, A3, A1}, 
                             {6, 5, 37, 36, A3, A4, A1}};
const int HIzPin = 48;
const int ShutDownPin = 49;
int faultState;

int i=502;
enum TECStates {cold, hot};
TECStates TecState = hot;
int TecLevel; //0 to 255

void setup() {
  //TCCR1B = TCCR1B & B11111000 | B00000001;  // for PWM frequency of 31.4 kHz
  //TCCR1B = TCCR1B & B11111000 | B00000010;  // for PWM frequency of 3.92 kHz
  //TCCR0B = TCCR0B & B11111000 | B00000010; // for PWM frequency of 7.812 kHz
  TCCR0B = TCCR0B & B11111000 | B00000011; // for PWM frequency of 976.56 Hz (The DEFAULT)
  Serial.begin(9600);
  Serial.println();
  Serial.println("TecNum, Fault, TempInC");
  pinMode(HIzPin, OUTPUT);
  pinMode(ShutDownPin, OUTPUT);
  digitalWrite(HIzPin, HIGH);
  digitalWrite(ShutDownPin, HIGH);

  pinMode(TEC[0][0], OUTPUT); // In+
  pinMode(TEC[0][1], OUTPUT); // In-
  analogWrite(TEC[0][0], 255);
  analogWrite(TEC[0][1], 0);


}

void loop() {
  Serial.print(millis());
  Serial.print(", ");
  for (int i=0; i<numOfTEC; i++){
    faultState = fault(i);

    Serial.print(i+1);
    Serial.print(", ");
    Serial.print(faultState*100);
    Serial.print(", ");
    Serial.print(readTherm(i, 4));
    Serial.print(", ");
    Serial.print(readTherm(i, 5));
    Serial.print(", ");
    Serial.print(readTherm(i, 6));



    if (faultState == 3){
      Serial.println("OverTemperature (over 130C)");
      Serial.println("---SHUTDOWN---");
      delay(100);
      digitalWrite(HIzPin, LOW);
      digitalWrite(ShutDownPin, LOW);
      exit(0);

    } 
    switch (faultState){
      case 1:
        Serial.print(", ");
        Serial.print("OverCurrent (over 4A)");

        delay(500);
        break;
      case 2:
        Serial.print(", ");
        Serial.print("UnderVoltage (under 2.8V)");
        delay(500);
        break;
      case 3:
        
        break;
    }
    
    Serial.println();
  }


    /*if (i>0){
    
      if (i>251){
        TecLevel = i-252;
        TecState = cold;
      } else {
        TecLevel = 251-i;
        TecState = hot;
      }

      if ((TecLevel == 1)||(TecLevel == 2)||(TecLevel == 3)||(TecLevel == 4)||(TecLevel == 5)||(TecLevel == 6)) TecLevel=0;

      Serial.print(i);
      Serial.print(", ");


      switch (TecState){
        case cold:
          Serial.print(0);
          Serial.print(", ");
          Serial.println(TecLevel);
          //analogWrite(12, 0);
          //analogWrite(11, TecLevel);
          break;
        case hot:
          Serial.print(TecLevel);
          Serial.print(", ");
          Serial.println(0);
          //analogWrite(11, 0);
          //analogWrite(12, TecLevel);
          break;
      }
    
    }*/

    i--;
    if (i==0) i=502;
    delay(500);
    //Serial.print();
}




float readTherm(int tec, int pin) { // Reads a thermistor and returns the temp
  volt0 = analogRead(TEC[tec][pin]);  // Gets voltage from pin
  R2 = R1[pin-4] * ( (1023.0 / volt0) - 1.0); // Thermistor resistance calculation (not dependent on power supply voltage)
  tempK = 1.0 / ( A + (B*log(R2)) + (C*log(R2)*log(R2)*log(R2)) ); // Steinhart-Hart equation
  tempC = tempK - 273.15; // Kelvin to celsius conversion
  tempF = ( (tempC * 9.0) / 5.0 ) + 32.0; // Celsius to fahrenheit conversion
  return tempC;
}

int fault(int numTEC){
  int state0 = digitalRead(TEC[numTEC][2]);
  int state1 = digitalRead(TEC[numTEC][3]);

  if ((state1 == 0) && (state0 == 0)) return 1; // Overcurrent (over 4A)
  if ((state1 == 0) && (state0 == 1)) return 2; // Undervoltage (under 2.8V)
  if ((state1 == 1) && (state0 == 0)) return 3; // Overtemperature (over 130C) (IC is disabled at 190C)
  if ((state1 == 1) && (state0 == 1)) return 0; // Normal Operation
}