function [d1 d2 d3]=part3(imagenum)
    Y2=part2_used(imagenum);
    [colSize, rowSize]=size(Y2);%height width
    digits = round((rowSize-40)/(colSize-40));
    d1=Y2(:,1:rowSize/digits);
    d2=Y2(:,rowSize/digits:2*rowSize/digits);
    d3=Y2(:,2*rowSize/digits:rowSize);
    %d1=[ones([])
    d1=imresize(d1,[28 28]);
    d2=imresize(d2,[28 28]);
    d3=imresize(d3,[28 28]);
    figure;
    imshow(d1);
    figure;imshow(d2);figure;imshow(d3);
end