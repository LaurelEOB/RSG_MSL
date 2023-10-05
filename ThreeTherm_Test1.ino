static const int numOfThermistors = 3; //**set to correct number
static const int analogPins[] = {A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11}; //**add other pins
int volt0;
float R1 = 10000; // Resistance of resistor (ohms)
float R2; // Resistance of thermistor (ohms)
float tempK; // Temperature (kelvin)
float tempC; // Temperature (Celsius)
float tempF; // Temperature (Fahrenheit)
//Steinhart-Hart coefficients, thermistor dependent, calculated
float A = 0.6455327490e-03, B = 2.249456145e-04, C = 0.7778564629e-07;
float therm1, therm2, therm3, thermAvg;


void setup() {
  Serial.begin(9600);

  Serial.println("Time(ms), Therm1, Therm2, Therm3, Average");

}



void loop(){
  therm1 = readTherm(0);
  therm2 = readTherm(1);
  therm3 = readTherm(2);
  thermAvg = (therm1+therm2+therm3) / 3;

  Serial.print(millis());
  Serial.print(", ");
  Serial.print(therm1);
  Serial.print(", ");
  Serial.print(therm2);
  Serial.print(", ");
  Serial.print(therm3);
  Serial.print(", ");
  Serial.println(thermAvg);

  delay(500);
}


float readTherm(int i) {
  volt0 = analogRead(analogPins[i]);  // Gets voltage from pin
  R2 = R1 * ( (1023.0 / volt0) - 1.0); // Thermistor resistance calculation (not dependent on power supply voltage)
  tempK = 1.0 / ( A + (B*log(R2)) + (C*log(R2)*log(R2)*log(R2)) ); // Steinhart-Hart equation
  tempC = tempK - 273.15; // Kelvin to celsius conversion
  tempF = ( (tempC * 9.0) / 5.0 ) + 32.0; // Celsius to fahrenheit conversion
  return tempC;
  
}
