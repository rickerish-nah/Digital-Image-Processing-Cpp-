#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include<math.h>



using namespace std;
unsigned char *floyd_stienberg(unsigned char *Image_data1, int height, int width, int bytesPerPixel);
unsigned char *JJN(unsigned char *Image_data2, int height, int width, int bytesPerPixel);
unsigned char *stucki(unsigned char *Image_data3, int height, int width, int bytesPerPixel);

int main(int argc, char *argv[])
{
	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel=1,N=2,width = 512 , height = 512,option=0,op=1;
	
	unsigned char *Imagedata= new unsigned char [width*height*BytesPerPixel];
	unsigned char *newimage_FS= new unsigned char [width*height*BytesPerPixel];
	unsigned char *newimage_JJN= new unsigned char [width*height*BytesPerPixel];
	unsigned char *newimage_ST= new unsigned char [width*height*BytesPerPixel];


	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), width*height*BytesPerPixel, file);
	fclose(file);

	cout<<"Choose a type of error diffusion:\n(1). Floyd Steinberg\n(2).Jarvis Judice and Ninke\n(3).Stucki\n";
	cin>>option;
	
	switch(option)
	{
		case 1:
		{
			newimage_FS=floyd_stienberg(Imagedata,height,width,BytesPerPixel);
			if (!(file=fopen(argv[2],"wb"))) {
			cout << "Cannot open file: " << argv[2] << endl;
			exit(1);
			}
			fwrite(newimage_FS, sizeof(unsigned char), width*height*BytesPerPixel, file);
			fclose(file);
			break;
		}
		case 2:
		{
			newimage_JJN=JJN(Imagedata,height,width,BytesPerPixel);
			if (!(file=fopen(argv[3],"wb"))) {
			cout << "Cannot open file: " << argv[2] << endl;
			exit(1);
			}
			fwrite(newimage_JJN, sizeof(unsigned char), width*height*BytesPerPixel, file);
			fclose(file);
			break;
		}
		case 3:
		{
			newimage_ST=stucki(Imagedata,height,width,BytesPerPixel);
			if (!(file=fopen(argv[4],"wb"))) {
			cout << "Cannot open file: " << argv[2] << endl;
			exit(1);
			}
			fwrite(newimage_ST, sizeof(unsigned char), width*height*BytesPerPixel, file);
			fclose(file);
			break;
		}

	}
	

	return 0;
}
//end of main


