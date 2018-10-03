#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include<math.h>

using namespace std;

//functions used:
unsigned char *fixed_Dithering(unsigned char *Image_data, int height, int width, int bytesPerPixel);
unsigned char *convert1_255(unsigned char *a, int height, int width, int bytesPerPixel);
int non_zero_min(int *a);
unsigned char *invert(unsigned char *a, int height, int width, int bytesPerPixel);
int * label_loop(unsigned char *fixed_image,int *label,int height,int width,int BytesPerPixel);
int checker(int x,int *a, int size);


int main(int argc, char *argv[])
{
	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel=1;
	int width = 372 , height = 372;

	unsigned char *Imagedata= new unsigned char [width*height*BytesPerPixel];
	int *label= new int [width*height*BytesPerPixel];

	unsigned char *final= new unsigned char [width*height*BytesPerPixel];
	unsigned char *newimage_Fixed= new unsigned char [width*height*BytesPerPixel];
	unsigned char *newimage_temp= new unsigned char [width*height*BytesPerPixel];
	
	int a[8]={0};
	int label_var=0,min=0;
	//read input
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), width*height*BytesPerPixel, file);
	fclose(file);

	newimage_Fixed=fixed_Dithering(Imagedata,height,width,BytesPerPixel);
	newimage_Fixed=invert(newimage_Fixed,height,width,BytesPerPixel);
	
	//labelling:
	for (int i=1; i<height-1 ; i++)
	{
		for( int j=1; j<width-1; j++)
		{
			if(newimage_Fixed[(i*width)+j]==0)
			{
				label[(i*width)+j]=0;
				//cout<<"Labled 0"<<endl;
				//cout<<"Class label:"<<(int)label[(i*width)+j]<<endl;
			}
			else if(newimage_Fixed[(i*width)+j]==1)
			{
				//
				//cout<<"Non-zerp Pixel:"<<endl;
				if(label[(i*width)+(j)]==0)
				{
					++label_var;
					//cout<<"New label:"<<label_var<<endl;
					label[(i*width)+(j)]=label_var;

					if(newimage_Fixed[(i*width)+(j+1)]!=0)
					{
						label[(i*width)+(j+1)]=label_var;
					}
					if(newimage_Fixed[((i-1)*width)+(j+1)]!=0)
					{
						label[((i-1)*width)+(j+1)]=label_var;
					}
					if(newimage_Fixed[((i-1)*width)+(j+1)]!=0)
					{
						label[((i-1)*width)+(j+1)]=label_var;
					}
					if(newimage_Fixed[((i-1)*width)+(j)]!=0)
					{
						label[((i-1)*width)+(j)]=label_var;
					}
					if(newimage_Fixed[((i-1)*width)+(j-1)]!=0)
					{
						label[((i-1)*width)+(j-1)]=label_var;
					}
					if(newimage_Fixed[(i*width)+(j-1)]!=0)
					{
						label[(i*width)+(j-1)]=label_var;
					}
					if(newimage_Fixed[((i+1)*width)+(j-1)]!=0)
					{
						label[((i+1)*width)+(j-1)]=label_var;
					}
					if(newimage_Fixed[((i+1)*width)+(j)]!=0)
					{
						label[((i+1)*width)+(j)]=label_var;
					}
					if(newimage_Fixed[((i+1)*width)+(j+1)]!=0)
					{
						label[((i+1)*width)+(j+1)]=label_var;
					}	

				}
				else
				{
					a[0]=label[(i*width)+(j+1)];
					a[1]=label[((i-1)*width)+(j+1)];
					a[2]=label[((i-1)*width)+(j)];
					a[3]=label[((i-1)*width)+(j-1)];
					a[4]=label[(i*width)+(j-1)];
					a[5]=label[((i+1)*width)+(j-1)];
					a[6]=label[((i+1)*width)+(j)];
					a[7]=label[((i+1)*width)+(j+1)];
					/*
					cout<<"The actual:";
					for(int k=0;k<8;k++)
						cout<<a[k]<<"\t";
					cout<<endl;
					*/ 
					min=non_zero_min(a);
					//cout<<"Min:"<<min<<endl;
					//{
						if(newimage_Fixed[(i*width)+j]!=0)
						{
							label[(i*width)+j]=min;
						}
						if(newimage_Fixed[(i*width)+(j+1)]!=0)
						{
							label[(i*width)+(j+1)]=min;
						}
						if(newimage_Fixed[((i-1)*width)+(j+1)]!=0)
						{
							label[((i-1)*width)+(j+1)]=min;
						}
						if(newimage_Fixed[((i-1)*width)+(j+1)]!=0)
						{
							label[((i-1)*width)+(j+1)]=min;
						}
						if(newimage_Fixed[((i-1)*width)+(j)]!=0)
						{
							label[((i-1)*width)+(j)]=min;
						}
						if(newimage_Fixed[((i-1)*width)+(j-1)]!=0)
						{
							label[((i-1)*width)+(j-1)]=min;
						}
						if(newimage_Fixed[(i*width)+(j-1)]!=0)
						{
							label[(i*width)+(j-1)]=min;
						}
						if(newimage_Fixed[((i+1)*width)+(j-1)]!=0)
						{
							label[((i+1)*width)+(j-1)]=min;
						}
						if(newimage_Fixed[((i+1)*width)+(j)]!=0)
						{
							label[((i+1)*width)+(j)]=min;
						}
						if(newimage_Fixed[((i+1)*width)+(j+1)]!=0)
						{
							label[((i+1)*width)+(j+1)]=min;
						}	
					//}
				}
				//cout<<"Class label:"<<(int)label[(i*width)+j]<<endl;
			}
		}
	}
	


	for(int k=0;k<200;k++)
	{
		label=label_loop(newimage_Fixed,label,height,width,BytesPerPixel);
	}

	int var=-2;
	int h=0;
	int d=0;
	int *ss= new int [h];
	int la[16]={0};
	for(int i=1;i<height-1;i++)
	{
		for(int j=1;j<width-1;j++)
		{
			if(label[(i*width)+j]!=0 && label[(i*width)+j]!=var)
			{
				ss[h++]=label[(i*width)+j];

			}
		}
	}
	cout<<"Actual Lable:"<<endl;
	for(int i=0;i<h;i++)
	{
		if(ss[i]==0)
			continue;
		else
		{
			la[d]=ss[i];
			d++;
			//cout<<"\t:"<<ss[i];
		}
		for(int j=i+1;j<h;j++)
		{
			if(ss[j]==ss[i])
			{
				ss[j]=0;
			}

		}
	}
	cout<<endl;
	for(int k=0;k<16;k++)
	{
		cout<<"\t:"<<la[k];
	}
	cout<<endl;
	cout<<"Modified Lable:"<<endl;
	for(int k=0;k<16;k++)
	{
		cout<<"\t:"<<k+1;
	}
	cout<<endl;
	
	
	for(int i=1;i<height-1;i++)
	{
		for(int j=1;j<width-1;j++)
		{	
			//cout<<"L"<<endl;
			if(label[(i*width)+j]!=0)//!=0 )//&& label[(i*width)+j]>var)
			{
				for(int g=0;g<16;g++)
				{
					if(label[(i*width)+j]==la[g])
						label[(i*width)+j]=g+1;

				}
				
			}
		}
	}
	
	var=-2;
	//cout<<"2:"<<endl;
	for(int i=1;i<height-1;i++)
	{
		for(int j=1;j<width-1;j++)
		{	
			//cout<<"L"<<endl;
			if(label[(i*width)+j]!=0 && label[(i*width)+j]>var)
			{
				cout<<"I:"<<i<<"\tJ:"<<j<<"\t\t\tClass label:"<<label[(i*width)+j]<<endl;
				var=label[(i*width)+j];
			}
		}
	}
	




	

	newimage_temp=convert1_255(newimage_Fixed,height,width,BytesPerPixel);
	//write output
	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(newimage_temp, sizeof(unsigned char), width*height*BytesPerPixel, file);
	fclose(file);


	return 0;
}
//END of CPP


