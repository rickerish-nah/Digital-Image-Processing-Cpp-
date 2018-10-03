#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include<math.h>
#include<random>


using namespace std;


unsigned char *fixed_Dithering(unsigned char *Image_data, int height, int width, int bytesPerPixel);
unsigned char *random_Dithering(unsigned char *Image_data, int height, int width, int bytesPerPixel);
unsigned char *matrix22_Dithering(unsigned char *Image_data, int height, int width, int bytesPerPixel);
unsigned char *matrix44_Dithering(unsigned char *Image_data, int height, int width, int bytesPerPixel);
unsigned char *matrix88_Dithering(unsigned char *Image_data, int height, int width, int bytesPerPixel);
//unsigned char *grey4level(unsigned char *Image_data1,unsigned char *Image_data2, int height, int width, int bytesPerPixel);
unsigned char *grey(unsigned char *Image1, unsigned char *Image2, int h,int w, int b);


int main(int argc, char *argv[])
{
	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel=1,N=2,width = 512 , height = 512,option=0;
	
	unsigned char rv=0; 
	unsigned char *Imagedata= new unsigned char [width*height*BytesPerPixel];
	unsigned char *newimage_Fixed= new unsigned char [width*height*BytesPerPixel];
	unsigned char *newimage_random= new unsigned char [width*height*BytesPerPixel];
	unsigned char *newimage_dithering22= new unsigned char [width*height*BytesPerPixel];
	unsigned char *newimage_dithering44= new unsigned char [width*height*BytesPerPixel];
	unsigned char *newimage_dithering88= new unsigned char [width*height*BytesPerPixel];
	unsigned char *output_image= new unsigned char [width*height*BytesPerPixel];


	cout<<"Enter the dithering you'd like to perform: They are,\n (1)fixed_Dithering\n (2)random_Dithering\n (3)matrix22_Dithering\n (4)matrix44_Dithering\n (5)matrix88_Dithering\n (6)All the above + 4 grey level image"<<endl;
	cin>>option;


	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), width*height*BytesPerPixel, file);
	fclose(file);


	switch(option)
	{
		case 1:
		{
			newimage_Fixed=fixed_Dithering(Imagedata,height,width,BytesPerPixel);
			if (!(file=fopen(argv[2],"wb"))) {
				cout << "Cannot open file: " << argv[2] << endl;
			exit(1);
			}
			fwrite(newimage_Fixed, sizeof(unsigned char), width*height*BytesPerPixel, file);
			fclose(file);
			break;

		}
		case 2:
		{
			newimage_random=random_Dithering(Imagedata,height,width,BytesPerPixel);
			if (!(file=fopen(argv[3],"wb"))) {
				cout << "Cannot open file: " << argv[2] << endl;
			exit(1);
			}
			fwrite(newimage_random, sizeof(unsigned char), width*height*BytesPerPixel, file);
			fclose(file);
			break;
		}
		case 3:
		{
			newimage_dithering22=matrix22_Dithering(Imagedata,height,width,BytesPerPixel);
			if (!(file=fopen(argv[4],"wb"))) {
				cout << "Cannot open file: " << argv[2] << endl;
			exit(1);
			}
			fwrite(newimage_dithering22, sizeof(unsigned char), width*height*BytesPerPixel, file);
			fclose(file);
			break;
		}
		case 4:
		{
			newimage_dithering44=matrix44_Dithering(Imagedata,height,width,BytesPerPixel);
			if (!(file=fopen(argv[5],"wb"))) {
				cout << "Cannot open file: " << argv[2] << endl;
			exit(1);
			}
			fwrite(newimage_dithering44, sizeof(unsigned char), width*height*BytesPerPixel, file);
			fclose(file);
			break;
		}
		case 5:
		{
			newimage_dithering88=matrix88_Dithering(Imagedata,height,width,BytesPerPixel);
			if (!(file=fopen(argv[6],"wb"))) {
				cout << "Cannot open file: " << argv[2] << endl;
			exit(1);
			}
			fwrite(newimage_dithering88, sizeof(unsigned char), width*height*BytesPerPixel, file);
			fclose(file);
			break;
		}
		case 6:
		{
			newimage_Fixed=fixed_Dithering(Imagedata,height,width,BytesPerPixel);
			newimage_random=random_Dithering(Imagedata,height,width,BytesPerPixel);
			newimage_dithering22=matrix22_Dithering(Imagedata,height,width,BytesPerPixel);
			newimage_dithering44=matrix44_Dithering(Imagedata,height,width,BytesPerPixel);
			newimage_dithering88=matrix88_Dithering(Imagedata,height,width,BytesPerPixel);
			output_image=grey(newimage_dithering44,newimage_dithering88,height,width,BytesPerPixel);


			//FIXED
			if (!(file=fopen(argv[2],"wb"))) {
				cout << "Cannot open file: " << argv[2] << endl;
			exit(1);
			}
			fwrite(newimage_Fixed, sizeof(unsigned char), width*height*BytesPerPixel, file);
			fclose(file);

			//RANDOM
			if (!(file=fopen(argv[3],"wb"))) {
				cout << "Cannot open file: " << argv[2] << endl;
			exit(1);
			}
			fwrite(newimage_random, sizeof(unsigned char), width*height*BytesPerPixel, file);
			fclose(file);

			//2x2
			if (!(file=fopen(argv[4],"wb"))) {
				cout << "Cannot open file: " << argv[2] << endl;
			exit(1);
			}
			fwrite(newimage_dithering22, sizeof(unsigned char), width*height*BytesPerPixel, file);
			fclose(file);

			//4x4
			if (!(file=fopen(argv[5],"wb"))) {
				cout << "Cannot open file: " << argv[2] << endl;
			exit(1);
			}
			fwrite(newimage_dithering44, sizeof(unsigned char), width*height*BytesPerPixel, file);
			fclose(file);

			//8x8
			if (!(file=fopen(argv[6],"wb"))) {
				cout << "Cannot open file: " << argv[2] << endl;
			exit(1);
			}
			fwrite(newimage_dithering88, sizeof(unsigned char), width*height*BytesPerPixel, file);
			fclose(file);


			//4 grey levels
			if (!(file=fopen(argv[7],"wb"))) {
				cout << "Cannot open file: " << argv[2] << endl;
			exit(1);
			}
			fwrite(output_image, sizeof(unsigned char), width*height*BytesPerPixel, file);
			fclose(file);
			break;



		}
	}

	return 0;


}
//end of main program



