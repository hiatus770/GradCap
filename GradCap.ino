/*
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3
*/

// include the library code:
#include <LiquidCrystal.h>
#include <IRremote.hpp>  // include the library

const int IR_RECEIVE_PIN = 6;

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7); 

void setup() {
  Serial.begin(9600);
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  Serial.print(F("Ready to receive IR signals of protocols: "));
  printActiveIRProtocols(&Serial);
}

void irDebug(){
    if (IrReceiver.decodedIRData.protocol == UNKNOWN) {
      Serial.println(F("Received noise or an unknown (or not yet enabled) protocol"));
      // We have an unknown protocol here, print extended info
      IrReceiver.printIRResultRawFormatted(&Serial, true);
      IrReceiver.resume();  // Do it here, to preserve raw data for printing with printIRResultRawFormatted()
    } else {
      IrReceiver.resume();  // Early enable receiving of the next IR frame
      IrReceiver.printIRResultShort(&Serial);
      IrReceiver.printIRSendUsage(&Serial);
    }
    Serial.println();
}

int scrollLimit = 0; 
int currentScrollCount = 0; 
int isScrollNeeded = 0; 
String message = ""; 

void sayCongrats(){
  lcd.clear(); 
  lcd.setCursor(0, 0); 
  lcd.print(" CONGRATS CLASS "); 
  lcd.setCursor(0, 1); 
  lcd.print("    OF 2024! ");
}

void saySigma(){
  lcd.clear(); 
  message = "maybe the real academic validation was the friends we made along the way"; 
  message = "                " + message; 
  scrollLimit = message.length(); 
}

void sayDebt(){
  lcd.clear(); 
  lcd.setCursor(0, 0); 
  lcd.print("Uni Pilled"); 
  lcd.setCursor(0, 1); 
  lcd.print("   Debt-maxxer"); 
}

void sayReactions(){
  lcd.clear(); 
  String reactions = "'this is awkful' - raymond       'what the sigma' - ashley     'do not put sigma on there' - oliver     'lol' - william "; 
  message = reactions;
  Serial.println(sizeof(reactions)); 
  message = "                " + message; 
  scrollLimit = message.length(); 
}

void sayError(){
  // he he 
  lcd.clear();
  lcd.setCursor(0, 0); 
  lcd.print("    ERROR");
  lcd.setCursor(0, 1); 
  lcd.print((IrReceiver.decodedIRData.command));
}


void loop() {

  uint16_t lastCommand;

  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  if (IrReceiver.decode()) {
    irDebug(); 

    /* MAIN TEXT DISPLAY LOGIC */
    switch (IrReceiver.decodedIRData.command){
      case 0x45: // POWER BUTTON 
        sayCongrats(); 
        break; 
      case 0x44: // Skip backwards, (0, -1)
        saySigma(); 
        break; 
      case 64: // Play/pause button (1, -1) 
        sayDebt(); 
        break; 
      case 71: // Func/STop button
        sayReactions();
        break; 
      default: 
        sayError(); 
        break; 
    }

    lastCommand = IrReceiver.decodedIRData.command; 

  }
  
  // Scrolling logic
  switch (lastCommand){
    case 0x44: // Say sigma 
      delay(300); 
      lcd.clear(); 
      lcd.setCursor(0, 0); 
      lcd.print(message.substring(currentScrollCount, currentScrollCount + 16)); 
      Serial.println(message.substring(currentScrollCount, currentScrollCount + message.length() - 16)); 
      currentScrollCount++; 
      if(currentScrollCount >= scrollLimit){
        currentScrollCount = 0; 
      }
      break; 
    case 71: // Say reactions! 
      delay(300); 
      lcd.clear(); 
      lcd.setCursor(0, 0); 
      lcd.print(message.substring(currentScrollCount, currentScrollCount + 16)); 
      Serial.println(message.substring(currentScrollCount, currentScrollCount + message.length() - 16)); 
      currentScrollCount++; 
      if(currentScrollCount >= scrollLimit){
        currentScrollCount = 0; 
      }
      lcd.setCursor(0, 1); 
      lcd.print("^what ppl said^");
      break; 

  } 
}