unsigned char *floyd_stienberg(unsigned char *newimage, int height, int width, int bytesPerPixel)
{
	float o=0,e=0;
	for(int i=0; i<height; i++)
	{
			if(i%2==0)
			{
				
				for(int j=0; j<width; j++)
				{
					o=newimage[( (i)*width )+ j ];
					if(o < 255-o)
					{
						newimage[ ( (i)*width )+ j ]=0;
						e=o;
					}
					else 
					{
						newimage[ ( (i)*width )+ j ]=255;
						e=o-255;
					}
					//
					if(j+1<width)
					{
						newimage[( (i)*width )+ (j+1)] += (unsigned char)(round)(e*7/16.0);
						newimage[( (i)*width )+ (j+1)] = (newimage[( (i)*width )+ (j+1)]>255)?255:newimage[( (i)*width )+ (j+1)];
						newimage[( (i)*width )+ (j+1)] = (newimage[( (i)*width )+ (j+1)]<0)?0:newimage[( (i)*width )+ (j+1)];
					}
					if(i+1<height && j > 0)
					{
					 	newimage[( (i+1)*width )+ (j-1)] += (unsigned char)(round)(e*3/16.0);
					 	newimage[( (i+1)*width )+ (j-1)]=(newimage[( (i+1)*width )+ (j-1)]>255)?255:newimage[( (i+1)*width )+ (j-1)];
					 	newimage[( (i+1)*width )+ (j-1)]=(newimage[( (i+1)*width )+ (j-1)]<0)?0:newimage[( (i+1)*width )+ (j-1)];

					}
					if(i+1<height)
					{
						newimage[( (i+1)*width )+ (j)] += (unsigned char)(round)(e*5/16.0);
					 	newimage[( (i+1)*width )+ (j)] = (newimage[( (i+1)*width )+ (j)]>255)?255:newimage[( (i+1)*width )+ (j)];
					 	newimage[( (i+1)*width )+ (j)] = (newimage[( (i+1)*width )+ (j)]<0)?0:newimage[( (i+1)*width )+ (j)];
					}
					if(i+1<height && j+1<width)
					{
						//cout<<"#"<<i<<":"<<j<<"\t i+1 : j+1"<<endl;
						newimage[( (i+1)*width )+ (j+1)] +=(unsigned char)(round) (e*1/16.0);
						newimage[( (i+1)*width )+ (j+1)] = (newimage[( (i+1)*width )+ (j+1)]>255)?255:newimage[( (i+1)*width )+ (j+1)];
					}
					

					
				}
			}
			
			else
			{
				
				for(int j=width-1; j>=0; j--)
				{
					
					//cout<<"#"<<i<<":"<<j<<"\tleft\n";
					//cout<<'\t'<<j<<endl;
				
					o=newimage[( (i)*width )+ j ];
					//cout<<"#"<<i<<":"<<j<<"o :"<<(int)o<<'\t';//<<endl;
					
					if(o < 255-o)
					{
						//cout<<"\t ohooooo";
						newimage[ ( (i)*width )+ j ]=0;
						e=o;
					}
					
					else 
					{
						//cout<<"\t ohhhhhh\t";
						newimage[ ( (i)*width )+ j ]=255;
						e=o-255;
					}
					
					//cout<<"o  new:"<<e<<endl;
					
					//e=Imagedata[( (i)*width )+ (j)]-newimage_Fixed[( (i)*width )+ (j)];
					if(j>0)
					{
						//cout<<"#"<<i<<":"<<j<<"\t i : j+1"<<endl;
						newimage[( (i)*width )+ (j-1)] = newimage[( (i)*width )+ (j-1)] + (unsigned char)(round)(e*7/16.0);
						newimage[( (i)*width )+ (j-1)] =(newimage[( (i)*width )+ (j-1)]>255)?255:newimage[( (i)*width )+ (j-1)];
						newimage[( (i)*width )+ (j-1)] =(newimage[( (i)*width )+ (j-1)]<0)?0:newimage[( (i)*width )+ (j-1)];
					}
					if(i+1<height && j > 0)
					{
						//cout<<"#"<<i<<":"<<j<<"\t i+1 : j-1"<<endl;
						//cout<<"#"<<i<<":"<<j<<"\t i+1 : j"<<endl;
					 	newimage[( (i+1)*width )+ (j-1)] = newimage[( (i+1)*width )+ (j-1)]+(unsigned char)(round)(e*1/16.0);
					 	newimage[( (i+1)*width )+ (j-1)] = (newimage[( (i+1)*width )+ (j-1)]>255)?255:newimage[( (i+1)*width )+ (j-1)];
					 	newimage[( (i+1)*width )+ (j-1)] = (newimage[( (i+1)*width )+ (j-1)]<0)?0:newimage[( (i+1)*width )+ (j-1)];

					}
					if(i+1<height)
					{
						newimage[( (i+1)*width )+ (j)] = newimage[( (i+1)*width )+ (j)]+ (unsigned char)(round)(e*5/16.0);
					 	newimage[( (i+1)*width )+ (j)] = (newimage[( (i+1)*width )+ (j)]>255)?255:newimage[( (i+1)*width )+ (j)];
					 	newimage[( (i+1)*width )+ (j)] = (newimage[( (i+1)*width )+ (j)]<0)?0:newimage[( (i+1)*width )+ (j)];
					}
					if(i+1<height && j>0)
					{
						//cout<<"#"<<i<<":"<<j<<"\t i+1 : j+1"<<endl;
						newimage[( (i+1)*width )+ (j-1)] = newimage[( (i+1)*width )+ (j-1)]+(unsigned char)(round)(e*1/16.0);
						newimage[( (i+1)*width )+ (j-1)] = (newimage[( (i+1)*width )+ (j-1)]>255)?255:newimage[( (i+1)*width )+ (j-1)];
						newimage[( (i+1)*width )+ (j-1)] = (newimage[( (i+1)*width )+ (j-1)]<0)?0:newimage[( (i+1)*width )+ (j-1)];
					}

				}
			}
	}
	return newimage;
}
unsigned char *JJN(unsigned char *newimage_FS, int height, int width, int bytesPerPixel)
{
	float o=0,e=0;
	for(int i=2; i<height-2; i++)
	{
		for(int j=2; j<width-2 ;j++)
		{
			o=newimage_FS[( (i)*width )+ j ];
			//cout<<"#"<<i<<":"<<j<<"o :"<<(int)o<<'\t';//<<endl;
					
			if(o < 255-o)
			{
				//cout<<"\t ohooooo";
				newimage_FS[ ( (i)*width )+ j ]=0;
				e=o;
			}
					
			else 
			{
				//cout<<"\t ohhhhhh\t";
				newimage_FS[ ( (i)*width )+ j ]=255;
				e=o-255;
			}
			/*
			if(j+1<width)
			{
				//cout<<"#"<<i<<":"<<j<<"\t i : j+1"<<endl;
				newimage_FS[( (i)*width )+ (j+1)] += (e*7/16.0);
			}
			if(i+1<height && j > 0)
			{
				//cout<<"#"<<i<<":"<<j<<"\t i+1 : j-1"<<endl;
				//cout<<"#"<<i<<":"<<j<<"\t i+1 : j"<<endl;
				newimage_FS[( (i+1)*width )+ (j-1)] += (e*3/16.0);
				newimage_FS[( (i+1)*width )+ (j)] += (e*5/16.0);
			}
			if(i+1<height && j+1<width)
			{
				//cout<<"#"<<i<<":"<<j<<"\t i+1 : j+1"<<endl;
				newimage_FS[( (i+1)*width )+ (j+1)] += (e*1/16.0);
			}
			*/
			//1
			newimage_FS[( (i)*width )+ (j+1)] = newimage_FS[( (i)*width )+ (j+1)] + (unsigned char)(round)(e*7/48.0);
			newimage_FS[( (i)*width )+ (j+1)] = (newimage_FS[( (i)*width )+ (j+1)]>255)?255:newimage_FS[( (i)*width )+ (j+1)];
			newimage_FS[( (i)*width )+ (j+1)] = (newimage_FS[( (i)*width )+ (j+1)]<0)?0:newimage_FS[( (i)*width )+ (j+1)];

			//2
			newimage_FS[( (i)*width )+ (j+2)] = newimage_FS[( (i)*width )+ (j+2)] + (unsigned char)(round)(e*5/48.0);
			newimage_FS[( (i)*width )+ (j+2)] = (newimage_FS[( (i)*width )+ (j+2)]>255)?255:newimage_FS[( (i)*width )+ (j+2)];
			newimage_FS[( (i)*width )+ (j+2)] = (newimage_FS[( (i)*width )+ (j+2)]<0)?0:newimage_FS[( (i)*width )+ (j+2)];


			//3
			newimage_FS[( (i+1)*width )+ (j-2)] = newimage_FS[( (i+1)*width )+ (j-2)] + (unsigned char)(round)(e*3/48.0);
			newimage_FS[( (i+1)*width )+ (j-2)] = (newimage_FS[( (i+1)*width )+ (j-2)]>255)?255:newimage_FS[( (i+1)*width )+ (j-2)];
			newimage_FS[( (i+1)*width )+ (j-2)] = (newimage_FS[( (i+1)*width )+ (j-2)]<0)?0:newimage_FS[( (i+1)*width )+ (j-2)];

			//4
			newimage_FS[( (i+1)*width )+ (j-1)] = newimage_FS[( (i+1)*width )+ (j-1)] + (unsigned char)(round)(e*5/48.0);
			newimage_FS[( (i+1)*width )+ (j-1)] = (newimage_FS[( (i+1)*width )+ (j-1)]>255)?255:newimage_FS[( (i+1)*width )+ (j-1)];
			newimage_FS[( (i+1)*width )+ (j-1)] = (newimage_FS[( (i+1)*width )+ (j-1)]<0)?0:newimage_FS[( (i+1)*width )+ (j-1)];

			//5
			newimage_FS[( (i+1)*width )+ (j)] = newimage_FS[( (i+1)*width )+ (j)] + (unsigned char)(round)(e*7/48.0);
			newimage_FS[( (i+1)*width )+ (j)] = (newimage_FS[( (i+1)*width )+ (j)]>255)?255:newimage_FS[( (i+1)*width )+ (j)];
			newimage_FS[( (i+1)*width )+ (j)] = (newimage_FS[( (i+1)*width )+ (j)]<0)?0:newimage_FS[( (i+1)*width )+ (j)];

			//6
			newimage_FS[( (i+1)*width )+ (j+1)] = newimage_FS[( (i+1)*width )+ (j+1)] + (unsigned char)(round)(e*5/48.0);
			newimage_FS[( (i+1)*width )+ (j+1)] = (newimage_FS[( (i+1)*width )+ (j+1)]>255)?255:newimage_FS[( (i+1)*width )+ (j+1)];
			newimage_FS[( (i+1)*width )+ (j+1)] = (newimage_FS[( (i+1)*width )+ (j+1)]<0)?0:newimage_FS[( (i+1)*width )+ (j+1)];

			//7
			newimage_FS[( (i+1)*width )+ (j+2)] = newimage_FS[( (i+1)*width )+ (j+2)] + (unsigned char)(round)(e*3/48.0);
			newimage_FS[( (i+1)*width )+ (j+2)] = (newimage_FS[( (i+1)*width )+ (j+2)]>255)?255:newimage_FS[( (i+1)*width )+ (j+2)];
			newimage_FS[( (i+1)*width )+ (j+2)] =(newimage_FS[( (i+1)*width )+ (j+2)]<0)?0:newimage_FS[( (i+1)*width )+ (j+2)];
			
			//8
			newimage_FS[( (i+2)*width )+ (j-2)] = newimage_FS[( (i+2)*width )+ (j-2)] + (unsigned char)(round)(e*1/48.0);
			newimage_FS[( (i+2)*width )+ (j-2)] = (newimage_FS[( (i+2)*width )+ (j-2)]>255)?255:newimage_FS[( (i+2)*width )+ (j-2)];
			newimage_FS[( (i+2)*width )+ (j-2)] = (newimage_FS[( (i+2)*width )+ (j-2)]<0)?0:newimage_FS[( (i+2)*width )+ (j-2)];


			//9
			newimage_FS[( (i+2)*width )+ (j-1)] = newimage_FS[( (i+2)*width )+ (j-1)] + (unsigned char)(round)(e*3/48.0);
			newimage_FS[( (i+2)*width )+ (j-1)] =(newimage_FS[( (i+2)*width )+ (j-1)]>255)?255:newimage_FS[( (i+2)*width )+ (j-1)];
			newimage_FS[( (i+2)*width )+ (j-1)] = (newimage_FS[( (i+2)*width )+ (j-1)]<0)?0:newimage_FS[( (i+2)*width )+ (j-1)];

			//10
			newimage_FS[( (i+2)*width )+ (j)] = newimage_FS[( (i+2)*width )+ (j)] + (unsigned char)(round)(e*5/48.0);
			newimage_FS[( (i+2)*width )+ (j)] = (newimage_FS[( (i+2)*width )+ (j)]>255)?255:newimage_FS[( (i+2)*width )+ (j)];
			newimage_FS[( (i+2)*width )+ (j)] =(newimage_FS[( (i+2)*width )+ (j)]<0)?0:newimage_FS[( (i+2)*width )+ (j)];

			//11
			newimage_FS[( (i+2)*width )+ (j+1)] = newimage_FS[( (i+2)*width )+ (j+1)] + (unsigned char)(round)(e*3/48.0);
			newimage_FS[( (i+2)*width )+ (j+1)] =(newimage_FS[( (i+2)*width )+ (j+1)]>255)?255:newimage_FS[( (i+2)*width )+ (j+1)];
			newimage_FS[( (i+2)*width )+ (j+1)] =(newimage_FS[( (i+2)*width )+ (j+1)]<0)?0:newimage_FS[( (i+2)*width )+ (j+1)];

			//12
			newimage_FS[( (i+2)*width )+ (j+2)] = newimage_FS[( (i+2)*width )+ (j+2)] + (unsigned char)(round)(e*1/48.0);
			newimage_FS[( (i+2)*width )+ (j+2)] = (newimage_FS[( (i+2)*width )+ (j+2)]>255)?255:newimage_FS[( (i+2)*width )+ (j+2)];
			newimage_FS[( (i+2)*width )+ (j+2)] = (newimage_FS[( (i+2)*width )+ (j+2)]<0)?0:newimage_FS[( (i+2)*width )+ (j+2)];

			
					

					
		}
	}	
	return newimage_FS;

}

