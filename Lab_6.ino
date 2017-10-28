int ledPin[4] = {2, 3, 4, 5};
int buttonPin[4] = {33, 35, 37, 39};
int buttonPinforward = 31;
int buttonPinback = 32;
boolean lastButtonStateforward = LOW;
boolean buttonStateforward = LOW;
boolean lastButtonStateback = LOW;
boolean buttonStateback = LOW;
int counter = 0;
boolean lastButtonState[4] = {LOW, LOW, LOW, LOW};
boolean buttonState[4] = {LOW, LOW, LOW, LOW};
boolean stepState[3][4] = {{false, false, false, false}, {false, false, false, false}, {false, false, false, false}};
int tempo = 0;
int currentStep = 0;
unsigned long lastStepTime = 0;
void setup() {
  pinMode(buttonPinforward, INPUT);
  pinMode(buttonPinback, INPUT);
  for (int i = 0; i < 4; i++) {
    pinMode(buttonPin[i], INPUT);
    pinMode(ledPin[i], OUTPUT);
  }
}
void loop() {
  buttonCounterforward();
  buttonCounterback();
  checkButtons();
  setLeds();
  sequence();
}
void checkButtons() {
  for (int i = 0; i < 4; i++) {
    lastButtonState[i] = buttonState[i];
    buttonState[i] = digitalRead(buttonPin[i]);
    if (buttonState[i] == HIGH && lastButtonState[i] == LOW) {
      if (stepState[counter][i] == false) {
        stepState[counter][i] = true;
      } else if (stepState[counter][i] == true) {
        stepState[counter][i] = false;
      }
    }
  }
}
/*
  void setLeds() {
  for (int i = 0; i < 4; i++) {
    if (stepState[i] == true) {
      //digitalWrite(ledPin[i], HIGH);
    } else if (stepState[i] == false) {
      //digitalWrite(ledPin[i], LOW);
    }
  }
  }
*/
void sequence() {
  tempo = analogRead(A5);

  if (millis() > lastStepTime + tempo) {
    //digitalWrite(ledPin[currentStep], LOW);
    //currentStep = currentStep + 1;
    if (stepState[0][currentStep] == true) {
      usbMIDI.sendNoteOff(60, 0, 1);
      usbMIDI.sendNoteOn(60, 127, 1);
    }
    if (stepState[1][currentStep] == true) {
      usbMIDI.sendNoteOff(62, 0, 1);
      usbMIDI.sendNoteOn(62, 127, 1);
    }
    if (stepState[2][currentStep] == true) {
      usbMIDI.sendNoteOff(64, 0, 1);
      usbMIDI.sendNoteOn(64, 127, 1);
    }
    currentStep = currentStep + 1;

    if (currentStep > 3) {
      currentStep = 0;

    }
    //digitalWrite(ledPin[currentStep], HIGH);
    lastStepTime = millis();
  }
}
void setLeds() {
  for (int i = 0; i < 4; i++) {
    if (i == currentStep) {
      analogWrite(ledPin[i], 250);
    } else if (stepState[counter][i] == true) {
      analogWrite(ledPin[i], 50);
    } else {
      analogWrite(ledPin[i], 0);
    }
  }
}
void buttonCounterforward() {
  lastButtonStateforward = buttonStateforward;
  buttonStateforward = digitalRead(buttonPinforward);
  if (buttonStateforward == HIGH && lastButtonStateforward == LOW) {
    counter = counter + 1;
  }
  if (counter > 2) {
    counter = 0;
  }
}
void buttonCounterback() {
  lastButtonStateback = buttonStateback;
  buttonStateback = digitalRead(buttonPinback);
  if (buttonStateback == HIGH && lastButtonStateback == LOW) {
    counter = counter - 1;
  }
  if (counter < 0) {
    counter = 2;
  }
}

