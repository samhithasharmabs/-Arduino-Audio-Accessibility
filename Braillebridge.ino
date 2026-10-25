/*
  BrailleBridge - Refreshable Braille Display
  -------------------------------------------
  Controller: Arduino Uno R3
  Servo drivers: 2 x PCA9685
  Driver addresses: 0x40 and 0x41

  Current implementation:
  - One six-dot Braille cell
  - Serial text input
  - Grade 1 English letter patterns A-Z
  - Space character
  - Previous, Next, Home and Reset buttons
  - Sequential servo movements to reduce simultaneous
    startup current demand
  - PCA9685 output enable control

  IMPORTANT:
  This is a firmware draft. Servo angles and mechanical
  calibration must be verified on the actual mechanism.
  The six-dot implementation uses channels 0-5 on PCA9685
  address 0x40. The second driver is reserved for expansion.
*/

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// -------------------- HARDWARE --------------------

Adafruit_PWMServoDriver driver1(0x40);
Adafruit_PWMServoDriver driver2(0x41);

const uint8_t OE_PIN = 8;

const uint8_t BUTTON_PREVIOUS = 2;
const uint8_t BUTTON_NEXT     = 3;
const uint8_t BUTTON_HOME     = 4;
const uint8_t BUTTON_RESET    = 5;

const uint8_t DOT_COUNT = 6;
const uint8_t SERVO_CHANNELS[DOT_COUNT] = {
  0, 1, 2, 3, 4, 5
};

// ---------------- SERVO CALIBRATION ----------------

// These are starting values, NOT validated mechanical
// positions. Calibrate them using the actual linkage.
// Check that the linkage cannot jam at either endpoint.

const uint8_t ANGLE_LOWERED = 60;
const uint8_t ANGLE_RAISED  = 120;

const uint16_t SERVO_MIN_PULSE = 110;
const uint16_t SERVO_MAX_PULSE = 510;

const uint16_t SERVO_FREQUENCY = 50;

// Delay between sequential servo commands.
const uint16_t SERVO_STEP_DELAY_MS = 20;

// ---------------- BRAILLE DATA ----------------

// Bit 0 = dot 1
// Bit 1 = dot 2
// Bit 2 = dot 3
// Bit 3 = dot 4
// Bit 4 = dot 5
// Bit 5 = dot 6
//
// Bit value 1 means the dot should be raised.

const uint8_t BRAILLE_A_TO_Z[26] = {
  0b000001, // A
  0b000011, // B
  0b001001, // C
  0b011001, // D
  0b010001, // E
  0b001011, // F
  0b011011, // G
  0b010011, // H
  0b001010, // I
  0b011010, // J
  0b000101, // K
  0b000111, // L
  0b001101, // M
  0b011101, // N
  0b010101, // O
  0b001111, // P
  0b011111, // Q
  0b010111, // R
  0b001110, // S
  0b011110, // T
  0b100101, // U
  0b100111, // V
  0b111010, // W
  0b101101, // X
  0b111101, // Y
  0b110101  // Z
};

// ---------------- TEXT BUFFER ----------------

const uint8_t MAX_TEXT_LENGTH = 80;

char textBuffer[MAX_TEXT_LENGTH + 1];
uint8_t textLength = 0;
uint8_t currentPosition = 0;

// ---------------- BUTTON STATE ----------------

const uint8_t BUTTON_COUNT = 4;

const uint8_t BUTTON_PINS[BUTTON_COUNT] = {
  BUTTON_PREVIOUS,
  BUTTON_NEXT,
  BUTTON_HOME,
  BUTTON_RESET
};

bool lastReading[BUTTON_COUNT] = {
  HIGH, HIGH, HIGH, HIGH
};

bool stableReading[BUTTON_COUNT] = {
  HIGH, HIGH, HIGH, HIGH
};

unsigned long lastChangeTime[BUTTON_COUNT] = {
  0, 0, 0, 0
};

const unsigned long DEBOUNCE_MS = 40;

// ---------------- SERVO HELPERS ----------------

uint16_t angleToPulse(uint8_t angle) {
  angle = constrain(angle, 0, 180);

  return map(
    angle,
    0,
    180,
    SERVO_MIN_PULSE,
    SERVO_MAX_PULSE
  );
}

void setDotAngle(uint8_t dot, uint8_t angle) {
  if (dot >= DOT_COUNT) {
    return;
  }

  driver1.setPWM(
    SERVO_CHANNELS[dot],
    0,
    angleToPulse(angle)
  );
}

void setAllDots(uint8_t angle) {
  for (uint8_t dot = 0; dot < DOT_COUNT; dot++) {
    setDotAngle(dot, angle);
  }
}

void moveBraillePattern(uint8_t pattern) {
  Serial.print(F("Braille mask: "));
  Serial.println(pattern, BIN);

  // Move one actuator at a time to reduce simultaneous
  // current demand during pattern changes.
  for (uint8_t dot = 0; dot < DOT_COUNT; dot++) {
    bool shouldRaise = pattern & (1U << dot);

    uint8_t targetAngle = shouldRaise
      ? ANGLE_RAISED
      : ANGLE_LOWERED;

    setDotAngle(dot, targetAngle);

    delay(SERVO_STEP_DELAY_MS);
  }
}

