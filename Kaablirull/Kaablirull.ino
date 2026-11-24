#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// 1. Mikrokontrolleri, anduri ja ekraani seadistamine
LiquidCrystal_I2C lcd(0x27, 16, 2);
const byte encoderA = 2, encoderB = 3;
const byte buttonPin = 4, motorPin = 8, homePin = 7;
const float metersPerPulse = 0.005; // Kalibreerida prototüübil
const unsigned long maxRunTime = 30000;
volatile long pulses = 0;
bool rewinding = false, previousButton = HIGH;
unsigned long motorStart = 0;

// 2. Anduri impulsside lugemine
void readEncoder() {
  if (digitalRead(encoderB) == HIGH) pulses++;
  else pulses--;
}

// 3. Kaabli pikkuse arvutamine
float cableLength() {
  noInterrupts();
  long count = pulses;
  interrupts();
  return max(0.0f, count * metersPerPulse);
}

// 4. Pikkuse kuvamine
void showLength(float length) {
  lcd.setCursor(0, 0);
  lcd.print("Kaabel: ");
  lcd.print(length, 2);
  lcd.print(" m   ");
  lcd.setCursor(0, 1);
  lcd.print(rewinding ? "Tagasikerimine  " : "Ootereziim      ");
}

// 10. Mootori peatamine
void stopMotor() {
  digitalWrite(motorPin, LOW);
  rewinding = false;
}

void setup() {
  pinMode(encoderA, INPUT_PULLUP);
  pinMode(encoderB, INPUT_PULLUP);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(homePin, INPUT_PULLUP);
  pinMode(motorPin, OUTPUT);
  stopMotor();
  attachInterrupt(digitalPinToInterrupt(encoderA), readEncoder, RISING);
  lcd.init();
  lcd.backlight();
}

void loop() {
  // 2-4. Pikkuse mõõtmine ja kuvamine
  bool home = digitalRead(homePin) == LOW;
  if (home) {
    noInterrupts();
    pulses = 0;
    interrupts();
  }
  float length = cableLength();
  bool button = digitalRead(buttonPin);
  // 5. Nupu kontroll
  if (!rewinding && previousButton == HIGH && button == LOW) {
    // 6. Kas kaablit on väljas?
    if (length > 0 && !home) {
      // 7. Mootori käivitamine
      rewinding = true;
      motorStart = millis();
      digitalWrite(motorPin, HIGH);
    } else stopMotor();
  }
  previousButton = button;
  // 8-9. Pikkuse uuendamine ja lõppasendi kontroll
  if (rewinding && (home || length <= 0 ||
      millis() - motorStart >= maxRunTime)) {
    stopMotor(); // 10. Mootori peatamine
  }
  showLength(length);
  delay(50);
  // Ooterežiim: loop kordub.
}
