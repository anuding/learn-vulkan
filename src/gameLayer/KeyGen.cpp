#include "KeyGen.h"

keytype KeyGen::genKey() {
	key++;
	return key;
}
