// 7-segment display pins (common cathode) - First display
const int segmentPins1[] = {2, 3, 4, 5, 6, 7, 8, 9};

// 7-segment display pins (common cathode) - Second display
const int segmentPins2[] = {11, 12, 13, A0, A1, A2, A4, A5};

// Buzzer pin
const int buzzerPin = 10;

// Define the segments for each number (0-9) - First display
const byte segments1[] = {
  B01111111, // 0
  B01110000, // 1
  B11011011, // 2
  B11111001, // 3
  B11110100, // 4
  B11101101, // 5
  B11101111, // 6
  B01111000, // 7
  B11111111, // 8
  B11111101  // 9
};

// Define the segments for each number (0-9) - Second display
const byte segments2[] = {
  B10111111, // 0
  B00110000, // 1
  B01011011, // 2
  B01111001, // 3
  B01110100, // 4
  B01101101, // 5
  B01101111, // 6
  B00111000, // 7
  B01111111, // 8
  B01111101, // 9
};

// Define a mapping of letters (A-J) to their corresponding numbers (0-9) for the first display
const char letterToNumberMap[] = "0123456789";

void setup() {
  // Initialize the display pins for the first display as OUTPUT
  for (int i = 0; i < 8; i++) {
    pinMode(segmentPins1[i], OUTPUT);
  }

  // Initialize the display pins for the second display as OUTPUT
  for (int i = 0; i < 7; i++) { // Adjust the number of pins based on your second display
    pinMode(segmentPins2[i], OUTPUT);
  }

  // Initialize the buzzer pin as OUTPUT
  pinMode(buzzerPin, OUTPUT);

  // Initialize serial communication
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
    char input = Serial.read();
    
    if (input >= 'A' && input <= 'J') {
      int number = letterToNumberMap[input - 'A'] - '0';
      displayNumber(number, segmentPins1, segments1);
    }
    else if (input >= '0' && input <= '9') {
      int number = input - '0';
      displayNumber(number, segmentPins2, segments2);
    }
    else if (input == 'S' || input == 's') {
      // Generate a tone with the buzzer
      tone(buzzerPin, 1000, 200); // 1000 Hz for 200 ms
    }
  }
}

void displayNumber(int number, const int segmentPins[], const byte segments[]) {
  if (number >= 0 && number <= 9) {
    for (int i = 0; i < 8; i++) {
      digitalWrite(segmentPins[i], bitRead(segments[number], i));
    }
  }
}
