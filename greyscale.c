#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

//http://dasl.mem.drexel.edu/alumni/bGreen/www.pages.drexel.edu/_weg22/colorBMP.html
//http://dasl.mem.drexel.edu/alumni/bGreen/www.pages.drexel.edu/_weg22/raster.html

//TODO: might need to add padding to top and bottom of roi

int BUFFER_SIZE = 3;//read 3 bytes
int n;
int count = 0;
int written = 0;


int finalFirstEdge, finalSecondEdge, finalLeftEdge, finalRightEdge, finalTopEdge, finalBottomEdge, finalRows,finalCols;

unsigned char smallImage[28][28];//2d global array to store resized image


long getImageInfo(FILE* inputFile, long offset, int numberOfChars)
{
    unsigned char		*ptrC;
    long			value = 0L;
    unsigned char		dummy;
    int			i;
    
    dummy = '0';
    ptrC = &dummy;
    
    fseek(inputFile, offset, SEEK_SET);
    
    for(i=1; i<=numberOfChars; i++)
    {
        fread(ptrC, sizeof(char), 1, inputFile);
      
        value = (long)(value + (*ptrC)*(pow(256, (i-1))));
    }
    return(value);
    
}


//offset is for which column to calculate
int sumOfCols(FILE *inputFile,int cols, int rows, int leftEdge, int rightEdge, int topEdge, int bottomEdge, int offset){
    int sum=0;
    int i;
    int startPixel=54+256*4;
    int startValue=ftell(inputFile);
    //printf("startValue=%d\n",startValue);
    fseek(inputFile,startPixel+(rows-bottomEdge)*cols+leftEdge+offset,SEEK_SET);
    unsigned char		*ptrC;
    for (i=0; i<bottomEdge-topEdge; i++) {
        fread(ptrC,1,1,inputFile);
        //reverse pixel
        if(*ptrC==255){
            *ptrC=0;
        }
        else
            *ptrC=255;
        sum+=*ptrC;
        fseek(inputFile,cols-1,SEEK_CUR);//go up one pixel
    }
    //fseek(inputFile,startValue,SEEK_SET);
    return sum;
}

void roiPrint(FILE *inputFile,int roiWidth, int roiHeight){
    unsigned char		*buffer;
    unsigned char		dummy;
    unsigned char roiArray[roiHeight][roiWidth/3];//row, col
    dummy = '0';
    buffer = &dummy;
    
    int startPixel=54+256*4;//start of raster data
    int startRoi=startPixel+(finalRows-finalBottomEdge)*finalCols+finalLeftEdge+finalFirstEdge;//bottom left of roi
    int digitWidth=roiWidth/3;//width of a digit
    int digitOffset=(1-1)*digitWidth;//digit offset from start of roi is width of a digit*the digit needed-1
    fseek(inputFile,(startRoi+(roiHeight*finalCols))+digitOffset,SEEK_SET);//seek to start of top of digit
    int i,j;
    for (i=0; i<roiHeight; i++) {
        for (j=0; j<roiWidth/3; j++) {
            fread(buffer,1,1,inputFile);
            if (*buffer==255) {
                *buffer=1;
            }
            //printf("%d",*buffer);
            roiArray[i][j]=*buffer;
        }
        printf("\n");
        fseek(inputFile,-finalCols-roiWidth/3,SEEK_CUR);
    }
    //printing roi works now try resize
    double rowScale=28.0/(double)roiHeight;
    double colScale=28.0/(double)digitWidth;
    int curRow=0;//=(1.0/rowScale)+1;//ceiling of 1/rowscale*i
    int curCol=0;//=(int)((1.0/colScale))+1;
    for (i=0; i<28; i++) {
        curRow=(int)(1.0/rowScale * (double)(i+1))-1;
        for (j=0; j<28; j++) {
            curCol=(int)((1.0/colScale * (double)(j+1)))-1;
            smallImage[i][j]=roiArray[curRow][curCol];
            printf("%d",smallImage[i][j]);
        }
        printf("\n");
        fseek(inputFile,-finalCols-roiWidth/3,SEEK_CUR);
    }
}

