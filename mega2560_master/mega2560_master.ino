// Include Arduino Wire library for I2C
#include <Wire.h>
 
// Define Slave I2C Address
#define SLAVE_ADDR 9
 
// Define Slave answer size
#define ANSWERSIZE 1

int button_pin[3] = {8, 9, 10};
int buttonState[3];
String select_menu = " ";

// Function prototypes
void i2c_start(int value);
 
void setup() {
 
  // Initialize I2C communications as Master
  Wire.begin();
  
  // Setup serial monitor
  Serial.begin(9600);
  Serial.println("I2C Master Demonstration");
  
  pinMode(button_pin[0],INPUT_PULLUP);
  pinMode(button_pin[1],INPUT_PULLUP);
  pinMode(button_pin[2],INPUT_PULLUP);

}
//---------------------- MAIN LOOP ----------------------------//
void loop() {
  
  readButtons();
  printButtons();
  if(buttonState[0]==LOW) write_i2c(1);
  else if(buttonState[1]==LOW) write_i2c(2);
  else if(buttonState[2]==LOW){
    write_i2c(3);
    select_menu = receive_i2c();
    Serial.println("Select Menu is "+select_menu);
  }

  delay(200);  
}

//---------------------- ALL FUNCTION BELOW HERE --------------------//

void readButtons() {
  for (int i = 0; i < 3; i++) {
    buttonState[i] = digitalRead(button_pin[i]);
  }
}

void printButtons() {
  for (int i = 0; i < 3; i++) {
    Serial.print(buttonState[i]);
    Serial.print(" , ");
  }
  Serial.println("");
}

void write_i2c(int value){
  // Write a charatre to the Slave
  Wire.beginTransmission(SLAVE_ADDR);
  Wire.write(value);
  Wire.endTransmission();

}

String receive_i2c(){
  Serial.println("Receive data");
  // Read response from Slave
  Wire.requestFrom(SLAVE_ADDR,ANSWERSIZE);
  
  // Add characters to string
  char response;
  while (Wire.available()) {
      response = Wire.read();
  } 
  return String(response);
}


