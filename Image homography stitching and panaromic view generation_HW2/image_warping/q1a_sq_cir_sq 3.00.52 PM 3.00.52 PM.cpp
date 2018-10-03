#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include<math.h>

using namespace std;

int roundoff( float x);



int main(int argc, char *argv[])
{
	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel=3,window=3,size=0;
	int width = 512 , height = 512; 
	unsigned char *Imagedata= new unsigned char [width*height*3];
	unsigned char *Image_Circle= new unsigned char [width*height*3];
	unsigned char *Image_Square= new unsigned char [width*height*3];
	//unsigned char Imagedata[512][512][3]={{{0}}};
	//unsigned char Image_Circle[512][512][3]={{{0}}};
	//variables for conversion
	int l,m,xxx=0,yyy=0,sign_x=1,sign_y=1,uu=0,vv=0;
	float x=0,y=0,xx=0,yy=0,uuu=0,vvv=0,u=0,v=0,den=0,diff_x=0,diff_y=0,diff_u=0,diff_v=0; //polar cordinates

	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), width*height*BytesPerPixel, file);
	fclose(file);

	for( int rgb=0; rgb<3; rgb++)
	{
		for(int i=0; i<height; i++) //loop 1
		{
			//cout<<"TIME:"<<i<<endl;
			x=(float)(i-255)/256;

			
			
			for(int j=0; j<width; j++) //loop 2
			{
				y=(float)(j-255)/256;

				// Square to ellipse: (Major and minor axis same)
				u = x * sqrt(1-(pow(y,2)/2));
				u = (u*256)+255;


				v = y * sqrt(1-(pow(x,2)/2));
				v= (v*256)+255;
				
				l = roundoff(u);
				diff_x=abs(u-l);

				m = roundoff(v);
				diff_y=abs(m-v);

				//cout<<"x:"<<x<<"\tu:"<<l<<"\t Y:"<<y<<"\t v:"<<m<<endl;
				
				Image_Circle[(l*width*3)+(m*3)+rgb]=Imagedata[(i*width*3)+(j*3)+rgb]*(1-diff_x)*(1-diff_y) + Imagedata[(i*width*3)+((j+1)*3)+rgb]*(diff_x)*(1-diff_y) + Imagedata[((i+1)*width*3)+(j*3)+rgb]*(1-diff_x)*(diff_y) + Imagedata[((i+1)*width*3)+((j+1)*3)+rgb]*(diff_x)*(diff_y);
				
				//Circle to 
				/*
				uuu = (float)((u-255)/256);
				vvv = (float)((v-255)/256);
				//cout<<"u:"<<l<<"\tUuu:"<<uuu<<"\tv:"<<m<<"\t V:"<<vvv<<endl;
				xx = 0.5*sqrt(2+pow(uuu,2)-pow(vvv,2)+(sqrt(2)*2*uuu)) - 0.5*sqrt(2+pow(uuu,2)-pow(vvv,2)-(sqrt(2)*2*uuu));
				xx = (xx * 256) + 255;
				xxx = roundoff(xx);
				diff_u = abs(xx-xxx);
				

				yy = 0.5*sqrt(2-pow(uuu,2)+pow(vvv,2)+(sqrt(2)*2*vvv)) - 0.5*sqrt(2-pow(uuu,2)+pow(vvv,2)-(sqrt(2)*2*vvv));
				yy = (yy*256) + 255;
				yyy=roundoff(yy);
				diff_v = abs(yy-yyy);
			

				uu=roundoff((uuu*256)+255);
				vv=roundoff((vvv*256)+255);
				//cout<<"UUU:"<<uuu<<"\tuu:"<<uu<<endl;
				//cout<<"x:"<<xx<<"\tu:"<<uuu<<"\t Y:"<<yy<<"\t v:"<<vvv<<endl;
				//cout<<"x:"<<xx<<"\tu:"<<x*256<<"\t Y:"<<yy<<"\t v:"<<y*256<<endl;
				Image_Square[(i*width*3)+(j*3)+rgb]=Image_Circle[(uu*width*3)+(vv*3)+rgb]*(1-diff_u)*(1-diff_v) + Image_Circle[(uu*width*3)+((vv+1)*3)+rgb]*(diff_u)*(1-diff_v) + Image_Circle[((uu+1)*width*3)+(vv*3)+rgb]*(1-diff_u)*(diff_v) + Image_Circle[((uu+1)*width*3)+((vv+1)*3)+rgb]*(diff_u)*(diff_v);
				*/
			}
			
		}
	}

	for( int rgb=0; rgb<3; rgb++)
	{
		for(int i=0; i<height; i++) //loop 1
		{
			//cout<<"TIME:"<<i<<endl;
			x=(float)(i-255)/256;

			
			
			for(int j=0; j<width; j++) //loop 2
			{
				y=(float)(j-255)/256;

				// Square to ellipse: (Major and minor axis same)
				u = x * sqrt(1-(pow(y,2)/2));
				u = (u*256)+255;


				v = y * sqrt(1-(pow(x,2)/2));
				v= (v*256)+255;
				
				l = roundoff(u);
				diff_x=abs(u-l);

				m = roundoff(v);
				diff_y=abs(m-v);

				
				//Circle to 

				uuu = (float)((u-255)/256);
				vvv = (float)((v-255)/256);
				//cout<<"u:"<<l<<"\tUuu:"<<uuu<<"\tv:"<<m<<"\t V:"<<vvv<<endl;
				xx = 0.5*sqrt(2+pow(uuu,2)-pow(vvv,2)+(sqrt(2)*2*uuu)) - 0.5*sqrt(2+pow(uuu,2)-pow(vvv,2)-(sqrt(2)*2*uuu));
				xx = (xx * 256) + 255;
				xxx = roundoff(xx);
				diff_u = abs(xx-xxx);
				

				yy = 0.5*sqrt(2-pow(uuu,2)+pow(vvv,2)+(sqrt(2)*2*vvv)) - 0.5*sqrt(2-pow(uuu,2)+pow(vvv,2)-(sqrt(2)*2*vvv));
				yy = (yy*256) + 255;
				yyy=roundoff(yy);
				diff_v = abs(yy-yyy);
			

				uu=roundoff((uuu*256)+255);
				vv=roundoff((vvv*256)+255);
				//cout<<"UUU:"<<uuu<<"\tuu:"<<uu<<endl;
				//cout<<"x:"<<xx<<"\tu:"<<uuu<<"\t Y:"<<yy<<"\t v:"<<vvv<<endl;
				//cout<<"x:"<<xx<<"\tu:"<<x*256<<"\t Y:"<<yy<<"\t v:"<<y*256<<endl;
				if(uu>=511)
				{
					uu=uu-1;
				}
				if(vv>=511)
				{
					vv=vv-1;
				}
				Image_Square[(i*width*3)+(j*3)+rgb]=Image_Circle[(uu*width*3)+(vv*3)+rgb]*(1-diff_u)*(1-diff_v) + Image_Circle[(uu*width*3)+((vv+1)*3)+rgb]*(diff_u)*(1-diff_v) + Image_Circle[((uu+1)*width*3)+(vv*3)+rgb]*(1-diff_u)*(diff_v) + Image_Circle[((uu+1)*width*3)+((vv+1)*3)+rgb]*(diff_u)*(diff_v);
				
			}
			
		}
	}
	
	
	




	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(Image_Circle, sizeof(unsigned char), width*height*BytesPerPixel, file);
	fclose(file);
	
	if (!(file=fopen(argv[3],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(Image_Square, sizeof(unsigned char), width*height*BytesPerPixel, file);
	fclose(file);
	
	return 0;
}

int roundoff(float num)
{
	float s = (int) num + 0.5;
	if(num >= s)
		return(s+0.5);
	else
		return(s-0.5);
}

