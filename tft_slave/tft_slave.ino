#include <MCUFRIEND_kbv.h>
#include <Adafruit_GFX.h> // Core graphics library

#include <Wire.h>
 
// Define Slave I2C Address
#define SLAVE_ADDR 9
 
// Define Slave answer size
#define ANSWERSIZE 1
 
// Define string with response to Master
 
#define LCD_RESET 1 // Can alternately just connect to Arduino's reset pin
#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0
 
// Assign human-readable names to some common 16-bit color values:
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF

// resolution is around 300 * 280
 
MCUFRIEND_kbv tft;

int menu = 0;
String mode_sel_name[4] = {"A1","B2","C3","D4"};
int sel_arrow = 0;
String read_but = "";


void receiveEvent() {
 
  // Read while data received
  byte x;
  while (0 < Wire.available()) {
    x = Wire.read();
  }
  read_but = String(x);
  // Print to Serial Monitor
  Serial.println("Receive event");
  Serial.println("x : "+String(x));
}
 
void requestEvent() {
 
  // Setup byte variable in the correct size
  byte response[ANSWERSIZE];
  String answer = String(sel_arrow);
  // Format answer as array
  for (byte i=0;i<ANSWERSIZE;i++) {
     response[i] = (byte)answer.charAt(i);
  }
  
  // Send response back to Master
  Wire.write(response,sizeof(response));
  
  // Print to Serial Monitor
  Serial.println("Request event");
}


void clear_arrow() {
  for (int i = 100; i < 300; i+=50) {
    tft.setTextColor(BLACK);
    tft.setTextSize(6);
    tft.setCursor(0, i);
    tft.print(">");
  }
}

void displayData() {
  if (menu == 0) {
    tft.reset();
    int16_t box_x = 0;
    int16_t box_y = 0;
    int16_t box_width = 300;
    int16_t box_height = 60;
    tft.fillRect(box_x, box_y, box_width, box_height, WHITE); // Draw a white rectangle
    tft.setTextColor(BLACK);
    tft.setTextSize(3);
    tft.setCursor(20, 20);
    tft.print("MENU SELECT");

    tft.setTextColor(WHITE);
    tft.setTextSize(5);
    tft.setCursor(50, 100);
    tft.print("1:" + mode_sel_name[0] + "");
    tft.setCursor(50, 150);
    tft.print("2:" + mode_sel_name[1] + "");
    tft.setCursor(50, 200);
    tft.print("3:" + mode_sel_name[2] + "");
    tft.setCursor(50, 250);
    tft.print("4:" + mode_sel_name[3] + "");
  }
  if(menu == 1){
    tft.setTextColor(WHITE);
    tft.setTextSize(6);
    tft.setCursor(1, 0);
    tft.print("Processing..");
  }

}

void sel_menu(){
  delay(60);
  Serial.print("sel_arrow : ");
  Serial.println(sel_arrow);
  if(menu == 0){
    tft.setTextColor(WHITE);
    tft.setTextSize(6);

    sel_arrow = sel_arrow % 300;
    if(sel_arrow == 0)sel_arrow = 100;

    tft.setCursor(0,sel_arrow);
    tft.print(">");
    
    if(read_but == "1" ){
      clear_arrow();
      sel_arrow += 50;
    }
    if(read_but == '2' ){
      if(sel_arrow == 100)sel_arrow =300;
      clear_arrow();
      sel_arrow -= 50;
    }
    if(read_but == '3'){
      clear_arrow();
      tft.reset();
      menu = 1;
    }
  }

  if(menu == 1){
    delay(100);
    if(read_but == '3' ){
      tft.reset();
      menu = 0;
    }
  }
  
}


void setup(void) {
  
  // Initialize I2C communications as Slave
  Wire.begin(SLAVE_ADDR);

  Serial.begin(9600);
  Serial.println(F("TFT LCD test"));
    
  tft.reset();
    
  uint16_t identifier = tft.readID();
  tft.begin(identifier);
  tft.fillScreen(BLACK);
  //tft.setRotation(3);
  // tft.setTextColor(WHITE);
  // tft.setTextSize(6);
  // Set up Timer1 interrupt

}

void loop(void) {
  unsigned long start = micros();
  displayData();
  while(menu == 0){
    //displayData();
    //Serial.println("menu still 0-------------------");
    //Function to run when data requested from master
    Wire.onRequest(requestEvent); 
    //Function to run when data received from master
    Wire.onReceive(receiveEvent);
    
    sel_menu();
    Serial.println(read_but);
    delay(100);
    read_but = "0";
    clear_arrow();
  }

}


