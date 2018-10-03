#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include<math.h>
#include<random>
using namespace std;
//FUNCTIONS USED

unsigned char *shrink_main(unsigned char * Image, int height, int wid, int BytesPerPixel);
unsigned char *pattern_gen_and_return_hit1(unsigned char *Image_data, int height, int width, int BytesPerPixel);
unsigned char *pattern_gen_and_return_hit2(unsigned char *Image_data,unsigned char *Image, int height, int width, int BytesPerPixel);
unsigned char shrink_cond(unsigned char *a, int size);
unsigned char unc_shrink_thin(unsigned char *a, int size);

unsigned char *fixed_Dithering(unsigned char *Image_data, int height, int width, int bytesPerPixel);
unsigned char *convert1_255(unsigned char *a);
unsigned char *copy(unsigned char *Image_data, int height, int width, int bytesPerPixel);
int counter(unsigned char *Image_data, int height, int width, int bytesPerPixel);

//Main Func
int main(int argc, char *argv[])
{
	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel=1;
	int width = 640 , height = 480;

	unsigned char *Imagedata= new unsigned char [width*height*BytesPerPixel];
	unsigned char *newimage_Fixed= new unsigned char [width*height*BytesPerPixel];
	unsigned char *newimage_temp= new unsigned char [width*height*BytesPerPixel];
	int len=0;
	int *star= new int (len);
	int *star_hist= new int (len);
	
	//read input
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), width*height*BytesPerPixel, file);
	fclose(file);

	
	newimage_Fixed=fixed_Dithering(Imagedata,height,width,BytesPerPixel); // 0 & 1
	newimage_temp=fixed_Dithering(Imagedata,height,width,BytesPerPixel);
	//newimage_temp=convert1_255(newimage_temp);
	
	
	for(int i=0;i<20;i++)
	{
		newimage_temp=shrink_main(newimage_Fixed,height,width,BytesPerPixel);
		newimage_Fixed=copy(newimage_temp,height,width,BytesPerPixel);
		star[len++]=counter(newimage_Fixed,height,width,BytesPerPixel);
	}
	cout<<"Showing number of stars for each size in the order of increasing size.....\n";
	cout<<"at i="<<0<<" #stars is:"<<star[0]<<endl;
	for(int i=1;i<len;i++)
	{
		cout<<"at i="<<i<<" #stars is:"<<star[i]-star[i-1]<<endl;
	}
	len=0;

	
	newimage_Fixed=convert1_255(newimage_temp);
	//write output
	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(newimage_Fixed, sizeof(unsigned char), width*height*BytesPerPixel, file);
	fclose(file);
	/*if (!(file=fopen(argv[3],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(hit_uncc, sizeof(unsigned char), width*height*BytesPerPixel, file);
	fclose(file);*/
	
	return 0;
}
int counter(unsigned char * newimage_Fixed,int height,int width, int BytesPerPixel)
{
	int c=0;
	int a[8]={0};
	for(int i=1;i<height-1;i++)
	{
		for(int j=1;j<width-1;j++)
		{
			if(newimage_Fixed[(i*width)+(j)]==0)
			{}
			else if(newimage_Fixed[(i*width)+(j)]==1)
			{
				a[0]=newimage_Fixed[(i*width)+(j+1)];
				a[1]=newimage_Fixed[((i-1)*width)+(j+1)];
				a[2]=newimage_Fixed[((i-1)*width)+(j)];
				a[3]=newimage_Fixed[((i-1)*width)+(j-1)];
				a[4]=newimage_Fixed[(i*width)+(j-1)];
				a[5]=newimage_Fixed[((i+1)*width)+(j-1)];
				a[6]=newimage_Fixed[((i+1)*width)+(j)];
				a[7]=newimage_Fixed[((i+1)*width)+(j+1)];
				if(a[0]==0 && a[1]==0 && a[2]==0 && a[3]==0 && a[4]==0 && a[5]==0 && a[6]==0 && a[7]==0 )
				{
					c++;
				}
			}


		}
	}
	return c;
}
unsigned char *shrink_main(unsigned char * actual_image, int height, int width, int BytesPerPixel)
{
	unsigned char *newimage_temp= new unsigned char [width*height*BytesPerPixel];
	
	unsigned char *hit_c= new unsigned char [width*height*BytesPerPixel]; //0&1
	unsigned char *hit_unc= new unsigned char [width*height*BytesPerPixel];//0&1
	

	hit_c=pattern_gen_and_return_hit1(actual_image,height,width,BytesPerPixel);

	
	hit_unc=pattern_gen_and_return_hit2(hit_c,actual_image,height,width,BytesPerPixel);


	return hit_unc;

}


