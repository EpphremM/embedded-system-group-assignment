// Temperature Regulation System with LM35 (With Resistors)

// Pin definitions
const int tempPin = A0;       // Analog pin A0 connected to LM35 temperature sensor output
const int greenLed = 9;       // Digital pin 9 for green LED (PWM capable)
const int yellowLed = 10;     // Digital pin 10 for yellow LED (PWM capable)
const int redLed = 11;        // Digital pin 11 for red LED (PWM capable)
const int buzzer = 5;         // Digital pin 5 for buzzer
const int fan = 6;            // Digital pin 6 for fan control (PWM capable)

// Temperature thresholds (in Celsius)
const float lowTemp = 25.0;    // Below this temperature is considered "cool"
const float medTemp = 35.0;    // Normal temperature range (25-35°C)
const float highTemp = 47.0;   // Above this temperature is considered "hot" (danger zone)

void setup() {
  Serial.begin(9600);  // Initialize serial communication at 9600 baud rate
  
  // Set all component pins as outputs
  pinMode(greenLed, OUTPUT); // Green light for low temperature
  pinMode(yellowLed, OUTPUT); // Yellow for medium temperature
  pinMode(redLed, OUTPUT); // Red for hot temperature
  pinMode(buzzer, OUTPUT); // Buzzer to indicate hot temperature
  pinMode(fan, OUTPUT); // The actual fan
  
  // Initialize all outputs to LOW/OFF state
  digitalWrite(greenLed, LOW);
  digitalWrite(yellowLed, LOW);
  digitalWrite(redLed, LOW);
  digitalWrite(buzzer, LOW);
  digitalWrite(fan, LOW);
  
  Serial.println("Temperature Monitoring System Started (No Resistors Version)");
}

void loop() {
  // Read raw analog value from temperature sensor (0-1023)
  int tempValue = analogRead(tempPin);
  
  // Convert analog reading to temperature in Celsius:
  // 1. Convert to voltage (5V reference, 10-bit ADC)
  // 2. LM35 outputs 10mV per °C, so multiply by 100 to get temperature
  float temperature = (tempValue * 5.0 / 1024.0) * 100.0;
  
  // Print temperature to serial monitor for debugging
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");
  
  // Temperature control logic:
  
  // COOL ZONE (below 25°C)
  if (temperature < lowTemp) {
    analogWrite(greenLed, 128);  // Green LED at 50% brightness (PWM value 128/255)
    analogWrite(yellowLed, 0);   // Yellow LED off
    analogWrite(redLed, 0);      // Red LED off
    analogWrite(fan, 0);         // Fan off
    noTone(buzzer);              // Stop any alarm sound
  } 
  
  // NORMAL ZONE (25-35°C)
  else if (temperature >= lowTemp && temperature < medTemp) {
    analogWrite(greenLed, 0);    // Green LED off
    analogWrite(yellowLed, 128); // Yellow LED at 50% brightness
    analogWrite(redLed, 0);      // Red LED off
    analogWrite(fan, 0);         // Fan off
    noTone(buzzer);              // Stop any alarm sound
  } 
  
  // WARM ZONE (35-47°C)
  else if (temperature >= medTemp && temperature < highTemp) {
    analogWrite(greenLed, 0);    // Green LED off
    analogWrite(yellowLed, 255); // Yellow LED at full brightness
    analogWrite(redLed, 0);      // Red LED off
    analogWrite(fan, 128);       // Fan at 50% speed
    noTone(buzzer);              // Stop any alarm sound
  } 
  
  // HOT ZONE (above 47°C) - ALARM STATE
  else {
    analogWrite(greenLed, 0);    // Green LED off
    analogWrite(yellowLed, 0);   // Yellow LED off
    // Red LED blinking (on for 100ms, off for 100ms using millis() timer)
    analogWrite(redLed, millis() % 200 < 100 ? 255 : 0);
    analogWrite(fan, 255);       // Fan at full speed
    tone(buzzer, 1000);          // Activate buzzer at 1kHz frequency
  }
  
  delay(500); // Wait 500ms (0.5 second) before next reading
}