unsigned char *stucki(unsigned char *newimage1, int height, int width, int bytesPerPixel)
{
	float o=0,e=0;
	for(int i=0; i<height-2; i++)
	{
		for(int j=2; j<width-2 ;j++)
		{
			o=newimage1[( (i)*width )+ j ];
			//cout<<"#"<<i<<":"<<j<<"o :"<<(int)o<<'\t';//<<endl;
					
			if(o < 255-o)
			{
				//cout<<"\t ohooooo";
				newimage1[ ( (i)*width )+ j ]=0;
				e=o;
			}
					
			else 
			{
				//cout<<"\t ohhhhhh\t";
				newimage1[ ( (i)*width )+ j ]=255;
				e=o-255;
			}
			/*
			if(j+1<width)
			{
				//cout<<"#"<<i<<":"<<j<<"\t i : j+1"<<endl;
				newimage_FS[( (i)*width )+ (j+1)] += (e*7/16.0);
			}
			if(i+1<height && j > 0)
			{
				//cout<<"#"<<i<<":"<<j<<"\t i+1 : j-1"<<endl;
				//cout<<"#"<<i<<":"<<j<<"\t i+1 : j"<<endl;
				newimage_FS[( (i+1)*width )+ (j-1)] += (e*3/16.0);
				newimage_FS[( (i+1)*width )+ (j)] += (e*5/16.0);
			}
			if(i+1<height && j+1<width)
			{
				//cout<<"#"<<i<<":"<<j<<"\t i+1 : j+1"<<endl;
				newimage_FS[( (i+1)*width )+ (j+1)] += (e*1/16.0);
			}
			*/
			//1
			newimage1[( (i)*width )+ (j+1)] = newimage1[( (i)*width )+ (j+1)] + (unsigned char)(round)(e*8/42.0);
			newimage1[( (i)*width )+ (j+1)] = (newimage1[( (i)*width )+ (j+1)]>255)?255:newimage1[( (i)*width )+ (j+1)];
			newimage1[( (i)*width )+ (j+1)] = (newimage1[( (i)*width )+ (j+1)]<0)?0:newimage1[( (i)*width )+ (j+1)];

			//2
			newimage1[( (i)*width )+ (j+2)] = newimage1[( (i)*width )+ (j+2)] + (unsigned char)(round)(e*4/42.0);
			newimage1[( (i)*width )+ (j+2)] = (newimage1[( (i)*width )+ (j+2)]>255)?255:newimage1[( (i)*width )+ (j+2)];
			newimage1[( (i)*width )+ (j+2)] = (newimage1[( (i)*width )+ (j+2)]<0)?0:newimage1[( (i)*width )+ (j+2)];

			//3
			newimage1[( (i+1)*width )+ (j-2)] = newimage1[( (i+1)*width )+ (j-2)] + (unsigned char)(round)(e*2/42.0);
			newimage1[( (i+1)*width )+ (j-2)] = (newimage1[( (i+1)*width )+ (j-2)]>255)?255:newimage1[( (i+1)*width )+ (j-2)];
			newimage1[( (i+1)*width )+ (j-2)] = (newimage1[( (i+1)*width )+ (j-2)]<0)?0:newimage1[( (i+1)*width )+ (j-2)];

			//4
			newimage1[( (i+1)*width )+ (j-1)] = newimage1[( (i+1)*width )+ (j-1)] + (unsigned char)(round)(e*4/42.0);
			newimage1[( (i+1)*width )+ (j-1)] = (newimage1[( (i+1)*width )+ (j-1)]>255)?255:newimage1[( (i+1)*width )+ (j-1)];
			newimage1[( (i+1)*width )+ (j-1)] = (newimage1[( (i+1)*width )+ (j-1)]<0)?0:newimage1[( (i+1)*width )+ (j-1)];

			//5
			newimage1[( (i+1)*width )+ (j)] = newimage1[( (i+1)*width )+ (j)] + (unsigned char)(round)(e*8/42.0);
			newimage1[( (i+1)*width )+ (j)] = (newimage1[( (i+1)*width )+ (j)]>255)?255:newimage1[( (i+1)*width )+ (j)];
			newimage1[( (i+1)*width )+ (j)] = (newimage1[( (i+1)*width )+ (j)]<0)?0:newimage1[( (i+1)*width )+ (j)];

			//6
			newimage1[( (i+1)*width )+ (j+1)] = newimage1[( (i+1)*width )+ (j+1)] + (unsigned char)(round)(e*4/42.0);
			newimage1[( (i+1)*width )+ (j+1)] = (newimage1[( (i+1)*width )+ (j+1)]>255)?255:newimage1[( (i+1)*width )+ (j+1)];
			newimage1[( (i+1)*width )+ (j+1)] = (newimage1[( (i+1)*width )+ (j+1)]<0)?0:newimage1[( (i+1)*width )+ (j+1)];

			//7
			newimage1[( (i+1)*width )+ (j+2)] = newimage1[( (i+1)*width )+ (j+2)] + (unsigned char)(round)(e*2/42.0);
			newimage1[( (i+1)*width )+ (j+2)] = (newimage1[( (i+1)*width )+ (j+2)]>255)?255:newimage1[( (i+1)*width )+ (j+2)];
			newimage1[( (i+1)*width )+ (j+2)] =(newimage1[( (i+1)*width )+ (j+2)]<0)?0:newimage1[( (i+1)*width )+ (j+2)];

			//8
			newimage1[( (i+2)*width )+ (j-2)] = newimage1[( (i+2)*width )+ (j-2)] + (unsigned char)(round)(e*1/42.0);
			newimage1[( (i+2)*width )+ (j-2)] = (newimage1[( (i+2)*width )+ (j-2)]>255)?255:newimage1[( (i+2)*width )+ (j-2)];
			newimage1[( (i+2)*width )+ (j-2)] = (newimage1[( (i+2)*width )+ (j-2)]<0)?0:newimage1[( (i+2)*width )+ (j-2)];

			//9
			newimage1[( (i+2)*width )+ (j-1)] = newimage1[( (i+2)*width )+ (j-1)] + (unsigned char)(round)(e*2/42.0);
			newimage1[( (i+2)*width )+ (j-1)] =(newimage1[( (i+2)*width )+ (j-1)]>255)?255:newimage1[( (i+2)*width )+ (j-1)];
			newimage1[( (i+2)*width )+ (j-1)] = (newimage1[( (i+2)*width )+ (j-1)]<0)?0:newimage1[( (i+2)*width )+ (j-1)];

			//10
			newimage1[( (i+2)*width )+ (j)] = newimage1[( (i+2)*width )+ (j)] + (unsigned char)(round)(e*4/42.0);	
			newimage1[( (i+2)*width )+ (j)] = (newimage1[( (i+2)*width )+ (j)]>255)?255:newimage1[( (i+2)*width )+ (j)];
			newimage1[( (i+2)*width )+ (j)] =(newimage1[( (i+2)*width )+ (j)]<0)?0:newimage1[( (i+2)*width )+ (j)];

			//11
			newimage1[( (i+2)*width )+ (j+1)] = newimage1[( (i+2)*width )+ (j+1)] + (unsigned char)(round)(e*2/42.0);
			newimage1[( (i+2)*width )+ (j+1)] =(newimage1[( (i+2)*width )+ (j+1)]>255)?255:newimage1[( (i+2)*width )+ (j+1)];
			newimage1[( (i+2)*width )+ (j+1)] =(newimage1[( (i+2)*width )+ (j+1)]<0)?0:newimage1[( (i+2)*width )+ (j+1)];

			//12
			newimage1[( (i+2)*width )+ (j+2)] = newimage1[( (i+2)*width )+ (j+2)] + (unsigned char)(round)(e*1/42.0);
			newimage1[( (i+2)*width )+ (j+2)] = (newimage1[( (i+2)*width )+ (j+2)]>255)?255:newimage1[( (i+2)*width )+ (j+2)];
			newimage1[( (i+2)*width )+ (j+2)] = (newimage1[( (i+2)*width )+ (j+2)]<0)?0:newimage1[( (i+2)*width )+ (j+2)];
			
					

					
		}
	}	
	return newimage1;

}