//shrink_cond Conditional Pattern
unsigned char *pattern_gen_and_return_hit1(unsigned char *newimage_Fixed, int height, int width,int BytesPerPixel)
{
	unsigned char *hit_c= new unsigned char [width*height*BytesPerPixel];
	int x=0,mark=0;
	unsigned char a[8]={0};
	for( int i=1;i<height;i++)
	{
		for(int j=1;j<width;j++)
		{
			if(newimage_Fixed[(i*width)+(j)]==0)
			{
				//cout<<"yes";
				hit_c[(i*width)+j]=0;
				//cout<<"I:"<<i<<"\tJ:"<<j<<"\tMark:0"<<endl;
				
				
			}
			else if(newimage_Fixed[(i*width)+(j)]==1)
			{
			

				a[0]=newimage_Fixed[(i*width)+(j+1)];
				a[1]=newimage_Fixed[((i-1)*width)+(j+1)];
				a[2]=newimage_Fixed[((i-1)*width)+(j)];
				a[3]=newimage_Fixed[((i-1)*width)+(j-1)];
				a[4]=newimage_Fixed[(i*width)+(j-1)];
				a[5]=newimage_Fixed[((i+1)*width)+(j-1)];
				a[6]=newimage_Fixed[((i+1)*width)+(j)];
				a[7]=newimage_Fixed[((i+1)*width)+(j+1)];
				//cout<<"Shrink"<<endl;
				mark=shrink_cond(a,x);
			
				//cout<<"I:"<<i<<"\tJ:"<<j<<"\tMark:"<<mark<<endl;
				hit_c[(i*width)+j]=mark;
				//cout<<"Mark:"<<(int)hit_c[(i*width)+j]<<endl;
				
			}
		}
	}
	return hit_c;
}

unsigned char *pattern_gen_and_return_hit2(unsigned char *newimage_Fixed,unsigned char *actual_image, int height, int width,int BytesPerPixel)
{
	//unsigned char *hit_c= new unsigned char [width*height*BytesPerPixel];
	int x=0,mark=0;
	unsigned char *rehit= new unsigned char [width*height*BytesPerPixel];
	unsigned char a[8]={0};
	for( int i=1;i<height;i++)
	{
		for(int j=1;j<width;j++)
		{
			if(newimage_Fixed[(i*width)+(j)]==0)
			{
				rehit[(i*width)+j]=actual_image[(i*width)+j];

				//continue;
			}
			else if (newimage_Fixed[(i*width)+(j)]==1)
			{
				a[0]=newimage_Fixed[(i*width)+(j+1)];
				a[1]=newimage_Fixed[((i-1)*width)+(j+1)];
				a[2]=newimage_Fixed[((i-1)*width)+(j)];
				a[3]=newimage_Fixed[((i-1)*width)+(j-1)];
				a[4]=newimage_Fixed[(i*width)+(j-1)];
				a[5]=newimage_Fixed[((i+1)*width)+(j-1)];
				a[6]=newimage_Fixed[((i+1)*width)+(j)];
				a[7]=newimage_Fixed[((i+1)*width)+(j+1)];
			
				//cout<<"reverse"<<endl;
				mark=unc_shrink_thin(a,x);
				if(mark==1)
				{
					rehit[(i*width)+j]=1;//actual_image[(i*width)+j];
					//actual_image[(i*width)+j]=255;
					//cout<<"he:\n";
					//rehit[(i*width)+j]=1;
					//actual_image[(i*width)+j]=1;
					continue;
				}
				else
				{
					rehit[(i*width)+j]=0;//255*(actual_image[(i*width)+j]);//255*actual_image[(i*width)+j];
					//cout<<"hele:\n";
				}

				
				
				//cout<<"Mark:"<<mark<<endl;
				

			}
		}
	}
	return rehit;
	//return actual_image;

}

