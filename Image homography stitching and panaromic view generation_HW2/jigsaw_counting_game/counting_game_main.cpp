#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include<math.h>

using namespace std;
int iden=0;
//functions used:
unsigned char *fixed_Dithering(unsigned char *Image_data, int height, int width, int bytesPerPixel);
unsigned char *convert1_255(unsigned char *a, int height, int width, int bytesPerPixel);
int non_zero_min(int *a);
unsigned char *invert(unsigned char *a, int height, int width, int bytesPerPixel);
int * label_loop(unsigned char *fixed_image,int *label,int height,int width,int BytesPerPixel);
int checker(int x,int *a, int size);
int count(int * l, int height,int width, int BytesPerPixel,int x);
int *dimension(int *i,int height,int width, int BytesPerPixel,int x);
void call_compare(int *q, int *d,int * label);
void compare(int *q,int *label, int *dim, int i, int j, int &el, int op);
void compare2(int *q,int *label, int *dim, int i, int j, int &el, int op);


int main(int argc, char *argv[])
{
	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel=1;
	int width = 372 , height = 372;

	unsigned char *Imagedata= new unsigned char [width*height*BytesPerPixel];
	int *label= new int [width*height*BytesPerPixel];

	unsigned char *final= new unsigned char [width*height*BytesPerPixel];
	unsigned char *newimage_Fixed= new unsigned char [width*height*BytesPerPixel];
	unsigned char *newimage_temp= new unsigned char [width*height*BytesPerPixel];
	
	int a[8]={0};
	int label_var=0,min=0;
	//read input
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), width*height*BytesPerPixel, file);
	fclose(file);

	newimage_Fixed=fixed_Dithering(Imagedata,height,width,BytesPerPixel);
	newimage_Fixed=invert(newimage_Fixed,height,width,BytesPerPixel);
	
	//labelling:
	for (int i=1; i<height-1 ; i++)
	{
		for( int j=1; j<width-1; j++)
		{
			if(newimage_Fixed[(i*width)+j]==0)
			{
				label[(i*width)+j]=0;
			}
			else if(newimage_Fixed[(i*width)+j]==1)
			{
				if(label[(i*width)+(j)]==0)
				{
					++label_var;
					//cout<<"New label:"<<label_var<<endl;
					label[(i*width)+(j)]=label_var;

					if(newimage_Fixed[(i*width)+(j+1)]!=0)
					{
						label[(i*width)+(j+1)]=label_var;
					}
					if(newimage_Fixed[((i-1)*width)+(j+1)]!=0)
					{
						label[((i-1)*width)+(j+1)]=label_var;
					}
					if(newimage_Fixed[((i-1)*width)+(j+1)]!=0)
					{
						label[((i-1)*width)+(j+1)]=label_var;
					}
					if(newimage_Fixed[((i-1)*width)+(j)]!=0)
					{
						label[((i-1)*width)+(j)]=label_var;
					}
					if(newimage_Fixed[((i-1)*width)+(j-1)]!=0)
					{
						label[((i-1)*width)+(j-1)]=label_var;
					}
					if(newimage_Fixed[(i*width)+(j-1)]!=0)
					{
						label[(i*width)+(j-1)]=label_var;
					}
					if(newimage_Fixed[((i+1)*width)+(j-1)]!=0)
					{
						label[((i+1)*width)+(j-1)]=label_var;
					}
					if(newimage_Fixed[((i+1)*width)+(j)]!=0)
					{
						label[((i+1)*width)+(j)]=label_var;
					}
					if(newimage_Fixed[((i+1)*width)+(j+1)]!=0)
					{
						label[((i+1)*width)+(j+1)]=label_var;
					}	

				}
				else
				{
					a[0]=label[(i*width)+(j+1)];
					a[1]=label[((i-1)*width)+(j+1)];
					a[2]=label[((i-1)*width)+(j)];
					a[3]=label[((i-1)*width)+(j-1)];
					a[4]=label[(i*width)+(j-1)];
					a[5]=label[((i+1)*width)+(j-1)];
					a[6]=label[((i+1)*width)+(j)];
					a[7]=label[((i+1)*width)+(j+1)];
					/*
					cout<<"The actual:";
					for(int k=0;k<8;k++)
						cout<<a[k]<<"\t";
					cout<<endl;
					*/ 
					min=non_zero_min(a);
					//cout<<"Min:"<<min<<endl;
					
						if(newimage_Fixed[(i*width)+j]!=0)
						{
							label[(i*width)+j]=min;
						}
						if(newimage_Fixed[(i*width)+(j+1)]!=0)
						{
							label[(i*width)+(j+1)]=min;
						}
						if(newimage_Fixed[((i-1)*width)+(j+1)]!=0)
						{
							label[((i-1)*width)+(j+1)]=min;
						}
						if(newimage_Fixed[((i-1)*width)+(j+1)]!=0)
						{
							label[((i-1)*width)+(j+1)]=min;
						}
						if(newimage_Fixed[((i-1)*width)+(j)]!=0)
						{
							label[((i-1)*width)+(j)]=min;
						}
						if(newimage_Fixed[((i-1)*width)+(j-1)]!=0)
						{
							label[((i-1)*width)+(j-1)]=min;
						}
						if(newimage_Fixed[(i*width)+(j-1)]!=0)
						{
							label[(i*width)+(j-1)]=min;
						}
						if(newimage_Fixed[((i+1)*width)+(j-1)]!=0)
						{
							label[((i+1)*width)+(j-1)]=min;
						}
						if(newimage_Fixed[((i+1)*width)+(j)]!=0)
						{
							label[((i+1)*width)+(j)]=min;
						}
						if(newimage_Fixed[((i+1)*width)+(j+1)]!=0)
						{
							label[((i+1)*width)+(j+1)]=min;
						}	
					
				}
				//cout<<"Class label:"<<(int)label[(i*width)+j]<<endl;
			}
		}
	}
	


	for(int k=0;k<200;k++)
	{
		label=label_loop(newimage_Fixed,label,height,width,BytesPerPixel);
	}

	int var=-2;
	int h=0;
	int d=0;
	int *ss= new int [h];
	int la[16]={0};
	for(int i=1;i<height-1;i++)
	{
		for(int j=1;j<width-1;j++)
		{
			if(label[(i*width)+j]!=0 && label[(i*width)+j]!=var)
			{
				ss[h++]=label[(i*width)+j];

			}
		}
	}
	cout<<"Actual Lable:"<<endl;                             ///////////////////////////////////////
	for(int i=0;i<h;i++)
	{
		if(ss[i]==0)
			continue;
		else
		{
			la[d]=ss[i];
			d++;
			//cout<<"\t:"<<ss[i];
		}
		for(int j=i+1;j<h;j++)
		{
			if(ss[j]==ss[i])
			{
				ss[j]=0;
			}

		}
	}
	//cout<<endl;
	for(int k=0;k<16;k++)
	{
		cout<<"\t:"<<la[k]; 							       	//////////////////////////////////////
	}
	cout<<endl;
	cout<<"Modified Lable:"<<endl;                           ////////////////////////////////////////
	int k=0;
	for(k=0;k<16;k++)
	{
		
		cout<<"\t:"<<k+1;
	}
	cout<<endl;
	cout<<"Number of Objects Indentified is :"<<k<<endl;       //////////////////////////////////////////////
	cout<<"\n\nIdentifying Objects:......\n\n\n";
	
	
	for(int i=1;i<height-1;i++)
	{
		for(int j=1;j<width-1;j++)
		{	
			//cout<<"L"<<endl;
			if(label[(i*width)+j]!=0)//!=0 )//&& label[(i*width)+j]>var)
			{
				for(int g=0;g<16;g++)
				{
					if(label[(i*width)+j]==la[g])
						label[(i*width)+j]=g+1;

				}
				
			}
		}
	}

	
	for(int g=0; g<16; g++)
	{
		for(int i=1;i<height-1;i++)
		{
			for(int j=1;j<width-1;j++)
			{
				if(label[(i*width)+j]!=0)
				{
					if(label[(i*width)+j]==g+1)
					{
						newimage_Fixed[(i*width)+j]= 15*(g+1);
						if(newimage_Fixed[(i*width)+j]==15)
							newimage_Fixed[(i*width)+j]=200;
					}
				}
			}
		}
	}


	
	var=-2;
	//cout<<"2:"<<endl;
	for(int i=1;i<height-1;i++)
	{
		for(int j=1;j<width-1;j++)
		{	
			//cout<<"L"<<endl;
			if(label[(i*width)+j]>0)// && label[(i*width)+j]>var)
			{
				//cout<<"I:"<<i<<"\tJ:"<<j<<"\t\t\tClass label:"<<label[(i*width)+j]<<endl;
				var=label[(i*width)+j];
			}
		}
	}
	int q[16]={0};
	int p=0;
	//int dim[16][6]={{0}};
	int *dim=new int (p);
	int *ddd= new int(p);
	//counter && range
	for(int i=0;i<16;i++)
	{
		for(int j=0;j<6;j++)
		{
			q[i]=count(label,height,width,BytesPerPixel,i+1);
		
			ddd=dimension(label,height,width,BytesPerPixel,i+1);
			dim[(i*6)+j]=ddd[j];
			
		}
		//cout<<"at I:"<<i+1<<"\t count="<<q[i]<<"\tX_range:"<<ddd[0]<<"\tYrange:"<<ddd[1]<<endl;
	}
	//Dimension
	for(int i=0;i<16;i++)
	{
		/*cout<<"at I:"<<i+1<<"\t count="<<q[i]<<"\tX_range:"<<dim[(i*6)+0]
		<<"\tYrange:"<<dim[(i*6)+1]<<"\tXmax:"<<dim[(i*6)+2]<<"\tXmin:"
		<<dim[(i*6)+3]<<"\tYmax:"<<dim[(i*6)+4]<<"\tYmin:"<<dim[(i*6)+5]<<endl;*/
		
	}
	call_compare(q,dim,label);
	//newimage_temp=convert1_255(newimage_Fixed,height,width,BytesPerPixel);
	//write output
	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(newimage_Fixed, sizeof(unsigned char), width*height*BytesPerPixel, file);
	fclose(file);
	//cout<<"Number of objects Uniquely Identified is: "<<iden<<endl;         //////////////////////////////////////
	//delete[] newimage_Fixed;
	//delete[] Imagedata;
	//delete[] newimage_temp;
	return 0;
}
//END of CPP
void call_compare(int *q, int *dim, int *label)
{
	int cmp=0,c=0,cc=0;
	int x=0,f=0,y=0,z=0;
	int *a=new int(x);
	int *b=new int(y);
	/*
	for(int i=0;i<16;i++)
	{
		//cout<<"at I:"<<i+1<<"\t count="<<q[i]<<"\tX_range:"<<dim[(i*6)+0]<<"\tYrange:"<<dim[(i*6)+1]<<"\tXmax:"<<dim[(i*6)+2]<<"\tXmin:"<<dim[(i*6)+3]<<"\tYmax:"<<dim[(i*6)+4]<<"\tYmin:"<<dim[(i*6)+5]<<endl;
		
	}*/
	
	for(int i=0;i<16;i++)
	{
		int el=0;
		for(int j=0;j<16;j++)
		{
			if(i!=j)
			{
				compare(q,label,dim,i,j,el,0);

			}
		}
		if(el==15)
		{
			cout<<"\tCompletely Unique Object:"<<i+1<<endl;                    ///////////////////////////////////
			a[x++]=i+1;
			iden++;
		}
		else if(el==14)
		{
			cout<<"\t\t\t\t\t 1 match Unique Object:"<<i+1<<endl;             ///////////////////////////////////
			a[x++]=i+1;
			++c;
			if(c==2)
				iden++;
		}
		else if(el==13)
		{
			cout<<"\t\t\t\t\t\t\t\t\t 2 match Unique Objects:"<<i+1<<endl;    /////////////////////////////////////
			a[x++]=i+1;
			++cc;
			if(cc==3)
				iden++;
		}

	}
	
	/*
	for(int i=0;i<x;i++)
		cout<<a[i]<<"\t";
	cout<<endl<<endl;
	*/
	int y1=0;
	for(int i=1;i<=16;i++)
	{
		
		for(int j=0;j<x;j++)
		{
			if(a[j]==i)
				break;
			else
				z++;
		}
		if(z==11)
		{
			
			//cout<<"I="<<i<<endl;  //##
			
			
		}
		
		z=0;
	
	}
	//from the previous cout statement.  //##
	b[0]=2;
	b[1]=6;
	b[2]=7;
	b[3]=11;
	b[4]=13;
	/*
	for(int i=0;i<5;i++)
	{
		cout<<b[i]<<'\t';
	}
	cout<<endl;
	*/
	
	for (int i=0; i<5;i++)
	{
		int el=0;
		for(int j=i+1;j<5;j++)
		{
			if(i!=j)
			{
				compare(q,label,dim,b[i]-1,b[j]-1,el,1); //check upside downs

			}

		}
	}
	//int el=0;
	//compare2(q,label,dim,1,6,el,1);
	
	for (int i=0; i<5;i++)
	{
		int el=0;
		for(int j=i+1;j<5;j++)
		{
			if(i!=j)
			{
				compare2(q,label,dim,b[i]-1,b[j]-1,el,1); //check rotations

			}

		}
	}
	//delete a;
	//delete b;
	
}
void compare2(int *q,int *label, int * dim, int a, int b, int &el,int op)
{
	//cout<<"Welcome"<<endl;
	int error=0;
	
	if( (dim[(a*6)+0]==dim[(b*6)+1]) && (dim[(a*6)+1]==dim[(b*6)+0]) && (abs((q[a]-q[b]))<20) )
	{
		int x=0,y=0;
		//cout<<"L:"<<a+1<<"\tL:"<<b+1<<endl;
		//rotation check:
		//a=1;
		//b=6;
		
		x=dim[(b*6)+3]-dim[(a*6)+5];
		y=dim[(b*6)+5]-dim[(a*6)+3];
		//cout<<"X_R:"<<x<<"\tY_R:"<<y<<endl;
		int f=0,g=0;
		for(int i=dim[(a*6)+3]; i<=dim[(a*6)+2]; i++)
		{
			for(int j=dim[(a*6)+5]; j<=dim[(a*6)+4]; j++)
			{
				//cout<<"I:"<<i<<"\t J:"<<j<<"\ti:"<<(dim[(b*6)+4]+dim[(b*6)+5])-(i+y)<<"\t j:"<<x+j<<endl;
				f=(dim[(b*6)+4]+dim[(b*6)+5])-(i+y);
				g=x+j;
				//cout<<"I:"<<i<<"\t J:"<<j<<"\ti:"<<f<<"\t j:"<<g<<endl;
				
				if((label[(i*372)+j]!=0) && (label[(f*372)+(g)])!=0)
				{
					//cout<<"y\t";
					//cout<<"L 6:"<<label[(i*372)+j]<<"\t\tL 13:"<<label[(((dim[(a*6)+3]+dim[(a*6)+2]-i)-x)*372)+(j+y)]<<endl;
				}
				else if((label[(i*372)+j]==0) && (label[(f*372)+(g)])==0)
				{
					//cout<<"y\t";
					//cout<<"L 6:"<<label[(i*372)+j]<<"\t\tL 13:"<<label[(((dim[(a*6)+3]+dim[(a*6)+2]-i)-x)*372)+(j+y)]<<endl;
				}
				else
				{
					error++;
					//cout<<"L 6:"<<label[(i*372)+j]<<"\t\tL 13:"<<label[(((dim[(a*6)+3]+dim[(a*6)+2]-i)-x)*372)+(j+y)]<<endl;
				}
				

			}
		}
		//if(1) check
		if(error<=30)
			{
				cout<<"\tMatches found (Rotation):"<<a+1<<"\t"<<b+1<<"\n";//<<"\tError:"<<error<<endl; //////////////////////////////////////////////
				//iden++;
				//el=0;
			}
			error=0;
		
		
	}

}
void compare(int *q,int *label, int * dim, int a, int b, int &el,int op)
{
	
	if(op==0)
	{
		if(  ((dim[(a*6)+0] + dim[(a*6)+1]) == (dim[(b*6)+0] + dim[(b*6)+1])) && (dim[(a*6)+0]==dim[(b*6)+0] || dim[(a*6)+0]==dim[(b*6)+1]) && (dim[(a*6)+1]==dim[(b*6)+0] || dim[(a*6)+1]==dim[(b*6)+1]) && (abs((q[a]-q[b]))<20)  )
		{}
		else
		{
			el++;
			//cout<<"\n\nNo matches found so far are:"<<a+1<<"\t"<<b+1<<"\t err:"<<el<<"\n\n\n";
			//ret=0;
		}
	}
	if(op==1)
	{
		int x=0;
		int y=0;
		int error=0;
		int c=0;
		x=dim[(a*6)+3]-dim[(b*6)+3];
		y=dim[(a*6)+5]-dim[(b*6)+5];
		//cout<<"\n\nmatches found so far are:"<<a+1<<"\t"<<b+1<<"\n\n\n";
		if(  ((dim[(a*6)+0] + dim[(a*6)+1]) == (dim[(b*6)+0] + dim[(b*6)+1])) && (dim[(a*6)+0]==dim[(b*6)+0] || dim[(a*6)+0]==dim[(b*6)+1]) && (dim[(a*6)+1]==dim[(b*6)+0] || dim[(a*6)+1]==dim[(b*6)+1]) && (abs((q[a]-q[b]))<20)  )
		
		//if(  (dim[(a*6)+0] == dim[(b*6)+0] ) &&  (dim[(a*6)+1]==dim[(b*6)+1]) )
		
		{
			error=0;
			//cout<<"\n\nmatches found so far are:"<<a+1<<"\t"<<b+1<<"\n";
			//Upside down check:
			for(int i=dim[(a*6)+3]; i<=dim[(a*6)+2]; i++)
			{
				for(int j=dim[(a*6)+5]; j<=dim[(a*6)+4]; j++)
				{
					//cout<<"I:"<<i<<"\t J:"<<j<<"\ti:"<<(dim[(a*6)+3]+dim[(a*6)+2]-i)-x<<"\t j:"<<j+y<<endl;
					
					if((label[(i*372)+j]!=0) && (label[((dim[(a*6)+3]+dim[(a*6)+2]-i-x)*372)+(j+y)])!=0)
					{
						//cout<<"y\t";
						//cout<<"L 6:"<<label[(i*372)+j]<<"\t\tL 13:"<<label[(((dim[(a*6)+3]+dim[(a*6)+2]-i)-x)*372)+(j+y)]<<endl;
					}
					else if((label[(i*372)+j]==0) && (label[((dim[(a*6)+3]+dim[(a*6)+2]-i-x)*372)+(j+y)])==0)
					{
						//cout<<"y\t";
						//cout<<"L 6:"<<label[(i*372)+j]<<"\t\tL 13:"<<label[(((dim[(a*6)+3]+dim[(a*6)+2]-i)-x)*372)+(j+y)]<<endl;
					}
					else
					{
						error++;
						//cout<<"L 6:"<<label[(i*372)+j]<<"\t\tL 13:"<<label[(((dim[(a*6)+3]+dim[(a*6)+2]-i)-x)*372)+(j+y)]<<endl;
					}

				}
			}
			if(error<=30)
			{
				cout<<"\tMatches found (Upside down):"<<a+1<<"\t"<<b+1<<"\n";//<<"\tError:"<<error<<endl; //////////////////////////////////////////////
				iden++;
				el=0;
			}
			error=0;
		}
		
		
		
	}
	
}


