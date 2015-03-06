for i=1:100
    filename=strcat('image',int2str(i),'.jpg');
    y=imread(filename);
    bmpname=strcat('image',int2str(i),'.bmp');
    imwrite(y,bmpname,'bmp');
end