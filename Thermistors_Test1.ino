static const int numOfThermistors = 3; //**set to correct number
static const int analogPins[] = {A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11}; //**add other pins
static const float resisValues[] = {100315, 100215, 100305,100225};
//static const float resisValues[] = {100000, 100000, 100000, 100000, 100000, 100000, 100000, 100000};
int volt0;

float R2; // Resistance of thermistor (ohms)
float tempK; // Temperature (kelvin)
float tempC; // Temperature (Celsius)
float tempF; // Temperature (Fahrenheit)
//Steinhart-Hart coefficients, thermistor dependent, calculated
float A = 0.6455327490e-03, B = 2.249456145e-04, C = 0.7778564629e-07;
//float A=1.009249522e-03, B=2.378405444e-04, C=2.019202697e-07;

void setup() {
  Serial.begin(9600);

  Serial.println("ThermistorPin, Temp(C)");

  

}

float readTherm(int i) {
  volt0 = analogRead(analogPins[i]);  // Gets voltage from pin
  R2 = resisValues[i] * ( (1023.0 / volt0) - 1.0); // Thermistor resistance calculation (not dependent on power supply voltage)
  tempK = 1.0 / ( A + (B*log(R2)) + (C*log(R2)*log(R2)*log(R2)) ); // Steinhart-Hart equation
  tempC = tempK - 273.15; // Kelvin to celsius conversion
  tempF = ( (tempC * 9.0) / 5.0 ) + 32.0; // Celsius to fahrenheit conversion
  return tempC;
}

void loop(){
  
  float temps[4];

  Serial.print(millis());
  Serial.print(", ");
  for (int i = 0; i < numOfThermistors; i++) { // Reads all thermistor values
    
    
    temps[i]=readTherm(i);
    Serial.print(temps[i]);
    Serial.print(", ");
    

  }
  float tempB = max(max(temps[1], temps[2]), max(temps[3], temps[0]));
  float tempC = min(min(temps[1], temps[2]), min(temps[3], temps[0]));
  Serial.println("100, 0");

delay(250);
}
