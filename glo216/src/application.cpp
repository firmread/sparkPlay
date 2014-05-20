
#include "application.h"




//this portion came from GLO-216 arduino libs

// Moves printing position to the first character of top line
char homeCursor() {  return 0x01;}
// Cycles the font size: normal -> tall -> wide -> big -> normal -> ...
char selectSize() {  return 0x02;}
// Sets font size to default of small on two lines of 16 characters
char defaultSize() {  return 0x03;}
// Behaves like the backspace key
char backspace() {  return 0x08;}
// Moves printing position to the next multiple of 4 location
char tab() {  return 0x09;}
// Moves the printing position down a line
char linefeed() {  return 0x0A;}
// Moves the printing position up a line
char verticalTab() {  return 0x0B;}
// Clears the screen and moves printing position to 0
char clearScreen() {  return 0x0C;}
// Moves to the first printing position of the next line
char carriageReturn() {  return 0x0D;}
// Turns on the OLED driver circuitry when it has been previously turned off
char turnOn() {  return 0x0E;}
// Turns off the OLED driver circuitry to save power
char turnOff() {  return 0x0F;}
// Sets the printing position according to value of next byte
char setPosition() {  return 0x10;}
// Prints text at the righthand end of a field of defined size from 2-8
char rightAlign() {  return 0x12;}
// Sets seven segment style font for large characters
char sevenSeg() {  return 0x13;}
// Sets text style font for large characters
char textStyle() {  return 0x14;}
// Begin multi-part instruction
char escape() {  return 0x1B;}
// See http://seetron.com/glo216/bpigloupgg.html
char bpkInstr() {  return 0xFE;}




int inverted = 1;

void setup() {
  Serial1.begin(9600);
  delay(10);
}


void loop() {


// clears the screen and select font size and style
  char instr1[] = {
    clearScreen(),
    defaultSize(),
    selectSize(), // tall
    selectSize(), // wide
    selectSize(), // big
    textStyle(),
    NULL
  };
  const char message[] = "Hi GLO!";
  Serial1.print(instr1);
  // print message according to prior instructions
  Serial1.print(message);
}








