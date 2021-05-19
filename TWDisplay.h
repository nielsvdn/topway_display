/*
  Test.h - Test library for Wiring - description
  Copyright (c) 2006 John Doe.  All right reserved.
*/

// ensure this library description is only included once
#ifndef TWDisplay_h
#define TWDisplay_h

// include types & constants of Wiring core API
#include "Arduino.h"

// library interface description
class TWDisplay
{
  // user-accessible "public" interface
  public:
    TWDisplay(HardwareSerial &print, long baud_rate);
    int readN16(long adressByte);
    void writeN16(int write_value, long address);
    void dispPage(int page_id);
    void writeN32(int32_t write_value, long address);

  // library-accessible "private" interface
  private:
     // Variables
     HardwareSerial* TW_serial;
     long baud_rate;
     byte head;
     byte N16_read_cmd;
     byte N16_write_cmd;
     byte N32_write_cmd;
     byte disp_page_cmd;
     byte tail[4];
     
     // Functions
     void writeTail();
     void readTail();
     void TW_write(byte address);
     void writeAddress(int32_t address);
     byte TW_read();
};
#endif
