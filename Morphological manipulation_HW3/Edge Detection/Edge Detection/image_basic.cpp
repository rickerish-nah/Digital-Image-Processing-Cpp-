//
//  image_basic.cpp
//  Edge Detection
//
//  Created by HariKrishna Prabhu on 3/19/18.
//  Copyright © 2018 HariKrishna Prabhu. All rights reserved.
//

#include "image_basic.hpp"
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include<cstdlib>
#include<ctime>
#include <math.h>
#include<string>
using namespace std;

image :: image() {}; //default constructor
image :: image(int x,int y,int z) //parameterized constructor
{
    //puts("Constructor");
    width=x;  //column
    height=y;  //rows
    BytesPerPixel=z;
}
image :: ~image() //destructor
{
    //printf("Destructor: Height:%d,\t Width:%d,\t BPP:%d\n",height,width,BytesPerPixel);
}
//functions
//1
unsigned char * image::DynamicMemoryAllocation(int height,int width,int BytesPerPixel)
{
    unsigned char * ImagePointer=new unsigned char [height*width*BytesPerPixel];
    return ImagePointer;
    
}
//2
unsigned char * image::FileRead( char *argv[],int index,int height,int width,int BytesPerPixel)
{
    FILE *file;
    unsigned char *Image=DynamicMemoryAllocation(height,width,BytesPerPixel);
    if (!(file=fopen(argv[index],"rb"))) {
        cout << "Cannot open file: " << argv[index] <<endl;
        exit(1);
    }
    fread(Image, sizeof(unsigned char), height*width*BytesPerPixel, file);
    fclose(file);
    return Image;
}
//3
void image::FileWrite( char *argv[],unsigned char *Image,int index,int height,int width,int BytesPerPixel)
{
    FILE *file;
    if (!(file=fopen(argv[index],"wb"))) {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }
    fwrite(Image, sizeof(unsigned char), height*width*BytesPerPixel, file);
    fclose(file);
}




int* image::convert_2_0_255(int* img,int height,int width,int min,int max)
{
    float *gx=new float[height*width];
    int *g=new int [10];
    //int x= 9/7;
    int mini=abs(min);
    int range = max + mini;
    //cout<<"Range:"<<range<<endl<<x<<endl;
    for(int i=0;i<height;i++)
    {
        for(int j=0;j<width;j++)
        {
            gx[(i*width)+j]=(img[(i*width)+j]+mini);
            gx[(i*width)+j]=gx[(i*width)+j]*255/range;
            gx[(i*width)+j]=round(gx[(i*width)+j]);
            g[(i*width)+j]=gx[(i*width)+j];
            //cout<<"@:"<<(i*width)+j<<"\tI:"<<img[(i*width)+j]<<"\tG:"<<g[(i*width)+j]<<endl;
        }
    }
    return g;
    
}
int *image:: min_max(int *image,int height,int width)
{
    int *mi_max=new int[2];
    int min=100;
    int max=0;
    for(int i=0;i<height;i++)
    {
        for(int j=0;j<width;j++)
        {
            //cout<<"Gx:@"<<(i*width)+j<<"\t="<<image[(i*width)+j]<<endl;
            if(image[(i*width)+j]<min)
                min=image[(i*width)+j];
            if(image[(i*width)+j]>max)
                max=image[(i*width)+j];
        }
    }
    mi_max[0]=min;
    mi_max[1]=max;
    //cout<<"MIN:"<<mi_max[0]<<"\t MAX:"<<mi_max[1]<<endl;
    //cout<<"MIN:"<<min<<"\t MAX:"<<max<<endl;
    return mi_max;
}
