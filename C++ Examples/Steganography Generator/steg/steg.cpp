/*
	Ben Golden
	11/29/2016
	steg.cpp

	steg.cpp takes an image or text file and hides it in an image using bitwise operations, only slightly altering the original image. It can also generate a text file containing the pixel values of a given image.
*/

#include <OpenImageIO/imageio.h>

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cmath>

#ifdef __APPLE__
#  pragma clang diagnostic ignored "-Wdeprecated-declarations"
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#ifndef PI
#define PI 3.1415926536
#endif

using namespace std;
OIIO_NAMESPACE_USING

unsigned char* pixels;
unsigned char* xpixels;
char* text;
float digraph[96][96];
int WIDTH = 1000;
int HEIGHT = 1000;
int xres;
int yres;
int tres;

//OpenGL Windows
int window1, window2;

//Reads the image from the given file and stores its data in a pixmap. Also stores width and height data in global variables
void readImage(string infile){

	////Some code to read images borrowed from OpenImageIO Programmer Documentation, page 55

	//Read image from file
	ImageInput *in = ImageInput::open(infile);

	if (! in)
		return;

	//Create ImageSpec and access image specifications
	const ImageSpec &spec = in->spec();

	WIDTH = spec.width;
	HEIGHT = spec.height;

	//Store image data
	pixels = new unsigned char[WIDTH*HEIGHT*4];

	in->read_image (TypeDesc::UINT8, &pixels[0]);
	in->close ();
	delete (in);

	//When an image has 3 channels (RGB), loops adjust pixmap for 4 channels (RGBA)
	if(spec.nchannels == 3){
		//First loop moves pixel values to appropriate positions, so that every fourth value can be used as an alpha channel
		for(int i = WIDTH*HEIGHT*3; i > 0; i -= 3){
			pixels[i*4/3-2] = pixels[i-1];
			pixels[i*4/3-3] = pixels[i-2];
			pixels[i*4/3-4] = pixels[i-3];
		}
		//Second loop sets alpha value of every fourth pixel value to 255
		for(int i = 3; i < WIDTH*HEIGHT*4; i += 4)
			pixels[i] = 255;
	}

	//When an image has 1 channel (monochrome), loops adjust pixmap for 4 channels (RGBA)
	if(spec.nchannels == 1){
		//First loop moves color values to RGB positions and leaves every fourth value to be used as an alpha channel
		for(int i = WIDTH*HEIGHT; i > 0; i--){
			pixels[i*4-2] = pixels[i-1];
			pixels[i*4-3] = pixels[i-1];
			pixels[i*4-4] = pixels[i-1];
		}
		//Second loop sets alpha value of every fourth pixel to 255
		for(int i = 3; i < WIDTH*HEIGHT*4; i += 4)
			pixels[i] = 255;
	}

}

//Reads the image to be encoded into a larger image
void readSecretImage(string infile){

	////Some code to read images borrowed from OpenImageIO Programmer Documentation, page 55

	//Read image from file
	ImageInput *in = ImageInput::open(infile);

	if (! in)
		return;

	//Create ImageSpec and access image specifications
	const ImageSpec &spec = in->spec();

	xres = spec.width;
	yres = spec.height;

	if(xres > WIDTH/2 || yres > HEIGHT/2){
		cerr << "Input image too large" << endl;
		return;
	}

	//Store image data
	xpixels = new unsigned char[xres*yres*4];

	in->read_image (TypeDesc::UINT8, &xpixels[0]);
	in->close ();
	delete (in);

	//When an image has 3 channels (RGB), loops adjust pixmap for 4 channels (RGBA)
	if(spec.nchannels == 3){
		//First loop moves pixel values to appropriate positions, so that every fourth value can be used as an alpha channel
		for(int i = xres*yres*3; i > 0; i -= 3){
			xpixels[i*4/3-2] = xpixels[i-1];
			xpixels[i*4/3-3] = xpixels[i-2];
			xpixels[i*4/3-4] = xpixels[i-3];
		}
		//Second loop sets alpha value of every fourth pixel value to 255
		for(int i = 3; i < xres*yres*4; i += 4)
			xpixels[i] = 255;
	}

	//When an image has 1 channel (monochrome), loops adjust pixmap for 4 channels (RGBA)
	if(spec.nchannels == 1){
		//First loop moves color values to RGB positions and leaves every fourth value to be used as an alpha channel
		for(int i = xres*yres; i > 0; i--){
			xpixels[i*4-2] = xpixels[i-1];
			xpixels[i*4-3] = xpixels[i-1];
			xpixels[i*4-4] = xpixels[i-1];
		}
		//Second loop sets alpha value of every fourth pixel to 255
		for(int i = 3; i < xres*yres*4; i += 4)
			xpixels[i] = 255;
	}

}