//UNConditional Pattern
unsigned char shrink_cond(unsigned char *a, int size)
{
	
	int ret=0;
	/*cout<<"\t The elements are:";//<<(int)pat<<endl;
	
	//cout<<"enter shrink:"<<endl;
	
	for(int g=0;g<8;g++)
	{
		cout<<"\t"<<(int)a[g];
		//cout<<"h";
	}
	cout<<endl;*/
	//s:bond 1
	//1
	if(a[0]==0 && a[1]==1 && a[2]==0 && a[3]==0 && a[4]==0 && a[5]==0 && a[6]==0 && a[7]==0 )
	{
		ret=1;
	}
	//3
	else if(a[0]==0 && a[1]==0 && a[2]==0 && a[3]==0 && a[4]==0 && a[5]==1 && a[6]==0 && a[7]==0 )
	{
		ret=1;
	}
	//2
	else if(a[0]==0 && a[1]==0 && a[2]==0 && a[3]==1 && a[4]==0 && a[5]==0 && a[6]==0 && a[7]==0 )
	{
		ret=1;
	}
	//4
	else if(a[0]==0 && a[1]==0 && a[2]==0 && a[3]==0 && a[4]==0 && a[5]==0 && a[6]==0 && a[7]==1 )
	{
		ret=1;
	}
	//S; bond 2:
	//1
	else if(a[0]==1 && a[1]==0 && a[2]==0 && a[3]==0 && a[4]==0 && a[5]==0 && a[6]==0 && a[7]==0 )
	{
		ret=1;
	}
	//2
	else if(a[0]==0 && a[1]==0 && a[2]==1 && a[3]==0 && a[4]==0 && a[5]==0 && a[6]==0 && a[7]==0)
	{
		ret=1;
	}
	//3
	else if(a[0]==0 && a[1]==0 && a[2]==0 && a[3]==0 && a[4]==1 && a[5]==0 && a[6]==0 && a[7]==0 )
	{
		ret=1;
	}
	//4
	else if(a[0]==0 && a[1]==0 && a[2]==0 && a[3]==0 && a[4]==0 && a[5]==0 && a[6]==1 && a[7]== 0)
	{
		ret=1;
	}
	//S: bond 3:
	//1
	else if(a[0]==1 && a[1]==1 && a[2]==0 && a[3]==0 && a[4]==0 && a[5]==0 && a[6]==0 && a[7]==0 )
	{
		ret=1;
	}
	//2
	else if(a[0]==0 && a[1]==1 && a[2]==1 && a[3]==0 && a[4]==0 && a[5]==0 && a[6]==0 && a[7]==0 )
	{
		ret=1;
	}
	//3
	else if(a[0]==0 && a[1]==0 && a[2]==1 && a[3]==1 && a[4]==0 && a[5]==0 && a[6]==0 && a[7]==0 )
	{
		ret=1;
	}
	//4
	else if(a[0]==0 && a[1]==0 && a[2]==0 && a[3]==1 && a[4]==1 && a[5]==0 && a[6]==0 && a[7]==0 )
	{
		ret=1;
	}
	//5
	else if(a[0]==0 && a[1]==0 && a[2]==0 && a[3]==0 && a[4]==1 && a[5]==1 && a[6]==0 && a[7]==0 )
	{
		ret=1;
	}
	//6
	else if(a[0]==0 && a[1]==0 && a[2]==0 && a[3]==0 && a[4]==0 && a[5]==1 && a[6]==1 && a[7]==0 )
	{
		ret=1;
	}
	//7
	else if(a[0]==0 && a[1]==0 && a[2]==0 && a[3]==0 && a[4]==0 && a[5]==0 && a[6]==1 && a[7]==1 )
	{
		ret=1;
	}
	//8
	else if(a[0]==1 && a[1]==0 && a[2]==0 && a[3]==0 && a[4]==0 && a[5]==0 && a[6]==0 && a[7]==1 )
	{
		ret=1;
	}

	//STK: Bond 4:
	//1
	else if(a[0]==1 && a[1]==1 && a[2]==0 && a[3]==0 && a[4]==0 && a[5]==0 && a[6]==0 && a[7]==1 )
	{
		ret=1;
	}
	//2
	else if(a[0]==0 && a[1]==1 && a[2]==1 && a[3]==1 && a[4]==0 && a[5]==0 && a[6]==0 && a[7]==0 )
	{
		ret=1;
	}
	//3
	else if(a[0]==0 && a[1]==0 && a[2]==0 && a[3]==1 && a[4]==1 && a[5]==1 && a[6]==0 && a[7]==0 )
	{
		ret=1;
	}
	//4
	else if(a[0]==0 && a[1]==0 && a[2]==0 && a[3]==0 && a[4]==0 && a[5]==1 && a[6]==1 && a[7]==1 )
	{
		ret=1;
	}
	//ST 1: bond5:
	//1
	else if(a[0]==1 && a[1]==0 && a[2]==1 && a[3]==1 && a[4]==0 && a[5]==0 && a[6]==0 && a[7]==0 )
	{
		ret=1;
	}
	//2
	else if(a[0]==1 && a[1]==0 && a[2]==1 && a[3]==0 && a[4]==0 && a[5]==0 && a[6]==0 && a[7]==1 )
	{
		ret=1;
	}
	//3
	else if(a[0]==0 && a[1]==1 && a[2]==1 && a[3]==0 && a[4]==1 && a[5]==0 && a[6]==0 && a[7]==0 )
	{
		ret=1;
	}
	//4
	else if(a[0]==1 && a[1]==1 && a[2]==0 && a[3]==0 && a[4]==0 && a[5]==0 && a[6]==1 && a[7]==0 )
	{
		ret=1;
	}
	//ST 2: Bond 5
	//1
	else if(a[0]==1 && a[1]==1 && a[2]==1 && a[3]==0 && a[4]==0 && a[5]==0 && a[6]==0 && a[7]==0 )
	{
		ret=1;
	}
	//2
	else if(a[0]==0 && a[1]==0 && a[2]==1 && a[3]==1 && a[4]==1 && a[5]==0 && a[6]==0 && a[7]==0 )
	{
		ret=1;
	}
	//3
	else if(a[0]==0 && a[1]==0 && a[2]==0 && a[3]==0 && a[4]==1 && a[5]==1 && a[6]==1 && a[7]==0 )
	{
		ret=1;
	}
	//4
	else if(a[0]==1 && a[1]==0 && a[2]==0 && a[3]==0 && a[4]==0 && a[5]==0 && a[6]==1 && a[7]==1 )
	{
		ret=1;
	}
	//ST : Bond 6
	//1
	else if(a[0]==1 && a[1]==0 && a[2]==1 && a[3]==1 && a[4]==0 && a[5]==0 && a[6]==0 && a[7]==1 )
	{
		ret=1;
	}
	//2
	else if(a[0]==0 && a[1]==1 && a[2]==1 && a[3]==0 && a[4]==1 && a[5]==1 && a[6]==0 && a[7]==0 )
	{
		ret=1;
	}
	//STK: Bond 6
	//1
	else if(a[0]==1 && a[1]==1 && a[2]==1 && a[3]==1 && a[4]==0 && a[5]==0 && a[6]==0 && a[7]==0 )
	{
		ret=1;
	}
	//2
	else if(a[0]==1 && a[1]==1 && a[2]==1 && a[3]==0 && a[4]==0 && a[5]==0 && a[6]==0 && a[7]==1 )
	{
		ret=1;
	}
	//3
	else if(a[0]==0 && a[1]==1 && a[2]==1 && a[3]==1 && a[4]==1 && a[5]==0 && a[6]==0 && a[7]==0 )
	{
		ret=1;
	}
	//4
	else if(a[0]==0 && a[1]==0 && a[2]==1 && a[3]==1 && a[4]==1 && a[5]==1 && a[6]==0 && a[7]==0)
	{
		ret=1;
	}
	//5
	else if(a[0]==0 && a[1]==0 && a[2]==0 && a[3]==1 && a[4]==1 && a[5]==1 && a[6]==1 && a[7]==0 )
	{
		ret=1;
	}
	//6
	else if(a[0]==0 && a[1]==0 && a[2]==0 && a[3]==0 && a[4]==1 && a[5]==1 && a[6]==1 && a[7]==1 )
	{
		ret=1;
	}
	//7
	else if(a[0]==1 && a[1]==0 && a[2]==0 && a[3]==0 && a[4]==0 && a[5]==1 && a[6]==1 && a[7]==1 )
	{
		ret=1;
	}
	//8
	else if(a[0]==1 && a[1]==1 && a[2]==0 && a[3]==0 && a[4]==0 && a[5]==0 && a[6]==1 && a[7]==1 )
	{
		ret=1;
	}
	//STK : bond 7
	//1
	else if(a[0]==1 && a[1]==1 && a[2]==1 && a[3]==1 && a[4]==0 && a[5]==0 && a[6]==0 && a[7]==1 )
	{
		ret=1;
	}
	//2
	else if(a[0]==0 && a[1]==1 && a[2]==1 && a[3]==1 && a[4]==1 && a[5]==1 && a[6]==0 && a[7]==0 )
	{
		ret=1;
	}
	//3
	else if(a[0]==0 && a[1]==0 && a[2]==0 && a[3]==1 && a[4]==1 && a[5]==1 && a[6]==1 && a[7]==1 )
	{
		ret=1;
	}
	//4
	else if(a[0]==1 && a[1]==1 && a[2]==0 && a[3]==0 && a[4]==0 && a[5]==1 && a[6]==1 && a[7]==1 )
	{
		ret=1;
	}
	//STK: bond 8
	//1
	else if(a[0]==1 && a[1]==1 && a[2]==1 && a[3]==0 && a[4]==0 && a[5]==0 && a[6]==1 && a[7]==1 )
	{
		ret=1;
	}
	//2
	else if(a[0]==1 && a[1]==1 && a[2]==1 && a[3]==1 && a[4]==1 && a[5]==0 && a[6]==0 && a[7]==0 )
	{
		ret=1;
	}
	//3
	else if(a[0]==0 && a[1]==0 && a[2]==1 && a[3]==1 && a[4]==1 && a[5]==1 && a[6]==1 && a[7]==0 )
	{
		ret=1;
	}
	//4
	else if(a[0]==1 && a[1]==0 && a[2]==0 && a[3]==0 && a[4]==1 && a[5]==1 && a[6]==1 && a[7]==1 )
	{
		ret=1;
	}
	//STK: bond 9
	//1
	else if(a[0]==1 && a[1]==1 && a[2]==1 && a[3]==1 && a[4]==0 && a[5]==0 && a[6]==1 && a[7]==1 )
	{
		ret=1;
	}
	//2
	else if(a[0]==1 && a[1]==1 && a[2]==1 && a[3]==0 && a[4]==0 && a[5]==1 && a[6]==1 && a[7]==1 )
	{
		ret=1;
	}
	//3
	else if(a[0]==1 && a[1]==1 && a[2]==1 && a[3]==1 && a[4]==1 && a[5]==1 && a[6]==0 && a[7]==0 )
	{
		ret=1;
	}
	//4
	else if(a[0]==1 && a[1]==1 && a[2]==1 && a[3]==1 && a[4]==1 && a[5]==0 && a[6]==0 && a[7]==1 )
	{
		ret=1;
	}
	//5
	else if(a[0]==0 && a[1]==1 && a[2]==1 && a[3]==1 && a[4]==1 && a[5]==1 && a[6]==1 && a[7]==0 )
	{
		ret=1;
	}
	//6
	else if(a[0]==0 && a[1]==0 && a[2]==1 && a[3]==1 && a[4]==1 && a[5]==1 && a[6]==1 && a[7]==1 )
	{
		ret=1;
	}
	//7
	else if(a[0]==1 && a[1]==0 && a[2]==0 && a[3]==1 && a[4]==1 && a[5]==1 && a[6]==1 && a[7]==1 )
	{
		ret=1;
	}
	//8
	else if(a[0]==1 && a[1]==1 && a[2]==0 && a[3]==0 && a[4]==1 && a[5]==1 && a[6]==1 && a[7]==1 )
	{
		ret=1;
	}
	//STK: Bond 10:
	//1
	else if(a[0]==0 && a[1]==1 && a[2]==1 && a[3]==1 && a[4]==1 && a[5]==1 && a[6]==1 && a[7]==1 )
	{
		ret=1;
	}
	//2
	else if(a[0]==1 && a[1]==1 && a[2]==0 && a[3]==1 && a[4]==1 && a[5]==1 && a[6]==1 && a[7]==1 )
	{
		ret=1;
	}
	//3
	else if(a[0]==1 && a[1]==1 && a[2]==1 && a[3]==1 && a[4]==0 && a[5]==1 && a[6]==1 && a[7]==1 )
	{
		ret=1;
	}
	//4
	else if(a[0]==1 && a[1]==1 && a[2]==1 && a[3]==1 && a[4]==1 && a[5]==1 && a[6]==0 && a[7]==1 )
	{
		ret=1;
	}
	else
	{
		ret=0;
	}


	
	return ret;
}