unsigned char *fixed_Dithering(unsigned char *Image_data, int height, int width, int bytesPerPixel)
{
	unsigned char *new_image = new unsigned char [width*height*bytesPerPixel];
	int threshold_fixed=127;
	for(int i=0 ; i<height ; i++)
	{
		for(int j=0; j<width ; j++)
		{
			if(Image_data[(i*width)+j]<threshold_fixed)
			{
				new_image[(i*width)+j]=0;
			}
			else if(Image_data[(i*width)+j]>=threshold_fixed)
			{
				new_image[(i*width)+j]=255;
			}
		}
	}

	return new_image;
}
unsigned char *random_Dithering(unsigned char *Imagedata, int height, int width, int bytesPerPixel)
{
	unsigned char *new_image = new unsigned char [width*height*bytesPerPixel];
	//random number generator
	unsigned char threshold_random=0;
	random_device rd;
	mt19937 gen(rd());
	uniform_real_distribution<> dis(0,1);
	/*for(int i =0 ; i<10 ;i++) //check random generator
	{
		rv=255*dis(gen);
		cout<<(int)rv<<'\n';
	}*/
	for(int i=0 ; i<height ; i++)
	{
		for(int j=0; j<width ; j++)
		{
			threshold_random=255*dis(gen);
			if((int)Imagedata[(i*width)+j]<(int)threshold_random)
			{
				new_image[(i*width)+j]=0;
			}
			else if((int)Imagedata[(i*width)+j]>=(int)threshold_random)
			{
				new_image[(i*width)+j]=255;
			}
		}
	}
	return new_image;

}
unsigned char *matrix22_Dithering(unsigned char *Image_data, int height, int width, int bytesPerPixel)
{
	unsigned char *new_image = new unsigned char [width*height*bytesPerPixel];
	unsigned char I[2][2]={ { 1, 2 },{ 3, 0 } },x=0,y=0;
	int N=2;
	float threshold_matrix=0;

	cout<<"The 2x2 Dithering matrix:\n";
	for(int i =0;i<2;i++)
	{
		for(int j=0; j<2 ;j++)
			cout<<(int)I[i][j]<<'\t';
		cout<<"\n";
	}


	for(int i=0; i<height ;i++)
	{
		for(int j=0; j<width ;j++)
		{
			x= i%N;
			y= j%N;
			//cout<<"x="<<(int)x<<endl;
			threshold_matrix = ((I[x][y]+0.5)/pow(N,2)*255);
			//cout<<"threshold_matrix="<<(int)threshold_matrix<<endl;
			if((int)Image_data[(i*width)+j]<=(int)threshold_matrix)
			{
				new_image[(i*width)+j]=0;
			}
			else if((int)Image_data[(i*width)+j]>(int)threshold_matrix)
			{
				new_image[(i*width)+j]=255;
			}
		}
	}
	return new_image;
}