//Reads text from a file
void readText(char* infile){

	//tres keeps track of number of characters in the text file
	tres = 0;

	text = new char[WIDTH*HEIGHT];

	ifstream File(infile);

	if(File.is_open()){

		while(File.get(text[tres])){
			tres++;
		}

		File.close();
	}

	if(tres > WIDTH*HEIGHT) cerr << "Input text file too large" << endl;

}

//Function to insert an image into another image
void encodeImageInImage(){

	unsigned char pixels2D[WIDTH*4][HEIGHT];
	unsigned char xpixels2D[xres*4][yres];

	//Pixel values are temporarily placed in 2D arrays
	for(int i = 0; i < WIDTH*4; i++){
		for(int j = 0; j < HEIGHT; j++){

			pixels2D[i][j] = pixels[i+j*WIDTH*4];
		}
	}

	for(int i = 0; i < xres*4; i++){
		for(int j = 0; j < yres; j++){

			xpixels2D[i][j] = xpixels[i+j*xres*4];
		}
	}

	//Loop places values from the secret image into the output image
	//Bitwise operations are utilized so that values from each pixel in the secret image are stored in the last two bits of an output pixel
	//x1x2x3x4x5x6x7x8 & 252 = x1x2x3x4x5x600; (y1y2y3y4y5y6y7y8 >> 6) & 3 = 000000y1y2; x1x2x3x4x5x600 + 000000y1y2 = x1x2x3x4x5x6y1y2
	for(int i = 0; i < xres*4; i++){
		for(int j = 0; j < yres; j++){

			pixels2D[i*2][j*2] = pixels2D[i*2][j*2] & 252;
			pixels2D[i*2][j*2] += (xpixels2D[i][j] >> 6) & 3;
			pixels2D[i*2+1][j*2] = pixels2D[i*2+1][j*2] & 252;
			pixels2D[i*2+1][j*2] += (xpixels2D[i][j] >> 4) & 3;
			pixels2D[i*2][j*2+1] = pixels2D[i*2][j*2+1] & 252;
			pixels2D[i*2][j*2+1] += (xpixels2D[i][j] >> 2) & 3;
			pixels2D[i*2+1][j*2+1] = pixels2D[i*2+1][j*2+1] & 252;
			pixels2D[i*2+1][j*2+1] += xpixels2D[i][j] & 3;

		}
	}

	//New pixel values are saved to the pixmap for the output image
	for(int i = 0; i < WIDTH*4; i++){
		for(int j = 0; j < HEIGHT; j++){

			pixels[i+j*WIDTH*4] = pixels2D[i][j];
		}
	}
}

//Function to extract pixel values hidden in an image
void extractImageInImage(){

	unsigned char pixels2D[WIDTH*4][HEIGHT];

	xres = (WIDTH+1)/2;
	yres = (HEIGHT+1)/2;
	xpixels = new unsigned char[xres*yres*4];
	unsigned char xpixels2D[xres*4][yres];

	for(int i = 0; i < WIDTH*4; i++){
		for(int j = 0; j < HEIGHT; j++){

			pixels2D[i][j] = pixels[i+j*WIDTH*4];
		}
	}

	//Bitwise operations are used to pull hidden pixel values from given pixels
	//(x1x2x3x4x5x6y1y2 & 3) = y1y2000000; y1y2000000 + 00y3y40000 + 0000y5y600 + 000000y7y8 = y1y2y3y4y5y6y7y8;
	for(int i = 0; i < xres*4; i++){
		for(int j = 0; j < yres; j++){

			xpixels2D[i][j] = (pixels2D[i*2][j*2] & 3) << 6;
			xpixels2D[i][j] += (pixels2D[i*2+1][j*2] & 3) << 4;
			xpixels2D[i][j] += (pixels2D[i*2][j*2+1] & 3) << 2;
			xpixels2D[i][j] += (pixels2D[i*2+1][j*2+1] & 3);

		}
	}

	for(int i = 0; i < xres*4; i++){
		for(int j = 0; j < yres; j++){

			xpixels[i+j*xres*4] = xpixels2D[i][j];
		}
	}
}

