#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include<math.h>



using namespace std;

int MBVQ_Type(unsigned char * Imagedata,int i,int j,int width, int BytesPerPixel);
int closest_vertex(unsigned char * Imagedata,int i,int j,int width, int BytesPerPixel, unsigned char mbvq[4][3]);
unsigned char minimum(unsigned char r,unsigned char g, unsigned char b);

int main(int argc, char *argv[])
{
	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel=3,N=2,width = 700 , height = 700,option=0,op=1;
	
	unsigned char *newimage= new unsigned char [width*height*BytesPerPixel];
	unsigned char *Imagedata= new unsigned char [width*height*BytesPerPixel];
	unsigned char *tempimageRGB_CMY= new unsigned char [width*height*BytesPerPixel];
	unsigned char *tempimageCMY_RGB= new unsigned char [width*height*BytesPerPixel];
	
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), width*height*BytesPerPixel, file);
	fclose(file);

	/*unsigned char CMYW[4][3]={{0,255,0},{255,0,0},{0,0,255},{0,0,0}},  //1.  CMY space
				  MYGC[4][3]={{255,0,0},{0,0,255},{0,255,255},{0,255,0}},  //2
				  RGMY[4][3]={{255,0,255},{0,255,255},{255,0,0},{0,0,255}},  //3
				  KRGB[4][3]={{255,255,255},{255,0,255},{0,255,255},{255,255,0}},  //4
				  RGBM[4][3]={{255,0,255},{0,255,255},{255,255,0},{255,0,0}},  //5
				  CMGB[4][3]={{0,255,0},{255,0,0},{0,255,255},{255,255,0}};  //6 */

	unsigned char CMYW[4][3]={{255,0,255},{0,255,255},{255,255,0},{255,255,255}},  //1.  RGB space
				  MYGC[4][3]={{0,255,255},{255,255,0},{255,0,0},{255,0,255}},  //2
				  RGMY[4][3]={{0,255,0},{255,0,0},{0,255,255},{255,255,0}},  //3
				  KRGB[4][3]={{0,0,0},{0,255,0},{255,0,0},{0,0,255}},  //4
				  RGBM[4][3]={{0,255,0},{255,0,0},{0,0,255},{0,255,255}},  //5
				  CMGB[4][3]={{255,0,255},{0,255,255},{255,0,0},{0,0,255}};  //6
	unsigned char k=0;
	/*
	for(int i=0; i<height;i++)
	{
		for(int j=0;j<width;j++)
		{
			//k=minimum(Imagedata[(i*width*3)+(j*3)+0],Imagedata[(i*width*3)+(j*3)+1],Imagedata[(i*width*3)+(j*3)+2]);

			tempimageRGB_CMY[(i*width*3)+(j*3)+0]=255-Imagedata[(i*width*3)+(j*3)+0];
			tempimageRGB_CMY[(i*width*3)+(j*3)+1]=255-Imagedata[(i*width*3)+(j*3)+1];
			tempimageRGB_CMY[(i*width*3)+(j*3)+2]=255-Imagedata[(i*width*3)+(j*3)+2];
		}
	}
*/
	for(int i=0; i<height;i++)
	{
		if(i%2==0)
		{
			for(int j=0;j<width;j++)
			{
				int type=0, vertex_pointer=0;
				type = MBVQ_Type(Imagedata,i,j,width,BytesPerPixel);
				unsigned char cmy[3]={0},e[3]={0};
				//cout<<"MBVQ:"<<type<<endl;
				switch(type)
				{
					case 1: //CMYW
					{
						vertex_pointer=closest_vertex(Imagedata,i,j,width,BytesPerPixel,CMYW);
						cmy[0]=CMYW[vertex_pointer][0];
						cmy[1]=CMYW[vertex_pointer][1];
						cmy[2]=CMYW[vertex_pointer][2];
						break;
					}
					case 2: //MYGC
					{
						vertex_pointer=closest_vertex(Imagedata,i,j,width,BytesPerPixel,MYGC);
						cmy[0]=MYGC[vertex_pointer][0];
						cmy[1]=MYGC[vertex_pointer][1];
						cmy[2]=MYGC[vertex_pointer][2];
						break;
					}
					case 3: //RGMY
					{	
						vertex_pointer=closest_vertex(Imagedata,i,j,width,BytesPerPixel,RGMY);
						cmy[0]=RGMY[vertex_pointer][0];
						cmy[1]=RGMY[vertex_pointer][1];
						cmy[2]=RGMY[vertex_pointer][2];
							break;
					}
					case 4: //KRGB
					{
							vertex_pointer=closest_vertex(Imagedata,i,j,width,BytesPerPixel,KRGB);
						break;
						cmy[0]=KRGB[vertex_pointer][0];
						cmy[1]=KRGB[vertex_pointer][1];
						cmy[2]=KRGB[vertex_pointer][2];
					}
					case 5:  //RGBM
					{
						vertex_pointer=closest_vertex(Imagedata,i,j,width,BytesPerPixel,RGBM);
						cmy[0]=RGBM[vertex_pointer][0];
						cmy[1]=RGBM[vertex_pointer][1];
						cmy[2]=RGBM[vertex_pointer][2];
						break;
					}
					case 6:  //CMGB
					{
						vertex_pointer=closest_vertex(Imagedata,i,j,width,BytesPerPixel,CMGB);
						cmy[0]=CMGB[vertex_pointer][0];
						cmy[1]=CMGB[vertex_pointer][1];
						cmy[2]=CMGB[vertex_pointer][2];
						break;
					}
					default:
					{
						cout<<"\n\n\t\tOOps sorry\n\n";
					}
				}
				e[0]=Imagedata[(i*width*BytesPerPixel)+(j*BytesPerPixel)+0]-cmy[0];
				e[1]=Imagedata[(i*width*BytesPerPixel)+(j*BytesPerPixel)+1]-cmy[1];
				e[2]=Imagedata[(i*width*BytesPerPixel)+(j*BytesPerPixel)+2]-cmy[2];

				for(int rgb=0;rgb<3;rgb++)
				{
					if(j+1<width)
					{
						Imagedata[( (i)*width*3 )+ ((j+1)*3) + rgb] = Imagedata[( (i)*width*3 )+ ((j+1)*3) + rgb] + (unsigned char)(round((e[rgb]*7/16.0)));
						//tempimageRGB_CMY[( (i)*width*3 )+ ((j+1)*3) + rgb] = (newimage[( (i)*width*3 )+ ((j+1)*3) + rgb]>255)?255:newimage[( (i)*width*3 )+ ((j+1)*3) + rgb];
						//tempimageRGB_CMY[( (i)*width*3 )+ ((j+1)*3) + rgb] = (newimage[( (i)*width*3 )+ ((j+1)*3) + rgb]<0)?0:newimage[( (i)*width*3 )+ ((j+1)*3) + rgb];
					}
					if(i+1<height && j-1 > 0)
					{
						Imagedata[( (i+1)*width*3 )+ ((j-1)*3) + rgb] = Imagedata[( (i+1)*width*3 )+ ((j-1)*3) + rgb]+(unsigned char)(round((e[rgb]*3/16.0)));
						//tempimageRGB_CMY[( (i+1)*width*3 )+ ((j-1)*3) + rgb]=(newimage[( (i+1)*width*3 )+ ((j-1)*3) + rgb]>255)?255:newimage[( (i+1)*width*3 )+ ((j-1)*3) + rgb];
						//tempimageRGB_CMY[( (i+1)*width*3 )+ ((j-1)*3) + rgb]=(newimage[( (i+1)*width*3 )+ ((j-1)*3) + rgb]<0)?0:newimage[( (i+1)*width*3 )+ ((j-1)*3) + rgb];
					}
					if(i+1<height)
					{
						Imagedata[( (i+1)*width*3 )+ (j*3) + rgb] = Imagedata[( (i+1)*width*3 )+ (j*3) + rgb] + (unsigned char)(round((e[rgb]*5/16.0)));
						//tempimageRGB_CMY[( (i+1)*width*3 )+ (j*3) + rgb] = (newimage[( (i+1)*width*3 )+ (j*3) + rgb]>255)?255:newimage[( (i+1)*width*3 )+ (j*3) + rgb];
						//tempimageRGB_CMY[( (i+1)*width*3 )+ (j*3) + rgb] = (newimage[( (i+1)*width*3 )+ (j*3) + rgb]<0)?0:newimage[( (i+1)*width*3 )+ (j*3) + rgb];
					}
					if(i+1<height && j-1>0)
					{
						//cout<<"#"<<i<<":"<<j<<"\t i+1 : j+1"<<endl;
						Imagedata[( (i+1)*width *3)+ ((j+1)*3)+rgb] = Imagedata[( (i+1)*width *3)+ ((j+1)*3)+rgb] + (unsigned char)(round((e[rgb]*1/16.0)));
						//tempimageRGB_CMY[( (i+1)*width *3)+ ((j+1)*3)+rgb] = (newimage[( (i+1)*width *3)+ ((j+1)*3)+rgb]>255)?255:newimage[( (i+1)*width *3)+ ((j+1)*3)+rgb];
						//tempimageRGB_CMY[( (i+1)*width *3)+ ((j+1)*3)+rgb] = (newimage[( (i+1)*width *3)+ ((j+1)*3)+rgb]<0)?0:newimage[( (i+1)*width *3)+ ((j+1)*3)+rgb];
					}
				}
			}
		}
		else
		{
			for(int j=width-1;j>=0;j--)
			{
				int type=0, vertex_pointer=0;
				type = MBVQ_Type(tempimageRGB_CMY,i,j,width,BytesPerPixel);
				unsigned char cmy[3]={0},e[3]={0};
				//cout<<"MBVQ:"<<type<<endl;
				switch(type)
				{
					case 1: //CMYW
					{
						vertex_pointer=closest_vertex(Imagedata,i,j,width,BytesPerPixel,CMYW);
						cmy[0]=CMYW[vertex_pointer][0];
						cmy[1]=CMYW[vertex_pointer][1];
						cmy[2]=CMYW[vertex_pointer][2];
						break;
					}
					case 2: //MYGC
					{
						vertex_pointer=closest_vertex(Imagedata,i,j,width,BytesPerPixel,MYGC);
						cmy[0]=MYGC[vertex_pointer][0];
						cmy[1]=MYGC[vertex_pointer][1];
						cmy[2]=MYGC[vertex_pointer][2];
						break;
					}
					case 3: //RGMY
					{	
						vertex_pointer=closest_vertex(Imagedata,i,j,width,BytesPerPixel,RGMY);
						cmy[0]=RGMY[vertex_pointer][0];
						cmy[1]=RGMY[vertex_pointer][1];
						cmy[2]=RGMY[vertex_pointer][2];
							break;
					}
					case 4: //KRGB
					{
							vertex_pointer=closest_vertex(Imagedata,i,j,width,BytesPerPixel,KRGB);
						break;
						cmy[0]=KRGB[vertex_pointer][0];
						cmy[1]=KRGB[vertex_pointer][1];
						cmy[2]=KRGB[vertex_pointer][2];
					}
					case 5:  //RGBM
					{
						vertex_pointer=closest_vertex(Imagedata,i,j,width,BytesPerPixel,RGBM);
						cmy[0]=RGBM[vertex_pointer][0];
						cmy[1]=RGBM[vertex_pointer][1];
						cmy[2]=RGBM[vertex_pointer][2];
						break;
					}
					case 6:  //CMGB
					{
						vertex_pointer=closest_vertex(Imagedata,i,j,width,BytesPerPixel,CMGB);
						cmy[0]=CMGB[vertex_pointer][0];
						cmy[1]=CMGB[vertex_pointer][1];
						cmy[2]=CMGB[vertex_pointer][2];
						break;
					}
					default:
					{
						cout<<"\n\n\t\tOOps sorry\n\n";
					}
				}
				e[0]=Imagedata[(i*width*BytesPerPixel)+(j*BytesPerPixel)+0]-cmy[0];
				e[1]=Imagedata[(i*width*BytesPerPixel)+(j*BytesPerPixel)+1]-cmy[1];
				e[2]=Imagedata[(i*width*BytesPerPixel)+(j*BytesPerPixel)+2]-cmy[2];

				for(int rgb=0;rgb<3;rgb++)
				{
					if(j>0)
					{
						Imagedata[( (i)*width*3 )+ ((j-1)*3) + rgb] = Imagedata[( (i)*width*3 )+ ((j-1)*3) + rgb]+(unsigned char)(round((e[rgb]*7/16.0)));
						//tempimageRGB_CMY[( (i)*width*3 )+ ((j-1)*3) + rgb] = (newimage[( (i)*width*3 )+ ((j-1)*3) + rgb]>255)?255:newimage[( (i)*width*3 )+ ((j-1)*3) + rgb];
						//tempimageRGB_CMY[( (i)*width*3 )+ ((j-1)*3) + rgb] = (newimage[( (i)*width*3 )+ ((j-1)*3) + rgb]<0)?0:newimage[( (i)*width*3 )+ ((j-1)*3) + rgb];
					}
					if(i+1<height && j > 0)
					{
						Imagedata[( (i+1)*width*3 )+ ((j-1)*3) + rgb] = Imagedata[( (i+1)*width*3 )+ ((j-1)*3) + rgb]+(unsigned char)(round((e[rgb]*1/16.0)));
						//tempimageRGB_CMY[( (i+1)*width*3 )+ ((j-1)*3) + rgb]=(newimage[( (i+1)*width*3 )+ ((j-1)*3) + rgb]>255)?255:newimage[( (i+1)*width*3 )+ ((j-1)*3) + rgb];
						//tempimageRGB_CMY[( (i+1)*width*3 )+ ((j-1)*3) + rgb]=(newimage[( (i+1)*width*3 )+ ((j-1)*3) + rgb]<0)?0:newimage[( (i+1)*width*3 )+ ((j-1)*3) + rgb];
					}
					if(i+1<height)
					{
						Imagedata[( (i+1)*width*3 )+ (j*3) + rgb] = Imagedata[( (i+1)*width*3 )+ (j*3) + rgb]+ (unsigned char)(round((e[rgb]*5/16.0)));
						//tempimageRGB_CMY[( (i+1)*width*3 )+ (j*3) + rgb] = (newimage[( (i+1)*width*3 )+ (j*3) + rgb]>255)?255:newimage[( (i+1)*width*3 )+ (j*3) + rgb];
						//tempimageRGB_CMY[( (i+1)*width*3 )+ (j*3) + rgb] = (newimage[( (i+1)*width*3 )+ (j*3) + rgb]<0)?0:newimage[( (i+1)*width*3 )+ (j*3) + rgb];
					}
					if(i+1<height && j>0)
					{
						//cout<<"#"<<i<<":"<<j<<"\t i+1 : j+1"<<endl;
						Imagedata[( (i+1)*width *3)+ ((j+1)*3)+rgb] = Imagedata[( (i+1)*width *3)+ ((j+1)*3)+rgb] + (unsigned char)(round((e[rgb]*3/16.0)));
						//tempimageRGB_CMY[( (i+1)*width *3)+ ((j+1)*3)+rgb] = (newimage[( (i+1)*width *3)+ ((j+1)*3)+rgb]>255)?255:newimage[( (i+1)*width *3)+ ((j+1)*3)+rgb];
						//tempimageRGB_CMY[( (i+1)*width *3)+ ((j+1)*3)+rgb] = (newimage[( (i+1)*width *3)+ ((j+1)*3)+rgb]<0)?0:newimage[( (i+1)*width *3)+ ((j+1)*3)+rgb];
					}
				}
			}
		}
	}
	
	for(int i=0; i<height;i++)
	{
		for(int j=0;j<width;j++)
		{
			tempimageCMY_RGB[(i*width*3)+(j*3)+0]=255-Imagedata[(i*width*3)+(j*3)+0];
			tempimageCMY_RGB[(i*width*3)+(j*3)+1]=255-Imagedata[(i*width*3)+(j*3)+1];
			tempimageCMY_RGB[(i*width*3)+(j*3)+2]=255-Imagedata[(i*width*3)+(j*3)+2];
		}
	}
	if (!(file=fopen(argv[2],"wb"))) {
	cout << "Cannot open file: " << argv[2] << endl;
	exit(1);
	}
	fwrite(tempimageCMY_RGB, sizeof(unsigned char), width*height*BytesPerPixel, file);
	fclose(file);









	return 0;
}
//find the closest vertex to the given pixel;
int closest_vertex(unsigned char * Imagedata,int i,int j,int width, int BytesPerPixel, unsigned char mbvq[4][3])
{
	float distance[4]={0}, l_2[4]={0};
	int min=1000,min_v=0;
	for(int i=0;i<4;i++)
	{
		distance[i]= ( pow((mbvq[i][0]-Imagedata[(i*width*BytesPerPixel)+(j*BytesPerPixel)+0]),2)+
					 pow((mbvq[i][1]-Imagedata[(i*width*BytesPerPixel)+(j*BytesPerPixel)+1]),2)+
					 pow((mbvq[i][2]-Imagedata[(i*width*BytesPerPixel)+(j*BytesPerPixel)+2]),2) );
		l_2[i] = sqrt(distance[i]);
		if(min>l_2[i])
		{
			min=l_2[i];
			min_v=i;
		}
	}
	return min_v;
}

unsigned char minimum(unsigned char r,unsigned char g, unsigned char b)
{ 
	unsigned char ret;
	if((r<g) && (r<b) )
		ret= r;
	if((g<r)&&(g<b))
		ret= g;
	if((b<r)&&(b<g))
		ret= b;
	return ret;
}

//MBVQ type:
int MBVQ_Type(unsigned char * Imagedata,int i,int j,int width, int BytesPerPixel)
{
	int r=Imagedata[(i*width*BytesPerPixel)+(j*BytesPerPixel)+0];
	int	g=Imagedata[(i*width*BytesPerPixel)+(j*BytesPerPixel)+1];
	int	b=Imagedata[(i*width*BytesPerPixel)+(j*BytesPerPixel)+2];
	if((r+g)>255)
	{
		if((g+b)>255)
		{
			if((r+g+b)>510)
				return 1;
			else
				return 2;
		}
		else
			return 3;
	}
	else
	{
		if(!((g+b)>255))
		{
			if(!((r+g+b)>255))
				return 4;
			else
				return 5;
		}
		else
		{
			return 6;
		}
	}
}









