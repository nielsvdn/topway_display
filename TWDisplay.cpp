/*
  Test.h - Test library for Wiring - implementation
  Copyright (c) 2006 John Doe.  All right reserved.
*/

// include core Wiring API
#include "Arduino.h"

// include this library's description file
#include "TWDisplay.h"

// include description files for other libraries used (if any)
#include "HardwareSerial.h"

// Constructor /////////////////////////////////////////////////////////////////
// Function that handles the creation and setup of instances

TWDisplay::TWDisplay( HardwareSerial &print, long baud_rate  )
{
  // initialize this instance's variables
  TW_serial = &print; //operate on the adress of print

  // initialize variables
  head = 0xaa;
  N16_read_cmd = 0x3e;
  N16_write_cmd = 0x3d;
  N32_write_cmd = 0x44;
  disp_page_cmd = 0x70;
  tail[0] = 0xcc;
  tail[1] = 0x33;
  tail[2] = 0xc3;
  tail[3] = 0x3c;

  // Setup
  TW_serial->begin(baud_rate);
}

// Public Methods //////////////////////////////////////////////////////////////
// Functions available in Wiring sketches, this library, and other libraries

int TWDisplay::readN16(long address){
  /*
   * Een functie om gemakkelijk de eerste 255 16-bit variabele te lezen op de HKT070DTA-1C.
   */

  // Send read request
  TW_write(head);        // Send Packet head
  TW_write(N16_read_cmd);    // Send N16 read command
  writeAddress(address);
  writeTail();

  // Wait until full response is available
  int size_of_response = 8;
  while (TW_serial->available()<size_of_response) {}

  // Get response
  TW_serial->read(); // Read header 
  TW_serial->read(); // Read command
  char value1   = TW_read(); // Read first value
  char value2   = TW_read(); // Read second value
  readTail();

  // Convert char/hex values to int
  int value = (int) value1+value2;

  // Return value
  return value;
}

void TWDisplay::writeN16(int write_value, long address){
  /*
   * Een functie om gemakkelijk 16-bit variabele te overschrijven op de HKT070DTA-1C.
   */

  // Convert decimal to byte
  int byte_h = highByte(write_value);
  int byte_l = lowByte(write_value);

  // Send Data
  TW_write(head);       // packet head
  TW_write(N16_write_cmd);  // VP_N16 write command
  writeAddress(address);
  TW_write(byte_h);     // VP_N16 data high byte
  TW_write(byte_l);     // VPN16 data low byte
  writeTail();

  // Delay
  delayMicroseconds(20000);
}

void TWDisplay::writeN32(int32_t write_value, long address){
  /*
   * Een functie om gemakkelijk 32-bit variabele te overschrijven op de HKT070DTA-1C.
   */

  // Convert decimal to byte
  
  union {
    struct {
	uint8_t byte1;
	uint8_t byte2;
	uint8_t byte3;
	uint8_t byte4;
    };
    int32_t longint;
  } split_write_value;

  split_write_value.longint = write_value;

  // Send Data
  TW_write(head);       // packet head
  TW_write(N32_write_cmd);  // VP_N16 write command
  writeAddress(address);
  TW_write(split_write_value.byte4);
  TW_write(split_write_value.byte3);
  TW_write(split_write_value.byte2);
  TW_write(split_write_value.byte1);
  writeTail();

  // Delay
  delayMicroseconds(20000);
}

void TWDisplay::dispPage(int page_id){
  /*
   * Een functie om gemakkelijk van pagina te wisselen op de HKT070DTA-1C.
   */

  // Convert id to byte
  int byte_h = highByte(page_id);
  int byte_l = lowByte(page_id);

  // Send Data
  TW_write(head);       // packet head
  TW_write(disp_page_cmd);  // VP_N16 write command
  TW_write(byte_h);     // VP_N16 data high byte
  TW_write(byte_l);     // VPN16 data low byte
  writeTail();

  // Delay
  delayMicroseconds(20000);
}


// Private Methods /////////////////////////////////////////////////////////////
// Functions only available to other functions in this library
void TWDisplay::writeTail(){
  for (int i = 0; (unsigned)i < sizeof(tail) ; i++) TW_write(tail[i]);
}

void TWDisplay::readTail(){
  for (int i = 0; (unsigned)i < sizeof(tail) ; i++) TW_serial->read();
}

void TWDisplay::TW_write(byte value){
  TW_serial->write(value);
}

byte TWDisplay::TW_read(){
  return TW_serial->read();
}

void TWDisplay::writeAddress( int32_t  address ) {
  unsigned char *p = (unsigned char*) & address;
  byte a[sizeof(int32_t)];
  for (unsigned i = 0; i < sizeof(int32_t); i++) {
    a[i] = (p[i]);
    //a[i] = ("0x%02x", p[i]);
  }

  TW_write(a[3]);        // Send adress
  TW_write(a[2]);        // Send adress
  TW_write(a[1]);        // Send adress
  TW_write(a[0]);  	// Send adress
}
