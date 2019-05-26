/*
   Creado por Ginés Cruz // 21-05-2019 ver.1
   Código original por Vasilakis Michalis
   Proyecto: Coche con Control Remoto vía Bluetooth
*/
#define BAUDRATE 9600

// --- Pins ---
// Motor
const int pinMotorA1 = 8;
const int pinMotorA2 = 9;
const int pinMotorB1 = 10;
const int pinMotorB2 = 11;
const int pinMotorENA = 3;
const int pinMotorENB = 4;

// Blinker LED
const int pinLED = 12;

// Buzzer
const int pinBuzzer = 3;

// Bluetooth State Pin
const int pinBtState = 2;

// --- Variables ---
// Delay-less delay
long previousTime = -250;
long interval = 250;
unsigned long currentTime;

// Car state
char state;
int spd = 200;
bool blinker = false;
bool blinkerOn = false;
bool buzzer = false;

// Correction factor for the second motor
const int corrFactor = 20;

void setup() {
  // Set up pins
  pinMode(pinMotorA1, OUTPUT);
  pinMode(pinMotorA2, OUTPUT);
  pinMode(pinMotorB1, OUTPUT);
  pinMode(pinMotorB2, OUTPUT);
  
  pinMode(pinLED, OUTPUT);
  pinMode(pinBtState, INPUT);

  // Begin Serial communication with Bluetooth module
  Serial.begin(BAUDRATE);
}

void loop() {
  // Car stops if the State pin reads low, which means connection was lost
  if (digitalRead(pinBtState) == LOW)
    state = 'S';

  // Read incoming data
  if (Serial.available())
    state = Serial.read();

  // Interpret incoming data
  handleCmd(state);
  handleDirection(state);
  handleSpeed(state);

  // Activate buzzer and blinker lights
  
  // Buzzer
  if(buzzer)
    tone(pinBuzzer, 1000);
  else
    noTone(pinBuzzer);

  // Blinker lights
  currentTime = millis();

  if((currentTime - previousTime > interval) && blinker)
  {
    previousTime = currentTime;
    blinkerOn = !blinkerOn;
    digitalWrite(pinLED, blinkerOn);
  }

  if(!blinker)
  {
    digitalWrite(pinLED, LOW);
  }
}

// Handles commands like honking and toggling blinkers.
void handleCmd(char c)
{
  // "B" - Blinker lights
  if(c == 'B') {
    blinker = !blinker;
  }

  // "G" - Start buzzing
  // "H" - Stop buzzing
  if(c == 'G')
    buzzer = true;
  else if (c == 'H')
    buzzer = false;
}

// Handles directional input.
void handleDirection(char d)
{
  switch (d)
  {
    // Forward
    case 'W':
      setDriverInput(1, 0, 1, 0);
      setPWM(255, 255);
      break;
      
    // Forward Left
    case 'Q':
      setDriverInput(1, 0, 1, 0);
      setPWM(255, 200);
      break;
      
    // Forward Right
    case 'E':
      setDriverInput(1, 0, 1, 0);
      setPWM(200, 255);
      break;
      
    // Back
    case 'X':
      setDriverInput(0, 1, 0, 1);
      setPWM(255, 255);
      break;
      
    // Back Left
    case 'Z':
      setDriverInput(0, 1, 0, 1);
      setPWM(200, 255);
      break;
      
    // Back Right
    case 'C':
      setDriverInput(0, 1, 0, 1);
      setPWM(255, 200);
      break;
      
    // Left
    case 'A':
      setDriverInput(0, 0, 1, 0);
      setPWM(0, 255);
      break;
      
    // Right
    case 'D':
      setDriverInput(1, 0, 0, 0);
      setPWM(255, 0);
      break;
      
    // Stop
    case 'S':
      setDriverInput(0, 0, 0, 0);
      setPWM(0, 0);
      break;
  }
}

// Handles switching between different speed modes.
void handleSpeed(char s)
{
  switch(s)
  {
    case '1':
      spd = 50;
      break;
    case '2':
      spd = 100;
      break;
    case '3':
      spd = 150;
      break;
    case '4':
      spd = 200;
      break;
    case '5':
      spd = 255;
      break;
  }
}

// Sets the input of the motor driver to control wheel direction.
void setDriverInput(bool IN1, bool IN2, bool IN3, bool IN4)
{
  digitalWrite(pinMotorA1, IN1);
  digitalWrite(pinMotorA2, IN2);
  digitalWrite(pinMotorB1, IN3);
  digitalWrite(pinMotorB2, IN4);
}

void setPWM(int ENA, int ENB)
{
  analogWrite(pinMotorENA, ENA);
  analogWrite(pinMotorENB, ENB - corrFactor);
}
