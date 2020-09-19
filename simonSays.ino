#define LED_STATE 1
#define BUTTON_STATE 2
#define VERIFY_STATE 3

#define BTN1 11
#define BTN2 7
#define BTN3 2

#define LED1 12
#define LED2 8
#define LED3 3

#define MAX_LEVEL 10
uint8_t led[MAX_LEVEL] = {};
uint8_t buton[MAX_LEVEL] = {};


void setup() {
  pinMode(BTN1, INPUT_PULLUP);
  pinMode(BTN2, INPUT_PULLUP);
  pinMode(BTN3, INPUT_PULLUP);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);

  randomSeed(analogRead(A0));
  Serial.begin(9600);
}
unsigned long timp = 0;
uint8_t level = 1;
uint8_t stare = LED_STATE;

void loop() {
  if (stare == LED_STATE) {
    generateRandomLed(level);
    lightLed(level);
  }
  else if (stare == BUTTON_STATE) {
    controlLed(LOW);
    readButtons(level);
  }
  else if (stare == VERIFY_STATE)
  {
    verifyLevel();
  }
}

void generateRandomLed(uint8_t l) {
  if (l == MAX_LEVEL + 1)
    l = MAX_LEVEL;
  for (uint8_t i = 0; i < l; i++) {
    uint8_t t = random(0, 3);
    if (t == 0)
      led[i] = LED1;
    else if (t == 1)
      led[i] = LED2;
    else
      led[i] = LED3;
  }
}

void readButtons(uint8_t l) {
  for (uint8_t i = 0; i < MAX_LEVEL; i++) {
    buton[i] = 0;
  }
  static bool apasat1 = false;
  static bool apasat2 = false;
  static bool apasat3 = false;
  uint8_t i = 0;
  while (i < l) {
    if (digitalRead(BTN1) == LOW && apasat1 == false) {
      apasat1 = true;
    }
    if (digitalRead(BTN1) == LOW && apasat1 == true && buton[i] == 0) {
      buton[i] = LED1;
      delay(100);
    }
    if (digitalRead(BTN1) == HIGH && apasat1 == true) {
      apasat1 = false;
      i++;
    }

    if (digitalRead(BTN2) == LOW && apasat2 == false) {
      apasat2 = true;
    }
    if (digitalRead(BTN2) == LOW && apasat2 == true && buton[i] == 0) {
      buton[i] = LED2;
      delay(100);
    }
    if (digitalRead(BTN2) == HIGH && apasat2 == true) {
      apasat2 = false;
      i++;
    }

    if (digitalRead(BTN3) == LOW && apasat3 == false) {
      apasat3 = true;
    }
    if (digitalRead(BTN3) == LOW && apasat3 == true && buton[i] == 0) {
      buton[i] = LED3;
      delay(100);
    }
    if (digitalRead(BTN3) == HIGH && apasat3 == true) {
      apasat3 = false;
      i++;
    }
  }//while
  stare = VERIFY_STATE;
}

void lightLed(uint8_t l) {
  for (uint8_t i = 0; i < l; i++) {
    digitalWrite(led[i], HIGH);
    delay(500);
    digitalWrite(led[i], LOW);
    delay(500);
  }
  stare = BUTTON_STATE;
  controlLed(LOW);
}

void controlLed(uint8_t v) {
  digitalWrite(LED1, v);
  digitalWrite(LED2, v);
  digitalWrite(LED3, v);
}

void levelTransition() {
  for (uint8_t u = 0; u < 3; u++) {
    controlLed(LOW);
    delay(100);
    controlLed(HIGH);
    delay(200);
  }
  controlLed(LOW);
  delay(1000);
}

void testPrint() {
  //afisare leduri
  Serial.print("leduri:  ");
  for (uint8_t i = 0; i < level; i++) {
    Serial.print(led[i]);
    Serial.print(", ");
  }

  //afisare butoane
  Serial.print("\nbutoane: ");
  for (uint8_t i = 0; i < level; i++) {
    Serial.print(buton[i]);
    Serial.print(", ");
  }
  Serial.print("\n");
}

void verifyLevel() {
  bool corect = true;
  for (uint8_t i = 0; i < level; i++) {
    if (led[i] != buton[i])
      corect = false;
  }
  if (corect == false) {
    level = 1;
  }
  else if (corect == true) {
    level++;
  }
  stare = LED_STATE;
  Serial.print("level este ");
  Serial.println(level);
  levelTransition();
}
