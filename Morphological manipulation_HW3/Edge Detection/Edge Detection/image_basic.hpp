//
//  image_basic.hpp
//  Edge Detection
//
//  Created by HariKrishna Prabhu on 3/19/18.
//  Copyright © 2018 HariKrishna Prabhu. All rights reserved.
//

#ifndef image_basic_hpp
#define image_basic_hpp

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
    int* convert_2_0_255(int*,int,int,int,int);
    int * min_max(int *,int,int);

};

#endif /* image_basic_hpp */
