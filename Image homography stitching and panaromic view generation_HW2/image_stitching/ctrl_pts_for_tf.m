left=imread('/Users/rickerish_nah/Documents/trial//matlab/left.png');
figure, imshow(left);
middle=imread('/Users/rickerish_nah/Documents/trial/matlab/middle.png');
right=imread('/Users/rickerish_nah/Documents/trial/matlab/right.png');
figure, imshow(middle);
hh=cpselect(right,middle);
%close(h);

x1=[fixedPoints(1,1) fixedPoints(1,2);
    fixedPoints(2,1) fixedPoints(2,2);
    fixedPoints(3,1) fixedPoints(3,2);
    fixedPoints(4,1) fixedPoints(4,2)];

x=[movingPoints(1,1) movingPoints(1,2);
    movingPoints(2,1) movingPoints(2,2);
    movingPoints(3,1) movingPoints(3,2);
    movingPoints(4,1) movingPoints(4,2)];


p=[x(1,1) x(1,2) 1 0 0 0 (-1*(x(1,1)*x1(1,1))) (-1*(x(1,2)*x1(1,1)));
   x(2,1) x(2,2) 1 0 0 0 (-1*(x(2,1)*x1(2,1))) (-1*(x(2,2)*x1(2,1)));
   x(3,1) x(3,2) 1 0 0 0 (-1*(x(3,1)*x1(3,1))) (-1*(x(3,2)*x1(3,1)));
   x(4,1) x(4,2) 1 0 0 0 (-1*(x(4,1)*x1(4,1))) (-1*(x(4,2)*x1(4,1)));
   0 0 0 x(1,1) x(1,2) 1 (-1*(x(1,1)*x1(1,2))) (-1*(x(1,2)*x1(1,2)));
   0 0 0 x(2,1) x(2,2) 1 (-1*(x(2,1)*x1(2,2))) (-1*(x(2,2)*x1(2,2)));
   0 0 0 x(3,1) x(3,2) 1 (-1*(x(3,1)*x1(3,2))) (-1*(x(3,2)*x1(3,2)));
   0 0 0 x(4,1) x(4,2) 1 (-1*(x(4,1)*x1(4,2))) (-1*(x(4,2)*x1(4,2)))];


b=[x1(1,1);x1(2,1);x1(3,1);x1(4,1);x1(1,2);x1(2,2);x1(3,2);x1(4,2)];

H=p\b;
H(9)=1;
m(1,1)=H(1);
m(1,2)=H(2);
m(1,3)=H(3);
m(2,1)=H(4);
m(2,2)=H(5);
m(2,3)=H(6);
m(3,1)=H(7);
m(3,2)=H(8);
m(3,3)=H(9);
h=inv(m);


y= ((h(1,1)*x1(1,1))+(h(1,2)*x1(1,2))+(h(1,3)))/(h(3,1)*x1(1,1))+(h(3,2)*x1(1,2))+(h(3,3));