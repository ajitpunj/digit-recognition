function bmpDigit=saveDigitBMP(digit)
for i=1:28
    for j=1:28
        if digit(i,j)==1
            bmpDigit(i,j)=255;
        else
            bmpDigit(i,j)=0;
        end
    end
end
imwrite(bmpDigit,s'image128.bmp','bmp');