int * label_loop(unsigned char * newimage_Fixed,int *label,int height,int width,int BytesPerPixel)
{
	
	int a[8]={0};
	int label_var=0,min=0;
	for( int i=0;i<height;i++)
	{
		for(int j=0;j<width;j++)
		{
			if(newimage_Fixed[(i*width)+j]==1)
			{
				//
				//cout<<"Non-zerp Pixel:"<<endl;
				if(label[(i*width)+(j)]!=0)
				{
					//cout<<"wowo:"<<endl;
					a[0]=label[(i*width)+(j+1)];
					a[1]=label[((i-1)*width)+(j+1)];
					a[2]=label[((i-1)*width)+(j)];
					a[3]=label[((i-1)*width)+(j-1)];
					a[4]=label[(i*width)+(j-1)];
					a[5]=label[((i+1)*width)+(j-1)];
					a[6]=label[((i+1)*width)+(j)];
					a[7]=label[((i+1)*width)+(j+1)];
					/*
					cout<<"The actual:";
					for(int k=0;k<8;k++)
						cout<<a[k]<<"\t";
					cout<<endl;
					*/ 
					min=non_zero_min(a);
					//cout<<"Min:"<<min<<endl;
					if(min == 0)
					{	min=++label_var;
						label[(i*width)+j]=min;
						//cout<<"Class label:"<<(int)label[(i*width)+j]<<endl;
					}
					else
					{
						if(newimage_Fixed[(i*width)+j]!=0)
						{
							label[(i*width)+j]=min;
						}
						if(newimage_Fixed[(i*width)+(j+1)]!=0)
						{
							label[(i*width)+(j+1)]=min;
						}
						if(newimage_Fixed[((i-1)*width)+(j+1)]!=0)
						{
							label[((i-1)*width)+(j+1)]=min;
						}
						if(newimage_Fixed[((i-1)*width)+(j+1)]!=0)
						{
							label[((i-1)*width)+(j+1)]=min;
						}
						if(newimage_Fixed[((i-1)*width)+(j)]!=0)
						{
							label[((i-1)*width)+(j)]=min;
						}
						if(newimage_Fixed[((i-1)*width)+(j-1)]!=0)
						{
							label[((i-1)*width)+(j-1)]=min;
						}
						if(newimage_Fixed[(i*width)+(j-1)]!=0)
						{
							label[(i*width)+(j-1)]=min;
						}
						if(newimage_Fixed[((i+1)*width)+(j-1)]!=0)
						{
							label[((i+1)*width)+(j-1)]=min;
						}
						if(newimage_Fixed[((i+1)*width)+(j)]!=0)
						{
							label[((i+1)*width)+(j)]=min;
						}
						if(newimage_Fixed[((i+1)*width)+(j+1)]!=0)
						{
							label[((i+1)*width)+(j+1)]=min;
						}	
					}
				}
				//cout<<"Class label:"<<(int)label[(i*width)+j]<<endl;
			}
		}
	}
	/*
	int var=-2;
	cout<<"2:"<<endl;
	for(int i=1;i<height-1;i++)
	{
		for(int j=1;j<width-1;j++)
		{	
			//cout<<"L"<<endl;
			if(label[(i*width)+j]!=0 && label[(i*width)+j]>var)
			{
				cout<<"I:"<<i<<"J:"<<j<<"Class label:"<<label[(i*width)+j]<<endl;
				var=label[(i*width)+j];
			}
		}
	}*/
	return label;
}

