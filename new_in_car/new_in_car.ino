#include <Key.h>
#include <Keypad.h>
#include <SevenSeg.h>
#include <SimpleTimer.h>

/*
Physical Plug Configuration
Segment Pins: 42
Bord Three Digits: 34
Board Two Digits: 19
Board One Digits: 22

Button Matrix: 50
Brightness Pot: A0
Toggle Switches: 16
*/

// Number to display
//int num;
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

// 7 Seg pins
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

// Toggle Switches
const int PinTcToggle = 16;
const int PinLightsToggle = 17;
const int PinIgnitionToggle = 18;

// Brightness pin and value variable
const int pBrightness = 0;
int val;

// Button Keys
// Keypad outputs
const char incBB = 'Q'; 
const char decBB = 'P'; 
const char incTC = 'V';
const char decTC = 'U'; 
const char incABS = '['; 
const char decABS = 'Z';
const char incMap = ';';
const char decMap = '.'; 
const char kReset = 'R';
const char kCombo1 = 'S'; 
const char kCombo2 = 'W';
const char kCombo3 = 'X';
const char kCombo4 = ']';
const char kCombo5 = 'Y';  
const char kStarter = 'T';
const char tTC = '!';
const char tLights = '@';
const char tIgnition = '#';

// Toggle Switch States
int tTcState;
int tLightsState;
int tIgnitionState;

// Define Button Matrix
const byte ROWS = 3; // Four rows  //24 Buttons
const byte COLS = 5; // six columns
// Define the Keymap
char hexaKeys[ROWS][COLS] = {
      {'P','Q','R','S','T'}, //Button connnected to PIN 50 GREEN
      {'U','V','W','X','Y'}, //Button connnected to PIN 48 BROWN
      {'Z','[',']',';','.'}, //Button connnected to PIN 46 YELLOW
     
//PIN:  44    51      49    47  45
//PIN:  BLUE  ORANGE  WHITE RED BLACK

};
byte rowPins[ROWS] = {50,48,46};
byte colPins[COLS] = {44,51,49,47,45}; 

// For Testing
SimpleTimer timer;
String currentBB;
unsigned long previousMillis = 0;
int state;

SevenSeg dBrakeBias(segA, segB, segC, segD, segE, segF, segG);
SevenSeg dTractionControl(segA, segB, segC, segD, segE, segF, segG);
SevenSeg dABS(segA, segB, segC, segD, segE, segF, segG);
SevenSeg dMap(segA, segB, segC, segD, segE, segF, segG);
SevenSeg dLapsToGo(segA, segB, segC, segD, segE, segF, segG);
SevenSeg dFuelPerLap(segA, segB, segC, segD, segE, segF, segG);
SevenSeg dFuelLeft(segA, segB, segC, segD, segE, segF, segG);
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS );

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
  /*
  Serial.print("Pot value = ");
//  Serial.println(val);
  Serial.print("brightness Value = ");
  Serial.println(brightness);  
*/
}


void setup() {
  // put your setup code here, to run once:
timer.setInterval(pause, incDisplay);

Serial.begin(9600);

// Pin Setup
pinMode(pBrightness, INPUT);
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
pinMode(PinTcToggle, INPUT);
pinMode(PinLightsToggle, INPUT);
pinMode(PinIgnitionToggle, INPUT);

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

// Need to get the Toggle switch states, Compare them to iRacing and then switch until iRacing matches Toggle switch states.

timer.run();

tTcState = digitalRead(PinTcToggle);
tLightsState = digitalRead(PinLightsToggle);
tIgnitionState = digitalRead(PinIgnitionToggle);

unsigned long currentMillis = millis();
if (currentMillis - previousMillis >= 2000) {
  previousMillis = currentMillis;
  Serial.print("Traction Control: ");
  Serial.println(tTcState);
  Serial.print("Headlights: ");
  Serial.println(tLightsState);
  Serial.print("Ignition State: ");
  Serial.println(tIgnitionState);
  Serial.println("");
}

 // read the state of the pushbutton value:Â 
char key = customKeypad.getKey();
if (key != NO_KEY){
  switch(key) {
    Serial.println(key);
    case incBB:
      currentBB = "Brake Bias +";
    break;
    case decBB:
      currentBB = "Brake Bias -";
      break;
    case incTC:
      currentBB = "Traction COntrol +";
    break;
    case decTC:
      currentBB = "Traction COntrol -";
      break;
    case incABS:
      currentBB = "ABS +";
      break;
    case decABS:
      currentBB = "ABS -";
    break;
    case incMap:
      currentBB = "Engine Map +";
    break;
   case decMap:
    currentBB = "Engine Map -";
    break;
    case kReset:
      currentBB = "Reset";
      break;
    case kCombo1:
      currentBB = "Combo 1";
      break;
    case kCombo2:
      currentBB = "Combo 2";
      break;
    case kCombo3:
      currentBB = "Combo 3";
      break;
    case kCombo4:
      currentBB = "Combo 4";
      break;
    case kCombo5:
      currentBB = "Combo 5";
      break;
    case kStarter:
      currentBB = "Starter";
      break;
  }  
  Serial.print("Keypad output: ");
  Serial.println(key);
  Serial.print("Control pressed: ");
  Serial.println(currentBB);
  }

// Set Brightness
val = analogRead(pBrightness);
brightness = map(val, 0, 1023, 0, 100);

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
