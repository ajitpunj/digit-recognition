function [Y2]=part2_used(imagenum)
%a = sprintf('image%d.jpg',imagenum);
y = imread(imagenum);
%y = img;
imshow(y)

greyscale=rgb2gray(y);

[height, width]=size(greyscale);
greyscaleScaled=greyscale(height/4:height*3/4,width/4:3*width/4);
%imshow(greyscale);
avgIntesity=mean2(greyscaleScaled);
bwThreshold=avgIntesity/255;
%bwThreshold=.5;
Y=im2bw(y,bwThreshold);
%imshow(Y);
%[BW, threshold]=edge(Y,'sobel');
%[BW, threshold]=edge(Y,'Prewitt');
%imshow(greyscaleScaled)
%find first white pixel (binary value 1) from top center
s=size(Y);
s(1);
s(2);
i=1;
%find first white pixel
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

%top and bottom found, find l and r edges
while Y(s(1)/2,i) ~=1
    i=i+5;
end
i

%find first black pixel in white area (i value is in white area now)
% while Y(s(1)/2,i) ~=0
%     i=i+1;
% end
% i;
%i has value of side1
j=s(2);
while Y(s(1)/2,j) ~=1
    j=j-5;
end
j;
%find first black pixel in white area (i value is in white area now)
% while Y(s(1)/2,j) ~=0
%     j=j-1;
% end
% firstRight=j;
% j;
% %left and right detected, cut out noise on right
% while Y(b,j) ~=1
%     j=j-1;
% end
% noiseWidth=j-firstRight
% if noiseWidth<=10
%     noiseEdge=j;
%     while Y(t,j-5)~=1
%         j=j-1;
%     end
% end
%j=right side
cropped=Y(t:b,i:j);

invertCropped=imcomplement(cropped);
%imshow(invertCropped);
detectedFullImage=false;
detectWhile=1;
 while detectedFullImage==false
    sumOfCols=sum(invertCropped,1);
    sumOfCols(1,704);
    while sumOfCols(1,detectWhile)==0
        sumOfCols(1,detectWhile);
        detectWhile=detectWhile+1;
    end
    firstEdge=detectWhile;
    while sumOfCols(1,detectWhile)~=0
        sumOfCols(1,detectWhile);
        detectWhile=detectWhile+1;
    end
    secondEdge=detectWhile;
%     detectedFullImage=true;
%     while sum(invertCropped(:,detectWhile),1)==0
%         sum(invertCropped(:,detectWhile));
%         detectWhile=detectWhile+1;
%     end
%     firstEdge=detectWhile;
%     while sum(invertCropped(:,detectWhile))~=0
%         detectWhile=detectWhile+1;
%     end
%     secondEdge=detectWhile;

    if secondEdge-firstEdge>30
        detectedFullImage=true;
    end
end
 %i=firstEdge;j=secondEdge;

% Y2=cropped(:,i:j);
Y2=Y(t-10:b+10,i+firstEdge:i+secondEdge);
%figure
%imshow(Y2)

end
