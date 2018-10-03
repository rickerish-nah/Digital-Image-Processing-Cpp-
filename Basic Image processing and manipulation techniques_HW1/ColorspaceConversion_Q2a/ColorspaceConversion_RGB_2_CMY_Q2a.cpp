//Harikrishna Prabhu
//3333077042
// Q1 a2 CMY(K)
#include <stdio.h>
#include <iostream>
#include <stdlib.h>

using namespace std;
unsigned char max(unsigned char a,unsigned char b,unsigned char c);
//unsigned char min(unsigned char a,unsigned char b,unsigned char c);

int main(int argc, char *argv[])

{
	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel;
	int size1 = 854 , size2 = 480;
	
	// Check for proper syntax
	if (argc < 3){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 1] [size = 256]" << endl;
		return 0;
	}
	// Allocate image data array
	BytesPerPixel = atoi(argv[5]);
	unsigned char Imagedata[size1][size2][BytesPerPixel];
	unsigned char Imagedata_cyan[size1][size2];
	unsigned char Imagedata_magenta[size1][size2];
	unsigned char Imagedata_yellow[size1][size2];
	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), size1*size2*BytesPerPixel, file);
	fclose(file);

	///////////////////////// PROCESSING CODE HERE /////////////////////////
	cout<<"sizeof Image="<<atoi(argv[5])<<endl;
	//RGB[0,255] to CMY[0,1]
	for(int i=0;i<size1;i++)
	{
		for(int j=0;j<size2;j++)
		{
			unsigned char k = 1-max(Imagedata[i][j][0]/255,Imagedata[i][j][1]/255,Imagedata[i][j][2]/255);
			Imagedata_cyan[i][j] = (1-(Imagedata[i][j][0]/255)-k)/(1-k);
			Imagedata_magenta[i][j] = (1-(Imagedata[i][j][1]/255)-k)/(1-k);
			Imagedata_yellow[i][j] = (1-(Imagedata[i][j][2]/255)-k)/(1-k);
		}
	}

	/*for(int i=0;i<size1;i++)
	{
		for(int j=0;j<size2;j++)
		{
			unsigned char w = 1-max(Imagedata[i][j][0],Imagedata[i][j][1],Imagedata[i][j][2]);
			Imagedata_cyan[i][j] = (1-w-(Imagedata[i][j][0]/255))/1-w;
			Imagedata_magenta[i][j] = (1-w-(Imagedata[i][j][1]/255))/1-w; 
			Imagedata_yellow[i][j] = (1-w-(Imagedata[i][j][2]/255))/1-w;
		}
	}*/
	// Write image data (filename specified by second argument) from image data matrix

	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(Imagedata_cyan, sizeof(unsigned char), size1*size2*BytesPerPixel, file);
	fclose(file);

	if (!(file=fopen(argv[3],"wb"))) {
		cout << "Cannot open file: " << argv[4] << endl;
		exit(1);
	}
	fwrite(Imagedata_magenta, sizeof(unsigned char), size1*size2*BytesPerPixel, file);
	fclose(file);
	if (!(file=fopen(argv[4],"wb"))) {
		cout << "Cannot open file: " << argv[4] << endl;
		exit(1);
	}
	fwrite(Imagedata_yellow, sizeof(unsigned char), size1*size2*BytesPerPixel, file);
	fclose(file);

	return 0;
}
unsigned char min(unsigned char a, unsigned char b, unsigned char c)
{
	unsigned char max = ((a>b)?a:b);
	return((max>c)?max:c);
}

