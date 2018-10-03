#include <stdio.h>
#include <iostream>
#include <stdlib.h>

using namespace std;
int roundoff(float s);
int main(int argc, char *argv[])

{
	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel;
	int size1 = 400 , size2 = 300;
	int R, G, B,r[256], g[256], b[256];
	double cdf_r[256],cdf_g[256],cdf_b[256];
	unsigned char cdf_r1[256],cdf_g1[256],cdf_b1[256]; //later will be initialised to zero (0)
	BytesPerPixel = 3;//atoi(argv[5]);
	unsigned char  Imagedata[size1][size2][BytesPerPixel];
	unsigned char New_Image[size1][size2][BytesPerPixel];
	unsigned char  Imagedata_red[size1][size2];
	unsigned char Imagedata_blue[size1][size2];
	unsigned char Imagedata_green[size1][size2];
	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), size1*size2*BytesPerPixel, file);
	fclose(file);

	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////

	int a=0,h=0,c=0;
	//cout<<"The value of R at each pixel Range0-255 :\n";
	//RED
	for(int k=0;k<256;k++)
	{
		R=0;
		G=0;
		B=0;
		for(int i=0;i<size1;i++)
		{
			for(int j=0;j<size2;j++)
			{
				/*
				Imagedata_red[i][j] = (Imagedata[i][j][0]);
				if(a<1000)
					cout<<"R at "<<a++<<": "<<"\t"<<(int)Imagedata_red[i][j]<<endl;	
				Imagedata_green[i][j] = (Imagedata[i][j][1]);
				//New_Image[i][j][0]=Imagedata[i][j][1];
				//cout<<"G at "<<h++<<": "<<"\t"<<(int)Imagedata_green[i][j]<<endl;
				Imagedata_blue[i][j] = (Imagedata[i][j][2]);
				//New_Image[i][j][0]=Imagedata[i][j][2];
				//cout<<"B at "<<c++<<": "<<"\t"<<(int)Imagedata_blue[i][j]<<endl;
				*/
				//cout<<"R at "<<a++<<": "<<"\t"<<Imagedata[i][j][0]<<endl;
				if(Imagedata[i][j][0] == k)
				{
					r[k]=++R;
				}
				if(Imagedata[i][j][1] == k)
				{
					g[k]=++G;
				}
				if(Imagedata[i][j][2] == k)
				{
					b[k]=++B;
				}
			}
		}
		
		cout<<r[k]<<endl;
	}
	//CDF calculation RED GREEN BLUE
	cdf_r[0]=r[0];
	cdf_g[0]=g[0];
	cdf_b[0]=b[0];
	for(int i=1;i<256;i++)
	{
		cdf_r[i]=((cdf_r[i-1] + r[i]));
		//cout<<"cdf_R at "<<i<<" : "<<cdf_r[i]<<endl;

		cdf_g[i]=((cdf_g[i-1] + g[i]));
		//cout<<"cdf_G at "<<i<<" : "<<cdf_g[i]<<endl;

		cdf_b[i]=((cdf_b[i-1] + b[i]));
		//cout<<"cdf_B at "<<i<<" : "<<cdf_b[i]<<endl;
	}
	for(int i =0; i<256 ; i++)
	{
		cdf_r[i]=(cdf_r[i]/(size1*size2))*255;
		//cdf_r[i]=(int) cdf_r[i];
		//cout<<"Normalised_cdf_R at "<<i<<" : "<<cdf_r[i]<<endl;

		cdf_g[i]=(cdf_g[i]/(size1*size2))*255;
		//cdf_g[i]=(int) cdf_g[i];
		//cout<<"cdf_G at "<<i<<" : "<<cdf_g[i]<<endl;

		cdf_b[i]=(cdf_b[i]/(size1*size2))*255;
		//cdf_b[i]=(int) cdf_b[i];
		//cout<<"cdf_b at "<<i<<" : "<<cdf_b[i]<<endl;
		
	}
	//making cdf's Unsigned Char
	for(int i =0; i<256 ; i++)
	{
		cdf_r1[i]=(unsigned char) cdf_r[i];
		//cout<<"Normalised_cdf_R at "<<i<<" : "<<cdf_r[i]<<endl;
		cdf_g1[i]=(unsigned char) cdf_g[i];
		cdf_b1[i]=(unsigned char) cdf_b[i];
		
	}
	//Histogram Equalisation 
    /*for(int i=0; i<size1 ;i++)
    {
   		for(int j=0; j<size2 ; j++)
   		{
   			New_Image[i][j][0] = cdf_r1[Imagedata[i][j][0]];
   			New_Image[i][i][1] = cdf_g1[Imagedata[i][j][1]];
   			New_Image[i][j][2] = cdf_b1[Imagedata[i][j][2]];
   		}
   	}*/
   	 for(int i=0; i<size1 ;i++)
    {
   		for(int j=0; j<size2 ; j++)
   		{
   			for(int u=0;u<256;u++)
   			{
   				if(Imagedata[i][j][0]==(unsigned char) u)
   				{
   					New_Image[i][j][0] = cdf_r1[u];
   				}
   				if(Imagedata[i][j][1]==(unsigned char) u)
   				{
   					New_Image[i][j][1] = cdf_g1[u];
   				}
   				if(Imagedata[i][j][2]==(unsigned char) u)
   				{
   					New_Image[i][j][2] = cdf_b1[u];
   				}
   			}
   			//New_Image[i][j][0] = cdf_r1[Imagedata[i][j][0]];
   			//New_Image[i][i][1] = cdf_g1[Imagedata[i][j][1]];
   			//New_Image[i][j][2] = cdf_b1[Imagedata[i][j][2]];
   		}
   	}
   
	// Write image data (filename specified by second argument) from image data matrix

	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(New_Image, sizeof(unsigned char), size1*size2*3, file);
	fclose(file);

	/*if (!(file=fopen(argv[3],"wb"))) {
		cout << "Cannot open file: " << argv[3] << endl;
		exit(1);
	}
	fwrite(Imagedata_green, sizeof(unsigned char), size1*size2*3, file);
	fclose(file);

	if (!(file=fopen(argv[4],"wb"))) {
		cout << "Cannot open file: " << argv[4] << endl;
		exit(1);
	}
	fwrite(Imagedata_blue, sizeof(unsigned char), size1*size2*3, file);
	fclose(file);*/
	return 0;
}
int roundoff( float num)
{
	if( (num + 0.5) >= (int(num) + 1) )
	return int(num)+1;
	else
	return int(num);
}


