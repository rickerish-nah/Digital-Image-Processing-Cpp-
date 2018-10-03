//
//  texture_classification.hpp
//  texture_Classification
//
//  Created by HariKrishna Prabhu on 3/15/18.
//  Copyright © 2018 HariKrishna Prabhu. All rights reserved.
//

#ifndef texture_classification_hpp
#define texture_classification_hpp

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

class image
{
    
public:
    int height,width,BytesPerPixel;
    unsigned char * Image1 = new unsigned char [1000000l];
    float * Image = new float [10000000];
    image();//default constructor
    image(int x,int y, int z); //parameterized constructor
    ~image(); //destructor
   
    
    unsigned char * FileRead( char*[],int,int,int,int);
    unsigned char * DynamicMemoryAllocation(int,int,int);
    void FileWrite( char*[],unsigned char *,int,int,int,int);
    void print_vec(float **,int,int,int,int);//2
    float ** convert_1D_2D(float *,int,int,int);//2
    
    double feature_ext(unsigned char *, float *,int);
    double filter_window5x5(unsigned char *,int,int,int,int);
    float *filter_segment(unsigned char *,int,int,int,int);//2
    unsigned char *filter_segment_image(unsigned char *,int,int,int,int);//2
    float pixel_energy(float *,int,int,int,int,int,int,int);
    float *filter_segment_PCA(unsigned char *,int,int,int,int);
    float *matrix(float [], float[]);
    
    //part 1a
    void k_cluster(int,int,double [][9]);
    int algo(int [12],int[][9],double [][9],int);
    float dist(double [9],int [9]);
    int cluster_type(float,float,float,float);
    //part 1b
    void k_means_cluster(int,int,int,float **,char*[]);//2
    //int algo_2(int *,float**,float **,int,int);
    int algo_2(int *,int**,float **,int,int,char*[],int);//function: see the #parameters are different
    float dist_2(float *,int *,int);
    int cluster_type_2(float,float,float,float,float,float);
    //part c
    
    
};


#endif /* texture_classification_hpp */
