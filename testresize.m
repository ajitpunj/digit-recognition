% cols=int8(colSize/28);rows=int8(rowSize/28);
% resizeRow=1;resizeCol=1;
% d1_28=[];
% d1_28_final=[];
% for i=1:28
%     for j=1:28
%         d1_28=[d1_28 d1(resizeRow,resizeCol)];
%         resizeCol=resizeCol+cols;
%         j;
%     end
%     d1_28_final=[d1_28_final;d1_28];
%     resizeRow=resizeRow+rows;
%     d1_28=[];
% end

%function img_out=imageresize(img_in, rowScale, colScale);
%IMAGERESIZE Resize image 
%   IMAGERESIZE resize an image to any scale. 
%   This is a simple implementation of IMRESIZE.
%   Example:
%            imin   = imread('cameraman.tif');
%            imoutL = imageresize(imin, 2, 2); % enlarge image
%            imoutS = imageresize(imin, 0.2, 0.2) % shrink image
%
%Created by Archezus at Yahoo dot Com - 20110317
%Department of Computer Engineering
%University of Indonesia

[row col]=size(d1);
rowScale=28/row; colScale=28/col;
%for i=1:floor(row*rowScale)
for i=1:28
    for j=1:28
        %row,col for d1:
        conversionvalue(i,j)=ceil(1/colScale * j);
        img_out(i,j)=d1(ceil(1/rowScale * i), ceil(1/colScale * j));
    end
    conversionvalue(i,j+1)=ceil(1/rowScale * i);;
end

% imshow(img_in); 
% title('original image');
% figure;imshow(img_out); 
% title('resized image');
% truesize(size(img_in)); % omit this to display as it is

    