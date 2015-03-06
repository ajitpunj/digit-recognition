function greyImage=greyscalenew(y)
    %y = imread('image1.jpg'); %red green blue
    greyImage=[];
    s=size(y); %size of image 1=height, 2 = width, 3= pixel depth
    for i=1:s(1); % loop through rows of pixels
        for j=1:s(2);
            red=y(i,j,1); green=y(i,j,2); blue=y(i,j,3);
            greyImage(i,j)=.299*red+.587*green+.114*blue;
            
        end
    end
    greyImage=uint8(greyImage);
    imshow(greyImage);
end