int checker(int num,int *a, int size)
{
	//cout<<"siz:"<<size<<endl;
	int check=1;
	/*for(int k=0;k<size;k++)
	{
		cout<<"\t"<<a[k];
	}
	cout<<endl;*/
	if(size==0)
		check=1;
	else
	{
		for(int i=0;i<size;i++)
		{
			if(a[i]!=0)
			{
				if(a[i]==num)
					check=0;
			}
		}
	}
	//cout<<check<<endl;
	return check;
}







int non_zero_min(int *a)
{
	int min=0;
	/*
	for(int k=0;k<8;k++)
		cout<<a[k]<<"\t";
	cout<<endl;
	*/
	for(int i=0;i<8;i++)
	{
		if(a[i]>0)
		{
			if(min<a[i])
			{
				min=a[i];
			}
		}
	}
	//cout<<"Return MIN:"<<min<<endl;
	return min;

}

unsigned char *fixed_Dithering(unsigned char *Image_data, int height, int width, int bytesPerPixel)
{
	unsigned char *new_image = new unsigned char [width*height*bytesPerPixel];
	int threshold_fixed=127;
	for(int i=0 ; i<height ; i++)
	{
		for(int j=0; j<width ; j++)
		{
			if(Image_data[(i*width)+j]<=threshold_fixed)
			{
				new_image[(i*width)+j]=0;
			}
			else if(Image_data[(i*width)+j]>threshold_fixed)
			{
				new_image[(i*width)+j]=1;
			}
		}
	}

	return new_image;
}
unsigned char *convert1_255(unsigned char *a,int height,int width,int BytesPerPixel)
{
	//cout<<"Covert 1 -> 255:\n";
	for(int i=0;i<height;i++)
	{
		for(int j=0;j<width;j++)
		{
			if(a[(i*width)+j]==0)
				continue;
			else
			{
				a[(i*width)+j]=255;
				//printf("%u\n",a[(i*640)+j]);
			}
		}
	}
	return a;
}
unsigned char *invert(unsigned char *a,int height,int width,int BytesPerPixel)
{
	//cout<<"Covert 1 -> 255:\n";
	for(int i=0;i<height;i++)
	{
		for(int j=0;j<width;j++)
		{
			if(a[(i*width)+j]==0)
				a[(i*width)+j]=1;
			else
			{
				a[(i*width)+j]=0;
				//printf("%u\n",a[(i*640)+j]);
			}
		}
	}
	return a;
}