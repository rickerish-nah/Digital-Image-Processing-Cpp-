// NOTE:	The code assumes that the image is of size 256 x 256 and is in the
//			RAW format. You will need to make corresponding changes to
//			accommodate images of different sizes and/or types

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include<cmath>

using namespace std;
int roundoff(float s);

int main(int argc, char *argv[])

{
	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel;
	int w1 = 512 , h1 = 512 , w2 = 650 , h2 = 650;
	//int w1=5,h1=5,w2=10,h2=10;
	float x_ratio = (float) (w1-1)/(w2-1) , y_ratio = (float) (h1-1)/(h2-1);
	cout<<"x-Ratio:"<<x_ratio<<endl<<"y_ratio:"<<y_ratio<<endl;
	
	// Allocate image data array
	BytesPerPixel = 3; //Colour Picture
	unsigned char *Imagedata = new unsigned char[w1*h1*BytesPerPixel];
	unsigned char *Imagedata_enlarged = new unsigned char[w2*h2*BytesPerPixel];
	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), w1*h1*BytesPerPixel, file);
	fclose(file);
	///////////////////////// PROCESSING CODE HERE /////////////////////////
	//512x512 to 650x650
	int i , j , k , x , y , l , m , pixel_num ;
	float diff_x , diff_y ;
	unsigned char A,B,C,D;
	for(i=0 ; i<w2 ; i++)
		{
			for(j=0 ; j<h2 ; j++)
			{
				x = x_ratio*j;
				x = roundoff(x);
				y = y_ratio*i; 
				y = roundoff(y);
				//cout<<"rounded off y: "<<y<<endl;
				diff_x = ((x_ratio*j) - x);
				//cout<<"delta x : "<<diff_x<<endl;
				//diff_x = abs(diff_x);
				diff_y = ((y_ratio*i) - y);
				//cout<<"delta y : "<<diff_y<<endl;
				pixel_num = (y*w1+x) ;                
            	//A = Imagedata[pixel_num] ;
            	//B = Imagedata[pixel_num + 1] ;
            	//C = Imagedata[pixel_num + w1] ;
            	//D = Imagedata[pixel_num + w1 + 1] ;
            	l = x;//roundoff(j*x_ratio);
			    m = y;//roundoff(i*y_ratio);
            	for (k=0 ; k<3 ; k++)
            	{
            		//cout<<"l: "<<l<<"\t m:"<<m<<"\t i :"<<i<<"\t j:"<<j<<"\t k:"<<k<<endl;
            		//Imagedata_enlarged[i*w2*3 + j*3 + k] = A + (diff_x * (B-A)) + (diff_y * ( C + (diff_x * (D-C)) - (A+ ( diff_x * (B-A) ) ) ) );
            		/*Bi-linear Interpolation  : Formula:
					A(1-x)(1-y)+B(x)(1-y)+C(1-x)(y)+D(x)(y)  is same as A + x(B-A) + y[ C + x(D-C) - (A+ x(B-A) ) ]*/
            		//Rotated and mirrored : :D haha //Imagedata_enlarged[i*w2*3 + j*3 + k] = Imagedata[l*w1*3 + m*3 + k] + (diff_x * (Imagedata[l*w1*3 + (m+1)*3 + k]-Imagedata[l*w1*3 + m*3 + k])) + (diff_y * (Imagedata[(l+1)*w1*3 + m*3 + k] + (diff_x * (Imagedata[(l+1)*w1*3 + (m+1)*3 + k]-Imagedata[(l+1)*w1*3 + m*3 + k])) - (Imagedata[l*w1*3 + m*3 + k] + (diff_x * (Imagedata[l*w1*3 + (m+1)*3 + k] - Imagedata[l*w1*3 + m*3 + k])))));

            		/*Formula 2*/ //Imagedata_enlarged[i*w2*3 + j*3 + k] = Imagedata[m*w1*3 + l*3 + k] + (diff_x * (Imagedata[m*w1*3 + (l+1)*3 + k]-Imagedata[m*w1*3 + l*3 + k])) + (diff_y * (Imagedata[(m+1)*w1*3 + l*3 + k] + (diff_x * (Imagedata[(m+1)*w1*3 + (l+1)*3 + k]-Imagedata[(m+1)*w1*3 + l*3 + k])) - (Imagedata[m*w1*3 + l*3 + k] + (diff_x * (Imagedata[m*w1*3 + (l+1)*3 + k] - Imagedata[m*w1*3 + l*3 + k])))));
            		/*Formula 1*/ Imagedata_enlarged[i*w2*3 + j*3 + k] = Imagedata[m*w1*3 + l*3 + k]*(1-diff_x)*(1-diff_y) + Imagedata[m*w1*3 + (l+1)*3 + k]*(diff_x)*(1-diff_y) + Imagedata[(m+1)*w1*3 + l*3 + k]*(1-diff_x)*(diff_y) + Imagedata[(m+1)*w1*3 + (l+1)*3 + k]*(diff_x)*(diff_y);
            	}
			}

		}
	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(Imagedata_enlarged, sizeof(float), w2*h2*BytesPerPixel, file);
	fclose(file); 
	delete[] Imagedata;
	delete[] Imagedata_enlarged;
	return 0;
}

int roundoff( float num)
{
	if( (num + 0.5) >= (int(num) + 1) )
	return int(num)+1;
	else
	return int(num);
}
