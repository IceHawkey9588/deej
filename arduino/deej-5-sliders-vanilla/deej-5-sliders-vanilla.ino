const int NUM_SLIDERS = 4;
const int analogInputs[NUM_SLIDERS] = {A0, A1, A2, A3};
int analogSliderValues[NUM_SLIDERS];

const int rowPins[] = {2, 3, 4, 5}; // Define row pins
const int colPins[] = {6, 7, 8};    // Define column pins
const int NUM_ROWS = sizeof(rowPins) / sizeof(rowPins[0]);
const int NUM_COLS = sizeof(colPins) / sizeof(colPins[0]);
bool buttonState[NUM_ROWS][NUM_COLS] = {false}; // Tracks previous state of each button

void setup() { 
  for (int i = 0; i < NUM_SLIDERS; i++) {
    pinMode(analogInputs[i], INPUT);
  }

  // Set up row and column pins
  for (int i = 0; i < NUM_ROWS; i++) {
    pinMode(rowPins[i], OUTPUT);
    digitalWrite(rowPins[i], HIGH); // Set rows to high initially
  }

  for (int j = 0; j < NUM_COLS; j++) {
    pinMode(colPins[j], INPUT_PULLUP); // Enable internal pull-up for columns
  }
  Serial.begin(9600);
}

void loop() {
  updateSliderValues();
  sendSliderValues(); // Actually send data (all the time)
  // printSliderValues(); // For debug
  checkButtonMatrix();
  delay(100);
}

void updateSliderValues() {
  for (int i = 0; i < NUM_SLIDERS; i++) {
     analogSliderValues[i] = analogRead(analogInputs[i]);
  }
}

void sendSliderValues() {
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

void checkButtonMatrix() {
  for (int row = 0; row < NUM_ROWS; row++) {
    digitalWrite(rowPins[row], LOW);

    for (int col = 0; col < NUM_COLS; col++) {
      bool currentState = (digitalRead(colPins[col]) == LOW);
      
      // Check if the state has changed
      if (currentState != buttonState[row][col]) {
        buttonState[row][col] = currentState;

        // Display appropriate message based on new state
        Serial.print(currentState ? "+" : "-");
        Serial.print((NUM_ROWS - row - 1) * NUM_COLS + (NUM_COLS - col));
        Serial.print("\n");

      }
    }

    digitalWrite(rowPins[row], HIGH);
  }
}
