//
//  texture_classification.cpp
//  texture_Classification
//
//  Created by HariKrishna Prabhu on 3/15/18.
//  Copyright © 2018 HariKrishna Prabhu. All rights reserved.
//
#include "texture_classification.hpp"
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include<ctime>
#include <math.h>
#include <string>
#include <ctime>
using namespace std;
int g=0,g1=0;//for iteration

image :: image() {}; //default constructor
image :: image(int x,int y,int z) //parameterized constructor
{
    //puts("Constructor");
    height=y;
    width=x;
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
void image:: print_vec(float **f_vector,int height,int width,int BytesPerPixel,int f)
{
    cout<<"\tThe feature vectors are:"<<endl;

    for(int i=0;i<height;i++)
    {
        for(int j=0;j<width;j++)
        {
            cout<<"Pixel: "<<(i*width)+j;
            for(int h=0;h<f;h++)
            {
                cout<<"\t"<<f_vector[((i*width)+j)][h];
            }
            cout<<endl;
        }
    }
}
float **image:: convert_1D_2D(float * feature_vector, int height,int width,int h) 
{
    float** f_vector = new float*[height*width];
    for(int i = 0; i < height*width; ++i)
        f_vector[i] = new float[h];
    for(int i=0;i<height;i++)
    {
        for(int j=0;j<width;j++)
        {
            for(int f=0;f<h;f++)
            {
                f_vector[((i*width)+j)][f]=feature_vector[((i*width)+j)*h + f];
                //cout<<"#"<<((i*width)+j)<<"f:"<<f<<"\t:"<<f_vector[((i*width)+j)][f]<<endl;
            }
        }
    }
    
    
    return f_vector; 
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//filter for q1a
double image::filter_window5x5(unsigned char *image,int height,int width,int BytesPerPixel,int op)
{
    float *temp = new float [height*width*BytesPerPixel]; //declared because the function we've created is for unsigned char only.
    unsigned char image_arr[25];
    int x=0,p=0,q=0;
    double average=0.0;
    for(int rgb=0;rgb<BytesPerPixel;rgb++)
    {
        for (int i=0;i<height;i++)
        {
            for(int j=0;j<width;j++)
            {
                 x=0;
                for(int m=i-2;m<=i+2;m++)
                {
                    for(int n=j-2;n<=j+2;n++)
                    {
                        if(m<0)
                            p=abs(m);
                        else if(m>=height)
                        {
                            int pp=m-(height-1);
                            p=(height-1)-pp;
                        }
                        else
                            p=m;
                        if(n<0)
                            q=abs(n);
                        else if(n>=width)
                        {
                            int qq=n-(width-1);
                            q=(width-1)-qq;
                        }
                        else
                            q=n;
                        image_arr[x++]=image[((p*width)+q)*BytesPerPixel +rgb];
                    }
                }
                switch(op)
                {
                    case 1:
                    {
                        //cout<<"Filter 1\n";
                        float filter[25]={1.0/36,2.0/36,0,-2.0/36,-1.0/36,2.0/36,4.0/36,0,-4.0/36,-2.0/36,0,0,0,0,0,-2.0/36,-4.0/36,0,4.0/36,2.0/36,-1.0/36,-2.0/36,0,2.0/36,1.0/36};
                        temp[((i*width)+j)*BytesPerPixel +rgb] = feature_ext(image_arr, filter,1);
                        //for(int i=0;i<25;i++)
                        // cout<<filter[i]<<endl;
                        break;
                    }
                    case 2:
                    {
                        //cout<<"Filter 2\n";
                        float filter[25]={1.0/24,2.0/24,0,-2.0/24,-1.0/24,0,0,0,0,0,-2.0/24,-4.0/24,0,4.0/24,2.0/24,0,0,0,0,0,1.0/24,2.0/24,0,-2.0/24,-1.0/24};
                        temp[((i*width)+j)*BytesPerPixel +rgb] = feature_ext(image_arr, filter,1);
                        //for(int i=0;i<25;i++)
                        //cout<<filter[i]<<endl;
                        break;
                    }
                    case 3:
                    {
                        //cout<<"Filter 3\n";
                        float filter[25]={1.0/36,2.0/36,0,-2.0/36,-1.0/36,-2.0/36,-4.0/36,0,4.0/36,2.0/36,0,0,0,0,0,2.0/36,4.0/36,0,-4.0/36,-2.0/36,-1.0/36,-2.0/36,0,2.0/36,1.0/36};
                        temp[((i*width)+j)*BytesPerPixel +rgb] = feature_ext(image_arr, filter,1);
                        //for(int i=0;i<25;i++)
                        //cout<<filter[i]<<endl;
                        break;
                    }
                    case 4:
                    {
                        //cout<<"Filter 4\n";
                        float filter[25]={1.0/24,0,-2.0/24,0,1.0/24,2.0/24,0,-4.0/24,0,2.0/24,0,0,0,0,0,-2.0/24,0,4.0/24,0,-2.0/24,-1.0/24,0,2.0/24,0,-1.0/24};
                        temp[((i*width)+j)*BytesPerPixel +rgb] = feature_ext(image_arr, filter,1);
                        //for(int i=0;i<25;i++)
                        //cout<<filter[i]<<endl;
                        break;
                    }
                    case 5:
                    {
                        //cout<<"Filter 5\n";
                        float filter[25]={1.0/16,0,-2.0/16,0,1.0/16,0,0,0,0,0,-2.0/16,0,4.0/16,0,-2.0/16,0,0,0,0,0,1.0/16,0,-2.0/16,0,1.0/16};
                        temp[((i*width)+j)*BytesPerPixel +rgb] = feature_ext(image_arr, filter,1);
                        //for(int i=0;i<25;i++)
                        //cout<<filter[i]<<endl;
                        break;
                    }
                    case 6:
                    {
                        //cout<<"Filter 6\n";
                        float filter[25]={1.0/24,0,-2.0/24,0,1.0/24,-2.0/24,0,4.0/24,0,-2.0/24,0,0,0,0,0,2.0/24,0,-4.0/24,0,2.0/24,-1.0/24,0,2.0/24,0,-1.0/24};
                        temp[((i*width)+j)*BytesPerPixel +rgb] = feature_ext(image_arr, filter,1);
                        //for(int i=0;i<25;i++)
                        //cout<<filter[i]<<endl;
                        break;
                    }
                    case 7:
                    {
                        //cout<<"Filter 7\n";
                        float filter[25]={1.0/36,-2.0/36,0,2.0/36,-1.0/36,2.0/36,-4.0/36,0,4.0/36,-2.0/36,0,0,0,0,0,-2.0/36,4.0/36,0,-4.0/36,2.0/36,-1.0/36,2.0/36,0,-2.0/36,1.0/36};
                        temp[((i*width)+j)*BytesPerPixel +rgb] = feature_ext(image_arr, filter,1);
                        //for(int i=0;i<25;i++)
                        //cout<<filter[i]<<endl;
                        break;
                    }
                    case 8:
                    {
                        //cout<<"Filter 8\n";
                        float filter[25]={1.0/24,-2.0/24,0,2.0/24,-1.0/24,0,0,0,0,0,-2.0/24,4.0/24,0,-4.0/24,2.0/24,0,0,0,0,0,1.0/24,-2.0/24,0,2.0/24,-1.0/24};
                        temp[((i*width)+j)*BytesPerPixel +rgb] = feature_ext(image_arr, filter,1);
                        //for(int i=0;i<25;i++)
                        //cout<<filter[i]<<endl;
                        break;
                    }
                    case 9:
                    {
                        //cout<<"Filter 9\n";
                        float filter[25]={1.0/36,-2.0/36,0,2.0/36,-1.0/36,-2.0/36,4.0/36,0,-4.0/36,2.0/36,0,0,0,0,0,2.0/36,-4.0/36,0,4.0/36,-2.0/36,-1.0/36,2.0/36,0,-2.0/36,1.0/36};
                        temp[((i*width)+j)*BytesPerPixel +rgb] = feature_ext(image_arr, filter,1);
                        //for(int i=0;i<25;i++)
                        //cout<<filter[i]<<endl;
                        break;
                    }
                }
            }
        }
    }
    for(int rgb=0;rgb<BytesPerPixel;rgb++)
    {
        for(int i=0;i<height;i++)
        {
            for(int j=0;j<width;j++)
            {
                average+=pow(temp[((i*width)+j)*BytesPerPixel+(rgb)],2);
            }
        }
    }
    //cout<<"AVG:"<<average<<endl;
    average = average/(128*128);
    return average;
}
//pixel value calculation
double image:: feature_ext(unsigned char *image, float *filter,int op)
{
    double pixel_value=0.0;
    switch(op)
    {
        case 1:
        {
            for (int i=0;i<25;i++)
            {
                pixel_value = pixel_value + (image[i]*filter[i]);
            }
            // cout<<"Pixel_Value:"<<pixel_value<<endl;
            break;
        }
        case 2:
        {
            for (int i=0;i<9;i++)
            {
                pixel_value = pixel_value + (image[i]*filter[i]);
            }
            // cout<<"Pixel_Value:"<<pixel_value<<endl;
            break;
        }
    }
    return pixel_value;
}
//pixel energy calc for Q1b
float image:: pixel_energy(float * temp,int height,int width,int i, int j,int ByterPerPixel,int w,int f)
{
    int p=0,q=0;
    float average=0.0;
    
    for(int m=i-w;m<=i+w;m++)
    {
        for(int n=j-w;n<=j+w;n++)
        {
            if(m<0)
                p=abs(m);
            else if(m>=height)
            {
                int pp=m-(height-1);
                p=(height-1)-pp;
            }
            else
                p=m;
            if(n<0)
                q=abs(n);
            else if(n>=width)
            {
                int qq=n-(width-1);
                q=(width-1)-qq;
            }
            else
                q=n;
            average = average + pow(temp[((p*width)+q)*ByterPerPixel],2);
            //cout<<"I:"<<p<<"\tJ:"<<q<<"\tAvg:"<<average<<endl;
        }
    }
    average = average/pow(((2*w)+1),2);
    //cout<<"I:"<<i<<"\tJ:"<<j<<"\tF:"<<f<<"\tAvg:"<<average<<endl;
    return average;
}

//filter- segmentation Q1b////////////////////////////////////////////////////////B///////////////
float *image::filter_segment(unsigned char *image,int height,int width,int BytesPerPixel,int op)
{
    float *temp = new float [height*width*BytesPerPixel];
    unsigned char image_arr[9]={0};
    int x=0,p=0,q=0;
    for(int rgb=0;rgb<BytesPerPixel;rgb++)
    {
        for (int i=0;i<height;i++)
        {
            for(int j=0;j<width;j++)
            {
                x=0;
                for(int m=i-1;m<=i+1;m++)
                {
                    for(int n=j-1;n<=j+1;n++)
                    {
                        if(m<0)
                            p=abs(m);
                        else if(m>=height)
                        {
                            int pp=m-(height-1);
                            p=(height-1)-pp;
                        }
                        else
                            p=m;
                        if(n<0)
                            q=abs(n);
                        else if(n>=width)
                        {
                            int qq=n-(width-1);
                            q=(width-1)-qq;
                        }
                        else
                            q=n;
                        //cout<<"I:"<<p<<"\tJ:"<<q<<endl; 
                        image_arr[x++]=image[((p*width)+q)*BytesPerPixel +rgb];
                    }
                }
                switch(op)
                {
                    case 1:
                    {
                        //cout<<"Filter 1\n";
                        //float filter[25]={1.0/36,2.0/36,0,-2.0/36,-1.0/36,2.0/36,4.0/36,0,-4.0/36,-2.0/36,0,0,0,0,0,-2.0/36,-4.0/36,0,4.0/36,2.0/36,-1.0/36,-2.0/36,0,2.0/36,1.0/36};
                        float filter[9]={1.0/4,0,-1.0/4,0,0,0,-1.0/4,0,1.0/4}; //EE
                        temp[((i*width)+j)*BytesPerPixel +rgb] = feature_ext(image_arr, filter,2);
                        //for(int i=0;i<25;i++)
                        // cout<<filter[i]<<endl;
                        break;
                    }
                    case 2:
                    {
                        //cout<<"Filter 2\n";
                        //float filter[25]={1.0/24,2.0/24,0,-2.0/24,-1.0/24,0,0,0,0,0,-2.0/24,-4.0/24,0,4.0/24,2.0/24,0,0,0,0,0,1.0/24,2.0/24,0,-2.0/24,-1.0/24};
                        float filter[9]={1.0/4,0,-1.0/4,-2.0/4,0,2.0/4,1.0/4,0,-1.0/4}; //ES
                        temp[((i*width)+j)*BytesPerPixel +rgb] = feature_ext(image_arr, filter,2);
                        //for(int i=0;i<25;i++)
                        //cout<<filter[i]<<endl;
                        break;
                    }
                    case 3:
                    {
                        //cout<<"Filter 3\n";
                        //float filter[25]={1.0/36,2.0/36,0,-2.0/36,-1.0/36,-2.0/36,-4.0/36,0,4.0/36,2.0/36,0,0,0,0,0,2.0/36,4.0/36,0,-4.0/36,-2.0/36,-1.0/36,-2.0/36,0,2.0/36,1.0/36};
                        float filter[9]={-1.0/12,0,1.0/12,-2.0/12,0,2.0/12,-1.0/12,0,1.0/12}; //EL
                        temp[((i*width)+j)*BytesPerPixel +rgb] = feature_ext(image_arr, filter,2);
                        //for(int i=0;i<25;i++)
                        //cout<<filter[i]<<endl;
                        break;
                    }
                    case 4:
                    {
                        //cout<<"Filter 4\n";
                        //float filter[25]={1.0/24,0,-2.0/24,0,1.0/24,2.0/24,0,-4.0/24,0,2.0/24,0,0,0,0,0,-2.0/24,0,4.0/24,0,-2.0/24,-1.0/24,0,2.0/24,0,-1.0/24};
                        float filter[9]={1.0/4,-2.0/4,-1.0/4,0,0,0,-1.0/4,2.0/4,1.0/4};  //SE
                        temp[((i*width)+j)*BytesPerPixel +rgb] = feature_ext(image_arr, filter,2);
                        //for(int i=0;i<25;i++)
                        //cout<<filter[i]<<endl;
                        break;
                    }
                    case 5:
                    {
                        //cout<<"Filter 5\n";
                        //float filter[25]={1.0/16,0,-2.0/16,0,1.0/16,0,0,0,0,0,-2.0/16,0,4.0/16,0,-2.0/16,0,0,0,0,0,1.0/16,0,-2.0/16,0,1.0/16};
                        float filter[9]={1.0/4,-2.0/4,-1.0/4,-2.0/4,4.0/4,2.0/4,-1.0/4,2.0/4,1.0/4}; //SS
                        temp[((i*width)+j)*BytesPerPixel +rgb] = feature_ext(image_arr, filter,2);
                        //for(int i=0;i<25;i++)
                        //cout<<filter[i]<<endl;
                        break;
                    }
                    case 6:
                    {
                        //cout<<"Filter 6\n";
                        //float filter[25]={1.0/24,0,-2.0/24,0,1.0/24,-2.0/24,0,4.0/24,0,-2.0/24,0,0,0,0,0,2.0/24,0,-4.0/24,0,2.0/24,-1.0/24,0,2.0/24,0,-1.0/24};
                        float filter[9]={1.0/12,-2.0/12,-1.0/12,-2.0/12,4.0/12,2.0/12,-1.0/12,2.0/12,1.0/12}; //SL
                        temp[((i*width)+j)*BytesPerPixel +rgb] = feature_ext(image_arr, filter,2);
                        //for(int i=0;i<25;i++)
                        //cout<<filter[i]<<endl;
                        break;
                    }
                    case 7:
                    {
                        //cout<<"Filter 7\n";
                        //float filter[25]={1.0/36,-2.0/36,0,2.0/36,-1.0/36,2.0/36,-4.0/36,0,4.0/36,-2.0/36,0,0,0,0,0,-2.0/36,4.0/36,0,-4.0/36,2.0/36,-1.0/36,2.0/36,0,-2.0/36,1.0/36};
                        float filter[9]={-1.0/12,-2.0/12,-1.0/12,0,0,0,1.0/12,2.0/12,1.0/12}; //LE
                        temp[((i*width)+j)*BytesPerPixel +rgb] = feature_ext(image_arr, filter,2);
                        //for(int i=0;i<25;i++)
                        //cout<<filter[i]<<endl;
                        break;
                    }
                    case 8:
                    {
                        //cout<<"Filter 8\n";
                        //float filter[25]={1.0/24,-2.0/24,0,2.0/24,-1.0/24,0,0,0,0,0,-2.0/24,4.0/24,0,-4.0/24,2.0/24,0,0,0,0,0,1.0/24,-2.0/24,0,2.0/24,-1.0/24};
                        float filter[9]={-1.0/12,-2.0/12,-1.0/12,2.0/12,4.0/12,2.0/12,-1.0/12,-2.0/12,-1.0/12}; //LS
                        temp[((i*width)+j)*BytesPerPixel +rgb] = feature_ext(image_arr, filter,2);
                        //for(int i=0;i<25;i++)
                        //cout<<filter[i]<<endl;
                        break;
                    }
                    case 9:
                    {
                        //cout<<"Filter 9\n";
                        //float filter[25]={1.0/36,-2.0/36,0,2.0/36,-1.0/36,-2.0/36,4.0/36,0,-4.0/36,2.0/36,0,0,0,0,0,2.0/36,-4.0/36,0,4.0/36,-2.0/36,-1.0/36,2.0/36,0,-2.0/36,1.0/36};
                        float filter[9]={1.0/36,2.0/36,1.0/36,2.0/36,4.0/36,2.0/36,-1.0/36,-2.0/36,-1.0/36}; //LL
                        temp[((i*width)+j)*BytesPerPixel +rgb] = feature_ext(image_arr, filter,2);
                        //for(int i=0;i<25;i++)
                        //cout<<filter[i]<<endl;
                        break;
                    }
                }
            }
        }
    }
    return temp;
    
}

unsigned char *image ::filter_segment_image(unsigned char *image,int height,int width,int BytesPerPixel,int op)
{
    unsigned char *temp = DynamicMemoryAllocation(height,width,BytesPerPixel);
 unsigned char image_arr[9]={0};
 int x=0,p=0,q=0;
 for(int rgb=0;rgb<BytesPerPixel;rgb++)
 {
 for (int i=0;i<height;i++)
 {
 for(int j=0;j<width;j++)
 {
 x=0;
 for(int m=i-1;m<=i+1;m++)
 {
 for(int n=j-1;n<=j+1;n++)
 {
 if(m<0)
 p=abs(m);
 else if(m>=height)
 {
 int pp=m-(height-1);
 p=(height-1)-pp;
 }
 else
 p=m;
 if(n<0)
 q=abs(n);
 else if(n>=width)
 {
 int qq=n-(width-1);
 q=(width-1)-qq;
 }
 else
 q=n;
 //cout<<"I:"<<p<<"\tJ:"<<q<<endl;
 image_arr[x++]=image[((p*width)+q)*BytesPerPixel +rgb];
 }
 }
 switch(op)
 {
 case 1:
 {
 //cout<<"Filter 1\n";
 //float filter[25]={1.0/36,2.0/36,0,-2.0/36,-1.0/36,2.0/36,4.0/36,0,-4.0/36,-2.0/36,0,0,0,0,0,-2.0/36,-4.0/36,0,4.0/36,2.0/36,-1.0/36,-2.0/36,0,2.0/36,1.0/36};
 float filter[9]={1.0/4,0,1.0/4,0,0,0,-1.0/4,0,1.0/4};
 temp[((i*width)+j)*BytesPerPixel +rgb] = feature_ext(image_arr, filter,2);
 //for(int i=0;i<25;i++)
 // cout<<filter[i]<<endl;
 break;
 }
 case 2:
 {
 //cout<<"Filter 2\n";
 //float filter[25]={1.0/24,2.0/24,0,-2.0/24,-1.0/24,0,0,0,0,0,-2.0/24,-4.0/24,0,4.0/24,2.0/24,0,0,0,0,0,1.0/24,2.0/24,0,-2.0/24,-1.0/24};
 float filter[9]={1.0/4,0,-1.0/4,-2.0/4,0,2.0/4,1.0/4,0,-1.0/4};
 temp[((i*width)+j)*BytesPerPixel +rgb] = feature_ext(image_arr, filter,2);
 //for(int i=0;i<25;i++)
 //cout<<filter[i]<<endl;
 break;
 }
 case 3:
 {
 //cout<<"Filter 3\n";
 //float filter[25]={1.0/36,2.0/36,0,-2.0/36,-1.0/36,-2.0/36,-4.0/36,0,4.0/36,2.0/36,0,0,0,0,0,2.0/36,4.0/36,0,-4.0/36,-2.0/36,-1.0/36,-2.0/36,0,2.0/36,1.0/36};
 float filter[9]={-1.0/12,0,1.0/12,-2.0/12,0,2.0/12,-1.0/12,0,1.0/12};
 temp[((i*width)+j)*BytesPerPixel +rgb] = feature_ext(image_arr, filter,2);
 //for(int i=0;i<25;i++)
 //cout<<filter[i]<<endl;
 break;
 }
 case 4:
 {
 //cout<<"Filter 4\n";
 //float filter[25]={1.0/24,0,-2.0/24,0,1.0/24,2.0/24,0,-4.0/24,0,2.0/24,0,0,0,0,0,-2.0/24,0,4.0/24,0,-2.0/24,-1.0/24,0,2.0/24,0,-1.0/24};
 float filter[9]={1.0/4,-2.0/4,-1.0/4,0,0,0,-1.0/4,2.0/4,1.0/4};
 temp[((i*width)+j)*BytesPerPixel +rgb] = feature_ext(image_arr, filter,2);
 //for(int i=0;i<25;i++)
 //cout<<filter[i]<<endl;
 break;
 }
 case 5:
 {
 //cout<<"Filter 5\n";
 //float filter[25]={1.0/16,0,-2.0/16,0,1.0/16,0,0,0,0,0,-2.0/16,0,4.0/16,0,-2.0/16,0,0,0,0,0,1.0/16,0,-2.0/16,0,1.0/16};
 float filter[9]={1.0/4,-2.0/4,-1.0/4,-2.0/4,4.0/4,2.0/4,-1.0/4,2.0/4,1.0/4};
 temp[((i*width)+j)*BytesPerPixel +rgb] = feature_ext(image_arr, filter,2);
 //for(int i=0;i<25;i++)
 //cout<<filter[i]<<endl;
 break;
 }
 case 6:
 {
 //cout<<"Filter 6\n";
 //float filter[25]={1.0/24,0,-2.0/24,0,1.0/24,-2.0/24,0,4.0/24,0,-2.0/24,0,0,0,0,0,2.0/24,0,-4.0/24,0,2.0/24,-1.0/24,0,2.0/24,0,-1.0/24};
 float filter[9]={1.0/12,-2.0/12,-1.0/12,-2.0/12,4.0/12,2.0/12,-1.0/12,2.0/12,1.0/12};
 temp[((i*width)+j)*BytesPerPixel +rgb] = feature_ext(image_arr, filter,2);
 //for(int i=0;i<25;i++)
 //cout<<filter[i]<<endl;
 break;
 }
 case 7:
 {
 //cout<<"Filter 7\n";
 //float filter[25]={1.0/36,-2.0/36,0,2.0/36,-1.0/36,2.0/36,-4.0/36,0,4.0/36,-2.0/36,0,0,0,0,0,-2.0/36,4.0/36,0,-4.0/36,2.0/36,-1.0/36,2.0/36,0,-2.0/36,1.0/36};
 float filter[9]={-1.0/12,-2.0/12,-1.0/12,0,0,0,1.0/12,2.0/12,1.0/12};
 temp[((i*width)+j)*BytesPerPixel +rgb] = feature_ext(image_arr, filter,2);
 //for(int i=0;i<25;i++)
 //cout<<filter[i]<<endl;
 break;
 }
 case 8:
 {
 //cout<<"Filter 8\n";
 //float filter[25]={1.0/24,-2.0/24,0,2.0/24,-1.0/24,0,0,0,0,0,-2.0/24,4.0/24,0,-4.0/24,2.0/24,0,0,0,0,0,1.0/24,-2.0/24,0,2.0/24,-1.0/24};
 float filter[9]={-1.0/12,-2.0/12,-1.0/12,2.0/12,4.0/12,2.0/12,-1.0/12,-2.0/12,-1.0/12};
 temp[((i*width)+j)*BytesPerPixel +rgb] = feature_ext(image_arr, filter,2);
 //for(int i=0;i<25;i++)
 //cout<<filter[i]<<endl;
 break;
 }
 case 9:
 {
 //cout<<"Filter 9\n";
 //float filter[25]={1.0/36,-2.0/36,0,2.0/36,-1.0/36,-2.0/36,4.0/36,0,-4.0/36,2.0/36,0,0,0,0,0,2.0/36,-4.0/36,0,4.0/36,-2.0/36,-1.0/36,2.0/36,0,-2.0/36,1.0/36};
 float filter[9]={1.0/36,2.0/36,1.0/36,2.0/36,4.0/36,2.0/36,-1.0/36,-2.0/36,-1.0/36};
 temp[((i*width)+j)*BytesPerPixel +rgb] = feature_ext(image_arr, filter,2);
 //for(int i=0;i<25;i++)
 //cout<<filter[i]<<endl;
 break;
 }
 }
 }
 }
 }
 return temp;
}
 





/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//k means clustring algorithm:
void image::k_cluster(int im,int f,double feature_vector[im][9])
{
    int type[12]={0},c[4][9]={{0}},t=0;
    /*srand(time(0));
    for(int i=0;i<9;i++)
    {
        c[0][i]=rand()%10+1;
        c[1][i]=rand()%10+1;
        c[2][i]=rand()%10+1;
        c[3][i]=rand()%10+1;
    }*/
    for(int j=0;j<9;j++)
    {
        c[0][j]=feature_vector[0][j];
        c[1][j]=feature_vector[2][j];
        c[2][j]=feature_vector[4][j];
        c[3][j]=feature_vector[6][j];
    }
    /*for(int i=0;i<9;i++)
        cout<<"c1:"<<c[0][i]<<"\tc2:"<<c[1][i]<<"\tc3:"<<c[2][i]<<"\tc4:"<<c[3][i]<<endl;
    cout<<endl;*/
    t=algo(type,c,feature_vector,im);
}
int image::algo(int type[12],int c[4][9],double feature_vector[12][9],int im)
{
    cout<<"Iteration:"<<++g<<endl;
    int type1[12]={0};
    double feature[9]={0};
    int check=0;
    float dist1,dist2,dist3,dist4;
    int t=0;
    int c1=0,c2=0,c3=0,c4=0;
    float tot[4][9]={0};
    for(int i=0;i<im;i++)
    {
        for(int j=0;j<9;j++)
        {
            feature[j]=feature_vector[i][j];
        }
        dist1=dist(feature,c[0]);
        dist2=dist(feature,c[1]);
        dist3=dist(feature,c[2]);
        dist4=dist(feature,c[3]);
        
        t=cluster_type(dist1,dist2,dist3,dist4);
        type1[i]=t;
        
        //cout<<"D1:"<<dist1<<"\tD2:"<<dist2<<"\tD3:"<<dist3<<"\tD4:"<<dist4<<"\tType:"<<type1[i];//<<endl;
        cout<<"\tImg:"<<i+1<<"=(k)"<<type1[i];
    }
    cout<<endl;
    //finding the mean for the centroid
    if(check==0)
    {
        for(int i=0;i<im;i++)
       {
           if(type1[i]==1)
           {
               for(int j=0;j<9;j++)
               {
                   tot[0][j]+=feature_vector[i][j];
               }
               c1++;
           }
           else if(type1[i]==2)
           {
               for(int j=0;j<9;j++)
               {
                   tot[1][j]+=feature_vector[i][j];
               }
               c2++;
           }
           else if(type1[i]==3)
           {
               for(int j=0;j<9;j++)
               {
                   tot[2][j]+=feature_vector[i][j];
               }
               c3++;
           }
           else if(type1[i]==4)
           {
               for(int j=0;j<9;j++)
               {
                   tot[3][j]+=feature_vector[i][j];
               }
               c4++;
           }
       }
    }
    for(int j=0;j<9;j++)
    {
        tot[0][j]=tot[0][j]/c1 ;
        tot[1][j]=tot[1][j]/c2 ;
        tot[2][j]=tot[2][j]/c3 ;
        tot[3][j]=tot[3][j]/c4 ;
    }
    //cout<<"cc1:"<<c1<<"\tcc2:"<<c2<<"\tcc3:"<<c3<<"\tcc4:"<<c4<<endl;
    //cout<<endl;
    /*
    for(int j=0;j<9;j++)
    {
        if(tot[0][j]==c[0][j] && tot[1][j]==c[2][j] && tot[2][j]==c[2][j] && tot[3][j]==c[3][j])
        {
            check=1;
            cout<<"yipeeeeeeeeeeeeeeeeeeee"<<endl;
        }
        else
        {
            check=0;
            break;
        }
    }*/
    for(int j=0;j<9;j++)
    {
        if(type1[j]!=type[j])
        {
            check = 0;
            break;
        }
        else
            check=1;
    }
    //update centroid:
    //1
    if(c1!=0)
    {
        for(int j=0;j<9;j++)
        {
            c[0][j]=tot[0][j];
        }
    }
    //2
    if(c2!=0)
    {
        for(int j=0;j<9;j++)
        {
            c[1][j]=tot[1][j];
        }
    }
    //3
    if(c3!=0)
    {
        for(int j=0;j<9;j++)
        {
            c[2][j]=tot[2][j];
        }
    }
    //4
    if(c4!=0)
    {
        for(int j=0;j<9;j++)
        {
            c[3][j]=tot[3][j];
        }
    }
    for(int i=0;i<9;i++)
    {
        //cout<<"c1:"<<c[0][i]<<"\tc2:"<<c[1][i]<<"\tc3:"<<c[2][i]<<"\tc4:"<<c[3][i]<<endl;
    }
    //cout<<"\tcheck:"<<check<<endl;
    for(int i=0;i<im;i++)
    {
        type[i]=type1[i];
    }
    if(check==0)
    {
        t=algo(type,c,feature_vector,im);
    }
    return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void image::k_means_cluster(int height,int width,int f,float ** f_vector,char *argv[])
{
    int t=0;
    int *type=new int [height*width];
    int **c= new int *[6];
    for(int i=0;i<6;i++)
        c[i]=new int [9];
    /*float **c= new float *[6];
    for(int i=0;i<6;i++)
        c[i]=new float [9];*/
    srand(time(NULL));
    int i1=rand()%(height*width),i2=rand()%(height*width),i3=rand()%(height*width),i4=rand()%(height*width),i5=rand()%(height*width),i6=rand()%(height*width);
    for(int j=0;j<f;j++)
    {
        /*c[0][j]=f_vector[100][j];
        c[1][j]=f_vector[300][j];
        c[2][j]=f_vector[500][j];
        c[3][j]=f_vector[138300][j];
        c[4][j]=f_vector[269500][j];
        c[5][j]=f_vector[269900][j];*/
        c[0][j]=f_vector[i1][j];
        c[1][j]=f_vector[i2][j];
        c[2][j]=f_vector[i3][j];
        c[3][j]=f_vector[i4][j];
        c[4][j]=f_vector[i5][j];
        c[5][j]=f_vector[i6][j];
        
    }
    
        t=algo_2(type,c,f_vector,height,width,argv,f);
}

int image::algo_2(int *type,int **c,float **f_vector,int height,int width,char *argv[],int ff)
{
    //cout<<"Iteration:"<<++g1<<endl;
    int *type1=new int[height*width];

    ///////////////////////CAUTION To CHANGE before K_MEANS
    //float feature[10]={0};
    //float tot[6][10]={0};
    float *feature=new float[10];
    float** tot = new float*[6]; //using feature vector for k-means calculation
    for(int i = 0; i < 6; ++i)
        tot[i] = new float[10];
    
    
    int check=0;
    float dist1,dist2,dist3,dist4,dist5,dist6;
    int t=0;
    //long
    int c1=0,c2=0,c3=0,c4=0,c5=0,c6=0;
    
    for(int i=0;i<height;i++)
    {
        for(int j=0;j<width;j++)
        {
            for(int f=0;f<ff;f++)
            {
                feature[f]=f_vector[(i*width)+j][f];
            }
            //cout<<"THe check::\n\n\n\n\n\n";
            for(int f=0;f<ff;f++)
            {
                //cout<<"\t"<<feature[f];
            }
            //cout<<endl;
            dist1=dist_2(feature,c[0],ff);
            dist2=dist_2(feature,c[1],ff);
            dist3=dist_2(feature,c[2],ff);
            dist4=dist_2(feature,c[3],ff);
            dist5=dist_2(feature,c[4],ff);
            dist6=dist_2(feature,c[5],ff);
            
            t=cluster_type_2(dist1,dist2,dist3,dist4,dist5,dist6);
            type1[(i*width)+j]=t;
            
            //cout<<"D1:"<<dist1<<"\tD2:"<<dist2<<"\tD3:"<<dist3<<"\tD4:"<<dist4<<"\tD5:"<<dist5<<"\tD6:"<<dist6<<"\tType:"<<type1[i];
            //cout<<"\tImg:"<<(i*width)+j<<"=(k)"<<type1[i]<<endl;
            
        }
    }
    //cout<<endl;
    /*for(int i=0;i<9;i++)
        cout<<"c1:"<<c[0][i]<<"\tc2:"<<c[1][i]<<"\tc3:"<<c[2][i]<<"\tc4:"<<c[3][i]<<endl;
    cout<<endl;*/
    //computing new centroid
    for(int i=0;i<height;i++)
    {
        for(int j=0;j<width;j++)
        {
            if(type1[(i*width)+j]==1)
            {
                for(int f=0;f<ff;f++)
                {
                    tot[0][f]=tot[0][f]+f_vector[(i*width)+j][f];
                }
                c1++;
            }
            else if(type1[(i*width)+j]==2)
            {
                for(int f=0;f<9;f++)
                {
                    tot[1][f]=tot[1][f]+f_vector[(i*width)+j][f];
                }
                c2++;
            }
            else if(type1[(i*width)+j]==3)
            {
                for(int f=0;f<9;f++)
                {
                    tot[2][f]=tot[2][f]+f_vector[(i*width)+j][f];
                }
                c3++;
            }
            else if(type1[(i*width)+j]==4)
            {
                for(int f=0;f<9;f++)
                {
                    tot[3][f]=tot[3][f]+f_vector[(i*width)+j][f];
                }
                c4++;
            }
            else if(type1[(i*width)+j]==5)
            {
                for(int f=0;f<9;f++)
                {
                    tot[4][f]=tot[4][f]+f_vector[(i*width)+j][f];
                }
                c5++;
            }
            else if(type1[(i*width)+j]==6)
            {
                for(int f=0;f<9;f++)
                {
                    tot[5][f]=tot[5][f]+f_vector[(i*width)+j][f];
                }
                c6++;
            }
        }
    }
    for(int j=0;j<ff;j++) //the new centroid values
    {
        tot[0][j]=tot[0][j]/(c1) ;
        tot[1][j]=tot[1][j]/(c2) ;
        tot[2][j]=tot[2][j]/(c3) ;
        tot[3][j]=tot[3][j]/(c4) ;
        tot[4][j]=tot[4][j]/(c5) ;
        tot[5][j]=tot[5][j]/(c6) ;
    }
    //cout<<"cc1:"<<c1<<"\tcc2:"<<c2<<"\tcc3:"<<c3<<"\tcc4:"<<c4<<"\tcc5:"<<c5<<"\tcc6:"<<c6<<endl;
    //cout<<endl;
    
    for(int j=0;j<height*width;j++)
    {
        if(type1[j]!=type[j])
        {
            check = 0;
            break;
        }
        else
            check=1;
    }
    /*for(int j=0;j<9;j++)
    {
        if(tot[0][j]==c[0][j] && tot[1][j]==c[2][j] && tot[2][j]==c[2][j] && tot[3][j]==c[3][j])
        {
            check=1;
            cout<<"yipeeeeeeeeeeeeeeeeeeee"<<endl;
        }
        else
        {
            check=0;
            break;
        }
    }*/
    
    //update centroid:
    //1
    if(c1!=0)
    {
        for(int j=0;j<9;j++)
        {
            c[0][j]=tot[0][j];
        }
    }
    //2
    if(c2!=0)
    {
        for(int j=0;j<9;j++)
        {
            c[1][j]=tot[1][j];
        }
    }
    //3
    if(c3!=0)
    {
        for(int j=0;j<9;j++)
        {
            c[2][j]=tot[2][j];
        }
    }
    //4
    if(c4!=0)
    {
        for(int j=0;j<9;j++)
        {
            c[3][j]=tot[3][j];
        }
    }
    //5
    if(c5!=0)
    {
        for(int j=0;j<9;j++)
        {
            c[4][j]=tot[4][j];
        }
    }
    //6
    if(c6!=0)
    {
        for(int j=0;j<9;j++)
        {
            c[5][j]=tot[5][j];
        }
    }
    for(int i=0;i<ff;i++)
    {
        //cout<<"c1:"<<c[0][i]<<"\tc2:"<<c[1][i]<<"\tc3:"<<c[2][i]<<"\tc4:"<<c[3][i]<<c[4][i]<<c[5][i]<<endl;
    }
    for(int i=0;i<height*width;i++)
    {
        type[i]=type1[i];
    }
    
    if(check==0)
        t=algo_2(type,c,f_vector,height,width,argv,ff);
    else
    {
        unsigned char * image = new unsigned char [height*width*1];
        for(int i=0;i<height;i++)
        {
            for(int j=0;j<width;j++)
            {
                if(type[(i*width)+j]==1)
                {
                    image[(i*width)+j]=0;
                }
                else if(type[(i*width)+j]==2)
                {
                   image[(i*width)+j]=51;
                }
                else if(type[(i*width)+j]==3)
                {
                    image[(i*width)+j]=102;
                }
                else if(type[(i*width)+j]==4)
                {
                    image[(i*width)+j]=153;
                }
                else if(type[(i*width)+j]==5)
                {
                    image[(i*width)+j]=204;
                }
                else if(type[(i*width)+j]==6)
                {
                    image[(i*width)+j]=255;
                }
            }
        }
        FileWrite(argv,image,2,height,width,1);
        
    }

    //for(int i=0;i<40;i++)
        //t=algo_2(type,c,f_vector,height,width);
    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
float image::dist(double feature[9], int c[9])
{
    /*for(int i=0;i<9;i++)
    {
        cout<<"\tC"<<i<<"\t:"<<c[i];
    }*/
    //cout<<endl;
    float distance=0.0,d=0.0;
    for(int i=0;i<9;i++)
    {
        d=pow( (feature[i]-c[i]) , 2 );
        distance+=d;
    }
    return sqrt(distance);
}
//part 2
float image::dist_2(float *feature,int *c,int f)
{
    /*for(int i=0;i<9;i++)
     {
     cout<<"\tC"<<i<<"\t:"<<c[i];
     }*/
    //cout<<endl;
    float distance=0.0,d=0.0;
    for(int i=0;i<f;i++)
    {
        d=pow( (feature[i]-c[i]) , 2 );
        distance+=d;
    }
    return sqrt(distance);
}
//part 1
int image::cluster_type(float d1 , float d2, float d3, float d4)
{
    int ret=0;
    if(d1<d2 && d1<d3 && d1<d4)
        ret=1;
    else if(d2<d1 && d2<d3 && d2<d4)
        ret=2;
    else if(d3<d1 && d3<d2 && d3<d4)
        ret=3;
    else if(d4<d1 && d4<d2 && d4<d3)
        ret=4;
    else
    {
        srand(time(0));
        ret=rand()%4+1;
    }
    return ret;
}
//part2
int image::cluster_type_2(float d1,float d2,float d3,float d4,float d5,float d6)
{
    int ret=0;
    if(d1<d2 && d1<d3 && d1<d4 && d1<d5 && d1<d6)
        ret=1;
    else if(d2<d1 && d2<d3 && d2<d4 && d2<d5 && d2<d6)
        ret=2;
    else if(d3<d1 && d3<d2 && d3<d4 && d3<d5 && d3<d6)
        ret=3;
    else if(d4<d1 && d4<d2 && d4<d3 && d4<d5 && d4<d6)
        ret=4;
    else if(d5<d1 && d5<d2 && d5<d3 && d5<d4 && d5<d6)
        ret=5;
    else if(d6<d1 && d6<d2 && d6<d3 && d6<d4 && d6<d5)
        ret=6;
    else
    {
        srand(time(0));
        ret=rand()%6+1;
    }
    //cout<<"Type:"<<ret<<endl;
    return ret;
}
///////////////////////////////////////////////////////////////////////////////////////

float *image::filter_segment_PCA(unsigned char *image,int height,int width,int BytesPerPixel,int op)
{
    float *temp = new float [height*width*BytesPerPixel];
    cout<<"OP:"<<op<<endl;
    
    unsigned char image_arr[25]={0};
    int x=0,p=0,q=0;
    for(int rgb=0;rgb<BytesPerPixel;rgb++)
    {
        for (int i=0;i<height;i++)
        {
            for(int j=0;j<width;j++)
            {
                x=0;
                for(int m=i-1;m<=i+1;m++)
                {
                    for(int n=j-1;n<=j+1;n++)
                    {
                        if(m<0)
                            p=abs(m);
                        else if(m>=height)
                        {
                            int pp=m-(height-1);
                            p=(height-1)-pp;
                        }
                        else
                            p=m;
                        if(n<0)
                            q=abs(n);
                        else if(n>=width)
                        {
                            int qq=n-(width-1);
                            q=(width-1)-qq;
                        }
                        else
                            q=n;
                        //cout<<"I:"<<p<<"\tJ:"<<q<<endl;
                        image_arr[x++]=image[((p*width)+q)*BytesPerPixel +rgb];
                    }
                }
                //cout<<"i:"<<(i*width)+j<<endl;
                switch(op)
                {
                    case 1:
                    {
                        //cout<<"\ncase1:"<<endl;
                        float f1[5]={1.0/16,4.0/16,6.0/16,4.0/16,1.0/16}; //L5
                        float f2[5]={1.0/16,4.0/16,6.0/16,4.0/16,1.0/16}; //L5
                        float *filter=new float [25];
                        filter= matrix(f1, f2);
                        temp[((i*width)+j)*BytesPerPixel +rgb] = feature_ext(image_arr, filter,2);
                        //for(int i=0;i<25;i++)
                        //cout<<filter[i]<<endl;
                        break;
                    }
                    case 2:
                    {
                        //cout<<"\ncase2:"<<endl;
                        float f1[5]={1.0/16,4.0/16,6.0/16,4.0/16,1.0/16};  //L5
                        float f2[5]={-1.0/6,-2.0/6,0,2.0/6,1.0/6};  //E5
                        float *filter=new float [25];
                        filter= matrix(f1, f2);
                        temp[((i*width)+j)*BytesPerPixel +rgb] = feature_ext(image_arr, filter,2);
                        //for(int i=0;i<25;i++)
                        //cout<<filter[i]<<endl;
                        break;
                    }
                    case 3:
                    {
                        //cout<<"\ncase3:"<<endl;
                        float f1[5]={1.0/16,4.0/16,6.0/16,4.0/16,1.0/16}; //L5
                        float f2[5]={-1.0/4,0,2.0/4,0,-1.0/4}; //S5
                        float *filter=new float [25];
                        filter= matrix(f1, f2);
                        temp[((i*width)+j)*BytesPerPixel +rgb] = feature_ext(image_arr, filter,2);
                        //for(int i=0;i<25;i++)
                        //cout<<filter[i]<<endl;
                        break;
                    }
                    case 4:
                    {
                        //cout<<"\ncase:"<<op<<endl;

                        float f1[5]={1.0/16,4.0/16,6.0/16,4.0/16,1.0/16}; //L5
                        float f2[5]={-1.0/6,2.0/6,0,-2.0/6,1.0/6};  //W5
                        float *filter=new float [25];
                        filter= matrix(f1, f2);
                        temp[((i*width)+j)*BytesPerPixel +rgb] = feature_ext(image_arr, filter,2);
                        //for(int i=0;i<25;i++)
                        //cout<<filter[i]<<endl;
                        break;
                    }
                    case 5:
                    {
                        //cout<<"\ncase:"<<op<<endl;
                        //cout<<"Filter 5\n";
                        float f1[5]={1.0/16,4.0/16,6.0/16,4.0/16,1.0/16}; //L5
                        float f2[5]={1.0/16,-4.0/16,6.0/16,-4.0/16,1.0/16}; //R5
                        float *filter=new float [25];
                        filter= matrix(f1, f2);
                        temp[((i*width)+j)*BytesPerPixel +rgb] = feature_ext(image_arr, filter,2);
                        //for(int i=0;i<25;i++)
                        //cout<<filter[i]<<endl;
                        break;
                    }
                    case 6:
                    {
                        //cout<<"\ncase:"<<op<<endl;
                        float f1[5]={-1.0/6,-2.0/6,0,2.0/6,1.0/6}; //E5
                        float f2[5]{1.0/16,4.0/16,6.0/16,4.0/16,1.0/16};  //L5
                        //cout<<"Filter 6\n";
                        
                        float *filter=new float [25];
                        filter= matrix(f1, f2);
                        temp[((i*width)+j)*BytesPerPixel +rgb] = feature_ext(image_arr, filter,2);
                        //for(int i=0;i<25;i++)
                        //cout<<filter[i]<<endl;
                        break;
                    }
                    case 7:
                    {
                        //cout<<"\ncase:"<<op<<endl;
                        //cout<<"Filter 7\n";
                        
                        float f1[5]={-1.0/6,-2.0/6,0,2.0/6,1.0/6}; //E5
                        float f2[5]={-1.0/6,-2.0/6,0,2.0/6,1.0/6}; //E5
                        float *filter=new float [25];
                        filter= matrix(f1, f2);
                
                        temp[((i*width)+j)*BytesPerPixel +rgb] = feature_ext(image_arr, filter,2);
                        //for(int i=0;i<25;i++)
                        //cout<<filter[i]<<endl;
                        break;
                    }
                    case 8:
                    {
                        //cout<<"\ncase:"<<op<<endl;
                        //cout<<"Filter 8\n";
                        float f1[5]={-1.0/6,-2.0/6,0,2.0/6,1.0/6}; //E5
                        float f2[5]={-1.0/4,0,2.0/4,0,-1.0/4}; //S5
                        float *filter=new float [25];
                        filter= matrix(f1, f2);
                        temp[((i*width)+j)*BytesPerPixel +rgb] = feature_ext(image_arr, filter,2);
                        //for(int i=0;i<25;i++)
                        //cout<<filter[i]<<endl;
                        break;
                    }
                    case 9:
                    {
                        //cout<<"\ncase:"<<op<<endl;
                        float f1[5]={-1.0/6,-2.0/6,0,2.0/6,1.0/6}; //E5
                        float f2[5]={-1.0/6,2.0/6,0,-2.0/6,1.0/6};  //W5
                        float *filter=new float [25];
                        filter= matrix(f1, f2);
                        
                        temp[((i*width)+j)*BytesPerPixel +rgb] = feature_ext(image_arr, filter,2);
                        //for(int i=0;i<25;i++)
                        //cout<<filter[i]<<endl;
                        break;
                    }
                    case 10:
                    {
                        //cout<<"\ncase:"<<op<<endl;
                        float f1[5]={-1.0/6,-2.0/6,0,2.0/6,1.0/6};  //E5
                        float f2[5]={1.0/16,-4.0/16,6.0/16,-4.0/16,1.0/16}; //R5
                        float *filter=new float [25];
                        filter= matrix(f1, f2);
                        
                        temp[((i*width)+j)*BytesPerPixel +rgb] = feature_ext(image_arr, filter,2);
                        //for(int i=0;i<25;i++)
                        //cout<<filter[i]<<endl;
                        break;
                    }
                    case 11:
                    {
                        //cout<<"\ncase:"<<op<<endl;
                        float f1[5]={-1.0/4,0,2.0/4,0,-1.0/4}; //S5
                        float f2[5]={1.0/16,4.0/16,6.0/16,4.0/16,1.0/16}; //L5
                        float *filter=new float [25];
                        filter= matrix(f1, f2);
                        
                        temp[((i*width)+j)*BytesPerPixel +rgb] = feature_ext(image_arr, filter,2);
                        //for(int i=0;i<25;i++)
                        //cout<<filter[i]<<endl;
                        break;
                    }
                    case 12:
                    {
                        //cout<<"\ncase:"<<op<<endl;
                        float f1[5]={-1.0/4,0,2.0/4,0,-1.0/4}; //S5
                        float f2[5]={-1.0/6,-2.0/6,0,2.0/6,1.0/6};  //E5
                        float *filter=new float [25];
                        filter= matrix(f1, f2);
                        
                        temp[((i*width)+j)*BytesPerPixel +rgb] = feature_ext(image_arr, filter,2);
                        //for(int i=0;i<25;i++)
                        //cout<<filter[i]<<endl;
                        break;
                    }
                    case 13:
                    {
                        //cout<<"\ncase:"<<op<<endl;
                        float f1[5]={-1.0/4,0,2.0/4,0,-1.0/4}; //S5
                        float f2[5]={-1.0/4,0,2.0/4,0,-1.0/4}; //S5
                        float *filter=new float [25];
                        filter= matrix(f1, f2);
                        
                        temp[((i*width)+j)*BytesPerPixel +rgb] = feature_ext(image_arr, filter,2);
                        //for(int i=0;i<25;i++)
                        //cout<<filter[i]<<endl;
                        break;
                    }
                    case 14:
                    {
                        //cout<<"\ncase:"<<op<<endl;
                        float f1[5]={-1.0/4,0,2.0/4,0,-1.0/4}; //S5
                        float f2[5]={-1.0/6,2.0/6,0,-2.0/6,1.0/6};  //W5
                        float *filter=new float [25];
                        filter= matrix(f1, f2);
                        
                        temp[((i*width)+j)*BytesPerPixel +rgb] = feature_ext(image_arr, filter,2);
                        //for(int i=0;i<25;i++)
                        //cout<<filter[i]<<endl;
                        break;
                    }
                    case 15:
                    {
                        //cout<<"\ncase:"<<op<<endl;
                        float f1[5]={-1.0/4,0,2.0/4,0,-1.0/4}; //S5
                        float f2[5]={1.0/16,-4.0/16,6.0/16,-4.0/16,1.0/16}; //R5
                        float *filter=new float [25];
                        filter= matrix(f1, f2);
                        
                        temp[((i*width)+j)*BytesPerPixel +rgb] = feature_ext(image_arr, filter,2);
                        //for(int i=0;i<25;i++)
                        //cout<<filter[i]<<endl;
                        break;
                    }
                    case 16:
                    {
                        //cout<<"\ncase:"<<op<<endl;
                        float f1[5]={-1.0/6,2.0/6,0,-2.0/6,1.0/6};  //W5
                        float f2[5]={1.0/16,4.0/16,6.0/16,4.0/16,1.0/16}; //L5
                        float *filter=new float [25];
                        filter= matrix(f1, f2);
                        
                        temp[((i*width)+j)*BytesPerPixel +rgb] = feature_ext(image_arr, filter,2);
                        //for(int i=0;i<25;i++)
                        //cout<<filter[i]<<endl;
                        break;
                    }
                    case 17:
                    {
                        //cout<<"\ncase:"<<op<<endl;
                        float f1[5]={-1.0/6,2.0/6,0,-2.0/6,1.0/6};  //W5
                        float f2[5]={-1.0/6,-2.0/6,0,2.0/6,1.0/6};  //E5
                        float *filter=new float [25];
                        filter= matrix(f1, f2);
                        
                        temp[((i*width)+j)*BytesPerPixel +rgb] = feature_ext(image_arr, filter,2);
                        //for(int i=0;i<25;i++)
                        //cout<<filter[i]<<endl;
                        break;
                    }
                    case 18:
                    {
                        //cout<<"\ncase:"<<op<<endl;
                        float f1[5]={-1.0/6,2.0/6,0,-2.0/6,1.0/6};  //W5
                        float f2[5]={-1.0/4,0,2.0/4,0,-1.0/4}; //S5
                        float *filter=new float [25];
                        filter= matrix(f1, f2);
                        
                        temp[((i*width)+j)*BytesPerPixel +rgb] = feature_ext(image_arr, filter,2);
                        //for(int i=0;i<25;i++)
                        //cout<<filter[i]<<endl;
                        break;
                    }
                    case 19:
                    {
                        //cout<<"\ncase:"<<op<<endl;
                        float f1[5]={-1.0/6,2.0/6,0,-2.0/6,1.0/6};  //W5
                        float f2[5]={-1.0/6,2.0/6,0,-2.0/6,1.0/6};  //W5
                        float *filter=new float [25];
                        filter= matrix(f1, f2);
                        
                        temp[((i*width)+j)*BytesPerPixel +rgb] = feature_ext(image_arr, filter,2);
                        //for(int i=0;i<25;i++)
                        //cout<<filter[i]<<endl;
                        break;
                    }
                    case 20:
                    {
                        //cout<<"\ncase:"<<op<<endl;
                        float f1[5]={-1.0/6,2.0/6,0,-2.0/6,1.0/6};  //W5
                        float f2[5]={1.0/16,-4.0/16,6.0/16,-4.0/16,1.0/16}; //R5
                        float *filter=new float [25];
                        filter= matrix(f1, f2);
                        
                        temp[((i*width)+j)*BytesPerPixel +rgb] = feature_ext(image_arr, filter,2);
                        //for(int i=0;i<25;i++)
                        //cout<<filter[i]<<endl;
                        break;
                    }
                    case 21:
                    {
                        //cout<<"\ncase:"<<op<<endl;
                        float f1[5]={1.0/16,-4.0/16,6.0/16,-4.0/16,1.0/16}; //R5
                        float f2[5]={1.0/16,4.0/16,6.0/16,4.0/16,1.0/16}; //L5
                        float *filter=new float [25];
                        filter= matrix(f1, f2);
                        
                        temp[((i*width)+j)*BytesPerPixel +rgb] = feature_ext(image_arr, filter,2);
                        //for(int i=0;i<25;i++)
                        //cout<<filter[i]<<endl;
                        break;
                    }
                    case 22:
                    {
                        //cout<<"\ncase:"<<op<<endl;
                        float f1[5]={1.0/16,-4.0/16,6.0/16,-4.0/16,1.0/16}; //R5
                        float f2[5]={-1.0/6,-2.0/6,0,2.0/6,1.0/6};  //E5
                        float *filter=new float [25];
                        filter= matrix(f1, f2);
                        
                        temp[((i*width)+j)*BytesPerPixel +rgb] = feature_ext(image_arr, filter,2);
                        //for(int i=0;i<25;i++)
                        //cout<<filter[i]<<endl;
                        break;
                    }
                    case 23:
                    {
                        //cout<<"\ncase:"<<op<<endl;
                        float f1[5]={1.0/16,-4.0/16,6.0/16,-4.0/16,1.0/16}; //R5
                        float f2[5]={-1.0/4,0,2.0/4,0,-1.0/4}; //S5
                        float *filter=new float [25];
                        filter= matrix(f1, f2);
                        
                        temp[((i*width)+j)*BytesPerPixel +rgb] = feature_ext(image_arr, filter,2);
                        //for(int i=0;i<25;i++)
                        //cout<<filter[i]<<endl;
                        break;
                    }
                    case 24:
                    {
                        //cout<<"\ncase:"<<op<<endl;
                        float f1[5]={1.0/16,-4.0/16,6.0/16,-4.0/16,1.0/16}; //R5
                        float f2[5]={-1.0/6,2.0/6,0,-2.0/6,1.0/6};  //W5
                        float *filter=new float [25];
                        filter= matrix(f1, f2);
                        
                        temp[((i*width)+j)*BytesPerPixel +rgb] = feature_ext(image_arr, filter,2);
                        //for(int i=0;i<25;i++)
                        //cout<<filter[i]<<endl;
                        break;
                    }
                    case 25:
                    {
                        //cout<<"\ncase:"<<op<<endl;
                        float f1[5]={1.0/16,-4.0/16,6.0/16,-4.0/16,1.0/16}; //R5
                        float f2[5]={1.0/16,-4.0/16,6.0/16,-4.0/16,1.0/16}; //R5
                        float *filter=new float [25];
                        filter= matrix(f1, f2);
                        temp[((i*width)+j)*BytesPerPixel +rgb] = feature_ext(image_arr, filter,2);
                        //for(int i=0;i<25;i++)
                        //cout<<filter[i]<<endl;
                        break;
                    }
                }
            }
        }
    }
    return temp;
    
}

float *image::matrix(float f1[5], float f2[5])
{
    float * filter=new float [25];
    for(int i=0;i<5;i++)
    {
        for(int j=0;j<5;j++)
        {
            filter[(5*i)+j]=f1[i]*f2[j];
            //cout<<"a:"<<filter[(5*i)+j]<<"\t";
        }
        //cout<<endl;
    }
    return filter;
    
}

