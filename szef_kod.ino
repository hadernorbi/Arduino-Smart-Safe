#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const byte ROWS = 4; 
const byte COLS = 4; 
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {9, 8, 7, 6}; 
byte colPins[COLS] = {5, 4, 3, 2}; 

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
Servo safeLock;

String masterCode = "1234";
String inputCode = "";    

void setup() {
  lcd.init();
  lcd.backlight();
  safeLock.attach(10);
  safeLock.write(0); 
  showReady();
}

void showReady() {
  lcd.clear();
  lcd.print("SZEF ZARVA");
  lcd.setCursor(0, 1);
  lcd.print("PIN: ");
  inputCode = "";
}

void loop() {
  char key = keypad.getKey();
  
  if (key) {
    if (key == '#') { 
      showReady();
    } 
    else if (key == 'D') { 
      lcd.clear();
      if (inputCode == masterCode) {
        lcd.print("KOD JO!");
        lcd.setCursor(0, 1);
        lcd.print("NYITAS...");
        safeLock.write(90); 
        delay(5000);       
        safeLock.write(0);  
        showReady();
      } else {
        lcd.print("ROSSZ KOD!");
        delay(2000);
        showReady();
      }
    } 
    else { // Szám beírása
      if (inputCode.length() < 4) {
        inputCode += key;
        lcd.print("*"); 
      }
    }
  }
}
