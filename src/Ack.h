#ifndef __ACK_H
#define __ACK_H

#include "common.h"

using namespace std;

class Ack {
public:
<<<<<<< HEAD
  ACK(Byte ack, unsigned int frame_number) {
    
=======
  Ack() {

  }
  Ack(Byte b, int frameNumber) {

>>>>>>> 1011e348b60b2f41e08af7fd11b62d15922f42c9
  }
  bool isValid() {

  }
  char* serialize() {

  }
  void unserialize(char* c) {

  }
  Byte getAck() {
    return value;
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
  Byte value;
  unsigned int frame_number;
  unsigned int checksum;
};

#endif