unsigned char unc_shrink_thin(unsigned char *a, int size)
{
	int ret=0;
	//spur
	//1
	if(a[0]==0 && a[1]==1 && a[2]==0 && a[3]==0 && a[4]==0 && a[5]==0 && a[6]==0 && a[7]==0)
	{
		ret=1;
	} 
	//2
	else if(a[0]==0 && a[1]==0 && a[2]==0 && a[3]==1 && a[4]==0 && a[5]==0 && a[6]==0 && a[7]==0)
	{
		ret=1;
	}
	//single 4 connection:
	//1
	else if(a[0]==0 && a[1]==0 && a[2]==0 && a[3]==0 && a[4]==0 && a[5]==0 && a[6]==1 && a[7]==0)
	{
		ret=1;
	}
	//2
	else if(a[0]==1 && a[1]==0 && a[2]==0 && a[3]==0 && a[4]==0 && a[5]==0 && a[6]==0 && a[7]==0)
	{
		ret=1;
	}
	// L cluster:
	//1
	else if(a[0]==1&& a[1]==1 && a[2]==0 && a[3]==0 && a[4]==0 && a[5]==0 && a[6]==0 && a[7]==0)
	{
		ret=1;
	}
	//2
	else if(a[0]==0 && a[1]==1 && a[2]==1 && a[3]==0 && a[4]==0 && a[5]==0 && a[6]==0 && a[7]==0)
	{
		ret=1;
	}
	//3
	else if(a[0]==0 && a[1]==0 && a[2]==1 && a[3]==1 && a[4]==0 && a[5]==0 && a[6]==0 && a[7]==0)
	{
		ret=1;
	}
	//4
	else if(a[0]==0 && a[1]==0 && a[2]==0 && a[3]==1 && a[4]==1 && a[5]==0 && a[6]==0 && a[7]==0)
	{
		ret=1;
	}
	//5
	else if(a[0]==0 && a[1]==0 && a[2]==0 && a[3]==0 && a[4]==1 && a[5]==1 && a[6]==0 && a[7]==0)
	{
		ret=1;
	}
	//6
	else if(a[0]==0 && a[1]==0 && a[2]==0 && a[3]==0 && a[4]==0 && a[5]==1 && a[6]==1 && a[7]==0)
	{
		ret=1;
	}
	//7
	else if(a[0]==0 && a[1]==0 && a[2]==0 && a[3]==0 && a[4]==0 && a[5]==0 && a[6]==1 && a[7]==1)
	{
		ret=1;
	}
	//8
	else if(a[0]==1 && a[1]==0 && a[2]==0 && a[3]==0 && a[4]==0 && a[5]==0 && a[6]==0 && a[7]==1)
	{
		ret=1;
	}
	//4-connected offset
	//1
	else if(a[0]==0 && a[1]==1 && a[2]==1 && a[3]==0 && a[4]==1 && a[5]==0 && a[6]==0 && a[7]==0)
	{
		ret=1;
	}
	//2
	else if(a[0]==1 && a[1]==0 && a[2]==1 && a[3]==1 && a[4]==0 && a[5]==0 && a[6]==0 && a[7]==0)
	{
		ret=1;
	}
	//3
	else if(a[0]==1 && a[1]==0 && a[2]==1 && a[3]==0 && a[4]==0 && a[5]==0 && a[6]==0 && a[7]==1)
	{
		ret=1;
	}
	//4
	else if(a[0]==1 && a[1]==1 && a[2]==0 && a[3]==0 && a[4]==0 && a[5]==0 && a[6]==1 && a[7]==0)
	{
		ret=1;
	}
	//Spur corner cluster
	//1
	else if((a[0]==1 || a[2]==1)==1 && a[1]==1  && a[3]==1 && a[4]==0 && a[5]==1 && a[6]==0 && a[7]==0)
	{
		ret=1;
	}
	//2
	else if(a[0]==0 && a[1]==0 && (a[2]==1 || a[4]==1)==1 && a[3]==0  && a[5]==0 && a[6]==0 && a[7]==1)
	{
		ret=1;
	}
	//3
	else if(a[0]==0 && a[1]==1 && a[2]==0 && a[3]==0 && (a[4]==1 || a[6]==1)==1 && a[5]==1 && a[7]==0)
	{
		ret=1;
	}
	//4
	else if((a[0]==1 || a[6]==1)==1 && a[1]==0 && a[2]==0 && a[3]==0 && a[4]==1 && a[5]==0 && a[7]==1)
	{
		ret=1;
	}
	//corner cluster
	else if(a[2]==1 && a[3]==1 && a[4]==1)
	{
		ret=1;
	}
		//Tee Branch
		//1
		else if((a[0]==1) && (a[1]==0 ) && a[2]==1 && a[4]==1 && (a[6]==0 ) && (a[7]==0 ))
		{
			ret=1;
		}
		//2
		else if((a[0]==1) && a[2]==1 && a[3]==0 && a[4]==1 && (a[5]==0 )&& (a[6]==0 ))
		{
			ret=1;
		}
		//3
		else if((a[0]==1) && a[2]==0 && a[3]==0 && a[4]==1 && (a[5]==0 )&& (a[6]==1 ))
		{
			ret=1;
		}
		//4
		else if((a[0]==1) && (a[1]==0 ) && a[2]==0 &&  a[4]==1 && (a[6]==1 ) && (a[7]==0 ))
		{
			ret=1;
		}
		//5
		else if((a[0]==0) && a[2]==1 && a[4]==1 && (a[5]==0 )&& (a[6]==1 ) && (a[7]==0 ))
		{
			ret=1;
		}
		//6
		else if((a[0]==0) && (a[1]==0 ) && a[2]==1 && a[3]==0 && a[4]==1 && (a[6]==1 ))
		{
			ret=1;
		}
		//7
		else if((a[0]==1) && (a[1]==0 ) && a[2]==1 && a[3]==0 && a[4]==0 && (a[6]==1 ))
		{
			ret=1;
		}
		//8
		else if((a[0]==1) && a[2]==1 && a[4]==0 && (a[5]==0 )&& (a[6]==1 ) && (a[7]==0 ))
		{
			ret=1;
		}
		//Vee branch
		//1
		else if( (a[1]==1 ) &&  a[3]==1 && (a[5]==1 || a[6]==1 || a[7]==1 )==1)
		{
			ret=1;
		}
		//2
		else if(a[3]==1 && a[5]==1 && (a[0]==1 || a[1]==1 || a[7]==1)==1)
		{
			ret=1;
		}
		//3
		else if((a[1]==1 || a[2]==1 || a[3]==1)==1 && a[5]==1 && a[7]==1)
		{
			ret=1;
		}
		//4
		else if((a[0]==1) && (a[7]==1 ) && (a[3]==1||a[4]==1||a[5]==1)==1)
		{
			ret=1;
		}
		//Diagnol Branch
		//1
		else if(a[0]==1 && a[2]==1 && a[5]==1 && a[1]==0 && a[4]==0 && a[6]==0)
		{
			ret=1;
		}
		//2
		else if(a[0]==0 && a[3]==0 && a[6]==0 && a[2]==1 && a[4]==1 && a[7]==1)
		{
			ret=1;
		}
		//3
		else if(a[1]==1 && a[4]==1 && a[6]==1 && a[0]==0 && a[2]==0 && a[5]==0)
		{
			ret=1;
		}
		//4
		else if(a[0]==1 && a[3]==1 && a[6]==1 && a[2]==0 && a[4]==0 && a[7]==0)
		{
			ret=1;
		}
		else
		{

			ret=0;
		}
		
	

	

	
	return ret;

}



