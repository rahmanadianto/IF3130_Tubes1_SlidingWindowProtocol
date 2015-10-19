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
    checksum = getCRC();
  }

  // check if checksum == generated checksum
  bool isValid() {
    return (checksum==getCRC() && soh==SOH && stx==STX && etx==ETX);
  }

  // convert this to char*
  char* serialize() {
    char* temp;
    temp[0]=soh;
    temp[1]=frame_number & 0xFF;
    temp[2]=(frame_number >> 8) & 0xFF;
    temp[3]=(frame_number >> 16) & 0xFF;
    temp[4]=(frame_number >> 24) & 0xFF;
    temp[5]=stx;
    temp[6]=data;
    temp[7]=etx;
    temp[8]=checksum;
    return temp;
  }

  // convert char* to this
  void unserialize(char* c) {
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

private:
  // return checksum computed based on data
  /*Byte getCRC() {
    char* temp;
    temp[0]=0;
    temp[1]=0;
    temp[2]=0;
    temp[3]=0;
    temp[4]=0;
    temp[5]=SOH;
    temp[6]=frame_number & 0xFF;
    temp[7]=(frame_number >> 8) & 0xFF;
    temp[8]=(frame_number >> 16) & 0xFF;
    temp[9]=(frame_number >> 24) & 0xFF;
    temp[10]=STX;
    temp[11]=data;
    temp[12]=ETX;
    temp[13]=0;
    temp[14]=0;
    temp[15]=0;

    Byte quotient = (Byte)temp[5-12];
    Byte divisor=0xEF;
    Byte temp2; //= (Byte)temp;

    while (quotient!=0x0) {
      temp2 = temp2 ^ divisor;
      divisor >> 1;
      quotient = temp2 & 0x7F8;
    }
    Byte remainder = temp2 & 0x1F;
    return remainder;
  }

  char *MakeCRC(char *BitString)
  {
    static char Res[9]; // CRC Result
    char CRC[8];
    int  i;
    char DoInvert;
   
    for (i=0; i<8; ++i)  CRC[i] = 0;                    // Init before calculation
   
    for (i=0; i<strlen(BitString); ++i)
    {
      DoInvert = ('1'==BitString[i]) ^ CRC[7];         // XOR required?

      CRC[7] = CRC[6];
      CRC[6] = CRC[5];
      CRC[5] = CRC[4] ^ DoInvert;
      CRC[4] = CRC[3] ^ DoInvert;
      CRC[3] = CRC[2];
      CRC[2] = CRC[1];
      CRC[1] = CRC[0];
      CRC[0] = DoInvert;
     }
      
    for (i=0; i<8; ++i)
      Res[7-i] = CRC[i] ? '1' : '0'; // Convert binary to ASCII
    Res[8] = 0;                      // Set string terminator

    return(Res);
   }*/

  Byte getCRC(char *BitString)
  {
    static char Res[9]; // CRC Result
    char CRC[8];
    int  i;
    char DoInvert;
    Byte result;
   
    for (i=0; i<8; ++i)
      CRC[i] = 0;
   
    for (i=0; i<strlen(BitString); ++i) {
      DoInvert = ('1'==BitString[i]) ^ CRC[7];
      CRC[7] = CRC[6];
      CRC[6] = CRC[5];
      CRC[5] = CRC[4] ^ DoInvert;
      CRC[4] = CRC[3] ^ DoInvert;
      CRC[3] = CRC[2];
      CRC[2] = CRC[1];
      CRC[1] = CRC[0];
      CRC[0] = DoInvert;
    }
      
    for (i=0; i<8; ++i)
      Res[7-i] = CRC[i] ? '1' : '0'; // Convert binary to ASCII
    Res[8] = 0;  

    for (int i=0;i<8;i++) {
      if (Res[i]=='0')
        result |= 0 << i;
      else
        result |= 1 << i;
    }
    return(result);
  }


  Byte soh;
  Byte stx;
  Byte etx;
  unsigned int frame_number;
  Byte checksum;
  Byte data;
};

#endif