unsigned char *matrix44_Dithering(unsigned char *Image_data, int height, int width, int bytesPerPixel)
{
	unsigned char *new_image = new unsigned char [width*height*bytesPerPixel];
	unsigned char I[4][4]={ {5,9,6,10}, {13,1,14,2}, {7,11,4,8}, {15,3,12,0} },x=0,y=0;
	int N=4;
	float threshold_matrix=0;

	cout<<"The 4x4 Dithering matrix:\n";
	for(int i =0;i<4;i++)
	{
		for(int j=0; j<4 ;j++)
			cout<<(int)I[i][j]<<'\t';
		cout<<"\n";
	}


	for(int i=0; i<height ;i++)
	{
		for(int j=0; j<width ;j++)
		{
			x= i%N;
			y= j%N;
			//cout<<"x="<<(int)x<<endl;
			threshold_matrix = ((I[x][y]+0.5)/pow(N,2)*255);
			//cout<<"threshold_matrix="<<(int)threshold_matrix<<endl;
			if((int)Image_data[(i*width)+j]<=(int)threshold_matrix)
			{
				new_image[(i*width)+j]=0;
			}
			else if((int)Image_data[(i*width)+j]>(int)threshold_matrix)
			{
				new_image[(i*width)+j]=255;
			}
		}
	}
	return new_image;
}
unsigned char *matrix88_Dithering(unsigned char *Image_data, int height, int width, int bytesPerPixel)
{
	unsigned char *new_image = new unsigned char [width*height*bytesPerPixel];
	unsigned char I[8][8]={ {21,37,25,41,22,38,26,42}, {53,5,57,9,54,6,58,10}, {29,45,17,33,30,46,18,34}, {61,13,49,1,62,14,50,2},{23,39,27,43,20,36,24,40},{55,7,59,11,52,4,56,8},{31,47,19,35,28,44,16,32},{63,15,51,3,60,12,48,0} },x=0,y=0;
	int N=8;
	float threshold_matrix=0;

	cout<<"The 8x8 Dithering matrix:\n";
	for(int i =0;i<8;i++)
	{
		for(int j=0; j<8 ;j++)
			cout<<(int)I[i][j]<<'\t';
		cout<<"\n";
	}


	for(int i=0; i<height ;i++)
	{
		for(int j=0; j<width ;j++)
		{
			x= i%N;
			y= j%N;
			//cout<<"x="<<(int)x<<endl;
			threshold_matrix = ((I[x][y]+0.5)/pow(N,2)*255);
			//cout<<"threshold_matrix="<<(int)threshold_matrix<<endl;
			if((int)Image_data[(i*width)+j]<=(int)threshold_matrix)
			{
				new_image[(i*width)+j]=0;
			}
			else if((int)Image_data[(i*width)+j]>(int)threshold_matrix)
			{
				new_image[(i*width)+j]=255;
			}
		}
	}
	return new_image;
}

unsigned char *grey(unsigned char *Image_data1,unsigned char *Image_data2, int height, int width, int bytesPerPixel)
{
	unsigned char *new_image = new unsigned char [width*height*bytesPerPixel];
	for( int i=0; i<height; i++)
	{
		for(int j=0; j<width; j++)
		{
			if(Image_data1[(i*width)+j]==0 && Image_data2[(i*width)+j]==0)
			{
				new_image[(i*width)+j]=0;
			}

			else if(Image_data1[(i*width)+j]==0 && Image_data2[(i*width)+j]==255)
			{
				new_image[(i*width)+j]=85;
			}
			else if(Image_data1[(i*width)+j]==255 && Image_data2[(i*width)+j]==0)
			{
				new_image[(i*width)+j]=170;
			}
			else if(Image_data1[(i*width)+j]==255 && Image_data2[(i*width)+j]==255)
			{
				new_image[(i*width)+j]=255;
			}
		}
	}

	return new_image;

}