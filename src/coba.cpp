#include <bits/stdc++.h>
#include "Frame.h"

using namespace std;

int main(int _argc, char *_argv[]) {
  Frame f(69, 'A');
  cout << f.isValid() << endl;
  cout << f.getFrameNumber() << endl;
  cout << f.getData() << endl;

  Byte* packet = f.serialize();
  Frame a;
  a.unserialize(packet);
  cout << a.isValid() << endl;
  cout << a.getFrameNumber() << endl;
  cout << a.getData() << endl;
  return 0;
}