int count(int * image_label, int height,int width, int BytesPerPixel,int label)
{
	//cout<<"label:"<<label<<endl;
	int count=0;
	for(int i=1;i<height-1;i++)
	{
		for(int j=1;j<width-1;j++)
		{
			if(image_label[(i*width)+j]==label)
				count++;
		}
	}
	return count;

}
int *dimension(int * image_label, int height,int width, int BytesPerPixel,int label)
{
	//cout<<"label:"<<label<<endl;
	
	int i_max=0,i_min=10000,j_max=0,j_min=100000,x_r=0,y_r=0;
	int *d= new int (i_max);
	for(int i=1;i<height-1;i++)
	{
		for(int j=1;j<width-1;j++)
		{
			if(image_label[(i*width)+j]==label)
			{
				if(i_max<i)
					i_max=i;
				if(i_min>i)
					i_min=i;
				if(j_max<j)
					j_max=j;
				if(j_min>j)
					j_min=j;

			}
		}
	}
	//cout<<"Xmax:"<<i_max<<"\tXmin:"<<i_min<<"\tYmax:"<<j_max<<"\tYmin:"<<j_min<<endl;
	

	x_r=i_max-i_min;
	y_r=j_max-j_min;
	d[0]=x_r;
	d[1]=y_r;
	d[2]=i_max;
	d[3]=i_min;
	d[4]=j_max;
	d[5]=j_min;
	return d;

}

