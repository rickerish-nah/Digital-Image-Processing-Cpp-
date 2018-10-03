#include <iostream>

#include <stdio.h>

#include <math.h>

#include <cstdlib>



using namespace std;

int main(int argc, char *argv[])



{

    // Define file pointer and variables

    FILE *file;

    int BytesPerPixel=3;

    int row=700, col=700, rgb=3;

    // Allocate image data array

    unsigned char Imagedata[row][col][rgb],Imagedata_new[row][col][rgb];

    

    

    // Read image (filename specified by first argument) into image data matrix

    if (!(file=fopen(argv[1],"rb"))) {

        cout << "Cannot open file: " << argv[1] <<endl;

        exit(1);

    }

    fread(Imagedata, sizeof(unsigned char), row*col*BytesPerPixel, file);

    fclose(file);

    

    //Processing Code//

    float error;

    int i,j;

    

    

    

    //floyd Steinberg
    for(int rgb =0;rgb<3;rgb++)
    {
    for(i=0;i<row;i++)

    {

        if (i%2==0)

        {

        for(j=0;j<col;j++)

        {

            if(Imagedata[i][j][rgb]<=127)

            {

                error=(float)Imagedata[i][j][rgb];

                Imagedata_new[i][j][rgb]=0;

            }

            else

            {

                error=(float)(Imagedata[i][j][rgb]-255);

                Imagedata_new[i][j][rgb]=255;

            }

            error=error/16;

            if(j<col-1)

            {

                if((((int)Imagedata[i][j+1][rgb])+(7*error))>255)

                    Imagedata[i][j+1][rgb]=255;

                else if((((int)Imagedata[i][j+1][rgb])+(7*error))<0)

                    Imagedata[i][j+1][rgb]=0;

                else

                    Imagedata[i][j+1][rgb]=Imagedata[i][j+1][rgb]+(unsigned char)(round(7*error));

            }

            if((j>=1) && (i<row-1))

            {

                if((((int)Imagedata[i+1][j-1][rgb])+(3*error))>255)

                    Imagedata[i+1][j-1][rgb]=255;

                else if((((int)Imagedata[i+1][j-1][rgb])+(3*error))<0)

                    Imagedata[i+1][j-1][rgb]=0;

                else

                    Imagedata[i+1][j-1][rgb]=Imagedata[i+1][j-1][rgb]+(unsigned char)(round(3*error));

            }

            if(i<row-1)

            {

                if((((int)Imagedata[i+1][j][rgb])+(5*error))>255)

                    Imagedata[i+1][j][rgb]=255;

                else if((((int)Imagedata[i+1][j][rgb])+(5*error))<0)

                    Imagedata[i+1][j][rgb]=0;

                else

                    Imagedata[i+1][j][rgb]=Imagedata[i+1][j][rgb]+(unsigned char)(round(5*error));

            }

            if((j<col-1) && (i<row-1))

            {

                if((((int)Imagedata[i+1][j+1][rgb])+(1*error))>255)

                    Imagedata[i+1][j+1][rgb]=255;

                else if((((int)Imagedata[i+1][j+1][rgb])+(1*error))<0)

                    Imagedata[i+1][j+1][rgb]=0;

                else

                    Imagedata[i+1][j+1][rgb]=Imagedata[i+1][j+1][rgb]+(unsigned char)(round(1*error));

            }

        }

        }

        else

        {

            for(j=col-1;j>=0;j--)

            {

                if(Imagedata[i][j][rgb]<=127)

                {

                    error=(float)Imagedata[i][j][rgb];

                    Imagedata_new[i][j][rgb]=0;

                }

                else

                {

                    error=(float)(Imagedata[i][j][rgb]-255);

                    Imagedata_new[i][j][rgb]=255;

                }

                error=error/16;

                if(j>=1)

                {

                    if((((int)Imagedata[i][j-1][rgb])+(7*error))>255)

                        Imagedata[i][j-1][rgb]=255;

                    else if((((int)Imagedata[i][j-1][rgb])+(7*error))<0)

                        Imagedata[i][j-1][rgb]=0;

                    else

                        Imagedata[i][j-1][rgb]=Imagedata[i][j-1][rgb]+(unsigned char)(round(7*error));

                }

                if((j>=1) && (i<row-1))

                {

                    if((((int)Imagedata[i+1][j-1][rgb])+(1*error))>255)

                        Imagedata[i+1][j-1][rgb]=255;

                    else if((((int)Imagedata[i+1][j-1][rgb])+(1*error))<0)

                        Imagedata[i+1][j-1][rgb]=0;

                    else

                        Imagedata[i+1][j-1][rgb]=Imagedata[i+1][j-1][rgb]+(unsigned char)(round(1*error));

                }

                if(i<row-1)

                {

                    if((((int)Imagedata[i+1][j][rgb])+(5*error))>255)

                        Imagedata[i+1][j][rgb]=255;

                    else if((((int)Imagedata[i+1][j][rgb])+(5*error))<0)

                        Imagedata[i+1][j][rgb]=0;

                    else

                        Imagedata[i+1][j][rgb]=Imagedata[i+1][j][rgb]+(unsigned char)(round(5*error));

                }

                if((j<col-1) && (i<row-1))

                {

                    if((((int)Imagedata[i+1][j+1][rgb])+(3*error))>255)

                        Imagedata[i+1][j+1][rgb]=255;

                    else if((((int)Imagedata[i+1][j+1][rgb])+(3*error))<0)

                        Imagedata[rgb][i+1][j+1]=0;

                    else

                        Imagedata[i+1][j+1][rgb]=Imagedata[i+1][j+1][rgb]+(unsigned char)(round(3*error));

                }

            }

        }

        error=0;
    }
    }


    if (!(file=fopen(argv[2],"wb"))) {
            cout << "Cannot open file: " << argv[2] << endl;
            exit(1);
            }
            fwrite(Imagedata_new, sizeof(unsigned char), row*col*BytesPerPixel, file);
            fclose(file);
            
    return 0;
}
