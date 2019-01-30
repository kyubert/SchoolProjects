#include <stdlib.h>
#include <iostream>

using namespace std;

int main(){

	int pixels2D[32][8];
	int xpixels2D[16][4];

	int xres = 4;
	int yres = 4;

	for(int j = 0; j < yres; j++){
		for(int i = 0; i < xres*4; i++){

			xpixels2D[i][j] = rand() % 256;
			pixels2D[i*2][j*2] = rand() % 256;
			pixels2D[i*2+1][j*2] = rand() % 256;
			pixels2D[i*2][j*2+1] = rand() % 256;
			pixels2D[i*2+1][j*2+1] = rand() % 256;
		}
	}

	for(int j = 0; j < yres; j++){
		for(int i = 0; i < xres*4; i++){

			pixels2D[i*2][j*2] = pixels2D[i*2][j*2] >> 2 << 2;
			cout << pixels2D[i*2][j*2] << " ";
			pixels2D[i*2][j*2] += (xpixels2D[i][j] >> 6) & 3;
			cout << pixels2D[i*2][j*2] << endl;
			pixels2D[i*2+1][j*2] = pixels2D[i*2+1][j*2] >> 2 << 2;
			cout << pixels2D[i*2+1][j*2] << " ";
			pixels2D[i*2+1][j*2] += (xpixels2D[i][j] >> 4) & 3;
			cout << pixels2D[i*2+1][j*2] << endl;
			pixels2D[i*2][j*2+1] = pixels2D[i*2][j*2+1] >> 2 << 2;
			cout << pixels2D[i*2][j*2+1] << " ";
			pixels2D[i*2][j*2+1] += (xpixels2D[i][j] >> 2) & 3;
			cout << pixels2D[i*2][j*2+1] << endl;
			pixels2D[i*2+1][j*2+1] = pixels2D[i*2+1][j*2+1] >> 2 << 2;
			cout << pixels2D[i*2+1][j*2+1] << " ";
			pixels2D[i*2+1][j*2+1] += xpixels2D[i][j] & 3;
			cout << pixels2D[i*2+1][j*2+1] << endl;

		}
	}


	for(int j = 0; j < yres; j++){
		for(int i = 0; i < xres*4; i++){

			cout << xpixels2D[i][j] << " ";
			xpixels2D[i][j] = (pixels2D[i*2][j*2] & 3) << 6;
			xpixels2D[i][j] += (pixels2D[i*2+1][j*2] & 3) << 4;
			xpixels2D[i][j] += (pixels2D[i*2][j*2+1] & 3) << 2;
			xpixels2D[i][j] += (pixels2D[i*2+1][j*2+1] & 3);
			cout << xpixels2D[i][j] << " ";

		}
		cout << endl;
	}

	return 0;
}