int * label_loop(unsigned char * newimage_Fixed,int *label,int height,int width,int BytesPerPixel)
{
	
	int a[8]={0};
	int label_var=0,min=0;
	for( int i=0;i<height;i++)
	{
		for(int j=0;j<width;j++)
		{
			if(newimage_Fixed[(i*width)+j]==1)
			{
				//
				//cout<<"Non-zerp Pixel:"<<endl;
				if(label[(i*width)+(j)]!=0)
				{
					//cout<<"wowo:"<<endl;
					a[0]=label[(i*width)+(j+1)];
					a[1]=label[((i-1)*width)+(j+1)];
					a[2]=label[((i-1)*width)+(j)];
					a[3]=label[((i-1)*width)+(j-1)];
					a[4]=label[(i*width)+(j-1)];
					a[5]=label[((i+1)*width)+(j-1)];
					a[6]=label[((i+1)*width)+(j)];
					a[7]=label[((i+1)*width)+(j+1)];
					/*
					cout<<"The actual:";
					for(int k=0;k<8;k++)
						cout<<a[k]<<"\t";
					cout<<endl;
					*/ 
					min=non_zero_min(a);
					//cout<<"Min:"<<min<<endl;
					if(min == 0)
					{	min=++label_var;
						label[(i*width)+j]=min;
						//cout<<"Class label:"<<(int)label[(i*width)+j]<<endl;
					}
					else
					{
						if(newimage_Fixed[(i*width)+j]!=0)
						{
							label[(i*width)+j]=min;
						}
						if(newimage_Fixed[(i*width)+(j+1)]!=0)
						{
							label[(i*width)+(j+1)]=min;
						}
						if(newimage_Fixed[((i-1)*width)+(j+1)]!=0)
						{
							label[((i-1)*width)+(j+1)]=min;
						}
						if(newimage_Fixed[((i-1)*width)+(j+1)]!=0)
						{
							label[((i-1)*width)+(j+1)]=min;
						}
						if(newimage_Fixed[((i-1)*width)+(j)]!=0)
						{
							label[((i-1)*width)+(j)]=min;
						}
						if(newimage_Fixed[((i-1)*width)+(j-1)]!=0)
						{
							label[((i-1)*width)+(j-1)]=min;
						}
						if(newimage_Fixed[(i*width)+(j-1)]!=0)
						{
							label[(i*width)+(j-1)]=min;
						}
						if(newimage_Fixed[((i+1)*width)+(j-1)]!=0)
						{
							label[((i+1)*width)+(j-1)]=min;
						}
						if(newimage_Fixed[((i+1)*width)+(j)]!=0)
						{
							label[((i+1)*width)+(j)]=min;
						}
						if(newimage_Fixed[((i+1)*width)+(j+1)]!=0)
						{
							label[((i+1)*width)+(j+1)]=min;
						}	
					}
				}
				//cout<<"Class label:"<<(int)label[(i*width)+j]<<endl;
			}
		}
	}
	/*
	int var=-2;
	cout<<"2:"<<endl;
	for(int i=1;i<height-1;i++)
	{
		for(int j=1;j<width-1;j++)
		{	
			//cout<<"L"<<endl;
			if(label[(i*width)+j]!=0 && label[(i*width)+j]>var)
			{
				cout<<"I:"<<i<<"J:"<<j<<"Class label:"<<label[(i*width)+j]<<endl;
				var=label[(i*width)+j];
			}
		}
	}*/
	return label;
}

