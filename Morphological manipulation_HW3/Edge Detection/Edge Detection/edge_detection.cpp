//
//  sobel.cpp
//  Edge Detection
//
//  Created by HariKrishna Prabhu on 3/19/18.
//  Copyright © 2018 HariKrishna Prabhu. All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>
#include<iostream>
#include <memory.h>
#include<math.h>
#include "edge_detection.hpp"
#include "image_basic.hpp"
using namespace std;


/*
float** gx = new float*[ob[i].height];  //using feature vector for k-means calculation
for(int i = 0; i < ob[i].height; ++i)
gx[i] = new float[ob[i].width];

float** gy = new float*[ob[i].height];   //using feature vector for k-means calculation
for(int i = 0; i < ob[i].height; ++i)
gy[i] = new float[ob[i].width];
*/


unsigned char * sobel::sobel_edge_detector(unsigned char * image,int height, int width,int bpp,char *argv[])
{
    //float gx=0,gy=0,g;
    float g;
    float *gx=new float [height*width];
    float *gy=new float [height*width];
    int *Gx=new int [height*width];
    int *Gy=new int [height*width];
    unsigned char *G=image::DynamicMemoryAllocation(height,width,bpp);
    unsigned char *gradient=image::DynamicMemoryAllocation(height,width,bpp);
    int p=0,q=0;
    unsigned char image_arr[9]={0};
    for (int i=0;i<height;i++)
    {
        for(int j=0;j<width;j++)
        {
            int x=0;
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
                    image_arr[x++]=image[((p*width)+q)*bpp];
                }
            }
            gx[(i*width)+j]=horizontal_filter(image_arr, height, width);
            gy[(i*width)+j]=vertical_filter(image_arr, height, width);
            g = pow(gx[(i*width)+j],2)+pow(gy[(i*width)+j],2);
            g=sqrt(g);
            G[(i*width)+j]=(unsigned char)g;
            //cout<<"G:"<<G[(i*width)+j]<<endl;
            Gx[(i*width)+j]=(int)round( gx[(i*width)+j]);
            Gy[(i*width)+j]=(int)round( gy[(i*width)+j]);
        }
    }
    //uncomment them while printing the X and Y gradients
    
    gradient=draw(Gy,height,width,bpp,argv,0);
    //draw(Gy,height,width,bpp,argv,index+1);
    //for the x and y gradient: 7-B gx, 8- B gy, 9- BN gx, 10-BN gy
    FileWrite(argv,gradient,10,height,width,bpp);
   
    //edge thresholding
    
    int cdf_threshold=sobel::cdf_threshold(G,height,width);
    G = sobel::threshold(G,cdf_threshold,height,width);
    
    return G;
}
unsigned char * sobel::draw(int *gx,int height,int width,int bpp,char* argv[],int index)
{
    unsigned char *g = DynamicMemoryAllocation(height,width,bpp);
    int * img=new int[height*width];
    int *m_M = new int [2];
    m_M=min_max(gx,height,width);
    img=convert_2_0_255(gx, height, width, m_M[0], m_M[1]);
    for(int i=0;i<height;i++)
    {
        for(int j=0;j<width;j++)
        {
            g[(i*width)+j]=(unsigned char)img[(i*width)+j];
        }
    }
    return g;
   
}

unsigned char * sobel::threshold(unsigned char * image,int threshold,int height,int width)
{
    for(int i=0;i<height;i++)
    {
        for(int j=0;j<width;j++)
        {
            if(image[(i*width)+j]>=threshold)
                image[(i*width)+j]=0;
            else
                image[(i*width)+j]=255;
        }
    }
    return image;
}
int sobel::cdf_threshold(unsigned char * image,int height,int width)
{
    int threshold=0;
    int r[256]={0};
    
    for(int k=0;k<256;k++)
    {
        int R=0;
        for(int i=0;i<height;i++)
        {
            for(int j=0;j<width;j++)
            {
                if(image[(i*width)+j] == k)
                {
                    r[k]=++R;
                }
            }
        }
        //cout<<r[k]<<endl;
    }
    //CDF calculation
    float cdf_r[255]={0};
    cdf_r[0]=r[0];
    for(int i=1;i<256;i++)
    {
        cdf_r[i]=((cdf_r[i-1] + r[i]));
        //cout<<"cdf_R at "<<i<<" : "<<cdf_r[i]<<endl;
    }
    for(int i =0; i<256 ; i++)
    {
        //cout<<"cdf_R at "<<i<<" : "<<cdf_r[i]<<endl;
        cdf_r[i]=(cdf_r[i]/(height*width))*255;
        cdf_r[i]=(int) cdf_r[i];
        //cout<<"Normalised_cdf_R at "<<i<<" : "<<cdf_r[i]<<endl;
    }
    float p=0.70;
    int k=(int)(height*width)*p;
    int index=0;
    k=(k*255)/(width*height);
    
    for(int i=0;i<256;i++)
    {
        if(cdf_r[i]>k)
        {
            index=i;
            break;
        }
        
    }
    //cout<<"K="<<k<<"threshold:"<<cdf_r[index]<<endl;
    threshold=index+1;
    cout<<"For:"<<100*p<<"%\n";
    cout<<"Threshold="<<threshold<<endl;

    return threshold;
    //return 200;
}