//Function to insert text into an image
void encodeTextInImage(){

	//Text values are separated into two bits and hidden in the last two bits of each pixel of the image
	//(t1t2t3t4t5t6t7t8 >> 6 & 3) = 000000t1t2; (x1x2x3x4x5x6x7x8 & 252) = x1x2x3x4x5x600; x1x2x3x4x5x600 + 000000t1t2 = x1x2x3x4x5x6t1t2;
	for(int i = 0; i < tres; i++){
		pixels[i*4] = (pixels[i*4] & 252) + ((text[i] >> 6) & 3);
		pixels[i*4+1] = (pixels[i*4+1] & 252) + ((text[i] >> 4) & 3);
		pixels[i*4+2] = (pixels[i*4+2] & 252) + ((text[i] >> 2) & 3);
		pixels[i*4+3] = (pixels[i*4+3] & 252) + (text[i] & 3);
	}

	//Length of the text is saved in the last eight pixels of the image
	pixels[WIDTH*HEIGHT*4-8] = (pixels[WIDTH*HEIGHT*4-8] & 252) + (tres >> 14 & 3);
	pixels[WIDTH*HEIGHT*4-7] = (pixels[WIDTH*HEIGHT*4-7] & 252) + (tres >> 12 & 3);
	pixels[WIDTH*HEIGHT*4-6] = (pixels[WIDTH*HEIGHT*4-6] & 252) + (tres >> 10 & 3);
	pixels[WIDTH*HEIGHT*4-5] = (pixels[WIDTH*HEIGHT*4-5] & 252) + (tres >> 8 & 3);
	pixels[WIDTH*HEIGHT*4-4] = (pixels[WIDTH*HEIGHT*4-4] & 252) + (tres >> 6 & 3);
	pixels[WIDTH*HEIGHT*4-3] = (pixels[WIDTH*HEIGHT*4-3] & 252) + (tres >> 4 & 3);
	pixels[WIDTH*HEIGHT*4-2] = (pixels[WIDTH*HEIGHT*4-2] & 252) + (tres >> 2 & 3);
	pixels[WIDTH*HEIGHT*4-1] = (pixels[WIDTH*HEIGHT*4-1] & 252) + (tres & 3);

}

//Function to extract text from an image
void extractTextInImage(){

	//Pulls the length of the text from the last six pixels of the image
	tres = (~pixels[WIDTH*HEIGHT*4-8] & 3) << 14;
	tres += (~pixels[WIDTH*HEIGHT*4-7] & 3) << 12;
	tres += (~pixels[WIDTH*HEIGHT*4-6] & 3) << 10;
	tres += (pixels[WIDTH*HEIGHT*4-5] & 3) << 8;
	tres += (pixels[WIDTH*HEIGHT*4-4] & 3) << 6;
	tres += (pixels[WIDTH*HEIGHT*4-3] & 3) << 4;
	tres += (pixels[WIDTH*HEIGHT*4-2] & 3) << 2;
	tres += (pixels[WIDTH*HEIGHT*4-1] & 3);

	text = new char[tres];

	//Pulls the hidden text values from the pixels of the image
	//(x1x2x3x4x5x6t1t2 & 3) = t1t2000000; t1t2000000 + 00t3t40000 + 0000t5t600 + 000000t7t8 = t1t2t3t4t5t6t7t8;
	for(int i = 0; i < tres; i++){

		if((pixels[i*4+1] & 3) == 3){
			text[i] = (pixels[i*4] & 1) << 6;
			text[i] += (pixels[i*4+1] & 3) << 4;
			text[i] += (pixels[i*4+2] & 3) << 2;
			text[i] += (pixels[i*4+3] & 3);
		}

		else{
			text[i] = ((pixels[i*4] + 1) & 1) << 6;
			text[i] += ((pixels[i*4+1] + 1) & 3) << 4;
			text[i] += ((pixels[i*4+2] + 1) & 3) << 2;
			text[i] += (pixels[i*4+3] & 3);
		}

	}
}

