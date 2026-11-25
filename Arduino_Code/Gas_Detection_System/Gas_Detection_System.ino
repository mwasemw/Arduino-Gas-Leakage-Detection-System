
#include <LiquidCrystal.h>


// 1. PIN CONFIGURATION


// Initialize the LCD using RS, Enable, D4, D5, D6 and D7 pins.
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

// Define the Arduino pins connected to the input and output devices.
const int MQ_PIN = A0;       // MQ-2 gas sensor digital signal (LogicToggle)
const int BUZZER_PIN = 8;    // Buzzer control pin
const int FAN_PIN = 9;       // MOSFET gate control for 12V fan
const int BUTTON_PIN = 10;   // Manual test push button



// 2. SYSTEM STATES


// Define the different operating states of the system.
enum SystemStatus {
  STATUS_UNKNOWN,  // Initial state before status is determined
  STATUS_NORMAL,   // No gas detected
  STATUS_GAS,      // Gas leakage detected
  STATUS_TEST      // Manual testing mode
};

// Store the current system state.
SystemStatus currentStatus = STATUS_UNKNOWN;



// 3. GLOBAL VARIABLES


// Store the status of the gas sensor and manual test button.
bool gasDetected = false;
bool manualTest = false;

// Variables used for push-button debouncing.
bool lastButtonReading = HIGH;
bool buttonState = HIGH;

// Store the time of the last button state change
// and the last serial monitoring update.
unsigned long lastButtonChange = 0;
unsigned long lastSerialUpdate = 0;

// Define timing intervals in milliseconds.
const unsigned long DEBOUNCE_DELAY = 50;  // Button debounce time
const unsigned long SERIAL_INTERVAL = 500; // Serial update interval



// 4. SETUP


void setup() {

  // Initialize the LCD display (16 columns and 2 rows).
  lcd.begin(16, 2);

  // Configure the sensor and push button as inputs.
  pinMode(MQ_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Configure the buzzer and fan control pins as outputs.
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(FAN_PIN, OUTPUT);

  // Ensure that the fan and buzzer are OFF at startup.
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(FAN_PIN, LOW);

  // Initialize serial communication at 9600 baud.
  Serial.begin(9600);

  // Display the system identification message.
  lcd.setCursor(3, 0);
  lcd.print("GAS");
  lcd.setCursor(3, 1);
  lcd.print("DETECTOR");

  // Send a startup message to the Virtual Terminal.
  Serial.println("GAS DETECTION SYSTEM STARTED");

  delay(500);
  lcd.clear();
}



// 5. READ GAS SENSOR


void readGasSensor() {

  // Read the digital signal from the MQ-2 sensor.
  // HIGH indicates gas detected; LOW indicates no gas.
  int gasValue = digitalRead(MQ_PIN);

  // Update the gas detection status.
  gasDetected = (gasValue == HIGH);
}



// 6. READ PUSH BUTTON


void readPushButton() {

  // Read the current state of the push button.
  bool reading = digitalRead(BUTTON_PIN);

  // Detect a change in button state and record its time.
  if (reading != lastButtonReading) {

    lastButtonChange = millis();
    lastButtonReading = reading;
  }

  // Accept the new button state only after it remains
  // stable for the specified debounce interval.
  if (millis() - lastButtonChange >= DEBOUNCE_DELAY) {

    buttonState = reading;
  }

  // INPUT_PULLUP makes LOW indicate that the button is pressed.
  manualTest = (buttonState == LOW);
}



// 7. CONTROL OUTPUT DEVICES


void controlOutputs(bool activate) {

  // Switch the buzzer and fan ON or OFF together.
  digitalWrite(BUZZER_PIN, activate ? HIGH : LOW);
  digitalWrite(FAN_PIN, activate ? HIGH : LOW);
}



// 8. DISPLAY SYSTEM STATUS


void displayStatus(SystemStatus status) {

  // Avoid unnecessary LCD updates if the status is unchanged.
  if (status == currentStatus) {
    return;
  }

  lcd.clear();

  // Display a message corresponding to the current system state.
  switch (status) {

    case STATUS_NORMAL:

      lcd.setCursor(1, 0);
      lcd.print("SYSTEM NORMAL");
      break;

    case STATUS_GAS:

      lcd.setCursor(1, 0);
      lcd.print("GAS DETECTED!");
      break;

    case STATUS_TEST:

      lcd.setCursor(3, 0);
      lcd.print("SYSTEM TEST");
      break;

    default:
      break;
  }

  // Update the stored system state.
  currentStatus = status;
}



// 9. SERIAL MONITORING


void monitorSerial() {

  // Transmit system information every 500 milliseconds.
  if (millis() - lastSerialUpdate >= SERIAL_INTERVAL) {

    lastSerialUpdate = millis();

    // Report the gas detection status.
    Serial.print("Gas Status: ");
    Serial.print(gasDetected ? "DETECTED" : "NORMAL");

    // Report whether manual testing is active.
    Serial.print(" | Manual Test: ");
    Serial.println(manualTest ? "ACTIVE" : "INACTIVE");
  }
}



// 10. MAIN PROGRAM


void loop() {

  // Read the input devices.
  readGasSensor();
  readPushButton();

  // Gas detection takes priority over manual testing.
  if (gasDetected) {

    // Activate the fan and buzzer and display the gas warning.
    controlOutputs(true);
    displayStatus(STATUS_GAS);
  }

  // Activate the outputs if manual testing is requested.
  else if (manualTest) {

    controlOutputs(true);
    displayStatus(STATUS_TEST);
  }

  // If neither condition is present, maintain normal operation.
  else {

    controlOutputs(false);
    displayStatus(STATUS_NORMAL);
  }

  // Periodically transmit system information to the Virtual Terminal.
  monitorSerial();
}
