#include <iostream>
#include <fstream>
#include <sstream>
#include "image.h"
#include <iomanip>
#include <string.h>

int main(){
	Image img("test.bmp", 1);
	img.getHeaderInfo();
	img.getImageInfo();
	img.testing();
	return 0;
}