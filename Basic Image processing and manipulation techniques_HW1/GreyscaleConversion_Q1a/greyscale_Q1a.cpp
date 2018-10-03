// Harikrishna prabhu
//3333077042
//Q1 a Grey Scale Conversion

#include <stdio.h>
#include <iostream>
#include <stdlib.h>

using namespace std;
unsigned char max(unsigned char a,unsigned char b,unsigned char c);
unsigned char min(unsigned char a,unsigned char b,unsigned char c);

int main(int argc, char *argv[])

{
	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel;
	int Size = 512;
	
	// Check for proper syntax
	if (argc < 3){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 1] [Size = 256]" << endl;
		return 0;
	}
	
	// Allocate image data array
	BytesPerPixel = atoi(argv[5]);
	unsigned char Imagedata[Size][Size][BytesPerPixel];
	unsigned char Imagedata_lightness[Size][Size];
	unsigned char Imagedata_average[Size][Size];
	unsigned char Imagedata_luminosity[Size][Size];

	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), Size*Size*BytesPerPixel, file);
	fclose(file);

	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////
	cout<<"Sizeof Image="<<atoi(argv[5])<<endl;
	for(int i=0;i<Size;i++)
	{
		for(int j=0;j<Size;j++)
		{
			//Method 1
			Imagedata_average[i][j]=(Imagedata[i][j][0]+Imagedata[i][j][1]+Imagedata[i][j][2])/3;
			//Method 2
			//Imagedata_luminosity[i][j]=((Imagedata[i][j][0]*0.21)+(Imagedata[i][j][1]*0.72)+(Imagedata[i][j][2]*0.07));
			//Method 3
			//Imagedata_lightness[i][j]=(max(Imagedata[i][j][0],Imagedata[i][j][1],Imagedata[i][j][2])+min(Imagedata[i][j][0],Imagedata[i][j][1],Imagedata[i][j][2]))/2;
		}
	}
	for(int i=0;i<Size;i++)
	{
		for(int j=0;j<Size;j++)
		{
			//Method 2
			Imagedata_luminosity[i][j]=((Imagedata[i][j][0]*0.21)+(Imagedata[i][j][1]*0.72)+(Imagedata[i][j][2]*0.07));
		}
	}
	for(int i=0;i<Size;i++)
	{
		for(int j=0;j<Size;j++)
		{
			//Method 3
			Imagedata_lightness[i][j]=(max(Imagedata[i][j][0],Imagedata[i][j][1],Imagedata[i][j][2])+min(Imagedata[i][j][0],Imagedata[i][j][1],Imagedata[i][j][2]))/2;
		}
	}
	// Write image data (filename specified by second argument) from image data matrix

	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(Imagedata_average, sizeof(unsigned char), Size*Size*BytesPerPixel, file);
	fclose(file);

	if (!(file=fopen(argv[3],"wb"))) {
		cout << "Cannot open file: " << argv[4] << endl;
		exit(1);
	}
	fwrite(Imagedata_luminosity, sizeof(unsigned char), Size*Size*BytesPerPixel, file);
	fclose(file);
	if (!(file=fopen(argv[4],"wb"))) {
		cout << "Cannot open file: " << argv[4] << endl;
		exit(1);
	}
	fwrite(Imagedata_lightness, sizeof(unsigned char), Size*Size*BytesPerPixel, file);
	fclose(file);

	return 0;
}
unsigned char max(unsigned char a, unsigned char b, unsigned char c)
{
	unsigned char t;
	unsigned char max = (a>b)?a:b;
	return((max>c)?max:c);
}
unsigned char min(unsigned char a, unsigned char b, unsigned char c)
{
	unsigned char t;
	unsigned char min = (a<b)?a:b;
	return((min<c)?min:c);
}