//28 by 28 image
//pass in input file, start of roi, digits, roiWidth, roiHeight
void imageResize(FILE *inputFile, int digit, int totalDigits, int roiWidth, int roiHeight){
    //seek to start of roi,
    int startPixel=54+256*4;//start of raster data
    int startRoi=startPixel+(finalRows-finalBottomEdge)*finalCols+finalLeftEdge+finalFirstEdge;//bottom left of roi
    int digitWidth=roiWidth/totalDigits;//width of a digit
    int digitOffset=(1-digit)*digitWidth;//digit offset from start of roi is width of a digit*the digit needed-1
    fseek(inputFile,(startRoi+(roiHeight*finalCols))+digitOffset,SEEK_SET);//seek to start of top of digit
    
        
    //unsigned char * buffer[1];
    unsigned char		*buffer;
    unsigned char		dummy;
    
    dummy = '0';
    buffer = &dummy;
    
    double rowScale=28.0/(double)roiHeight;
    double colScale=28.0/(double)digitWidth;
    
    //float oneOverRowScale=1/rowScale;
    printf("height=%d width=%d rowScale=%f colScale=%f\n",roiHeight,digitWidth,rowScale,colScale);
    int curRow=0;//=(1.0/rowScale)+1;//ceiling of 1/rowscale*i
    int curCol=0;//=(int)((1.0/colScale))+1;
    printf("row =%d col=%d\n",curRow,curCol);
    int oldCol=0;//=curCol;
    int oldRow=0;//=curRow;
    //printf("before seek=%d\n",ftell(inputFile));
    //fseek(inputFile,(finalCols * -curRow),SEEK_CUR);//go to curRow in roi
    //printf("after seek=%d\n",ftell(inputFile));
    int i,j;
    int blah=0;
    
    for (i=0; i<28; i++) {
        //seek to new row by going down rows (BMP format) then subtracting curRow to get to start of digit column
        curRow=(int)(1.0/rowScale * (double)(i+1))+1;
        
        fseek(inputFile,(finalCols*-(curRow-oldRow))-curCol,SEEK_CUR);
        oldRow=curRow;
        //printf("\n");
        for (j=0; j<28; j++) {
            curCol=(int)((1.0/colScale * (double)(j+1)))+1;
            //printf("curCol=%d old=%d",curCol, oldCol);
            fseek(inputFile,(curCol-oldCol),SEEK_CUR);//seek to new col
            oldCol=curCol;
            fread(buffer,1,1,inputFile);
            fseek(inputFile,-1,SEEK_CUR);
            smallImage[i][j]=*buffer;//read curRow,curCol into smallImage global 2d array
            //printf("curCol=%d ",curCol);
        }
        
        
    }
    
    //TEST PRINT
    int toPrint;
    for (i=0; i<28; i++) {
        for (j=0; j<28; j++) {
            toPrint=smallImage[i][j];
            if (toPrint==255) {
                toPrint=1;//ascii 1
            }
            
            printf("%d",toPrint);
        }
        printf("\n");
    }
    
    
}

