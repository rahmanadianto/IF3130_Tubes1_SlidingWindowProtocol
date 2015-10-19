#ifndef __FRAME_H
#define __FRAME_H

#include "common.h"

class Frame {
public:
  Frame() {

  }
  Frame(int frameNumber, Byte data) {

  }

  // check if checksum == generated checksum
  bool isValid() {

  }

  // convert this to char*
  char* serialize() {

  }

  // convert char* to this
  void unserialize(char* c) {

  }

  unsigned int getFrameNumber() {

  }

private:
  // return checksum computed based on data
  unsigned int generateChecksum() {

  }
  unsigned int hash(unsigned int input) {
    unsigned int val = 5381;
    val += ((input << 5) ^ ((input & 0xf8000000) >> 27));
    return val;
  }

  Byte soh;
  unsigned int frame_number;
  Byte stx;
  Byte etx;
  unsigned int checksum;

  time_t timeout;
  Byte data;
};

#endif