float sobel::horizontal_filter(unsigned char* Image,int height,int width)
{
    float avg=0;
    int filter[9]={-1,0,1,-2,0,2,-1,0,1};
    for(int i=0;i<9;i++)
    {
        avg+=(Image[i]*filter[i]);
    }
    return avg;
    
}

float sobel::vertical_filter(unsigned char* Image,int height,int width)
{
    float avg=0;
    int filter[9]={1,2,1,0,0,0,-1,-2,-1};
    for(int i=0;i<9;i++)
    {
        avg+=(Image[i]*filter[i]);
    }
    return avg;
}
unsigned char * sobel::color_2_greyscale(unsigned char * color_image,int height,int width,int bpp)
{
    unsigned char *grey_image=image::DynamicMemoryAllocation(height,width,1);
    //luminosity type
    for(int i=0;i<height;i++)
    {
        for(int j=0;j<width;j++)
        {
            grey_image[(i*width)+j]=((color_image[((i*width)+j)*3 + 0]*0.21)+(color_image[((i*width)+j)*3 + 1]*0.72)+(color_image[((i*width)+j)*3 + 2]*0.07));
        }
    }
    
    return grey_image;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char * LoG::LoG_filter(unsigned char * image,int height, int width,int bpp)
{
    int *mi_max=new int[2];
    int *mi_max2=new int[2];

    
    int *gx=new int[height*width];
    int *gy=new int[height*width];
    
    //int r=0;
    int p=0,q=0;
    
    int *knee_pts=new int[2];
    //int *cdf=new int[255];
    //int *pdf_val=new  int[255];
    
    unsigned char image_arr[81]={0};
    unsigned char *G=image::DynamicMemoryAllocation(height,width,bpp);
    unsigned char *ternary_map=image::DynamicMemoryAllocation(height,width,bpp);
    int *bin=new int [height*width];

    for (int i=0;i<height;i++)
    {
        for(int j=0;j<width;j++)
        {
            int x=0;
            for(int m=i-4;m<=i+4;m++)
            {
                for(int n=j-4;n<=j+4;n++)
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
                    image_arr[x++]=image[((p*width)+q)*bpp];
                }
            }
            gx[(i*width)+j]=LoG_algo(image_arr, height, width);
            //cout<<"Gx:@"<<(i*width)+j<<"\t="<<gx[(i*width)+j]<<endl;
            //cout<<gx[(i*width)+j]<<endl; //pour l'histogram
          
        }
    }
    
    
    //cout<<"Done"<<endl;
    mi_max=min_max(gx,height,width);
    //cout<<"MIN:"<<mi_max[0]<<"\t MAX:"<<mi_max[1]<<endl;
    
    gy=convert_2_0_255(gx,height,width,mi_max[0],mi_max[1]);
    
    mi_max2=min_max(gy,height,width);
    //cout<<"MIN:"<<mi_max2[0]<<"\t MAX:"<<mi_max2[1]<<endl;

    //knee_pts=knee_points(gy,height,width,mi_max2[0],mi_max2[1]);

    knee_pts[0]=135;
    knee_pts[1]=145;
    bin=ranger_1_0_1(gy,height,width,knee_pts[0],knee_pts[1]);
    
     //to create ternary map
    for(int i=0;i<height;i++)
    {
        for(int j=0;j<width;j++)
        {
            if(bin[(i*width)+j]==-1)
                ternary_map[(i*width)+j]=64;
            else if (bin[(i*width)+j]==0)
                ternary_map[(i*width)+j]=128;
            else if (bin[(i*width)+j]==1)
                ternary_map[(i*width)+j]=192;
        }
    }
    G=make_edge(bin,height,width);
    
    return ternary_map;
    //return G;
}




