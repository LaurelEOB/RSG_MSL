const int numOfTEC = 1;
// [# of TEC][pins] = {in+, in-, Fault0, Fault1, Therm1, Therm2, Therm3}
const int TEC[4][6] = {{12, 11, 43, 42, A0, A1}, 
                             {10, 9, 41, 40, A2, A3}, 
                             {8, 7, 39, 38, A4, A5}, 
                             {6, 5, 37, 36, A6, A7}};
const int HIzPin = 48;
const int ShutDownPin = 49;


void setup() {
  Serial.begin(9600);
  pinMode(HIzPin, OUTPUT);
  digitalWrite(HIzPin, HIGH);
  pinMode(ShutDownPin, OUTPUT);
  digitalWrite(ShutDownPin, HIGH);
  
  for (int i=0; i<numOfTEC; i++){
    pinMode(TEC[i][2], INPUT);
    pinMode(TEC[i][3], INPUT);
  }


}

void loop() {
  /*Serial.print("Fault: ");
  for (int i=0; i<numOfTEC; i++){
    Serial.print(fault(i));
    Serial.print(" ");
  }

  delay(5000);*/

}

int fault(int numTEC){
  int state0 = digitalRead(TEC[numTEC][2]);
  int state1 = digitalRead(TEC[numTEC][3]);

  if ((state1 == 0) && (state0 == 0)) return 1; // Overcurrent (over 4A)
  if ((state1 == 0) && (state0 == 1)) return 2; // Undervoltage (under 2.8V)
  if ((state1 == 1) && (state0 == 0)) return 3; // Overtemperature (over 130C) (IC is disabled at 190C)
  if ((state1 == 1) && (state0 == 1)) return 4; // Normal Operation
}