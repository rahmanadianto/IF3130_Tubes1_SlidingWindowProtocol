#ifndef __ACK_H
#define __ACK_H

#include "common.h"

using namespace std;

class Ack {
public:   
  Ack() {}
  Ack(Byte b, int frameNumber) {
    value = b;
    frame_number = frameNumber;
    checksum = 0;
    checksum = getCRC(serialize());
  }
  bool isValid() {
    return (checksum==getCRC(serialize()));
  }
  Byte* serialize() {
    Byte* temp = new Byte[6];
    temp[0]=value;
    temp[1]=(frame_number >> 24) & 0xFF;
    temp[2]=(frame_number >> 16) & 0xFF;
    temp[3]=(frame_number >> 8) & 0xFF;
    temp[4]=frame_number & 0xFF;
    temp[5]=checksum;
    return temp;
  }
  void unserialize(Byte* c) {
    value = c[0];
    frame_number = (unsigned char)c[1] << 24 | (unsigned char)c[2] << 16 | (unsigned char)c[3] << 8 | (unsigned char)c[4];
    checksum = c[5];
  }
  Byte getAck() {
    return value;
  }
  unsigned int getFrameNumber() {
    return frame_number;
  }

private:
  // return checksum computed based on data
  Byte getCRC(Byte* BitString) {
    unsigned long x = 0;
    for (int i=0; i<5; i++)
      x = (x<<8) + (unsigned long) BitString[i];
    x = x<<8;
    unsigned long polynomial = 0xEA8000000000;
    for (int i=0; i<40; i++) {
      if ((x>>(47-i)) % 2 == 1)
        x = x ^ polynomial;
      else
        x = x ^ 0x0;
      polynomial = polynomial >> 1;
    }
    return (Byte)x;
  }
  Byte value;
  unsigned int frame_number;
  Byte checksum;
};

#endif
