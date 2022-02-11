
#include <CAN.h>                                      //CAN Bus Communication
#include <OBD2.h>                                     //For vehicle data from ECU
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>


// array of PID's to print values of
const int PIDS[] = {
  ENGINE_RPM,
  VEHICLE_SPEED,
};

//CAN Network Speed Constant
#define CAN_500KBPS                             16

// Reset pin not used but needed for library
#define OLED_RESET                              4
Adafruit_SSD1306 display(OLED_RESET);
#define SCREEN_REFRESH_DELAY                    1000

//Global variables
int    current_Displayed_Speed_MPH = 0;            //Current speed displayed on gauge
int    current_Displayed_RPM = 0;                  //Current RPM reading displayed on gauge


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  // initialize OLED with I2C addr 0x3C
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  while (!Serial);
  // Clear the display
  display.clearDisplay();

  // Set up to write to display
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0,0);

  display.println(F("OBD2 Key Stats"));
  display.display();
  delay(SCREEN_REFRESH_DELAY);

  // Attempt to connect to vehicle CAN bus
  while (true) {
    display.clearDisplay();
    display.print(F("Attempting to connect to OBD2 CAN bus ... "));
    display.display();
    delay(SCREEN_REFRESH_DELAY);

    if (!OBD2.begin()) {
      display.clearDisplay();
      display.println(F("failed!"));
      display.display();

      delay(SCREEN_REFRESH_DELAY);
    } else {
      display.clearDisplay();
      display.println(F("success"));
      display.display();
      break;
    }
  }

  Serial.println();

} //end function setup

void loop() {
            
   //Update RPM value
  float current_RPM = OBD2.pidRead(ENGINE_RPM);                 //Request RPM from ECU
  current_Displayed_RPM = current_RPM;

  //Update speed value
  float current_Speed = OBD2.pidRead(VEHICLE_SPEED);            //Request speed from ECU, returns speed in km/h
  current_Speed = current_Speed / 1.609344;                     //Convert to mph
  current_Displayed_Speed_MPH = current_Speed;

  //Update display
  delay(SCREEN_REFRESH_DELAY);
  UpdateDisplay();
  display.display();

} //end function loop

void UpdateDisplay() {

    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(1);

    display.setCursor(0,0);
    display.print("OBDII Key Stats: ");

    display.setCursor(0,10);
    display.print("Current Speed:  ");
    display.print(current_Displayed_Speed_MPH);
    display.print(" MPH");

    display.setCursor(0,20);
    display.print("Current RPM:    ");
    display.print(current_Displayed_RPM);
    display.print(" RPM");
  
} //end function UpdateDisplay

