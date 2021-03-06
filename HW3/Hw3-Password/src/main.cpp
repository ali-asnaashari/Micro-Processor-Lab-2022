#include <Arduino.h>
#include <LiquidCrystal.h>
#include <Keypad.h>

#define Interrupt_BTN 2

const byte ROWS = 4;  //Four Rows
const byte COLS = 3; //Three Columns

char keys[ROWS][COLS] = {
{'1','2','3'},
{'4','5','6'},
{'7','8','9'},
{'*','0','#'}
};

byte rowPins[ROWS] = {13, 12, 11, 10};
byte colPins[COLS] = {9, 8, 7};
Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

const int rs = 6, en = 5, d4 = 4, d5 = 3, d6 = 1, d7 = 0;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

String password;
String enteredPassword;
String secretPassword;

volatile bool showPassword = false;
volatile bool showOnLCD = false;

void button_intr_handle(void) {
  showPassword = !showPassword;
  showOnLCD = true;
}

int getKeyPadValue() {
   if (kpd.getKeys())
    {
        for (int i=0; i<LIST_MAX; i++)
        {
            if ( kpd.key[i].stateChanged && kpd.key[i].kstate == PRESSED )
            {
              return kpd.key[i].kchar;
            }
        }
    }
    return -1;
}

void setInitialValues() {
  password = "1379";
  enteredPassword = "";
  secretPassword = "";
}

void showValueOnLCD(){
  lcd.clear();
  if (showPassword) {
    lcd.println(enteredPassword);
  } else {
    lcd.println(secretPassword);
  }
}

void setup() {
  lcd.begin(16, 2);
  setInitialValues();
  pinMode(Interrupt_BTN, INPUT);
  attachInterrupt(digitalPinToInterrupt(Interrupt_BTN), button_intr_handle, RISING);
}

void loop() {
   int value;
   if((value = getKeyPadValue()) != -1) {
     if(value >= '0' && value <= '9') {
       enteredPassword += (value - '0');
       secretPassword += "*";
       showOnLCD = true;
     }
     else if (value == '*') {
       lcd.clear();
       if (enteredPassword == password)
       {
         lcd.println("Access is Granted");
       } else {
          lcd.println("Wrong Password");
       }
       delay(1000);
       lcd.clear();
       setInitialValues();
     }
     else if (value == '#') {
       enteredPassword.remove(enteredPassword.length() - 1);
       secretPassword.remove(secretPassword.length() - 1);
       showOnLCD = true;
     }
   }
   if(showOnLCD) {
       showValueOnLCD();
       showOnLCD = false;
   }
}