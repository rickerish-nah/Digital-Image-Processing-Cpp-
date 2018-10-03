//
//  sobel.hpp
//  Edge Detection
//
//  Created by HariKrishna Prabhu on 3/19/18.
//  Copyright © 2018 HariKrishna Prabhu. All rights reserved.
//

#ifndef sobel_hpp
#define sobel_hpp

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "image_basic.hpp"


class sobel: public image
{
public:
    unsigned char *sobel_edge_detector(unsigned char *,int,int,int,char*[]);
    unsigned char * color_2_greyscale(unsigned char *,int,int,int);
    float horizontal_filter(unsigned char*,int,int);
    float vertical_filter(unsigned char*,int,int);
    int cdf_threshold(unsigned char *,int,int);
    unsigned char * threshold(unsigned char *,int,int,int);
    unsigned char* draw(int *,int,int,int,char*[],int);
    
    
};
class LoG: public image
{
public:
    unsigned char *zeroXing_edge_detector(unsigned char *,int,int,int);
    unsigned char *LoG_filter(unsigned char *,int,int,int);
    float LoG_algo(unsigned char*,int,int);
    int *knee_points(int*,int,int,int,int);
    int *pdf(int*,int,int,int,int);
    int * ranger_1_0_1(int*,int,int,int,int);
    unsigned char* make_edge(int*,int,int);
    
    
    
};

#endif /* sobel_hpp */
