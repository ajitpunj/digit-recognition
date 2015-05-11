for i=1:10
    filename=strcat('image',int2str(i),'.jpg');
    y=imread(filename);
    d1=imresize(y,[480 640]);
    bmpname=strcat('image',int2str(i),'r','.bmp');
    imwrite(d1,bmpname,'bmp');
end