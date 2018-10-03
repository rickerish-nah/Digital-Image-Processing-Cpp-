#include <stdio.h>
#include <iostream>
#include <stdlib.h>

using namespace std;
int main(int argc, char *argv[])

{
	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel;
	int size1 = 400 , size2 = 300;
	int R, G, B,r[256], g[256], b[256];
	double cdf_r[256],cdf_g[256],cdf_b[256], avg=size1*size2/256;
	unsigned char cdf_r1[256],cdf_g1[256],cdf_b1[256]; //later will be initialised to zero (0)
	BytesPerPixel = 3;//atoi(argv[5]);
	unsigned char  Imagedata[size1][size2][BytesPerPixel];
	unsigned char New_Image[size1][size2][BytesPerPixel];
	int check[size1][size2][BytesPerPixel];
	for(int i=0;i<size1;i++)
		for(int j=0;j<size2;j++)
			for(int k=0;k<3;k++)
			{
				check[i][j][k]=0;
				New_Image[i][j][k]=Imagedata[i][j][k];
			}
	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), size1*size2*BytesPerPixel, file);
	fclose(file);

	int a=0;

	int countr=(int)avg,countg=(int)avg,countb=(int)avg,cr=0,cg=0,cb=0;
	for(int k=0;k<256;k++)
	{	
		for(int i=0;i<size1;i++)
		{
			for( int j=0;j<size2;j++)
			{
				//RED
				if(Imagedata[i][j][0]==k && check[i][j][0]==0)
				{
					New_Image[i][j][0]=cr;
					countr--;
					check[i][j][0]=1;
					if(countr==0)
					{
						cr++;
						countr=(int)avg;
					}
				}
				//GREEN
				if(Imagedata[i][j][1]==k && check[i][j][1]==0)
				{
					New_Image[i][j][1]=cg;
					countg--;
					check[i][j][1]=1;
					if(countg==0)
					{
						cg++;
						countg=(int)avg;
					}
				}
				//BLUE
				if(Imagedata[i][j][2]==k && check[i][j][2]==0)
				{
					New_Image[i][j][2]=cb;
					countb--;
					check[i][j][2]=1;
					if(countb==0)
					{
						cb++;
						countb=(int)avg;
					}
				}
			}

		}
		//cout<<"pixel_R at "<<k<<" : "<<r[k]<<endl;
	}

for(int k=0;k<256;k++)
	{
		R=0;
		G=0;
		B=0;
		for(int i=0;i<size1;i++)
		{
			for(int j=0;j<size2;j++)
			{
			
				if(New_Image[i][j][0] == k)
				{
					r[k]=++R;
				}
				if(New_Image[i][j][1] == k)
				{
					g[k]=++G;
				}
				if(New_Image[i][j][2] == k)
				{
					b[k]=++B;
				}
			}
		}
		//cout<<"pixel_R at "<<k<<" : "<<r[k]<<endl;
	}
	//CDF calculation RED GREEN BLUE
	cdf_r[0]=r[0];
	cdf_g[0]=g[0];
	cdf_b[0]=b[0];
	for(int i=1;i<256;i++)
	{
		cdf_r[i]=((cdf_r[i-1] + r[i]));
		//cout<</*"cdf_R at "<<i<<" : "<<*/cdf_r[i]<<endl;

		cdf_g[i]=((cdf_g[i-1] + g[i]));
		//cout<<"cdf_G at "<<i<<" : "<<cdf_g[i]<<endl;

		cdf_b[i]=((cdf_b[i-1] + b[i]));
		//cout<<"cdf_B at "<<i<<" : "<<cdf_b[i]<<endl;
	}
	for(int i =0; i<256 ; i++)
	{
		cdf_r[i]=(cdf_r[i]/(size1*size2))*255;
		//cdf_r[i]=(int) cdf_r[i];
		//cout<<"Normalised_cdf_R at "<<i<<" : "
		//cout<<cdf_r[i]<<endl;

		cdf_g[i]=(cdf_g[i]/(size1*size2))*255;
		//cdf_g[i]=(int) cdf_g[i];
		//cout<<"cdf_G at "<<i<<" : "
		cout<<cdf_g[i]<<endl;

		cdf_b[i]=(cdf_b[i]/(size1*size2))*255;
		//cdf_b[i]=(int) cdf_b[i];
		//cout<<"cdf_b at "<<i<<" : "
		cout<<cdf_b[i]<<endl;
		
	}
	//making cdf's Unsigned Char
	for(int i =0; i<256 ; i++)
	{
		cdf_r1[i]=(unsigned char) cdf_r[i];
		//cout<<"Normalised_cdf_R at "<<i<<" : "<<
		//cout<<cdf_r[i]<<endl;
		cdf_g1[i]=(unsigned char) cdf_g[i];
		cdf_b1[i]=(unsigned char) cdf_b[i];
		
	}
	// Write image data (filename specified by second argument) from image data matrix

	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(New_Image, sizeof(unsigned char), size1*size2*3, file);
	fclose(file);





	return 0;
}