//Function to initialize digraph
void digraphInit(){

	//Starts digraph values at 0
	for(int i = 0; i < 96; i++){
		for(int j = 0; j < 96; j++){
			digraph[i][j] = 0.0;
		}
	}

	int count = 0;

	char a, b;

	//Reads in a text file; in this case, a review of the worst 20 science fiction movies by Brian Rose from textfiles.com
	ifstream File("worst20.txt");

	if(File.is_open()){

		//b and a are set to first two characters in the file
		File.get(b);

		a = b;

		//Replaces all characters with values under 32 (non-printing characters) with spaces
		if(int(a) < 32) a = ' ';

		while(File.good()){

			//b is set to next character in file. In each iteration, b will be one character ahead of a
			File.get(b);

			if(int(b) < 32) b = ' ';

			digraph[int(a-32)][int(b-32)] += 1.0;

			count++;

			a = b;

		}

	}

	File.close();

	//Loops through each proportion and divides by the count
	if(count > 0){

		for(int i = 0; i < 96; i++){
			for(int j = 0; j < 96; j++){

				digraph[i][j] = digraph[i][j] / double(count);

			}
		}

	}
}

//Finds the most frequent occurence in a group of digraphs
void digraphCmp(unsigned char a, int i){

	int b = int(a >> 4);
	int c = int(a % 16);

	float max = 0.0;
	char max1, max2;

	for(int j = 0; j < 6; j++){
		for(int k = 0; k < 6; k++){

			if(digraph[j*16+b][k*16+c] > max){
				max = digraph[j*16+b][k*16+c];
				max1 = char(j*16+b+32);
				max2 = char(k*16+c+32);
			}
		}
	}

	text[2*i] = max1;
	text[2*i+1] = max2;
}

//Encodes image into digraphed text
void encodeImageInText(){

	if(WIDTH > 256 || HEIGHT > 256) cerr << "file too large" << endl;

	//Initializes the digraph and text length
	digraphInit();

	tres = WIDTH*HEIGHT*8 + 4;
	text = new char[tres];

	for(int i = 0; i < WIDTH*HEIGHT*4; i++) digraphCmp(pixels[i], i);

	//Saves the width and height in the last 6 characters of text
	unsigned char a;

	a = char(WIDTH);
	digraphCmp(a, WIDTH*HEIGHT*4);
	a = char(HEIGHT);
	digraphCmp(a, WIDTH*HEIGHT*4 + 1);

}

//Extract image into digraphed text
void extractImageInText(){

	//Get width and height values from last 4 characters in text file
	WIDTH = int((text[tres-4] % 16) << 4) + int(text[tres-3] % 16);
	HEIGHT = int((text[tres-2] % 16) << 4) + int(text[tres-1] % 16);

	pixels = new unsigned char[WIDTH*HEIGHT*4];

	//Put pixel values in pixel array
	for(int i = 0; i < WIDTH*HEIGHT*4; i++){
		pixels[i] = text[i*2+1] % 16;
		pixels[i] += text[i*2] << 4;
	}
}