unsigned char* LoG::make_edge(int* image,int height,int width)
{
    unsigned char *G=image::DynamicMemoryAllocation(height,width,1);
    int x=0;
    unsigned char image_arr[9]={0};
    int p,q;
    for(int i=0;i<height;i++)
    {
        for(int j=0;j<width;j++)
        {
            ///////////////////////////////////
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
                    image_arr[x++]=image[((p*width)+q)*1];
                }
            }
            //////////////////////////////////////
            //int check=0;
            if(image_arr[4]==0)
            {
                for(int y=0;y<=9;y++)
                {
                    if(y!=4)
                    {
                        if(image_arr[4]!=image_arr[y])
                        {
                            G[((i*width)+j)*1]=255;
                            break;
                            //++check;
                        }
                    }
                }
            }
            if(image_arr[4]==1)
            {
                for(int y=0;y<=9;y++)
                {
                    if(y!=4)
                    {
                        if(image_arr[4]!=image_arr[y])
                        {
                            G[((i*width)+j)*1]=255;
                            break;
                            //++check;
                        }
                    }
                }
            }
            if((int)image_arr[4]==-1)
            {
                for(int y=0;y<=9;y++)
                {
                    if(y!=4)
                    {
                        if(image_arr[4]!=image_arr[y])
                        {
                            G[((i*width)+j)*1]=255;
                            break;
                            //++check;
                        }
                    }
                }
            }
            
            
            /////////////////////////////////////
        }
    }
    return G;
    
}

int *LoG:: ranger_1_0_1(int* img,int height,int width,int min_k,int max_k)
{
    for(int i=0;i<height;i++)
    {
        for(int j=0;j<width;j++)
        {
            //cout<<"Gx:@"<<(i*width)+j<<"\t="<<img[(i*width)+j]<<endl;
            if(img[(i*width)+j]<min_k)
                img[(i*width)+j]=-1;
            else if(img[(i*width)+j]>max_k)
                img[(i*width)+j]=1;
            else
                img[(i*width)+j]=0;
        }
    }
    return img;
    
}
float LoG::LoG_algo(unsigned char* Image,int height,int width)
{
    float avg=0;
    
    int filter[9*9]={0,1,1,2,2,2,1,1,0,
                     1,2,3,5,5,5,4,2,1,
                     1,4,5,3,0,3,5,4,1,
                     2,5,3,-12,-24,-12,3,5,2,
                     2,5,0,-24,-40,-24,0,5,2,
                     2,5,3,-12,-24,-12,3,5,2,
                     1,4,5,3,0,3,5,4,1,
                     1,2,3,5,5,5,4,2,1,
                     0,1,1,2,2,2,1,1,0};
    for(int i=0;i<9*9;i++)
    {
        avg+=(Image[i]*filter[i]);
    }
    return avg;
}


int *LoG::knee_points(int* im,int height,int width,int min,int max)
{
    int *knee_pts=new int[2];
    //cout<<"MIN:"<<min<<"\t MAX:"<<max<<endl;
    int mini=abs(min);
    //cout<<"MIN:"<<mini<<"\t MAX:"<<max<<endl;
    int *pdf_val=new int [mini+max+1];
    pdf_val=pdf(im,height,width,min,max);
    
    return knee_pts;
    
}
int *LoG::pdf(int* im,int height,int width,int min,int max)
{
    //cout<<"MIN:"<<min<<"\t MAX:"<<max<<endl;
    int mini=abs(min);
    int p=0,it=0;
    //cout<<"MIN:"<<min<<"\t MAX:"<<max<<endl;
    int *pdf_val=new int [mini+max+1];
    for(int k=min;k<=max;k++)
    {
        //cout<<"K:"<<k<<"actual:"<<k-mini<<endl;
        p=0;
        for(int i=0;i<height;i++)
        {
            for(int j=0;j<width;j++)
            {
                if(im[(i*width)+j]==k)
                {
                   
                    pdf_val[k+mini]=++p;
                    it=p;
                }
            }
        }
        
        //if(pdf_val[k+mini]==0)
           // cout<<"@:"<<k<<"pdf:"<<pdf_val[k+mini]<<endl;
        
    }
    //forward
    for(int k=min+1;k<=max;k++)
    {
        if((pdf_val[k+mini]-pdf_val[k+mini-1])>10)
            cout<<"@"<<k<<": "<<pdf_val[k+mini]<<endl;
            //ww<<"\t Diff:"<<pdf_val[k+mini]-pdf_val[k+mini-1]<<endl;
    }
    
    return pdf_val;
}

