#include <Servo.h>
#include <Keypad.h>
#include <LiquidCrystal.h>

#define BUZZER A8
#define LM35 A0
#define LDR A1
#define MQ2 A2
#define RAIN A3
#define GASBUZZ 300
#define MAXTEMP 40

#define WINDOW_CLOSE 180
#define WINDOW_OPEN 60
#define GARDEN_CLOSE 0
#define GARDEN_OPEN 130
#define GARAGE_CLOSE 30
#define GARAGE_OPEN 150

int w = 0 ;
int d = 0 ;
int g = 0 ;

Servo window ;
Servo gardenDoor ;
Servo garage ;


const byte ROWS = 4;
const byte COLS = 4;

LiquidCrystal lcd(2, 3, 4, 5, 6, 7);
int c = 0;
int i = 0;
int error = 0;
char keys[ROWS][COLS] =
{ {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

char enteredPass[6];
char pass[6] = {'4', '5', '6', '4', '5', '6'};

byte rowPins[ROWS] = {29, 28, 27, 26};
byte colPins[COLS] = {25, 24, 23, 22};

int count = 0;
int f = 0;
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  window.attach(9);
  gardenDoor.attach(10);
  garage.attach(11);


  pinMode(30, OUTPUT);
  pinMode(31, OUTPUT);
  pinMode(32, OUTPUT);
  pinMode(33, OUTPUT);
  pinMode(34, OUTPUT);
  pinMode(35, OUTPUT);
  pinMode(36, OUTPUT);
  pinMode(37, OUTPUT);

  digitalWrite(30, LOW);
  digitalWrite(31, LOW);
  digitalWrite(32, LOW);
  digitalWrite(33, LOW);
  digitalWrite(34, LOW);
  digitalWrite(35, LOW);
  digitalWrite(36, LOW);
  digitalWrite(37, LOW);

  pinMode(LM35, INPUT);
  pinMode(LDR, INPUT);
  pinMode(MQ2, INPUT);
  pinMode(RAIN, INPUT);

  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, LOW);

  lcd.begin(16, 2);

  print1();

}

void print1()
{
  float val = analogRead(LM35);
  float mv = (val / 1024.0) * 5000 ;
  float temp = mv / 10 ;

  delay(500);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Smart House");
  lcd.setCursor(0, 1);
  lcd.print("temp is ");
  lcd.print(temp);
  if (temp > MAXTEMP) {
    digitalWrite(BUZZER, HIGH);
  }
  else {
    digitalWrite(BUZZER, LOW);
  }
}

void loop() {
  if (Serial.available()) {
    char data = Serial.read();
    if (data == 'A') {
      digitalWrite(32, HIGH);
    }
    else if (data == 'a') {
      digitalWrite(32, LOW);
    }
    else if (data == 'B') {
      digitalWrite(31, HIGH);
    }
    else if (data == 'b') {
      digitalWrite(31, LOW);
    }
    else if (data == 'C') {
      digitalWrite(37, HIGH);
    }
    else if (data == 'c') {
      digitalWrite(37, LOW);
    }
    else if (data == 'D') {
      digitalWrite(33, HIGH);
    }
    else if (data == 'd') {
      digitalWrite(33, LOW);
    }
    else if (data == 'E') {
      digitalWrite(34, HIGH);
    }
    else if (data == 'e') {
      digitalWrite(34, LOW);
    }
    else if (data == 'F') {
      digitalWrite(35, HIGH);
    }
    else if (data == 'f') {
      digitalWrite(35, LOW);
    }
    else if (data == 'G') {
      g = 1 ;
      digitalWrite(36, HIGH);
    }
    else if (data == 'g') {
      g = 0 ;
      digitalWrite(36, LOW);
    }
    else if (data == 'H') {
      //WINDOW OPEN
      w = 1 ;
    }
    else if (data == 'h') {
      //WINDOW CLOSE
      w = 0 ;
    }
    else if (data == 'I') {
      //GARDEN DOOR OPEN
      d = 1 ;
    }
    else if (data == 'i') {
      //GARDEN DOOR CLOSE
      d = 0 ;
    }
  }
  ///////////////////////////////////////////////
  if (w == 1) {
    window.write(WINDOW_OPEN);
  }
  else {
    window.write(WINDOW_CLOSE);
  }
  ///////
  if (d == 1) {
    gardenDoor.write(GARDEN_OPEN);
  }
  else {
    gardenDoor.write(GARDEN_CLOSE);
  }
  ///////
  if (g == 1) {
    garage.write(GARAGE_OPEN);
  }
  else {
    garage.write(GARAGE_CLOSE);
  }
  ///////////////////////////////////////////////
  ///////////sensors ///////////////
  float val = analogRead(LM35);
  float mv = (val / 1024.0) * 5000 ;
  float temp = mv / 10 ;

  int light = analogRead(LDR);
  int rain = digitalRead(RAIN);
  int gas = analogRead(MQ2);


  lcd.setCursor(0, 1);
  lcd.print("temp is ");
  lcd.print(temp);
  delay(200);

  Serial.print("temp  = ");
  Serial.print(temp);
  Serial.print(" || ");

  Serial.print("light = ");
  Serial.print(light);
  Serial.print(" || ");

  Serial.print("gas = ");
  Serial.println(gas);

  //////// sensors action //////////

  if (gas > GASBUZZ) {

    lcd.setCursor(0, 0);
    lcd.print("gas leak !!!!!!!");
    digitalWrite(BUZZER, HIGH);
  }
  /////////////////////////////////
  else if (temp > MAXTEMP) {
    digitalWrite(BUZZER, HIGH);
  }
  /////////////////////////////////
  else if (rain == 1) {
    w = 0;
    lcd.setCursor(0, 0);
    lcd.print("it is raining   ");
  }
  else {
    digitalWrite(BUZZER, LOW);
  }
  /////////////////////////////////

  if (light > 920) {
    digitalWrite(30, HIGH);
  }
  else {
    digitalWrite(30, LOW);
  }
  /////////////////////////////////

  /////////keypad///////////////

  char key = keypad.getKey();
  if (key != NO_KEY && i < 6) {
    if (f == 0) {
      delay(500);
      lcd.clear();
      f = 1;
    }
    lcd.setCursor(0, 0);
    lcd.print("Pass: ");
    lcd.setCursor(i + 7, 0);
    lcd.print("*");
    enteredPass[i] = key;
    if (enteredPass[i] == pass[i])c++;
    i++;
  }

  if (c == 6)
  {
    delay(500);
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("Correct Password");

    lcd.setCursor(0, 1);
    lcd.print("WELCOME");
    gardenDoor.write(GARDEN_OPEN);
    delay(3000);
    gardenDoor.write(GARDEN_CLOSE);

    char enteredPass[] = {'0', '0', '0', '0', '0', '0'};
    print1();
    c = 0;
    i = 0;
    f = 0;
  }

  else if (c < 6 && i == 6) {
    delay(500);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("WRONG Password");
    lcd.setCursor(0, 1);
    lcd.print("please try again");
    delay(3000);
    i = 0;
    c = 0;
    f = 0;
    error++;
    if (error < 3)
    {
      print1();
    }
    if (error == 3) {
      delay(500);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("WRONG PASSWORD");
      lcd.setCursor(0, 1);
      lcd.print("3 TIMES !!!!!!");
      for (int j = 0 ; j <= 20 ; j++) {
        digitalWrite(BUZZER, HIGH);
        delay(200);
        digitalWrite(BUZZER, LOW);
        delay(200);
      }
      print1();
    }
  }
  ////////////////////////////////////
}
