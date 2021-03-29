// Description: Wash station arduino software control
// Revision: 01
// Date: 03/09/2021
// Author: Roman Paz

const int airPin = 5;
const int vacPin = 6;
int airTime = 10000;
int vacTime = 5000;
const String VERSION_STR = "20210222";
String inputStr = "";
bool stringComplete = false;
bool airControl = false;
bool vacControl = false;
bool totalControl = false;

void setup() {
//  pinMode(airPin, OUTPUT);
//  pinMode(vacPin, OUTPUT);
  analogWrite(airPin, 0);
  analogWrite(vacPin, 0);
  Serial.begin(9600);
  Serial.println("Start your engines");
}

// create timenow function

void air() {
  analogWrite(airPin, 255);
  delay(airTime);
  Serial.print(" Airtime = ");
  Serial.print(airTime);
  delay(50);
  analogWrite(airPin, 0);
  delay(10);
}

void vac() {
  analogWrite(vacPin, 151);
  delay(vacTime);
  Serial.print(" Vactime = ");
  Serial.print(vacTime);
  delay(50);
  analogWrite(vacPin, 0);
  delay(10);
}

void off() {
  if (vacControl == true) {
    vacControl = false;
    delay(10);
  }
  
  if (airControl == true) {
    airControl = false;
    delay(10);   
  }
}

int parseCommandInt(String inStr) {
  int isLen = inStr.length();
  int convertedInt = -1;
  if (isLen >= 2) {
    // TODO: Need to confirm that it is actually an int o.w. toInt returns '0'
    convertedInt = inStr.substring(1, isLen).toInt();
  }
  return convertedInt;
}

String handle_command(String cmd_str) {
  String ret_str = "OK";
  switch (cmd_str.charAt(0)) {
    case 'v':
      vacControl = !vacControl;
      break;
    case 'a':
      airControl = !airControl;
      break;
    case 'o':
      totalControl = !totalControl;
      break;
    case '#':
      Serial.print("Version "); Serial.println(VERSION_STR);
      break;
    default:
      ret_str = "?";
  }
  return (ret_str);
}


void loop() {
  if (stringComplete) {
    Serial.println(handle_command(inputStr));
    inputStr = "";
    stringComplete = false;
//    report_status();
  }
  if (vacControl == true) {
    vac();
  }
  if (airControl == true) {
    air();
  }
  if (totalControl == true) {
    off();
  }
  
}

void serialEvent() {
  int num_chars_read = 0;
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char) Serial.read();
    // add it to the inputString:
    inputStr += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
