
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <istream>
#include <fstream>
#include <vector>
#include <math.h>
#include "texture_classification.hpp"
using namespace std;

int main(int argc, char *argv[])
{
    int option=0;
    cout<<"Solve for \nQ1.Texture Classification\nQ2.Texture Segmentation\nQ3.PCA\n";
    cin>>option;
    cout<<"okay..\n";
    //float f1[25]={1.0/16,4.0/16,6.0/16,4.0/16,1.0/16};
    //float f2[25]={-1.0/6,-2.0/6,0,2.0/6,1.0/6};
    //float *fil=new float[25];
    image o;
    //fil=o.matrix(f1, f2);
    switch(option)
    {
        case 1:
        {
            int dim[3]={0},im=12;
            cout<<"Enter the following: #columns, #rows, BytesPerPixel\n";
            cin>>dim[0]>>dim[1]>>dim[2];
            cout<<"Okay...\n\n";
            double feature_vector[12][9]={{0}};
            image ob1(dim[0],dim[1],dim[2]);
            image ob[12]; //initializing 12 objects for 12 images
            for(int i=0;i<12;i++)
            {
                ob[i]=ob1;//copying attributes to each object
            }
            
            for(int img=0;img<im;img++)
            {
                unsigned char * Imagedata=ob[img].FileRead(argv,img+1,ob[img].height,ob[img].width,ob[img].BytesPerPixel);  //read images
                float avg=0;
                /*
                for(int i=0;i<ob1.height;i++)
                {
                    for(int j=0;j<ob1.width;j++)
                    {
                        avg=avg+pow(Imagedata[(i*ob1.width)+j],1);
                    }
                }
                avg=avg/(ob1.height*ob1.width);
                int av=round(avg);
                for(int i=0;i<ob1.height;i++)
                {
                    for(int j=0;j<ob1.width;j++)
                    {
                        Imagedata[(i*ob1.width)+j]-=av;
                    }
                }*/
                for(int k=0;k<9;k++)
                {
                    feature_vector[img][k]=ob[img].filter_window5x5(Imagedata,ob[img].height,ob[img].width,ob[img].BytesPerPixel,k+1);
                }
                
                cout<<"#img:"<<img+1<<"\tThe feature vectors are:"<<endl;
                cout<<feature_vector[img][0]<<"\t"<<feature_vector[img][1]<<"\t"<<feature_vector[img][2]<<"\t"<<feature_vector[img][3]<<"\t"<<feature_vector[img][4]<<"\t"<<feature_vector[img][5]<<"\t"<<feature_vector[img][6]<<"\t"<<feature_vector[img][7]<<"\t"<<feature_vector[img][8]<<endl<<endl;
                
                //ob[img].FileWrite(argv,Imagedata,img+im+1,ob[img].height,ob[img].width,ob[img].BytesPerPixel); //write images
                
            }
            cout<<endl;
            ob1.k_cluster(im,9,feature_vector);
            break;
        }
        case 2:
        {
            int dim[3]={0},im=1,window=11;
            cout<<"Enter the following: #columns, #rows, BytesPerPixel\n";
            cin>>dim[0]>>dim[1]>>dim[2];
            cout<<"Okay...\n\n";
            //float feature_vector[600][450][9]={{{0}}};
            image ob1(dim[0],dim[1],dim[2]);
            image ob[9],ob_op[9];
            for(int i=0;i<12;i++)
            {
                ob[i]=ob1;//copying attributes to each object
                ob_op[i]=ob1;
            }
            float *feature_vector = new float [ob1.height*ob1.width*9]; //for calculating the feature vector
            float** f_vector = new float*[ob1.height*ob1.width]; //using feature vector for k-means calculation
            for(int i = 0; i < ob1.height*ob1.width; ++i)
                f_vector[i] = new float[9];
            
            unsigned char * Imagedata=ob1.FileRead(argv,im,ob1.height,ob1.width,ob1.BytesPerPixel);  //read images
            for(int f=0;f<9;f++)
            {
                ob[f].Image=ob[f].filter_segment(Imagedata,ob1.height,ob1.width,ob1.BytesPerPixel,f+1);
                
                //ob_op[f].Image1=ob_op[f].filter_segment_image(Imagedata,ob_op[f].height,ob_op[f].width,ob_op[f].BytesPerPixel,f+1);
                //ob_op[f].FileWrite(argv,ob_op[f].Image1,f+1+1,ob_op[f].height,ob_op[f].width,ob_op[f].BytesPerPixel);
                for(int i=0;i<ob1.height;i++)
                {
                    for(int j=0;j<ob1.width;j++)
                    {
                        feature_vector[((i*ob[f].width)+j)*9 + f]= ob[f].pixel_energy(ob[f].Image,ob[f].height,ob[f].width,i,j,ob1.BytesPerPixel,window,f);
                    }
                }
            }
            /*
            for(int i=0;i<ob1.height;i++)
            {
                for(int j=0;j<ob1.width;j++)
                {
                    feature_vector[((i*ob[0].width)+j)*9 + 0]= feature_vector[((i*ob[0].width)+j)*9 + 0]/feature_vector[((i*ob[0].width)+j)*9 + 8];
                    feature_vector[((i*ob[0].width)+j)*9 + 1]= feature_vector[((i*ob[0].width)+j)*9 + 1]/feature_vector[((i*ob[0].width)+j)*9 + 8];
                    feature_vector[((i*ob[0].width)+j)*9 + 2]= feature_vector[((i*ob[0].width)+j)*9 + 2]/feature_vector[((i*ob[0].width)+j)*9 + 8];
                    feature_vector[((i*ob[0].width)+j)*9 + 3]= feature_vector[((i*ob[0].width)+j)*9 + 3]/feature_vector[((i*ob[0].width)+j)*9 + 8];
                    feature_vector[((i*ob[0].width)+j)*9 + 4]= feature_vector[((i*ob[0].width)+j)*9 + 4]/feature_vector[((i*ob[0].width)+j)*9 + 8];
                    feature_vector[((i*ob[0].width)+j)*9 + 5]= feature_vector[((i*ob[0].width)+j)*9 + 5]/feature_vector[((i*ob[0].width)+j)*9 + 8];
                    feature_vector[((i*ob[0].width)+j)*9 + 6]= feature_vector[((i*ob[0].width)+j)*9 + 6]/feature_vector[((i*ob[0].width)+j)*9 + 8];
                    feature_vector[((i*ob[0].width)+j)*9 + 7]= feature_vector[((i*ob[0].width)+j)*9 + 7]/feature_vector[((i*ob[0].width)+j)*9 + 8];
                }
            }*/
            
            //Normalizing the features
            for(int i=0;i<ob1.height;i++)
            {
                for(int j=0;j<ob1.width;j++)
                {
                    ob[0].Image[(i*ob1.width)+j]=ob[0].Image[(i*ob1.width)+j]/ob[8].Image[(i*ob1.width)+j];
                    ob[1].Image[(i*ob1.width)+j]=ob[1].Image[(i*ob1.width)+j]/ob[8].Image[(i*ob1.width)+j];
                    ob[2].Image[(i*ob1.width)+j]=ob[2].Image[(i*ob1.width)+j]/ob[8].Image[(i*ob1.width)+j];
                    ob[3].Image[(i*ob1.width)+j]=ob[3].Image[(i*ob1.width)+j]/ob[8].Image[(i*ob1.width)+j];
                    ob[4].Image[(i*ob1.width)+j]=ob[4].Image[(i*ob1.width)+j]/ob[8].Image[(i*ob1.width)+j];
                    ob[5].Image[(i*ob1.width)+j]=ob[5].Image[(i*ob1.width)+j]/ob[8].Image[(i*ob1.width)+j];
                    ob[6].Image[(i*ob1.width)+j]=ob[6].Image[(i*ob1.width)+j]/ob[8].Image[(i*ob1.width)+j];
                    ob[7].Image[(i*ob1.width)+j]=ob[7].Image[(i*ob1.width)+j]/ob[8].Image[(i*ob1.width)+j];
                    
                    //cout<<"I:"<<i<<"\tJ:"<<j<<endl;
                    //feature_vector[((i*ob[f].width)+j)*9 + f]= ob[f].pixel_energy(ob[f].Image,ob[f].height,ob[f].width,i,j,ob1.BytesPerPixel,window,f);
                }
            }
            for(int f=0;f<9;f++)
            {
                for(int i=0;i<ob1.height;i++)
                {
                    for(int j=0;j<ob1.width;j++)
                    {
                        feature_vector[((i*ob[f].width)+j)*9 + f]= ob[f].pixel_energy(ob[f].Image,ob[f].height,ob[f].width,i,j,ob1.BytesPerPixel,window,f);
                    }
                }
                
            }
            cout<<"\t\t\t\t\tFeatures Done\n";

            f_vector=ob1.convert_1D_2D(feature_vector,ob1.height,ob1.width,9);
            cout<<"\t\t\t\t\tFeatures converted Done\n";
            //to print the feature vectors:  Caution: takes time
            //ob1.print_vec(f_vector,ob1.height,ob1.width,ob1.BytesPerPixel,9);
            ob1.k_means_cluster(ob1.height,ob1.width,9,f_vector,argv);
            cout<<"\t\t\t\t\tKmeans Done\n";
            
           
            break;
        }
        case 3:
        {
            int dim[3]={0},im=1,window=11;
            cout<<"Enter the following: #columns, #rows, BytesPerPixel\n";
            cin>>dim[0]>>dim[1]>>dim[2];
            cout<<"Okay...\n\n";
            //float feature_vector[600][450][9]={{{0}}};
            image ob1(dim[0],dim[1],dim[2]);
            image ob[25],ob_op[25];
            for(int i=0;i<25;i++)
            {
                ob[i]=ob1;//copying attributes to each object
                ob_op[i]=ob1;
            }
            float *feature_vector = new float [ob1.height*ob1.width*25]; //for calculating the feature vector
            float** f_vector = new float*[ob1.height*ob1.width]; //using feature vector for k-means calculation
            for(int i = 0; i < ob1.height*ob1.width; ++i)
                f_vector[i] = new float[25];
            unsigned char * Imagedata=ob1.FileRead(argv,im,ob1.height,ob1.width,ob1.BytesPerPixel);  //read images
            
            for(int f=0;f<25;f++)
            {
                cout<<"#"<<f<<endl;
                
                ob[f].Image=ob[f].filter_segment_PCA(Imagedata,ob1.height,ob1.width,ob1.BytesPerPixel,f+1);
                for(int i=0;i<ob1.height;i++)
                {
                    for(int j=0;j<ob1.width;j++)
                    {
                        
                        //cout<<"I:"<<i<<"\tJ:"<<j<<endl;
                        feature_vector[((i*ob[f].width)+j)*25 + f]= ob[f].pixel_energy(ob[f].Image,ob[f].height,ob[f].width,i,j,ob1.BytesPerPixel,window,f);
                    }
                }
            }
            cout<<"\t\t\t\t\tFeatures Done\n";
            f_vector=ob1.convert_1D_2D(feature_vector,ob1.height,ob1.width,25);
            cout<<"\t\t\t\t\tFeatures converted Done\n";
            //to print the feature vectors:  Caution: takes time
            //ob1.print_vec(f_vector,ob1.height,ob1.width,ob1.BytesPerPixel,25);

            ofstream ofs;
            ofs.open("/Users/rickerish_nah/Desktop/Question1c.csv");
            for(int i=0;i<ob1.height;i++)
            {
                for(int j=0;j<ob1.width;j++)
                {
                    for(int k=0;k<25;k++)
                    {
                        ofs<<","<<f_vector[(i*ob1.width)+j][k];
                    }
                    ofs<<'\n';
                }
            }
            ofs.close();
            cout<<"\t\t\t\t\twrite Done\n";
            //////////////
            float** f_v = new float*[ob1.height*ob1.width]; //using feature vector for k-means calculation
            for(int i = 0; i < ob1.height*ob1.width; ++i)
                f_v[i] = new float[10];
            ifstream file ( "/Users/rickerish_nah/Desktop/PCA_Question1c_1.csv" ); // declare file stream:
            string value;
            for(int row = 0; row < ob1.height*ob1.width; ++row)
            {
                std::string line;
                std::getline(file, line);
                if ( !file.good() )
                    break;
                
                std::stringstream iss(line);
                
                for (int col = 0; col < 10; ++col)
                {
                    std::string val;
                    std::getline(iss, val, ',');
                    if ( !iss.good() )
                        break;
                    
                    std::stringstream convertor(val);
                    convertor >> f_v[row][col];
                }
            }
            /*
            for(int i=0;i<ob1.height;i++)
            {
                for(int j=0;j<ob1.width;j++)
                {
                    for(int k=0;k<10;k++)
                    {
                        cout<<"@:"<<(i*ob1.width)+j<<":\t"<<f_v[(i*ob1.width)+j][k]<<endl;
                    }
                }
            }*/
            ob1.k_means_cluster(ob1.height,ob1.width,5,f_vector,argv);   //check line 780
            cout<<"\t\t\t\t\tKmeans Done\n";
            break;
            
        }
        default:
            cout<<"Try Again\n";
    }
    cout<<"\nProgram Exit. Thank You"<<endl;
    return 0;
    
}
