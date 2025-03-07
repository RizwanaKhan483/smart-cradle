#include <DHT.h>
#include <Servo.h>

// Pin Definitions
#define DHTPIN 4      // D2 pin for DHT11 sensor
#define DHTTYPE DHT11 // DHT11 Sensor
#define SOIL_PIN A0   // Analog input for moisture sensor
#define SERVO_PIN 14  // D5 pin for MG90S servo motor

DHT dht(DHTPIN, DHTTYPE);  // Initialize DHT11 sensor
Servo myservo;             // Create a Servo object

void setup() {
  // Start Serial Monitor for debugging
  Serial.begin(115200);
  
  // Initialize DHT11 sensor
  dht.begin();
  
  // Initialize Servo
  myservo.attach(SERVO_PIN);
  
  // Print initial message to Serial Monitor
  Serial.println("Smart Cradle Monitoring System");
  delay(2000);  // Wait for 2 seconds
}

void loop() {
  // Read temperature and humidity from DHT11
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Sensor Error!");
    return;
  }

  // Read moisture sensor value (for simplicity, using an analog pin)
  int moistureValue = analogRead(SOIL_PIN);

  // Print the sensor readings to Serial Monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C | ");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print(" % | ");
  Serial.print("Moisture Value: ");
  Serial.println(moistureValue);

  // If temperature exceeds threshold (e.g., 30°C), rotate servo from 0° to 180° and back
  if (temperature > 30) {
    Serial.println("Temperature is high, moving the cradle...");

    // Rotate servo from 0° to 180° in 10° increments
    for (int pos = 0; pos <= 180; pos += 10) {
      myservo.write(pos);  // Move servo to 'pos' degree
      Serial.print("Servo Position: ");
      Serial.println(pos);
      delay(500);  // Wait for 0.5 seconds to let the servo reach the position
    }

    // Rotate servo back from 180° to 0° in 10° increments
    for (int pos = 180; pos >= 0; pos -= 10) {
      myservo.write(pos);  // Move servo to 'pos' degree
      Serial.print("Servo Position: ");
      Serial.println(pos);
      delay(500);  // Wait for 0.5 seconds to let the servo reach the position
    }

    Serial.println("Cradle movement completed.");
  }

  // Control based on moisture level (e.g., if moisture > 500)
  if (moistureValue < 500) {  // Adjust threshold as needed
    Serial.println("High Moisture Detected.");
  } else {
    Serial.println("Moisture Level Normal.");
  }

  delay(2000);  // Delay for 2 seconds before the next loop
}
