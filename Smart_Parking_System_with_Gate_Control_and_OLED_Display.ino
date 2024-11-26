#include <Servo.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64
#define OLED_RESET -1   

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define ENTRY_IR_PIN 7     
#define EXIT_IR_PIN 6     

#define MAX_PARKING_SPOTS 6 
int parkingSpots = MAX_PARKING_SPOTS; 

Servo gateServo;

void setup() {
  Serial.begin(9600);
  gateServo.attach(9);  
  
  pinMode(ENTRY_IR_PIN, INPUT); 
  pinMode(EXIT_IR_PIN, INPUT);  
  
  gateServo.write(90);
  
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (true);
  }
  
  display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("PARKINGSYSTEM");
  display.display();
  delay(3000); 
  
  updateOLED(); 
}

void openGate() {
  gateServo.write(190);  
}

void closeGate() {
  gateServo.write(90);  
}

void updateOLED() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(2);
  display.print("Available Spots: ");
  display.setTextSize(4);
  display.setCursor(70, 20);
  display.println(parkingSpots);
  
  if (parkingSpots == 0) {
    display.setTextSize(1);
    display.setCursor(20, 50);
    display.println("PARKING FULL");
  }
  
  display.display();
}

void loop() {
  if (digitalRead(ENTRY_IR_PIN) == LOW && parkingSpots > 0) {
    openGate();
    while (digitalRead(ENTRY_IR_PIN) == LOW) {}
    delay(2000);
    closeGate();
    if (parkingSpots > 0) {
      parkingSpots--; 
      updateOLED();
    }
  }

  if (digitalRead(EXIT_IR_PIN) == LOW && parkingSpots < MAX_PARKING_SPOTS) {
    openGate();
    while (digitalRead(EXIT_IR_PIN) == LOW) {}
    delay(2000);
    closeGate();
    if (parkingSpots < MAX_PARKING_SPOTS) {
      parkingSpots++; 
      updateOLED();
    }
  }

  delay(1000); 
}
