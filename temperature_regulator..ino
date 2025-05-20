// Temperature Regulation System with LM35 (With Resistors)

// Pin definitions
const int tempPin = A0;       // LM35 output pin
const int greenLed = 9;       // Green LED pin (PWM)
const int yellowLed = 10;     // Yellow LED pin (PWM)
const int redLed = 11;        // Red LED pin (PWM)
const int buzzer = 5;         // Buzzer pin
const int fan = 6;            // Fan control pin (PWM)

// Temperature thresholds
const float lowTemp = 25.0;    // Below this is "cool"
const float medTemp = 35.0;    // Between lowTemp and this is "normal"
const float highTemp = 47.0;   // Above this is "hot"

void setup() {
  Serial.begin(9600);
  
  // Set pin modes
  pinMode(greenLed, OUTPUT);
  pinMode(yellowLed, OUTPUT);
  pinMode(redLed, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(fan, OUTPUT);
  
  // Initially turn all outputs off
  digitalWrite(greenLed, LOW);
  digitalWrite(yellowLed, LOW);
  digitalWrite(redLed, LOW);
  digitalWrite(buzzer, LOW);
  digitalWrite(fan, LOW);
  
  Serial.println("Temperature Monitoring System Started (No Resistors Version)");
}

void loop() {
  // Read temperature from LM35
  int tempValue = analogRead(tempPin);
  float temperature = (tempValue * 5.0 / 1024.0) * 100.0; // Convert to Celsius
  
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");
  
  // Control LEDs and fan based on temperature
  if (temperature < lowTemp) {
    // Cool temperature - green LED at half brightness
    analogWrite(greenLed, 128);  // PWM at 50%
    analogWrite(yellowLed, 0);
    analogWrite(redLed, 0);
    analogWrite(fan, 0);
    noTone(buzzer);
  } 
  else if (temperature >= lowTemp && temperature < medTemp) {
    // Normal temperature - yellow LED at half brightness
    analogWrite(greenLed, 0);
    analogWrite(yellowLed, 128);
    analogWrite(redLed, 0);
    analogWrite(fan, 0);
    noTone(buzzer);
  } 
  else if (temperature >= medTemp && temperature < highTemp) {
    // Warm temperature - yellow LED at full brightness, fan at half speed
    analogWrite(greenLed, 0);
    analogWrite(yellowLed, 255);
    analogWrite(redLed, 0);
    analogWrite(fan, 128);
    noTone(buzzer);
  } 
  else {
    // Hot temperature - red LED blinking, fan full speed, buzzer alarm
    analogWrite(greenLed, 0);
    analogWrite(yellowLed, 0);
    analogWrite(redLed, millis() % 200 < 100 ? 255 : 0); // Blink
    analogWrite(fan, 255);
    tone(buzzer, 1000); // 1kHz alarm tone
  }
  
  delay(500); // Update twice per second
}