//todo: add pass in image string
int roiDetection(){
    FILE *source;
    FILE *graySource;
    FILE *BinaryImage;
    FILE *destination;
    int red,green,blue, greypixels;
    unsigned char *pChar;
	source=fopen("image1.bmp","rb");
    graySource=fopen("image1gray.bmp","rb");
    
    unsigned char buffer[BUFFER_SIZE];//read 8 bits/1 byte
    //unsigned char header[54];
    unsigned char * header=malloc(54*sizeof(char));
    
    int cols=(int)getImageInfo(source, 18, 4);
    int rows=(int)getImageInfo(source, 22, 4);
    //int colors=(int)getImageInfo(source, 46, 4);//no color table for 24 bit image
    int bitsPerPixel=(int)getImageInfo(source, 28, 4);
    //raster data starts at bit 54 for 24 bit bmp
    fseek(source,0,SEEK_SET);//begining of file
    //int buffer[BUFFER_SIZE];
    destination = fopen("image1c.bmp", "wb");
    BinaryImage=fopen("image1binary.bmp","wb");
    n = fread(header, 1, 54, source);//read header info
    header[10]=0x36;
    header[11]=0x04;
    header[28]=8;
    fwrite(header, 1, n, destination);//write header info
    /*fseek(destination,28,SEEK_SET);//change byte 28 to 8 bit
     *pChar=8;
     fwrite(pChar,1,1,destination);
     fseek(destination,54,SEEK_SET);//seek to byte 54 to start color table*/
    printf("rows = %d cols=%d pixel bits=%d\n", rows,cols, bitsPerPixel);
    
    //unsigned char colorTable[256*4];
    unsigned char * colorTable=malloc(256*4*sizeof(char));
    fseek(graySource,54,SEEK_SET);
    n = fread(colorTable, 1, 256*4, graySource);//read color info
    fwrite(colorTable, 1, n, destination);//write color info
    fclose(graySource);
    //int graycolors=256;
    //printf("graycolors=%d\n", graycolors);
    int graypixels;
    //fseek(destination, (54 + 4*256), SEEK_SET);
    int testRow=rows;
    int testCol=0;
    
    int intensity=0;
    int pixelCount=0;
    while (!feof(source)) {
        n = fread(buffer, 1, BUFFER_SIZE, source);//read 3 bytes
        
        red=(int)buffer[2];//&0xFF;
        green=(int)buffer[1];//&0xFF;
        blue=(int)buffer[0];//&0xFF;
        
        /*
         int redmult=((int)(red*.299));//&0xFF;//lower 8 bits of red
         int greenmult=((int)(green*.587));//&0xFF;
         int bluemult=((int)(blue*.114));//&0xFF;
         */
        
        graypixels=(int)(0.2989*red + 0.587*green + 0.114*blue);
        count += n;
        /*
         buffer[0]=redmult;
         buffer[1]=greenmult;
         buffer[2]=bluemult;
         */
        //printf("reyvalue=%d\n",graypixels);
        //*pChar=graypixels;
        buffer[0]=graypixels;
        //printf("write\n");
        fwrite(buffer, 1, 1, destination);
        testCol++;
        if((testCol>=cols/4 && testCol<=cols*3/4)&&(testRow>=rows/4 && testRow<=rows*3/4)){
            pixelCount++;
            intensity+=graypixels;
        }
        else if(testCol==cols){
            testCol=0;
            testRow--;
        }
        //printf("graypixels=%d",graypixels);
        
    }
    fclose(source);
    //printf("count=%d testCol=%d testRow=%d\n", count,testCol,testRow-1);
    int averageIntensity=intensity/pixelCount;
    printf("average intensity=%d pixelCount=%d\n",averageIntensity,pixelCount);
    fclose(destination);
    source=fopen("image1c.bmp", "rb");
    int pixelStart=(int)getImageInfo(source,0x0A, 4);
    printf("pixelStart=%d\n",pixelStart);
    //unsigned char fullHeader[54+256*4];
    fseek(source,0,SEEK_SET);
    n=fread(header,1,54,source);
    fwrite(header,1,n,BinaryImage);
    n=fread(colorTable,1,256*4,source);
    fwrite(colorTable,1,n,BinaryImage);
    fseek(source,54+256*4,SEEK_SET);
    //fseek(BinaryImage,54+256*4,SEEK_SET);
    free(colorTable);
    int destPixel;
    //printf("test\n");
    
    //averageIntensity=159;
    //unsigned char binaryImageArray[rows][cols];//2d array of binary pixels
    int binaryRows=rows-1;
    int binaryCols=0;
    while(!feof(source)){
        fread(buffer, 1, 1, source);//read 1 pixel
        destPixel=buffer[0];
        //printf("destPixel=%d\n",destPixel);
        if (destPixel<averageIntensity) {
            destPixel=0;//black if less than intensity
        }
        else
            destPixel=255;//white
        
        buffer[0]=destPixel;
        fwrite(buffer,1,1,BinaryImage);
        /*
         binaryImageArray[binaryRows][binaryCols]=destPixel;
         binaryCols++;
         if(binaryCols==cols){
         binaryRows--;
         binaryCols=0;
         }
         //printf("col %d row %d",binaryCols,binaryRows);
         */
    }
    fclose(source);
    fclose(BinaryImage);
    source=fopen("image1binary.bmp", "rb");
    //destination=fopen("image1c.bmp","wb");
    fseek(source,54+256*4+cols*rows/2,SEEK_SET);//move to pixels
    //fseek(source,cols*rows/2,SEEK_CUR);//move to middle of image
    /*//debug
     unsigned char * testshit=malloc(40*cols*sizeof(char));
     n=fread(testshit,1,40*cols,source);
     fseek(destination,54+256*4,SEEK_SET);
     fwrite(testshit,1,n,destination);
     printf("n=%d\n",n);
     fseek(source,54+256*4+cols*rows/2,SEEK_SET);//move to pixels
     // /debug*/
    int ftellInt=ftell(source);
    //printf("ftell=%d\n",ftellInt);
    int leftEdge,rightEdge,topEdge,bottomEdge;
    leftEdge=0;
    do {
        fread(buffer, 1, 1, source);
        destPixel=buffer[0];
        leftEdge++;
    } while (destPixel==0);
    printf("leftEdge=%d\n",leftEdge);
    
    fseek(source,54+256*4+cols*rows/2+cols-1,SEEK_SET);
    //ftellInt=ftell(source);
    //printf("ftell=%d\n",ftellInt);
    rightEdge=0;
    do {
        fread(buffer, 1, 1, source);
        fseek(source,-2,SEEK_CUR);
        destPixel=buffer[0];
        rightEdge++;
        //ftellInt=ftell(source);
        //printf("ftell=%d\n",ftellInt);
    } while (destPixel==0);
    rightEdge=cols-rightEdge;
    printf("rightEdge=%d\n",rightEdge);
    
    fseek(source,54+256*4+cols/2,SEEK_SET);//seek to bottom middle
    //ftellInt=ftell(source);
    //printf("ftell=%d\n",ftellInt);
    bottomEdge=0;
    do {
        fread(buffer, 1, 1, source);
        fseek(source,cols-1,SEEK_CUR);//move up one pixel
        destPixel=buffer[0];
        bottomEdge++;
        //ftellInt=ftell(source);
        //printf("ftell=%d\n",ftellInt);
    } while (destPixel==0);
    //bottomEdge=rows-bottomEdge;
    printf("bottomEdge=%d\n",bottomEdge);
    
    /*
     * ADDITION AFTER WORKING!
     */
    do {
        fread(buffer, 1, 1, source);
        fseek(source,cols-1,SEEK_CUR);//move up one pixel
        destPixel=buffer[0];
        bottomEdge++;
        //ftellInt=ftell(source);
        //printf("ftell=%d\n",ftellInt);
    } while (destPixel!=0);
    bottomEdge=rows-bottomEdge;
    printf("bottomEdge2=%d\n",bottomEdge);
    
    /*
     * /ADDITION AFTER WORKING!
     */
    
    fseek(source,54+256*4+cols/2+cols*(rows-1),SEEK_SET);//seek to top middle
    //ftellInt=ftell(source);
    //printf("ftell=%d\n",ftellInt);
    topEdge=0;
    do {
        fread(buffer, 1, 1, source);
        fseek(source,(-cols)-1,SEEK_CUR);//move up one pixel
        destPixel=buffer[0];
        topEdge++;
        //ftellInt=ftell(source);
        //printf("ftell=%d\n",ftellInt);
    } while (destPixel==0);
    //bottomEdge=rows-bottomEdge;
    printf("topEdge=%d\n",topEdge);
    
    /*
     * ADDITION AFTER WORKING!
     */
    do {
        fread(buffer, 1, 1, source);
        fseek(source,(-cols)-1,SEEK_CUR);//move up one pixel
        destPixel=buffer[0];
        topEdge++;
        //ftellInt=ftell(source);
        //printf("ftell=%d\n",ftellInt);
    } while (destPixel!=0);
    //bottomEdge=rows-bottomEdge;
    printf("topEdge2=%d\n",topEdge);
    
    /*
     * ADDITION AFTER WORKING!
     */
    
    //centered to white portion of screen, now find real roi
    int roiRows=bottomEdge-topEdge;
    int roiCols=rightEdge-leftEdge;
    //works till here
    
    
    //printf("before call=%d\n",(int)ftell(source));
    //printf("sum=%d\n",sumOfCols(source,cols,rows,leftEdge,rightEdge,topEdge,bottomEdge,0));
    //printf("after call=%d\n",(int)ftell(source));
    ftell(source);
    
    //roi detection
    bool detectedFullImage=false;
    int detectWhile=1;
    int firstEdge, secondEdge;
    int sumOfCol;
    while (detectedFullImage==false) {
        sumOfCol=sumOfCols(source,cols,rows,leftEdge,rightEdge,topEdge,bottomEdge,detectWhile);
        while (sumOfCol==0) {
            detectWhile++;
            sumOfCol=sumOfCols(source,cols,rows,leftEdge,rightEdge,topEdge,bottomEdge,detectWhile);
            //printf("detectWhile=%d\n",detectWhile);
        }
        firstEdge=detectWhile;
        while (sumOfCol!=0) {
            //printf("detectWhile=%d\n",detectWhile);
            detectWhile++;
            sumOfCol=sumOfCols(source,cols,rows,leftEdge,rightEdge,topEdge,bottomEdge,detectWhile);
        }
        secondEdge=detectWhile;
        if (secondEdge-firstEdge>200) {
            detectedFullImage=true;
        }
        //printf("detectWhile=%d\n",detectWhile);
    }
    printf("first=%d second=%d\n",firstEdge,secondEdge);
    
    finalTopEdge=topEdge-10;//ADDED PADDING, REVERT IF ERRORS
    finalBottomEdge=bottomEdge+10;
    finalFirstEdge=firstEdge;
    finalSecondEdge=secondEdge;
    finalLeftEdge=leftEdge;
    finalRightEdge=rightEdge;
    finalRows=rows;
    finalCols=cols;
    
    fclose(destination);
    fclose(BinaryImage);
}