int checker(int num,int *a, int size)
{
	//cout<<"siz:"<<size<<endl;
	int check=1;
	/*for(int k=0;k<size;k++)
	{
		cout<<"\t"<<a[k];
	}
	cout<<endl;*/
	if(size==0)
		check=1;
	else
	{
		for(int i=0;i<size;i++)
		{
			if(a[i]!=0)
			{
				if(a[i]==num)
					check=0;
			}
		}
	}
	//cout<<check<<endl;
	return check;
}
int non_zero_min(int *a)
{
	int min=0;
	/*
	for(int k=0;k<8;k++)
		cout<<a[k]<<"\t";
	cout<<endl;
	*/
	for(int i=0;i<8;i++)
	{
		if(a[i]>0)
		{
			if(min<a[i])
			{
				min=a[i];
			}
		}
	}
	//cout<<"Return MIN:"<<min<<endl;
	return min;

}

unsigned char *fixed_Dithering(unsigned char *Image_data, int height, int width, int bytesPerPixel)
{
	unsigned char *new_image = new unsigned char [width*height*bytesPerPixel];
	int threshold_fixed=127;
	for(int i=0 ; i<height ; i++)
	{
		for(int j=0; j<width ; j++)
		{
			if(Image_data[(i*width)+j]<=threshold_fixed)
			{
				new_image[(i*width)+j]=0;
			}
			else if(Image_data[(i*width)+j]>threshold_fixed)
			{
				new_image[(i*width)+j]=1;
			}
		}
	}

	return new_image;
}
unsigned char *convert1_255(unsigned char *a,int height,int width,int BytesPerPixel)
{
	//cout<<"Covert 1 -> 255:\n";
	for(int i=0;i<height;i++)
	{
		for(int j=0;j<width;j++)
		{
			if(a[(i*width)+j]==0)
				continue;
			else
			{
				a[(i*width)+j]=255;
				//printf("%u\n",a[(i*640)+j]);
			}
		}
	}
	return a;
}
unsigned char *invert(unsigned char *a,int height,int width,int BytesPerPixel)
{
	//cout<<"Covert 1 -> 255:\n";
	for(int i=0;i<height;i++)
	{
		for(int j=0;j<width;j++)
		{
			if(a[(i*width)+j]==0)
				a[(i*width)+j]=1;
			else
			{
				a[(i*width)+j]=0;
				//printf("%u\n",a[(i*640)+j]);
			}
		}
	}
	return a;
}