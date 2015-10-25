#include "Frame.h"

int main() {
	Frame wot(12,0x12);
	printf("%d\n",wot.getChecksum());
}