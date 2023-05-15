#include<LiquidCrystal.h>
#include <HCSR04.h>

#define led 4
#define triggerPin1 6
#define triggerPin2 7
#define echoPin1 5
#define echoPin2 3
#define delayTime 60
#define delayAfterCount 100
#define deltaPass 500
#define minDist 50

int count = 0;
unsigned long time1 = 0;
unsigned long time2 = 0;
int delta = 0;

LiquidCrystal lcd(13, 12, 11, 10, 9, 8);
HCSR04 hc1(triggerPin1, echoPin1);
HCSR04 hc2(triggerPin2, echoPin2);

void setup()
{
  pinMode(led, OUTPUT);
  ledOff();
  Serial.begin(9600);
  initializeLcd();
}

void ledOff() {
  digitalWrite(led, HIGH);
}

void ledOn() {
  digitalWrite(led, LOW);
}

void initializeLcd() {
  lcd.begin(16,2);
  lcd.print("Contador de pessoas");
  delay(1000);
  lcd.clear();
  showCountOnLCD(count);
}

void showCountOnLCD(int count) {
  lcd.clear();
  lcd.print("N. pessoas: ");
  lcd.setCursor(0,1);
  lcd.print(count);
}

void loop()
{
  time1 = getTimePassenger1(time1);
  delay(delayTime);
  time2 = getTimePassenger2(time2);
  delay(delayTime);
  
  printDist();

  if (time1 > 0 && time2 > 0 ) {
    printTime(time1, time2, delta);
    delta = time2 - time1;
    if (abs(delta) < deltaPass) {
      if (time1 < time2) {
        IN();
      } else if (time2 < time1) {
        OUT();
      }
    }

    time1 = 0;
    time2 = 0;
    Serial.println("");
    delay(500);
  }

  showCountOnLCD(count);
  // printSerial(time1, time2);
}

unsigned long getTimePassenger1(unsigned long lastTime) {
  unsigned long time = lastTime;
  float dist = hc1.dist();

  time = decideTime(dist, lastTime);

  return time;
}

unsigned long decideTime(float dist, unsigned long lastTime) {
  unsigned long newTime = millis();
  int deltaLastTime;

  if (dist == 0 || dist > minDist) {
    deltaLastTime = abs(newTime - lastTime);

    if (deltaLastTime > 1000) {
      newTime = 0;
    } else {
      newTime = lastTime;
    }
  }

  return newTime;
}

unsigned long getTimePassenger2(unsigned long lastTime) {
  unsigned long time = lastTime;
  float dist = hc2.dist();

  time = decideTime(dist, lastTime);

  return time;
}

void IN()
{
    ledOn();
    count++;
    Serial.print("   IN");
}

void OUT()
{
  if (count > 0) {
    count--;
  }

  if (count == 0) {
    ledOff();
  }
  Serial.print("   OUT");
}

// void printSerial(unsigned long time1, unsigned long time2) {
//   printTime(time1, time2);
//   printDist();
// }

void printTime(unsigned long time1, unsigned long time2, int delta) {
  if (time1 || time2) {
    Serial.print("S1: ");
    Serial.print(time1);
    Serial.print("   S2: ");
    Serial.print(time2);
    Serial.print("    DIF: ");
    Serial.print(delta);
  }
}

void printDist() {
  float d1 = hc1.dist();
  float d2 = hc2.dist();

  Serial.print("S1: ");
  Serial.print(d1);
  Serial.print("cm   ");
  Serial.print("S2: ");
  Serial.print(d2);
  Serial.println("cm");
}