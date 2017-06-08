#include <SevenSeg.h>
#include <SimpleTimer.h>

// Number to display
int num;
//decimal Place
//int dp = 0;
// display brightness
int brightness = 100;
int pause = 100;
int i;

// Digit Delay & refresh Rate
const long int RefreshRate = 500;
const int DigitDelay = (1/(17 * RefreshRate)) * 1000000;

// Displays number of digits
const int dThreeDigits = 3;
const int dTwoDigits = 2;
const int dOneDigit = 1;

// Variable for incoming serial data
byte serialData;
const char delimiter = '~'; //Serial data delimiter

// Data variables
unsigned int carspeed;
unsigned int gear;
unsigned int fuel;
byte speeddata = 0;
byte geardata = 0;
byte fueldata = 0;

// Define pins
// Brightness Pot pin
// const int potPin = 0;
const int segA = 42;
const int segB = 40;
const int segC = 38;
const int segD = 36;
const int segE = 43;
const int segF = 41;
const int segG = 39; 
const int segDP = 37;

// Brake Bias Digits
const int dBbD1 = 22;
const int dBbD2 = 24;
const int dBbD3 = 26;

// Traction Control Digits
const int dTcD1 = 28;
const int dTcD2 = 23;

// ABS Digits
const int dAbsD1 = 25;
const int dAbsD2 = 27;

// Engine Map digit
const int dMapD1 = 29;

// Fuel per Lap digits
const int dFplD1 = 31;
const int dFplD2 = 33;
const int dFplD3 = 35;

// Fuel Remaining Digits
const int dFtgD1 = 30;
const int dFtgD2 = 32;
const int dFtgD3 = 34;

// Laps to go Digits
const int dLtgD1 = 19;
const int dLtgD2 = 20;
const int dLtgD3 = 21;

SimpleTimer timer;

SevenSeg dBrakeBias(segA, segB, segC, segD, segE, segF, segG);
SevenSeg dTractionControl(segA, segB, segC, segD, segE, segF, segG);
SevenSeg dABS(segA, segB, segC, segD, segE, segF, segG);
SevenSeg dMap(segA, segB, segC, segD, segE, segF, segG);
SevenSeg dLapsToGo(segA, segB, segC, segD, segE, segF, segG);
SevenSeg dFuelPerLap(segA, segB, segC, segD, segE, segF, segG);
SevenSeg dFuelLeft(segA, segB, segC, segD, segE, segF, segG);

const int dBbDigitPins[dThreeDigits]={dBbD1, dBbD2, dBbD3};
const int dTcDigitPins[dTwoDigits]={dTcD1, dTcD2};
const int dMapDigitPins[dOneDigit]={dMapD1};
const int dAbsDigitPins[dTwoDigits]={dAbsD1, dAbsD2};
const int dLtgDigitPins[dThreeDigits]={dLtgD1, dLtgD2, dLtgD3};
const int dFplDigitPins[dThreeDigits]={dFplD1, dFplD2, dFplD3};
const int dFtgDigitPins[dThreeDigits]={dFtgD1, dFtgD2, dFtgD3};

void incDisplay() {
  i++;
  if (i > 999) {i = 0;}
/*
  dBrakeBias.write(i);
  dBrakeBias.clearDisp();
  dTractionControl.write(i);
  dTractionControl.clearDisp();
  dABS.write(i);
  dABS.clearDisp();
  dMap.write(i);
  dMap.clearDisp();
  dLapsToGo.write(i);
  dLapsToGo.clearDisp();
  dFuelPerLap.write(i);
  dFuelPerLap.clearDisp();
  dFuelLeft.write(i);
  dFuelLeft.clearDisp();
*/  
  Serial.print("Pot value = ");
//  Serial.println(val);
  Serial.print("brightness Value = ");
  Serial.println(brightness);  
}


void setup() {
  // put your setup code here, to run once:
timer.setInterval(pause, incDisplay);

Serial.begin(9600);

// Pin Setup
// pinMode(potPin, INPUT);
pinMode(dBbD1, OUTPUT);
pinMode(dBbD2, OUTPUT);
pinMode(dBbD3, OUTPUT);
pinMode(dTcD1, OUTPUT);
pinMode(dTcD2, OUTPUT);
pinMode(dAbsD1, OUTPUT);
pinMode(dAbsD2, OUTPUT);
pinMode(dMapD1, OUTPUT);
pinMode(dFplD1, OUTPUT);
pinMode(dFplD2, OUTPUT);
pinMode(dFplD3, OUTPUT);
pinMode(dFtgD1, OUTPUT);
pinMode(dFtgD2, OUTPUT);
pinMode(dFtgD3, OUTPUT);
pinMode(dLtgD1, OUTPUT);
pinMode(dLtgD2, OUTPUT);
pinMode(dLtgD3, OUTPUT);

// Define Displays
dBrakeBias.setDigitPins(dThreeDigits, dBbDigitPins);
dTractionControl.setDigitPins(dTwoDigits, dTcDigitPins);
dABS.setDigitPins(dTwoDigits, dAbsDigitPins);
dMap.setDigitPins(dOneDigit, dMapDigitPins);
dLapsToGo.setDigitPins(dThreeDigits, dLtgDigitPins);
dFuelPerLap.setDigitPins(dThreeDigits, dFplDigitPins);
dFuelLeft.setDigitPins(dThreeDigits, dFtgDigitPins);

dBrakeBias.setCommonCathode();
dTractionControl.setCommonCathode();
dABS.setCommonCathode();
dMap.setCommonCathode();
dLapsToGo.setCommonCathode();
dFuelPerLap.setCommonCathode();
dFuelLeft.setCommonCathode();

dBrakeBias.setDPPin(segDP);
dTractionControl.setDPPin(segDP);
dABS.setDPPin(segDP);
dMap.setDPPin(segDP);
dLapsToGo.setDPPin(segDP);
dFuelPerLap.setDPPin(segDP);
dFuelLeft.setDPPin(segDP);

dBrakeBias.setDigitDelay(DigitDelay);
dTractionControl.setDigitDelay(DigitDelay);
dABS.setDigitDelay(DigitDelay);
dMap.setDigitDelay(DigitDelay);
dLapsToGo.setDigitDelay(DigitDelay);
dFuelPerLap.setDigitDelay(DigitDelay);
dFuelLeft.setDigitDelay(DigitDelay);

dBrakeBias.setRefreshRate(RefreshRate);
dTractionControl.setRefreshRate(RefreshRate);
dABS.setRefreshRate(RefreshRate);
dMap.setRefreshRate(RefreshRate);
dLapsToGo.setRefreshRate(RefreshRate);
dFuelPerLap.setRefreshRate(RefreshRate);
dFuelLeft.setRefreshRate(RefreshRate);

}

void loop() {
  // put your main code here, to run repeatedly:
timer.run();

dBrakeBias.write(i);
dBrakeBias.setDutyCycle(brightness);
dBrakeBias.clearDisp();

dTractionControl.write(i);
dTractionControl.setDutyCycle(brightness);
dTractionControl.clearDisp();

dABS.write(i);
dABS.setDutyCycle(brightness);
dABS.clearDisp();

dMap.write(i);
dMap.setDutyCycle(brightness);
dMap.clearDisp();

dLapsToGo.write(i);
dLapsToGo.setDutyCycle(brightness);
dLapsToGo.clearDisp();

dFuelPerLap.write(i);
dFuelPerLap.setDutyCycle(brightness);
dFuelPerLap.clearDisp();

dFuelLeft.write(i);
dFuelLeft.setDutyCycle(brightness);
dFuelLeft.clearDisp();

}