int main(void){

    roiDetection();
    int roiHeight=finalBottomEdge-finalTopEdge;
    int roiWidth=finalSecondEdge-finalFirstEdge;
    float digitsFloat=((float)(roiWidth-40)/(float)(roiHeight-40));//subtract padding for digit calc
    int digits=(int)(digitsFloat+.5);
    printf("digits=%d\n",digits);
    
    FILE *source;
    source=fopen("image1binary.bmp","rb");
    
    //before resize, try printing roi with 1's and 0's
    roiPrint(source, roiWidth,roiHeight);
    
    
    //imageResize(source,1,digits,roiWidth,roiHeight);
    
    /*
     //can't write to image!!!!!!!
    FILE *source=fopen("image1binary.bmp","rb");
    FILE *destination=fopen("image1c.bmp","wb");
    fseek(destination,54+256*4+finalCols*(finalRows-(finalBottomEdge-finalTopEdge)),SEEK_SET);
    
    fseek(source,54+256*4+(finalRows-finalBottomEdge)*finalCols+finalLeftEdge+finalFirstEdge,SEEK_SET);
    
    unsigned char		*ptrC;
    unsigned char		dummy;
    
    dummy = '0';
    ptrC = &dummy;
    
    unsigned char buffer[finalSecondEdge-finalFirstEdge];
    
    *ptrC=0;
    
    int i,j;
    
    for (i=0; i<(finalBottomEdge-finalTopEdge); i++) {
        for (j=0; j<finalCols-(finalSecondEdge-finalFirstEdge); j++) {
            //printf("tell=%d\n",ftell(destination));
            fwrite(ptrC,1,1,destination);
            *ptrC=0;
        }
        n=fread(buffer,1,(finalSecondEdge-finalFirstEdge),source);
        fwrite(buffer,1,n,destination);
        //printf("test\n");
    }
    
    fclose(destination);
     */
    return 0;
}
