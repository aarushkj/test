/*
  ESP32 Distance Sensor
  Board:     ESP32 DevKit V1 (ESP32 Dev Module)
  Sensor:    HC-SR04 ultrasonic
  Libraries: none (uses the Arduino core only)

  Connections
    HC-SR04 VCC  -> VIN (5 V)
    HC-SR04 GND  -> GND
    HC-SR04 TRIG -> GPIO 5
    HC-SR04 ECHO -> GPIO 18 (through a 1k / 2k voltage divider)
    LED anode    -> GPIO 2 (through 220 ohm)
    LED cathode  -> GND
*/

const int TRIG_PIN = 5;
const int ECHO_PIN = 18;
const int LED_PIN = 2;

const float ALERT_DISTANCE_CM = 20.0;
const unsigned long ECHO_TIMEOUT_US = 30000;  // about 5 m round trip

float readDistanceCm() {
  // A 10 microsecond pulse on TRIG starts one measurement
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  unsigned long duration = pulseIn(ECHO_PIN, HIGH, ECHO_TIMEOUT_US);
  if (duration == 0) {
    return -1.0;  // no echo received
  }

  // Sound travels 0.0343 cm per microsecond; halve for the round trip
  return duration * 0.0343 / 2.0;
}

void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  float distance = readDistanceCm();

  if (distance < 0) {
    Serial.println("Out of range");
    digitalWrite(LED_PIN, LOW);
  } else {
    Serial.print("Distance: ");
    Serial.print(distance, 1);
    Serial.println(" cm");
    digitalWrite(LED_PIN, distance < ALERT_DISTANCE_CM ? HIGH : LOW);
  }

  delay(500);
}