void moveHome() {
  Serial.println(F("Moving to home pattern."));
  moveBraillePattern(0);
}

// ---------------- CHARACTER MAPPING ----------------

bool characterToBraille(char input, uint8_t &pattern) {
  if (input >= 'a' && input <= 'z') {
    input = input - 'a' + 'A';
  }

  if (input >= 'A' && input <= 'Z') {
    pattern = BRAILLE_A_TO_Z[input - 'A'];
    return true;
  }

  if (input == ' ') {
    pattern = 0;
    return true;
  }

  // Unsupported punctuation and numbers are rejected
  // rather than silently mapped to incorrect patterns.
  return false;
}

void displayCurrentCharacter() {
  if (textLength == 0) {
    Serial.println(F("No text loaded."));
    moveHome();
    return;
  }

  if (currentPosition >= textLength) {
    currentPosition = 0;
  }

  char character = textBuffer[currentPosition];
  uint8_t pattern = 0;

  Serial.print(F("Position "));
  Serial.print(currentPosition + 1);
  Serial.print(F("/"));
  Serial.print(textLength);
  Serial.print(F(" | Character: "));
  Serial.println(character);

  if (characterToBraille(character, pattern)) {
    moveBraillePattern(pattern);
  } else {
    Serial.println(
      F("Unsupported character. Displaying blank cell.")
    );
    moveBraillePattern(0);
  }
}

// ---------------- SERIAL INPUT ----------------

void clearTextBuffer() {
  textLength = 0;
  currentPosition = 0;
  textBuffer[0] = '\0';

  Serial.println(F("Text buffer cleared."));
  moveHome();
}

void acceptSerialText() {
  while (Serial.available() > 0) {
    char input = Serial.read();

    if (input == '\r') {
      continue;
    }

    if (input == '\n') {
      if (textLength > 0) {
        textBuffer[textLength] = '\0';
        currentPosition = 0;

        Serial.print(F("Received text: "));
        Serial.println(textBuffer);

        displayCurrentCharacter();
      }

      return;
    }

    if (textLength < MAX_TEXT_LENGTH) {
      textBuffer[textLength++] = input;
    } else {
      Serial.println(
        F("Buffer full. Extra characters ignored.")
      );
    }
  }
}

// ---------------- BUTTON HANDLING ----------------

bool buttonPressed(uint8_t index) {
  bool reading = digitalRead(BUTTON_PINS[index]);
  unsigned long now = millis();

  if (reading != lastReading[index]) {
    lastReading[index] = reading;
    lastChangeTime[index] = now;
  }

  if (now - lastChangeTime[index] >= DEBOUNCE_MS) {
    if (reading != stableReading[index]) {
      stableReading[index] = reading;

      // INPUT_PULLUP: LOW means pressed.
      if (stableReading[index] == LOW) {
        return true;
      }
    }
  }

  return false;
}

void handleButtons() {
  if (buttonPressed(0)) {
    if (textLength > 0 && currentPosition > 0) {
      currentPosition--;
      displayCurrentCharacter();
    } else {
      Serial.println(F("Already at the first character."));
    }
  }

  if (buttonPressed(1)) {
    if (textLength > 0 &&
        currentPosition + 1 < textLength) {
      currentPosition++;
      displayCurrentCharacter();
    } else {
      Serial.println(F("Already at the last character."));
    }
  }

  if (buttonPressed(2)) {
    currentPosition = 0;

    if (textLength > 0) {
      displayCurrentCharacter();
    } else {
      moveHome();
    }
  }

  if (buttonPressed(3)) {
    clearTextBuffer();
  }
}

// ---------------- SETUP ----------------

void setup() {
  Serial.begin(115200);
  Wire.begin();

  pinMode(OE_PIN, OUTPUT);

  // PCA9685 OE is active LOW.
  // Keep servo outputs disabled during initialisation.
  digitalWrite(OE_PIN, HIGH);

  for (uint8_t i = 0; i < BUTTON_COUNT; i++) {
    pinMode(BUTTON_PINS[i], INPUT_PULLUP);
  }

  Serial.println();
  Serial.println(F("BrailleBridge firmware starting."));

  driver1.begin();
  driver2.begin();

  driver1.setPWMFreq(SERVO_FREQUENCY);
  driver2.setPWMFreq(SERVO_FREQUENCY);

  delay(10);

  // Prepare the six channels at their initial lowered
  // command positions before enabling the outputs.
  setAllDots(ANGLE_LOWERED);

  // Enable outputs after initialisation.
  digitalWrite(OE_PIN, LOW);

  delay(500);

  Serial.println(F("Initialisation complete."));
  Serial.println(F("Enter text in Serial Monitor."));
  Serial.println(F("Set baud rate to 115200."));
  Serial.println(F("Select Newline as the line ending."));
  Serial.println(F("Supported input: A-Z and spaces."));
  Serial.println(F("Buttons: Previous, Next, Home, Reset."));
}

// ---------------- MAIN LOOP ----------------

void loop() {
  acceptSerialText();
  handleButtons();
}