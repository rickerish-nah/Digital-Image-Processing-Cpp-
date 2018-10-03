#include <stdio.h>
#include <iostream>
#include <stdlib.h>

using namespace std;
int roundoff(float s);
unsigned char mode3(unsigned char *array, int q);
unsigned char sort(unsigned char *a,int size);
unsigned char array(int a,int b,int c,int d);



int main(int argc, char *argv[])
{
	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel=3,window=3,size=0;
	int width = 1800 , height = 1200; //for starwars: w=600 and h=338
	unsigned char *Imagedata= new unsigned char [width*height*3];
	unsigned char *image64= new unsigned char [width*height*3];
	unsigned char *Image33= new unsigned char [width*height*3];
	unsigned char *mode33= new unsigned char [9];
	int pixel_avg = (width*height)/8; // /4 for 64 color channel and /8 for 512 color channel
	
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), width*height*BytesPerPixel, file);
	fclose(file);

	//HISTOGRAM
	int R,G,B,r[3][256]={{0}},x=0,y=0,z=0;//x:RED y:GREEN z:BLUE
	float cdf_r[3][256]={0},cdf_g[256]={0},cdf_b[256]={0},pixel_color[3][8]={{0}};// 3x4 for 64 colors and 3x8 for 512 colors
	int count[256]={pixel_avg};
	int c=0,a=0,b=0,d=0;
	for (int rgb=0; rgb<3;rgb++) // Calculate CDF
	{
		for(int k=0;k<256;k++)
		{
			R=0;
			//G=0;
			//B=0;
			for(int i=0;i<height;i++)
			{
				for(int j=0;j<width;j++)
				{
					if(Imagedata[(i*width*3)+(j*3)+rgb] == k)
					{
						r[rgb][k]=++R; //PDF
					}
				}
			}
			//RED
			if(rgb==0)
			{
				//cout<<"R:"<<r[rgb][k]<<endl;

				if(k==0)
				{
					cdf_r[rgb][k]=r[rgb][k];
				}
				else
				{
					cdf_r[rgb][k] = cdf_r[rgb][k-1] + r[rgb][k];
				}
				//cout<<"CDF pixel_R at "<<k<<" : "<<cdf_r[k]<<endl;
			}
			//GREEN
			if(rgb==1)
			{
				if(k==0)
				{
					cdf_r[rgb][k]=r[rgb][k]; //CDF
				}
				else
				{
					cdf_r[rgb][k] = cdf_r[rgb][k-1] + r[rgb][k];
				}
				//cout<<"CDF pixel_G at "<<k<<" : "<<cdf_g[k]<<endl;
			}
			//BLUE
			if(rgb==2)
			{
				if(k==0)
				{
					cdf_r[rgb][k]=r[rgb][k];
				}
				else
				{
					cdf_r[rgb][k] = cdf_r[rgb][k-1] + r[rgb][k];
				}
				//cout<<"CDF pixel_B at "<<k<<" : "<<cdf_b[k]<<endl;
			}
			
		}
	}
	int rr[3][8]={{0}}; // 3x4 for 64 colors and 3x8 for 512 colors
	for(int rgb=0; rgb<3;rgb++)
	{
		for(int k=0; k<256; k++)
		{
			if(cdf_r[rgb][k] <= pixel_avg)
			{
				pixel_color[rgb][0]+= (float)( k * r[rgb][k]);	
				rr[rgb][0]=k;
			}
			if(cdf_r[rgb][k] > pixel_avg && cdf_r[rgb][k] <= 2*pixel_avg)
			{
				//cout<<"q2\n";
				pixel_color[rgb][1]+=(float)( k * r[rgb][k]);	
				rr[rgb][1]=k;
			}
			if(cdf_r[rgb][k] > 2*pixel_avg && cdf_r[rgb][k] <= 3*pixel_avg)
			{
				//cout<<"q3\n";
				pixel_color[rgb][2]+=(float)( k * r[rgb][k]);	
				rr[rgb][2]=k;
			}
			if(cdf_r[rgb][k] > 3*pixel_avg && cdf_r[rgb][k] <= 4*pixel_avg)
			{
				//cout<<"q4\n";
				pixel_color[rgb][3]+=(float)( k * r[rgb][k]);	
				rr[rgb][3]=k;
			}
			// for 512 colors.
			if(cdf_r[rgb][k] > 4*pixel_avg && cdf_r[rgb][k] <= 5*pixel_avg)
			{
				pixel_color[rgb][4]+= (float)( k * r[rgb][k]);	
				rr[rgb][4]=k;
			}
			if(cdf_r[rgb][k] > 5*pixel_avg && cdf_r[rgb][k] <= 6*pixel_avg)
			{
				//cout<<"q2\n";
				pixel_color[rgb][5]+=(float)( k * r[rgb][k]);	
				rr[rgb][5]=k;
			}
			if(cdf_r[rgb][k] > 6*pixel_avg && cdf_r[rgb][k] <= 7*pixel_avg)
			{
				//cout<<"q3\n";
				pixel_color[rgb][6]+=(float)( k * r[rgb][k]);	
				rr[rgb][6]=k;
			}
			if(cdf_r[rgb][k] > 7*pixel_avg && cdf_r[rgb][k] <= 8*pixel_avg)
			{
				//cout<<"q4\n";
				pixel_color[rgb][7]+=(float)( k * r[rgb][k]);	
				rr[rgb][7]=k;
			}

		}
		pixel_color[rgb][0]= roundoff(pixel_color[rgb][0]/pixel_avg);
		pixel_color[rgb][1]= roundoff(pixel_color[rgb][1]/pixel_avg);
		pixel_color[rgb][2]= roundoff(pixel_color[rgb][2]/pixel_avg);
		pixel_color[rgb][3]= roundoff(pixel_color[rgb][3]/pixel_avg);
		pixel_color[rgb][4]= roundoff(pixel_color[rgb][4]/pixel_avg);
		pixel_color[rgb][5]= roundoff(pixel_color[rgb][5]/pixel_avg);
		pixel_color[rgb][6]= roundoff(pixel_color[rgb][6]/pixel_avg);
		pixel_color[rgb][7]= roundoff(pixel_color[rgb][7]/pixel_avg);
		//cout<<"COLOR:"<<rgb<<"\nQ1 Color:"<<pixel_color[rgb][0]<<endl<<"Q2 Color:"<<pixel_color[rgb][1]<<endl<<"Q3 Color:"<<pixel_color[rgb][2]<<endl<<"Q4 Color:"<<pixel_color[rgb][3]<<endl;
	
	}
	for(int rgb=0; rgb<3; rgb++)
	{
		//cout<<"COLOR:"<<rgb<<endl;
		for(int i=0; i<height; i++)
		{
			for(int j=0; j<width; j++)
			{
				for(int k=0; k<256; k++)
				{
					//if(i==0 && j==0)
						//cout<<"\tshade :"<<k<<endl;
					if(Imagedata[(i*width*3)+(j*3)+rgb]==(unsigned char)k)
					{
						if(cdf_r[rgb][k] <= pixel_avg)
						{
							//cout<<"Q1:"<<endl;
							image64[(i*width*3)+(j*3)+rgb]=pixel_color[rgb][0];
						}
						if(cdf_r[rgb][k] > pixel_avg && cdf_r[rgb][k] <= 2*pixel_avg)
						{
							//cout<<"Q2:"<<endl;
							image64[(i*width*3)+(j*3)+rgb]=pixel_color[rgb][1];
							
						}
						if(cdf_r[rgb][k] > 2*pixel_avg && cdf_r[rgb][k] <= 3*pixel_avg)
						{
							//cout<<"Q3:"<<endl;
							image64[(i*width*3)+(j*3)+rgb]=pixel_color[rgb][2];

						}
						if(cdf_r[rgb][k] > 3*pixel_avg && cdf_r[rgb][k] <= 4*pixel_avg)
						{
							//cout<<"Q4:"<<endl;
							image64[(i*width*3)+(j*3)+rgb]=pixel_color[rgb][3];
						}
						//for 512 colors:
						if(cdf_r[rgb][k] > 4*pixel_avg && cdf_r[rgb][k] <= 5*pixel_avg)
						{
							//cout<<"Q1:"<<endl;
							image64[(i*width*3)+(j*3)+rgb]=pixel_color[rgb][4];
						}
						if(cdf_r[rgb][k] > 5*pixel_avg && cdf_r[rgb][k] <= 6*pixel_avg)
						{
							//cout<<"Q2:"<<endl;
							image64[(i*width*3)+(j*3)+rgb]=pixel_color[rgb][5];
							
						}
						if(cdf_r[rgb][k] > 6*pixel_avg && cdf_r[rgb][k] <= 7*pixel_avg)
						{
							//cout<<"Q3:"<<endl;
							image64[(i*width*3)+(j*3)+rgb]=pixel_color[rgb][6];

						}
						if(cdf_r[rgb][k] > 7*pixel_avg && cdf_r[rgb][k] <= 8*pixel_avg)
						{
							//cout<<"Q4:"<<endl;
							image64[(i*width*3)+(j*3)+rgb]=pixel_color[rgb][7];
						}
					
					}
				}

			}
		}		
	}
	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(image64, sizeof(unsigned char), width*height*BytesPerPixel, file);
	fclose(file);

	//WINDOW: OIL Painting effect:
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
				for(int k=i-2;k<=i+2;k++) //i: -1 to +1 for 3x3; i:-2 to +2 for 5x5; i:-3 to +3 for 7x7; i:-4 to +4 for 7x7
				{
					/*if(k<0)
					{
						k=abs(k);
					}
					if(k>height)
					{
						int d=k-height;
						k=height-d;
					}*/
					for(int l=j-2;l<=j+2;l++) //i: -1 to +1 for 3x3; i:-2 to +2 for 5x5; i:-3 to +3 for 7x7; i:-4 to +4 for 7x7
					{
						/*if(l<0)
						{
							l=abs(l);
						}
						if(l>width)
						{
							int d=l-width;
							k=width-d;
						}*/
						mode33[x++]=image64[(k*width*3)+(l*3)+rgb];
					}
						
				}
				//for(int g=0;g<x;g++)
				//cout<<(int)mode33[g]<<endl;
				Image33[(i*width*3)+(j*3)+rgb]=mode3(mode33,x);
			}
		}
	}
	if (!(file=fopen(argv[3],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(Image33, sizeof(unsigned char), width*height*BytesPerPixel, file);
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

unsigned char mode3(unsigned char *a, int size)
{
	int countmode=0,count=0;
	unsigned char mode=0;
	*a=sort(a,size);
	/*cout<<"The elements are:\n";
	for(int g=0;g<size;g++)
	{
		cout<<"\t"<<(int)a[g];
	}*/
	//cout<<endl;
	for (int i=0;i<size;i++)
	{
		for(int j=0;j<size;j++)
		{
			if(i!=j)
			{
				if(a[i] == a[j])
					++countmode;
				if(countmode>count)
				{
					count=countmode;
					mode=a[i];
				}
			}
			//cout<<a[i]<<'\t'<<a[j]<<'\t'<<mode<<endl;
		}
		countmode=0;
	}
	//cout<<"The Mode:\t"<<(int)mode<<endl;
	//cout<<"End of MODE:";
	return mode;
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
