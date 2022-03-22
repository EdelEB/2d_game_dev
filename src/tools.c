#include "tools.h"

int simple_random(int lower, int upper) {
	return ((rand()%(upper-lower+1))+lower);
}