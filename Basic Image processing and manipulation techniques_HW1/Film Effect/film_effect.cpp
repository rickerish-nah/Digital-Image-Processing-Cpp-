#include <stdio.h>
#include <iostream>
#include <stdlib.h>

using namespace std;
int main(int argc, char *argv[])

{
	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel=3,window=3,size=0;
	int width = 256, height = 256;
	unsigned char *Imagedata= new unsigned char [width*height*3];
	unsigned char *invertimage= new unsigned char [width*height*3];
	unsigned char *newimage= new unsigned char [width*height*3];
	int shrink[3][2];
	cout<<"Enter Shrink Values for R then G then B:"<<endl;
	for(int i=0;i<3;i++)
	{
		for(int j=0; j<2; j++)
		{
			cout<<"I:"<<i<<"J:"<<j<<endl;

			cin>>shrink[i][j];
		}
	}
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), width*height*BytesPerPixel, file);
	fclose(file);
	for(int rgb=0; rgb<3; rgb++)
	{
		for(int i=0;i<height;i++)
		{
			for(int j=0;j<width;j++)
			{
				invertimage[(i*width*3)+((width-j)*3)+rgb]=255-Imagedata[(i*width*3)+(j*3)+rgb];
			}
		}
	}
	int R,r[256]={0},min[3]={0},max[3]={0};
	for (int rgb=0; rgb<3;rgb++) // Calculate CDF
	{
		R=0;
		for(int i=0;i<height;i++)
		{
			for(int j=0;j<width;j++)
			{
				if(min[rgb]>invertimage[(i*width*3)+(j*3)+rgb])
					min[rgb]=invertimage[(i*width*3)+(j*3)+rgb];
				if(max[rgb]<invertimage[(i*width*3)+(j*3)+rgb])
					max[rgb]=invertimage[(i*width*3)+(j*3)+rgb];
			}
		}
		cout<<"COLOR:"<<rgb<<endl<<"min:"<<min[rgb]<<endl<<"max:"<<max[rgb]<<endl;	
	}
	//max[0]=255;
	for(int rgb=0; rgb<3; rgb++)
	{
		cout<<"Ratio:"<<(float)(shrink[rgb][1]-shrink[rgb][0]) / (max[rgb]-min[rgb])<<endl;
		//for(int k=0; k<256; k++)s
		//{
			for(int i=0; i<height; i++)
			{
				//cout<<"In I:"<<i<<endl;
				for(int j=1; j<=width; j++)
				{
					//cout<<"\tIn J:"<<j<<endl;

					//if(invertimage[(i*width*3)+((width-j)*3)+rgb]==k)
					//{
						//newimage[(i*width*3)+((width-j)*3)+rgb]=( (  (shrink[rgb][1]-shrink[rgb][0]) / (max[rgb]-min[rgb]) ) * (invertimage[(i*width*3)+((width-j)*3)+rgb]-min[rgb]) ) + shrink[rgb][0];
						newimage[(i*width*3)+((width-j)*3)+rgb]=( ( (float) (shrink[rgb][1]-shrink[rgb][0]) / (max[rgb]-min[rgb]) ) * (invertimage[(i*width*3)+((width-j)*3)+rgb]-min[rgb]) ) + shrink[rgb][0];
				

					//}	
				}
			}
			//cout<<"Fault at I:"<<endl;

		//}
		
	}

if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(invertimage, sizeof(unsigned char), width*height*BytesPerPixel, file);
	fclose(file);
	
if (!(file=fopen(argv[3],"wb"))) {
		cout << "Cannot open file: " << argv[3] << endl;
		exit(1);
	}
	fwrite(newimage, sizeof(unsigned char), width*height*BytesPerPixel, file);
	fclose(file);




	return 0;
}