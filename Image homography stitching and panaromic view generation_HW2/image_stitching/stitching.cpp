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
	int width = 480 , height = 640,i=0,j=0,j1=0,j2=0;
	int w2=4*width,h2=height+1000; 
	unsigned char *Imagedata_Left= new unsigned char [width*height*3];
	unsigned char *Imagedata_Middle= new unsigned char [width*height*3];
	unsigned char *Imagedata_Right= new unsigned char [width*height*3];
	unsigned char *Newdata_Left= new unsigned char [width*height*3];
	unsigned char *Newdata_Middle= new unsigned char [width*height*3];
	unsigned char *Newdata_Right= new unsigned char [width*height*3];
	int *flag= new int [width*height*3];
	unsigned char *Newdata= new unsigned char [(w2)*(h2)*3];
	float w22=0;
	double h1[3][3]={{0}},hh[3][3]={{0}};

	//LEFT::MIDDLE
	h1[0][0]=0.5035958997298;
	h1[0][1]=-0.0116069821699;
	h1[0][2]=-162.3952686517217;
	h1[1][0]= -0.1670364399071;
	h1[1][1]= 0.7019166034412;
	h1[1][2]=-65.6165589961529;
	h1[2][0]=-0.0005525895999;
	h1[2][1]= -0.0000098826942;
	h1[2][2]= 1.1820657827293;




	hh[0][0]=1.178072615006;
	hh[0][1]=0.010386917840;
	hh[0][2]=-1122.859708549953;
	hh[1][0]= 0.187658735801;
	hh[1][1]= 1.095391190198;
	hh[1][2]=-533.584464480850;
	hh[2][0]=0.000648912703;
	hh[2][1]= 0.000019366970;
	hh[2][2]= 0.377074448145;

	double xold[3]={0},yold=0,wold=0,xnew[3]={0};
	int cnewx=0,cnewy=0;
	//read images
	/*for(int i=0;i<3;i++)
	{
		for(int j=0;j<3;j++)
		{
			cout<<h1[i][j]<<'\t';
		}
		cout<<'\n';
	}*/

	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata_Left, sizeof(unsigned char), width*height*BytesPerPixel, file);
	fclose(file);
	
	
	if (!(file=fopen(argv[2],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata_Middle, sizeof(unsigned char), width*height*BytesPerPixel, file);
	fclose(file);

	if (!(file=fopen(argv[3],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata_Right, sizeof(unsigned char), width*height*BytesPerPixel, file);
	fclose(file);

	
	for(int rgb=0; rgb<3; rgb++)
	{
		for(i=300;i<(height+300); i++)	
		{
			for(j=260+width; j<((2*width)+260); j++)
			{

					Newdata[((i)*(w2)*BytesPerPixel)+(j*BytesPerPixel)+rgb]=Imagedata_Middle[((i-300)*(width)*BytesPerPixel)+((j-260-width)*BytesPerPixel)+rgb];

				
			}
		}
	}

	//left middle
	for( int rgb=0;rgb<3;rgb++)
	{
		for(int i=0;i<h2;i++)
		{
			for(int j=0;j<w2;j++)
			{

				xnew[0]=i;
				xnew[1]=j;
				xnew[2]=1;
				xold[0]= ((h1[0][0]*i)+(h1[0][1]*j)+(h1[0][2]));
				xold[1]= ((h1[1][0]*i)+(h1[1][1]*j)+(h1[1][2]));
				xold[2]= ((h1[2][0]*i)+(h1[2][1]*j)+(h1[2][2]));

				cnewx=(int)round(xold[0]/xold[2]);
				cnewy=(int)round(xold[1]/xold[2]);
				//cout<<cnewx<<"\t"<<cnewy<<"\t"<<xold[2]<<endl;
				if(cnewx>=0 && cnewx<width && cnewy>=0 && cnewy<height )
				{
					
						//cout<<"Entered Y:\n";
						//cout<<(cnewx)<<"\t"<<(cnewy)<<"\t"<<xold[2]<<endl;
					
					
					if(cnewx>=260+width && cnewx<((2*width)+260) && cnewy>=100 && cnewy<(height+100))
					{
						cout<<"Yes"<<endl;
						Newdata[((j)*(w2)*BytesPerPixel)+((i)*BytesPerPixel)+rgb]=(Imagedata_Left[((cnewy)*(width)*BytesPerPixel)+((cnewx)*BytesPerPixel)+rgb] + Imagedata_Left[((cnewy)*(width)*BytesPerPixel)+((cnewx)*BytesPerPixel)+rgb])/2;
					}
					else
					{
						Newdata[((j)*(w2)*BytesPerPixel)+((i)*BytesPerPixel)+rgb]=Imagedata_Left[((cnewy)*(width)*BytesPerPixel)+((cnewx)*BytesPerPixel)+rgb];
						flag[((cnewy)*(width)*BytesPerPixel)+((cnewx)*BytesPerPixel)+rgb]=1;
						Newdata_Right[((cnewy)*(width)*BytesPerPixel)+((cnewx)*BytesPerPixel)+rgb]=Imagedata_Left[((cnewy)*(width)*BytesPerPixel)+((cnewx)*BytesPerPixel)+rgb];
					}
						//}

					
				}
			}
		}
	}
	//right middle
	
	for( int rgb=0;rgb<3;rgb++)
	{
		for(int i=0;i<h2;i++)
		{
			for(int j=0;j<w2;j++)
			{

				xnew[0]=i;
				xnew[1]=j;
				xnew[2]=1;
				xold[0]= ((hh[0][0]*i)+(hh[0][1]*j)+(hh[0][2]));
				xold[1]= ((hh[1][0]*i)+(hh[1][1]*j)+(hh[1][2]));
				xold[2]= ((hh[2][0]*i)+(hh[2][1]*j)+(hh[2][2]));

				cnewx=(int)round(xold[0]/xold[2]);
				cnewy=(int)round(xold[1]/xold[2]);
				//cout<<cnewx<<"\t"<<cnewy<<"\t"<<xold[2]<<endl;
				if(cnewx>=0 && cnewx<width && cnewy>=0 && cnewy<height )
				{
					
						//cout<<"Entered Y:\n";
						//cout<<(cnewx)<<"\t"<<(cnewy)<<endl;
					//if(flag[((cnewy)*(width)*BytesPerPixel)+((cnewx)*BytesPerPixel)+rgb]==0)
					//if(i>=(2*width)+260)
						Newdata[((j)*(w2)*BytesPerPixel)+((i)*BytesPerPixel)+rgb]=Imagedata_Right[((cnewy)*(width)*BytesPerPixel)+((cnewx)*BytesPerPixel)+rgb];
						//fl//ag[((cnewy)*(width)*BytesPerPixel)+((cnewx)*BytesPerPixel)+rgb]=1;
						//Newdata_Right[((cnewy)*(width)*BytesPerPixel)+((cnewx)*BytesPerPixel)+rgb]=Imagedata_Left[((cnewy)*(width)*BytesPerPixel)+((cnewx)*BytesPerPixel)+rgb];
					

					
				}
			}
		}
	}
	
	
	//write images
	if (!(file=fopen(argv[4],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(Newdata, sizeof(unsigned char), (w2*h2)*BytesPerPixel, file);
	fclose(file);
	
	/*
	if (!(file=fopen(argv[5],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(Newdata_Middle, sizeof(unsigned char), width*height*BytesPerPixel, file);
	fclose(file);
	*/
	/*
	if (!(file=fopen(argv[4],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(Newdata_Right, sizeof(unsigned char), width*height*BytesPerPixel, file);
	fclose(file);*/
	return 0;
	
}



