#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include<math.h>

using namespace std;
int roundoff(float s);
unsigned char median3(unsigned char *array, int q);
unsigned char mean3(unsigned char *array, int q);
unsigned char sort(unsigned char *a,int size);
unsigned char array(int a,int b,int c,int d);



int main(int argc, char *argv[])
{
	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel=3,window=3,size=0;
	int width = 512 , height = 512;
	unsigned char *tester= new unsigned char [width*height*3];
	unsigned char *Imagedata= new unsigned char [width*height*3];
	unsigned char *original= new unsigned char [width*height*3];
	unsigned char *imagedenoisemix= new unsigned char [width*height*3];
	unsigned char *imagedenoisegaussian= new unsigned char [width*height*3];
	unsigned char *mean33= new unsigned char [9];
	unsigned char *gauss33= new unsigned char [9];
	double PSNR[3]={0},max=255,MSE[3]={0};
if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), width*height*BytesPerPixel, file);
	fclose(file);
	if (!(file=fopen(argv[2],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(original, sizeof(unsigned char), width*height*BytesPerPixel, file);
	fclose(file);

//Mix Noise: Median Filter

	for(int rgb=0; rgb<3; rgb++)
	{
		//cout<<"COLOR:"<<rgb<<endl;
		for(int i=0; i<height; i++)
		{
			for(int j= 0;j<width; j++)
			{
				int x=0;
				//cout<<"AND it is here:"<<endl;
				//cout<<"I:"<<i<<"\t J:"<<j<<endl;
				for(int k=i-2;k<=i+2;k++)
				{
					if(k<0)
					{
						k=abs(k);
					}
					if(k>height)
					{
						int d=k-height;
						k=height-d;
					}
					for(int l=j-2;l<=j+2;l++)
					{
						if(l<0)
						{
							l=abs(l);
						}
						if(l>width)
						{
							int dd=l-width;
							k=width-dd;
						}

						mean33[x++]=Imagedata[(k*width*3)+(l*3)+rgb];
					}
						
				}
				//for(int g=0;g<x;g++)
				//cout<<(int)mode33[g]<<endl;
				if(rgb==0  || rgb==1 || rgb==2)
				{
					imagedenoisemix[(i*width*3)+(j*3)+rgb]=median3(mean33,x);
				}
				//imagedenoisemix[(i*width*3)+(j*3)+rgb]=Imagedata[(i*width*3)+(j*3)+rgb];
				
			}
		}
	}

	for( int times=0; times<10;times++)
	{
		for(int rgb=0; rgb<3; rgb++)
		{
		//cout<<"COLOR:"<<rgb<<endl;
			for(int i=0; i<height; i++)
			{
				for(int j= 0;j<width; j++)
				{
					int x=0;
				//cout<<"AND it is here:"<<endl;
				//cout<<"I:"<<i<<"\t J:"<<j<<endl;
					for(int k=i-1;k<=i+1;k++)
					{
						if(k<0)
						{
							k=abs(k);
						}
						if(k>height)
						{
							int d=k-height;
							k=height-d;
						}
						for(int l=j-1;l<=j+1;l++)
						{
							if(l<0)
							{
								l=abs(l);
							}
							if(l>width)
							{
								int dd=l-width;
								k=width-dd;
							}
							mean33[x++]=imagedenoisemix[(k*width*3)+(l*3)+rgb];
						}
						
					}
			
					//cout<<(int)mode33[g]<<endl;
					if(rgb==0  || rgb==2) // only RED and BLUE channel
					{
						imagedenoisemix[(i*width*3)+(j*3)+rgb]=median3(mean33,x);
					}
				//imagedenoisemix[(i*width*3)+(j*3)+rgb]=Imagedata[(i*width*3)+(j*3)+rgb];
				
				}
			}
		}
	}
	
//GAUSS Noise: Median filter and Mean filter
	for(int rgb=0; rgb<3; rgb++)
	{
		//cout<<"COLOR:"<<rgb<<endl;
		for(int i=0; i<height; i++)
		{
			for(int j= 0;j<width; j++)
			{
				int x=0;
				//cout<<"AND it is here:"<<endl;
				//cout<<"I:"<<i<<"\t J:"<<j<<endl;
				for(int k=i-1;k<=i+1;k++)
				{
					if(k<0)
					{
						k=abs(k);
					}
					if(k>height)
					{
						int d=k-height;
						k=height-d;
					}
					for(int l=j-1;l<=j+1;l++)
					{
						if(l<0)
						{
							l=abs(l);
						}
						if(l>width)
						{
							int dd=l-width;
							k=width-dd;
						}
						gauss33[x++]=imagedenoisemix[(k*width*3)+(l*3)+rgb];
					}
						
				}
				//for(int g=0;g<x;g++)
				//cout<<(int)mode33[g]<<endl;
				
				imagedenoisegaussian[(i*width*3)+(j*3)+rgb]=mean3(gauss33,x);

			}
		}
	}
	

	//PSNR Calculation
	for( int rgb=0; rgb<3; rgb++)
	{
		for(int i=0; i<height; i++)
		{
			for(int j=0; j<width; j++)
			{
				//MSE[rgb]+=(pow((imagedenoisemix[(i*width*3)+(j*3)+rgb]-(original[(i*width*3)+(j*3)+rgb])),2));
				MSE[rgb]= MSE[rgb]+ (pow(  (imagedenoisegaussian[(i*width*3)+(j*3)+rgb] - original[(i*width*3)+(j*3)+rgb]) , 2 ) );
			}
		}
		//
		//MSE[rgb][0]=MSE[rgb][0]/(width*height);
		MSE[rgb]=MSE[rgb]/(width*height);
		//cout<<MSE[rgb]<<endl;
	
		PSNR[rgb]=10*log10( (max*max) / (MSE[rgb]) );
		
		cout<<"Signal-2-Noise for Mix for COLOR:"<<rgb<<"\nPSNR Gaussian:"<<PSNR[rgb]<<endl;
		//cout<<pow(max,2)<<endl;
	}
	
	if (!(file=fopen(argv[3],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(imagedenoisemix, sizeof(unsigned char), width*height*BytesPerPixel, file);
	fclose(file);
	if (!(file=fopen(argv[4],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(imagedenoisegaussian, sizeof(unsigned char), width*height*BytesPerPixel, file);
	fclose(file);
	return 0;

}
int roundoff( float num)
{
	if( num>=255)
		return(255);
	if( num< 0)
		return(0);
	if( (num + 0.5) >= (int(num) + 1) )
		return int(num)+1;
	else
		return int(num);
}

unsigned char median3(unsigned char *a, int size)
{
	unsigned char median=0;
	*a=sort(a,size);
	/*cout<<"The elements are:\n";
	for(int g=0;g<size;g++)
	{
		cout<<"\t"<<(int)a[g];
	}
	cout<<endl;*/
	if(size%2 == 0)
	{
		median = a[size/2]+a[(size/2)+1];
	}
	else if(size%2 == 1)
	{
		int q= roundoff(size/2);
		median=a[q];
	}
	
	//cout<<"The Median:\t"<<(int)median<<endl;
	
	return median;
}
unsigned char mean3(unsigned char *a, int size)
{
	
	unsigned char mean=0;
	*a=sort(a,size);
	/*cout<<"The elements are:\n";
	for(int g=0;g<size;g++)
	{
		cout<<"\t"<<(int)a[g];
	}
	cout<<endl;*/
	for(int g=0;g<size;g++)
	{
		mean+=(float)(a[g]/size);
	}
	
	//cout<<"The Median:\t"<<(int)median<<endl;
	
	return mean;
}
unsigned char sort(unsigned char *a,int size)
{
	//cout<<"Arranging:\n";
	unsigned char min=a[0],temp=0;;
	for(int i=0;i<size;i++)
	{
		for(int j=i+1;j<size;j++)
		{
			if((int)a[i]>(int)a[j])
			{
			temp=a[i];
			a[i]=a[j];
			a[j]=temp;
			}

		}
	}
	//cout<<"Size:"<<size<<endl;
	
	return *a;
}
