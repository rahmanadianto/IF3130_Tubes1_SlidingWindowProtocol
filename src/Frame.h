#ifndef __FRAME_H
#define __FRAME_H

#include "common.h"

class Frame {
public:
  Frame() {}
  Frame(int frameNumber, Byte Data) {
    soh = SOH;
    stx = STX;
    etx = ETX;
    frame_number = frameNumber;
    data = Data;
    checksum = 0;
    checksum = getCRC(serialize());
  }

  // check if checksum == generated checksum
  bool isValid() {
    return (checksum==getCRC(serialize()) && soh==SOH && stx==STX && etx==ETX);
  }

  // convert this to char*
  Byte* serialize() {
    Byte* temp = new Byte[9];
    temp[0]=soh;
    temp[1]=(frame_number >> 24) & 0xFF;
    temp[2]=(frame_number >> 16) & 0xFF;
    temp[3]=(frame_number >> 8) & 0xFF;
    temp[4]=frame_number & 0xFF;
    temp[5]=stx;
    temp[6]=data;
    temp[7]=etx;
    temp[8]=checksum;
    return temp;
  }

  // convert char* to this
  void unserialize(Byte* c) {
    frame_number = (unsigned char)c[1] << 24 | (unsigned char)c[2] << 16 | (unsigned char)c[3] << 8 | (unsigned char)c[4];
    checksum = c[8];
    data = c[6];
  }

  unsigned int getFrameNumber() {
    return frame_number;
  }

  Byte getData() {
    return data;
  }

  Byte getChecksum() {
    return checksum;
  }

private:
  // return checksum computed based on data
  Byte getCRC(Byte* BitString) {
    unsigned long x = 0;
    for (int i=0; i<8; i++)
      x = (x<<8) + (unsigned long) BitString[i];
    unsigned long polynomial = 0xEA80000000000000;
    for (int i=0; i<56; i++) {
      if ((x>>(63-i)) % 2 == 1)
        x = x ^ polynomial;
      else
        x = x ^ 0x0;
      polynomial = polynomial >> 1;
    }

    x = x<<8;
    polynomial = 0xEA80;
    for (int i=0; i<8; i++) {
      if ((x>>(15-i)) % 2 == 1)
        x = x ^ polynomial;
      else
        x = x ^ 0x0;
      polynomial = polynomial >> 1;
    }

    return (Byte)x;
  }

  Byte soh;
  Byte stx;
  Byte etx;
  unsigned int frame_number;
  Byte checksum;
  Byte data;
};

#endif
