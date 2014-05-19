// GLO-216G serial OLED display
// Natalia Fargasch Norman @ theelectronicshobbyist.com/blog

// GLO-216Y/G information: http://www.seetron.com/glo216.html
// Programming reference: http://seetron.com/glo216/glo216prog.html

#ifndef GLO216_h
#define GLO216_h

// Moves printing position to the first character of top line
char homeCursor();

// Cycles the font size: normal -> tall -> wide -> big -> normal -> ...
char selectSize();

// Sets font size to default of small on two lines of 16 characters
char defaultSize();

// Behaves like the backspace key
char backspace();

// Moves printing position to the next multiple of 4 location
char tab();

// Moves the printing position down a line
char linefeed();

// Moves the printing position up a line
char verticalTab();

// Clears the screen and moves printing position to 0
char clearScreen();

// Moves to the first printing position of the next line
char carriageReturn();

// Turns on the OLED driver circuitry when it has been previously turned off
char turnOn();

// Turns off the OLED driver circuitry to save power
char turnOff();

// Sets the printing position according to value of next byte
char setPosition();

// Prints text at the righthand end of a field of defined size from 2-8
char rightAlign();

// Sets seven segment style font for large characters
char sevenSeg();

// Sets text style font for large characters
char textStyle();

// Begin multi-part instruction
char escape();

// See http://seetron.com/glo216/bpigloupgg.html
char bpkInstr();

#endif
