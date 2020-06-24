#include <SoftwareSerial.h>
#include <Keypad.h> //keypad library
#include <LiquidCrystal.h> //lcd library

//########################Entering passwords
int m = 0;
bool finished = false;
int result[6] = {-1,-1,-1,-1,-1,-1};
bool message_set = false;
int led1 = 22; //led compartment 1
int led2 = 24; //led compartment 2
bool passwordcheck = true; //used in checking password
int compartment = 0;

//########################Entering passwords




//#######################Setting up keypad
const byte ROWS = 4; // Four rows
const byte COLS = 3; // Three columns
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'#','0','*'}
};
byte rowPins[ROWS] = { 3, 8, 7, 5 };
byte colPins[COLS] = { 4, 2, 6 }; 
Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
//#######################Setting up keypad

//#########################Setting LCD
LiquidCrystal lcd(53,51,49,47,45,43);
//#########################Setting LCD


//##########################Setting passwords
int i = 0;
int j = 0; 
bool start = false;
int password1[] = {0,0,0,0,0,0};
int password2[] = {0,0,0,0,0,0};
//##########################Setting passwords


void setup() {
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  Serial3.begin(9600); // For Bluetooth
  Serial.begin(9600); // For the IDE monitor Tools -> Serial Monitor
  lcd.begin(16,2);
  lcd.print("enter password");
  lcd.setCursor(0,1);
}

void loop() {

//##########################Setting passwords  
  int data = Serial3.read();
  data = data - 48;
  
  if(data != -49){
    if(start != true){
      if(data == 1){
        i = 1;
        Serial.println("Compartment 1");
        start = true;
      }
      else if(data == 2){
        i = 2;
        Serial.println("Compartment 2");
        start = true;       
      }
    }
    else if(start == true){
      if(i ==1){
        password1[j] = data;
        Serial.println(password1[j]);
      }
      else if(i == 2){
        password2[j] = data;
        Serial.println(password2[j]);
      }
      j = j + 1;
      if(j == 6){
        i = 0;
        start = false;
        j = 0;
      }
    }
  }
//##########################Setting passwords


//###########################Entering password
  if(finished == false)
  {
    char key = kpd.getKey();
    if(key)  // Check for a valid key.
    {
      switch(key)
      {
        case '#':
          if(m > 0){ //otherwise there is no previous number to remove
          result[m-1] = -1; //previous int to null
          m -= 1;
          lcd.setCursor(m,1);
          lcd.print(" ");
          break;
        case '*':
          finished = true;
          break;
        default:
          result[m] = key-48; //current int to key
          lcd.setCursor(m,1);
          lcd.print(key);
          m += 1; 
      }
    }
  }
  if((finished == true) && (message_set == false)){
    lcd.clear();
    lcd.print("entered password"); 
    Serial.println("Entered password");


//###########################Entering password


//############################Checking correctness

      for(int n = 0; n < 6 && passwordcheck; n++){
        Serial.println(result[n]);
        Serial.println(password1[n]);
        if(result[n]!=password1[n]) passwordcheck = false;
      }
      if(passwordcheck == true){
        compartment = 1;
      }
      passwordcheck = true;
      for(int n = 0; n < 6 && passwordcheck && compartment == 0; n++){
        if(result[n]!=password2[n]) passwordcheck = false;
      }
      if(passwordcheck == true && compartment == 0){
        compartment = 2;
      }
       
      if(passwordcheck==false){ //if the is a mistake in the password |||  if(!password_check){...} would work aswell 
          lcd.clear();
          lcd.print("wrong password, try again");
          delay(1000);
          lcd.clear();
          m = 0; //for resetting result procedure
          lcd.print("Enter password");
          finished = false;
        }
      else{
        passwordcheck = true; 
        lcd.clear();
        lcd.print("Compartment open");
        if (compartment==1){
          Serial.println("compartment 1");
          digitalWrite(led1, HIGH);
        }
        else{
          digitalWrite(led2, HIGH);
          Serial.println("compartment 2");
        }
        compartment = 0;
      }
    }
  }
}

