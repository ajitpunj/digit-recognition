function [Y2]=part2(imagenum)
a = sprintf('image%d.jpg',imagenum);
%y=imread('image20.jpg');
y = imread(a);
%imshow(y)
greyscale=rgb2gray(y);
[height, width]=size(greyscale);
greyscaleScaled=greyscale(width/4:3*width/4,height/4:height*3/4);
%imshow(greyscale);
avgIntesity=mean2(greyscaleScaled);
bwThreshold=avgIntesity/255;
Y=im2bw(y,bwThreshold);
%imshow(Y);
%[BW, threshold]=edge(Y,'sobel');
[BW, threshold]=edge(Y,'Prewitt');
%imshow(greyscaleScaled)
%find first white pixel (binary value 1) from top center
s=size(Y);
s(1);
s(2);
i=1;
%find first white pixel
while Y(s(1)/2,i) ~=1
    i=i+5;
end
i;
%find first black pixel in white area (i value is in white area now)
while Y(s(1)/2,i) ~=0
    i=i+1;
end
i;
%i has value of side1
j=s(2);
while Y(s(1)/2,j) ~=1
    j=j-5;
end
j;
%find first black pixel in white area (i value is in white area now)
while Y(s(1)/2,j) ~=0
    j=j-1;
end
%j=right side
j;

t=1;
while Y(t,s(2)/2) ~=1
    t=t+5;
end

while Y(t,s(2)/2) ~=0
    t=t+1;
end
b=s(1);
while Y(b,s(2)/2) ~=1
    b=b-5;
end


while Y(b,s(2)/2) ~=0
    b=b-1;
end
b;
t;

Y2=Y(t-20:b+20,i-20:j+20);
figure
imshow(Y2)
%figure
%imshow(Y)
end