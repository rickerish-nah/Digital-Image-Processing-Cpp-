#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include "image_basic.hpp"
#include "edge_detection.hpp"
using namespace std;

int main(int argc, char *argv[])
{
    int option=0;
    cout<<"Solve for \nQ1.Sobel Edge Detector\nQ2.Zero-Crossing\n";
    cin>>option;
    cout<<"okay..\n";
    int dim[3]={0};
    cout<<"Enter the following: #columns, #rows, BytesPerPixel\n";
    cin>>dim[0]>>dim[1]>>dim[2];
    cout<<"Okay...\n\n";
    switch(option)
    {
        case 1:
        {
            image ob1(dim[0],dim[1],dim[2]);
            image ob[2];
            sobel ap;
            
            for(int i=0;i<2;i++)
            {
                ob[i]=ob1;
            }
            for(int i=1;i<2;i++)
            {
                unsigned char * Imagedata=ob[i].FileRead(argv,i+1,ob[i].height,ob[i].width,ob[i].BytesPerPixel);//read img
                unsigned char *edge_detc = ob[i].DynamicMemoryAllocation(ob[i].height,ob[i].width,ob[i].BytesPerPixel);
                unsigned char *grey = ob[i].DynamicMemoryAllocation(ob[i].height,ob[i].width,1);
                
                grey=ap.color_2_greyscale(Imagedata,ob[i].height,ob[i].width,ob[i].BytesPerPixel);
                ob1.FileWrite( argv,grey,i+1+2,ob[i].height,ob[i].width,1);
                
                edge_detc=ap.sobel_edge_detector(grey,ob[i].height,ob[i].width,1,argv);
                ob1.FileWrite( argv,edge_detc,i+1+2+2,ob[i].height,ob[i].width,1);
            }
            
            break;
        }
        case 2:
        {
            image ob1(dim[0],dim[1],dim[2]);
            image ob[2];
            sobel ap;
            LoG obl;
            for(int i=0;i<2;i++)
            {
                ob[i]=ob1;
            }
            
            for(int i=0;i<2;i++)
            {
                unsigned char * Imagedata=ob[i].FileRead(argv,i+1,ob[i].height,ob[i].width,ob[i].BytesPerPixel);//read img
                //unsigned char *edge_detc = ob[i].DynamicMemoryAllocation(ob[i].height,ob[i].width,ob[i].BytesPerPixel);
                unsigned char *temp_edge_detc = ob[i].DynamicMemoryAllocation(ob[i].height,ob[i].width,ob[i].BytesPerPixel);
                unsigned char *grey = ob[i].DynamicMemoryAllocation(ob[i].height,ob[i].width,1);
                
                grey=ap.color_2_greyscale(Imagedata,ob[i].height,ob[i].width,ob[i].BytesPerPixel);
                ob1.FileWrite( argv,grey,i+1+2,ob[i].height,ob[i].width,1);
                
                temp_edge_detc=obl.LoG_filter(grey,ob[i].height,ob[i].width,1);
                
                ob1.FileWrite( argv,temp_edge_detc,i+1+2+2,ob[i].height,ob[i].width,1);
            }
            
            break;
        }
        default:
        {
            cout<<"\nOops try again..\n";
        }
    }
    return 0;
}
