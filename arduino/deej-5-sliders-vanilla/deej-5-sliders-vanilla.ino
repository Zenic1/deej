const int NUM_SLIDERS = 4;
const int analogInputs[NUM_SLIDERS] = {A3, A2, A1, A0};

// minimum slider change before sending new value
// 2 is good for good pots that just sometimes waver between two values
// 1 will send every change
// 0 effectively turns off the "send on change" and sends the state every loop
const int MIN_CHANGE = 2; 

int lastSliderValues[NUM_SLIDERS];
int analogSliderValues[NUM_SLIDERS];

void setup() { 
  for (int i = 0; i < NUM_SLIDERS; i++) {
    pinMode(analogInputs[i], INPUT);
  }

  // init with impossible values so the first loop always sends current state
  for (int i = 0; i < NUM_SLIDERS; i++) {
     lastSliderValues[i] = -1;
  }

  Serial.begin(9600);
}

void loop() {
  updateSliderValues();
  sendSliderValues(); // Actually send data (all the time)
  // printSliderValues(); // For debug
  delay(10);
}

void updateSliderValues() {
  for (int i = 0; i < NUM_SLIDERS; i++) {
     analogSliderValues[i] = analogRead(analogInputs[i]);
  }
}

void sendSliderValues() {
  if (!isChange()) {
    return;
  }

  updateLastValues();

  String builtString = String("");

  for (int i = 0; i < NUM_SLIDERS; i++) {
    builtString += String((int)analogSliderValues[i]);

    if (i < NUM_SLIDERS - 1) {
      builtString += String("|");
    }
  }
  
  Serial.println(builtString);
}

void printSliderValues() {
  for (int i = 0; i < NUM_SLIDERS; i++) {
    String printedString = String("Slider #") + String(i + 1) + String(": ") + String(analogSliderValues[i]) + String(" mV");
    Serial.write(printedString.c_str());

    if (i < NUM_SLIDERS - 1) {
      Serial.write(" | ");
    } else {
      Serial.write("\n");
    }
  }
}

// true if at least one slider has changed by at least MIN_CHANGE
// false otherwise
bool isChange() {
  for (int i = 0; i < NUM_SLIDERS; i++) {
    int diff = abs(analogSliderValues[i] - lastSliderValues[i]);
    if (diff >= MIN_CHANGE) {
      return true;
    }
  }
  return false;
}

void updateLastValues() {
  for (int i = 0; i < NUM_SLIDERS; i++) {
    lastSliderValues[i] = analogSliderValues[i];
  }
}
