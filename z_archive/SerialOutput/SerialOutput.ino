/*==========================================================================================================*/
/*		2CP - TeamEscape - Engineering
 *		by Abdullah Saei
 *
 *		- It just generats a continuous serial output for testing logging script
 */
/*==========================================================================================================*/

const String title = String("Serial Output");
const String brainName = "Br1: ";
unsigned int delayTime = 1000;
unsigned long int now = millis();
unsigned int entryId = 0;
String logEntry;
const int readPin =  A0;      // the number of the LED pin

void setup() {

  Serial.begin(115200);
	delay(10);
  // initialize the read pin as an output:
  pinMode(readPin, OUTPUT);
  // turn Write mode on:
  digitalWrite(readPin, HIGH);
  print_logo_infos(title);
  Serial.println("Your Logger is working");
  // turn Write mode off:
  delay(50);
  digitalWrite(readPin, LOW);
  delay(20);
}

/*============================================================================================================
//===LOOP=====================================================================================================
//==========================================================================================================*/

void loop() {
    logEntry = createLogEntry();
    // turn Write mode on:
    digitalWrite(readPin, HIGH);
    Serial.println(logEntry);
    // turn Write mode off:
    delay(50);
    digitalWrite(readPin, LOW);
    delay(delayTime);
}

/*============================================================================================================
//===BASICS===================================================================================================
//==========================================================================================================*/
void print_logo_infos(String progTitle) {
  Serial.println();
  Serial.println(F("+-----------------------------------+"));
  Serial.println(F("|    TeamEscape HH&S ENGINEERING    |"));
  Serial.println(F("+-----------------------------------+"));
  Serial.print(brainName);
  Serial.println(progTitle);
  Serial.println();
}

String createLogEntry()
{
  String logEntry;
  now = millis();
  entryId ++; 
  logEntry = brainName + String(entryId) + " , " + String(now) + " , "  + " , " + " Done. ";
  return logEntry;
}
