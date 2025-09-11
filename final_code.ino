// Pin assignments
const int RED = 11;
const int YELLOW = 10;
const int GREEN = 9;
const int BLUE1 = 6;
const int BLUE2 = 5;
const int BUTTON = 2;
const int POTENTIOMETER = A0;

// Mode control
int mode = 0;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;
int lastButtonState = HIGH;   // HIGH is off for this button
int buttonState = HIGH;   // HIGH is off for this button

// Timing for LED updates
unsigned long previousTime = 0;
const unsigned long timeDelay = 500; // 1 second delay for traffic light
int step = 0;

void setup() {
  pinMode(RED, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE1, OUTPUT);
  pinMode(BLUE2, OUTPUT);
  pinMode(BUTTON, INPUT);
  pinMode(POTENTIOMETER, INPUT);
}

// Brightness Function
// Takes variable brightness as determined by the potentiometer
void setBrightnessForAllLEDs (int brightness) {
  analogWrite(RED, brightness);
  analogWrite(YELLOW, brightness);
  analogWrite(GREEN, brightness);
  analogWrite(BLUE1, brightness);
  analogWrite(BLUE2, brightness);
}


void loop() {
  handleButtonPress();
  unsigned long currentTime = millis();
  if (currentTime - previousTime >= timeDelay) {   // handles time delays without using delay()
    previousTime = currentTime;
    runModes();
  }
}

void handleButtonPress() {    // checks for button press
  int reading = digitalRead(BUTTON);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {    // prevents long button presses from being read as multiple presses
    if (reading != buttonState) {   // checks that button was pressed
      buttonState = reading;
      if (buttonState == LOW) {   // increments mode if button is pressed
        mode = (mode + 1) % 6;
        step = 0;
        allOff(); // turns the LEDs off at the end of each mode
      }
    }
  }

  lastButtonState = reading;
}

void runModes() {   // calls the modes based on the value of the mode variable
  switch (mode) {
    case 0: allOff(); break;
    case 1: allOn(); break;
    case 2: trafficLights(); break;
    case 3: leftBlink(); break;
    case 4: rightBlink(); break;
    case 5: hazardLights(); break;
  }
}

void allOn() { // turns all the LEDs on
  digitalWrite(RED, HIGH);
  digitalWrite(YELLOW, HIGH);
  digitalWrite(GREEN, HIGH);
  digitalWrite(BLUE1, HIGH);
  digitalWrite(BLUE2, HIGH);

  int potentiometerValue = analogRead(POTENTIOMETER); // reads value of potentiometer
  int brightness = map(potentiometerValue, 0, 1023, 1, 255); // converts the potentiometer range to the arduino brightness range
  setBrightnessForAllLEDs(brightness); // uses brightness function to set the brightness for all LEDs
}

void allOff() {    // turns all the LEDs off
  digitalWrite(RED, LOW);
  digitalWrite(YELLOW, LOW);
  digitalWrite(GREEN, LOW);
  digitalWrite(BLUE1, LOW);
  digitalWrite(BLUE2, LOW);

}

void trafficLights() {    //simulates a traffic light
  digitalWrite(BLUE1, HIGH);
  digitalWrite(BLUE2, HIGH);
  switch (step) {
    case 0:   // red is on, yellow and green are off
      digitalWrite(RED, HIGH);
      digitalWrite(YELLOW, LOW);
      digitalWrite(GREEN, LOW);
      break;
    case 1:   // yellow is on, red and green are off
      digitalWrite(RED, LOW);
      digitalWrite(YELLOW, HIGH);
      digitalWrite(GREEN, LOW);
      break;
    case 2:     // green is on, red and yellow are off
      digitalWrite(RED, LOW);
      digitalWrite(YELLOW, LOW);
      digitalWrite(GREEN, HIGH);
      break;
  }

  step++;
  if (step > 2) step = 0;
}

void leftBlink() {    //simulates a left turn indicator

  switch (step) {
    case 0:   // left blue turned on
      digitalWrite(BLUE1, HIGH);
      break;
    case 1:   // left blue turned off
      digitalWrite(BLUE1, LOW);
      break;
  }

  step++;
  if (step > 1) step = 0;
}

void rightBlink() {    //simulates a right turn indicator

  switch (step) {
    case 0:   // right blue is turned on
      digitalWrite(BLUE2, HIGH);
      break;
    case 1:   // right blue is turned off
      digitalWrite(BLUE2, LOW);
      break;
  }

  step++;
  if (step > 1) step = 0;
}

void hazardLights() {    //simulates hazard lights

  switch (step) {
    case 0:   // both blues are turned on
      digitalWrite(BLUE1, HIGH);
      digitalWrite(BLUE2, HIGH);
      break;
    case 1:   // both blus are turned off
      digitalWrite(BLUE1, LOW);
      digitalWrite(BLUE2, LOW);
      break;
  }

  step++;
  if (step > 1) step = 0;
}