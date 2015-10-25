#include <bits/stdc++.h>
#include "Frame.h"
#include "Ack.h"

using namespace std;

int main(int _argc, char *_argv[]) {
  /*
  Frame f(69, 'A');
  cout << f.isValid() << endl;
  cout << f.getFrameNumber() << endl;
  cout << f.getData() << endl;
*/
/*
  Byte* packet = f.serialize();
  Frame a;
  a.unserialize(packet);
  cout << a.isValid() << endl;
  cout << a.getFrameNumber() << endl;
  cout << a.getData() << endl;
*/
  Ack a(4, 69);
  cout << a.isValid() << endl;
  cout << a.getFrameNumber() << endl;
  cout << a.getAck() << endl;
  Byte * b = a.serialize();

  Ack c;
  c.unserialize(b);
  cout << c.isValid() << endl;
  cout << c.getFrameNumber() << endl;
  cout << c.getAck() << endl;
  return 0;
}