unsigned char *fixed_Dithering(unsigned char *Image_data, int height, int width, int bytesPerPixel)
{
	unsigned char *new_image = new unsigned char [width*height*bytesPerPixel];
	int threshold_fixed=140;
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
				new_image[(i*width)+j]=1;
			}
		}
	}

	return new_image;
}
unsigned char *copy(unsigned char *Image_data, int height, int width, int bytesPerPixel)
{
	//cout<<"Copy:\n";
	unsigned char *new_image = new unsigned char [width*height*bytesPerPixel];
	int threshold_fixed=1;
	for(int i=0 ; i<height ; i++)
	{
		for(int j=0; j<width ; j++)
		{
			if(Image_data[(i*width)+j]==threshold_fixed)
			{
				new_image[(i*width)+j]=1;
			}
			else 
			{
				new_image[(i*width)+j]=0;
			}
		}
	}

	return new_image;
	//return a;
}
unsigned char *convert1_255(unsigned char *a)
{
	//cout<<"Covert 1 -> 255:\n";
	for(int i=0;i<480;i++)
	{
		for(int j=0;j<640;j++)
		{
			if(a[(i*640)+j]==0)
				continue;
			else
			{
				a[(i*640)+j]=255;
				//printf("%u\n",a[(i*640)+j]);
			}
		}
	}
	return a;
}