//Display callback for original image
void displayImage(){

	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

	//Draws image using the given pixels
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0 ,WIDTH, 0, HEIGHT, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glRasterPos2i(0, HEIGHT);
	glPixelZoom(1, -1);
	glDrawPixels(WIDTH, HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
}


//Display callback for hidden image
void displaySecretImage(){

	//Draws image using the given pixels
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, xres, 0, yres, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glRasterPos2i(0, yres);
	glPixelZoom(1, -1);
	glDrawPixels(xres, yres, GL_RGBA, GL_UNSIGNED_BYTE, xpixels);
}

//Writes image to given file, if any
void writeImage(char* outfile){

	////Some code to write images borrowed from OpenIO Programmer Documentation, page 27

	ImageOutput *out = ImageOutput::create (outfile);

	if (! out)
		return;

	//Writes image data to given file
	ImageSpec spec (WIDTH, HEIGHT, 4, TypeDesc::UINT8);
	out->open (outfile, spec);
	out->write_image (TypeDesc::UINT8, pixels);
	out->close ();
	delete (out);

}

//Writes image to given file, if any
void writeSecretImage(char* outfile){

	////Some code to write images borrowed from OpenIO Programmer Documentation, page 27

	ImageOutput *out = ImageOutput::create (outfile);

	if (! out)
		return;

	//Writes image data to given file
	ImageSpec spec (xres, yres, 4, TypeDesc::UINT8);
	out->open (outfile, spec);
	out->write_image (TypeDesc::UINT8, xpixels);
	out->close ();
	delete (out);

}

//Writes text to a given file, if any
void writeText(char* outfile){

	ofstream File(outfile);

	if(File.is_open()){

		for(int i = 0; i < tres; i++){
			File << text[i];
		}
		File.close();
	}
}

int main(int argc, char* argv[]){

	char* o = NULL;

	//-n encodes image in image, -x extracts image in image, -nt encodes text in image, -xt extracts text in image, -ni encodes image in text, -xi extracts image in text
	if(argv[1][2] == 't'){
		if(argv[1][1] == 'n'){
			//Reads first image, text file, and outfile from command line
			if(argc != 5){
				cerr << "Incorrect command line" << endl;
				return 1;
			}

			char* i = argv[2];
			char* t = argv[3];
			o = argv[4];

			glutInit(&argc, argv);

			readImage(i);

			readText(t);

			encodeTextInImage();
		}

		else if(argv[1][1] == 'x'){
			//Reads image and outfile from command line
			if(argc > 4 || argc < 3){
				cerr << "Incorrect command line" << endl;
				return 1;
			}

			char* i = argv[2];
			if(argc == 4) o = argv[3];

			glutInit(&argc, argv);

			readImage(i);

			extractTextInImage();

		}

		//Creates window using given parameters
		glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
		glutInitWindowSize(WIDTH, HEIGHT);

		//First window for unaltered image
		window1 = glutCreateWindow("Image Hiding Text");
		glutDisplayFunc(displayImage);	// display callback

		cout << text << endl;

		if(argc == 5 && argv[1][1] == 'n') writeImage(o);
		if(argc == 4 && argv[1][1] == 'x') writeText(o);

		glutMainLoop();
	}

	else if(argv[1][2] == 'i'){
		if(argv[1][1] == 'n'){
			//Reads image and output text file in command line
			if(argc > 4 || argc < 3){
				cerr << "Incorrect command line" << endl;
				return 1;
			}

			char* i = argv[2];
			if(argc == 4) o = argv[3];

			glutInit(&argc, argv);

			readImage(i);

			encodeImageInText();
		}

		else if(argv[1][1] == 'x'){
			//Reads text and output image in command line
			if(argc > 4 || argc < 3){
				cerr << "Incorrect command line" << endl;
				return 1;
			}

			char* t = argv[2];
			if(argc == 4) o = argv[3];

			glutInit(&argc, argv);

			readText(t);

			extractImageInText();

		}

		//Creates window using given parameters
		glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
		glutInitWindowSize(WIDTH, HEIGHT);

		//First window for unaltered image
		window1 = glutCreateWindow("Image Hiding Text");
		glutDisplayFunc(displayImage);	// display callback

		if(argc == 4 && argv[1][1] == 'n') writeText(o);
		if(argc == 4 && argv[1][1] == 'x') writeImage(o);

		glutMainLoop();
	}

	else{
		if(argv[1][1] == 'n'){
			//Reads first image, second image, and outfile from command line
			if(argc > 5 || argc < 4){
				cerr << "Incorrect command line" << endl;
				return 1;
			}

			char* i1 = argv[2];
			char* i2 = argv[3];
			if(argc == 5) o = argv[4];

			glutInit(&argc, argv);

			readImage(i1);

			readSecretImage(i2);

			encodeImageInImage();
		}

		else if(argv[1][1] == 'x'){
			//Reads first image and outfile from command line
			if(argc > 4 || argc < 3){
				cerr << "Incorrect command line" << endl;
				return 1;
			}

			char* i1 = argv[2];
			if(argc == 4) o = argv[3];

			glutInit(&argc, argv);

			readImage(i1);

			extractImageInImage();
		}

		//Creates window using given parameters
		glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
		glutInitWindowSize(WIDTH, HEIGHT);

		//First window for unaltered image
		window1 = glutCreateWindow("Image Hiding Image");
		glutDisplayFunc(displayImage);	// first display callback

		//Second window for warped image
		window2 = glutCreateWindow("Hidden Image");
		glutDisplayFunc(displaySecretImage);	// second display callback

		glutReshapeWindow(xres, yres);

		if(argc == 5 && argv[1][1] == 'n') writeImage(o);
		if(argc == 4 && argv[1][1] == 'x') writeSecretImage(o);

		glutMainLoop();
	}

	delete pixels;
	delete xpixels;
	delete text;

	return 0;
}
