//
//  128image.c
//  
//
//  Created by Ajit P on 3/4/15.
//
//

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

unsigned char smallImageReshape[28*28];//784 linear array
unsigned char smallImage[28][28];
double finalB1L1[200][1];
double finalB1L2[201][1];
double finalSoftmaxTheta[10][200];
double finalW1L1[200][784];
double finalW1L2[200][200];


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

void reshape(void){
    //use global 28 by 28 to convert to 784 array
    int globalIndex=0;
    int i,j;
    for (i=0; i<28; i++) {
        for (j=0; j<28; j++) {
            smallImageReshape[globalIndex]=smallImage[j][i];//take columnwise for reshape
            //printf("%d\n",(int)smallImageReshape[globalIndex]);
            globalIndex++;
            
        }
    }
}

int readfinalB1L1(void){
    char buffer[1024] ;
    char *record,*line;
    int i=0,j=0;
    FILE *fstream = fopen("finalB1L1.csv","r");
    if(fstream == NULL)
    {
        printf("\n file opening failed ");
        return -1 ;
    }
    while((line=fgets(buffer,sizeof(buffer),fstream))!=NULL)
    {
        record = strtok(line,",");
        //while(record != NULL)
        //{
            //printf("record : %s",record) ;    //here you can put the record into the array as per your requirement.
        finalB1L1[i][0] = atof(record) ;
        //printf("record: %f\n",finalB1L1[i][1]);
        //}
        i++ ;
    }
    return 0;
}

int readfinalB1L2(void){
    char buffer[1024] ;
    char *record,*line;
    int i=0,j=0;
    FILE *fstream = fopen("finalB1L2.csv","r");
    if(fstream == NULL)
    {
        printf("\n file opening failed ");
        return -1 ;
    }
    while((line=fgets(buffer,sizeof(buffer),fstream))!=NULL)
    {
        record = strtok(line,",");
        //while(record != NULL)
        //{
        //printf("record : %s",record) ;    //here you can put the record into the array as per your requirement.
        finalB1L2[i][0] = atof(record) ;
        //printf("record: %f\n",finalB1L1[i][1]);
        //}
        i++ ;
    }
    return 0;
}

int readfinalSoftmaxTheta(void){
    char buffer[10*200] ;
    char *record,*line;
    int i=0,j=0;
    int cols;
    FILE *fstream = fopen("finalSoftmaxTheta.csv","r");
    if(fstream == NULL)
    {
        printf("\n file opening failed ");
        return -1 ;
    }
    while((line=fgets(buffer,sizeof(buffer),fstream))!=NULL)
    {
        
        //while(record != NULL)
        //{
        //printf("record : %s",line) ;
        record = strtok(line,",");
        for (cols=0; cols<200; cols++) {
            
            
            //printf("record : %s",record) ;    //here you can put the record into the array as per your requirement.
            finalSoftmaxTheta[i][cols] = atof(record) ;
            //printf("%f ",finalSoftmaxTheta[i][cols]);
            record = strtok(NULL,",");
        //}
        }
        //printf("\n");
        i++ ;
        //j=0;
    }
    return 0;
}

int readfinalW1L1(void){
    char buffer[10*784] ;
    char *record,*line;
    int i=0,j=0;
    int cols;
    FILE *fstream = fopen("finalW1L1.csv","r");
    if(fstream == NULL)
    {
        printf("\n file opening failed ");
        return -1 ;
    }
    while((line=fgets(buffer,sizeof(buffer),fstream))!=NULL)
    {
        
        //while(record != NULL)
        //{
        //printf("record : %s",line) ;
        record = strtok(line,",");
        for (cols=0; cols<784; cols++) {
            
            
            //printf("record : %s",record) ;    //here you can put the record into the array as per your requirement.
            finalW1L1[i][cols] = atof(record) ;
            //printf("%f ",finalW1L1[i][cols]);
            record = strtok(NULL,",");
            //}
        }
        //printf("\n\n\n\n\n\n");
        i++ ;
        //j=0;
    }
    return 0;
}

int readfinalW1L2(void){
    char buffer[10*200] ;
    char *record,*line;
    int i=0,j=0;
    int cols;
    FILE *fstream = fopen("finalW1L2.csv","r");
    if(fstream == NULL)
    {
        printf("\n file opening failed ");
        return -1 ;
    }
    while((line=fgets(buffer,sizeof(buffer),fstream))!=NULL)
    {
        
        //while(record != NULL)
        //{
        //printf("record : %s",line) ;
        record = strtok(line,",");
        for (cols=0; cols<200; cols++) {
            
            
            //printf("record : %s",record) ;    //here you can put the record into the array as per your requirement.
            finalW1L2[i][cols] = atof(record) ;
           // printf("%f ",finalW1L2[i][cols]);
            record = strtok(NULL,",");
            //}
        }
       // printf("\n\n\n\n\n\n");
        i++ ;
        //j=0;
    }
    return 0;
}

void findDigit(void){
    double finalW1L1TimesDigit[200][1];
    double z1[200][1];
    double z2[200][1];
    double z3[10][1];
    //200 rows by 784 cols, digit is 784 rows, 1 col
    int i,j;
    double sum=0.0;
    double test;
    for (i=0; i<200; i++) {//change back to 200!
        for (j=0; j<784; j++) {
            if (smallImageReshape[j]==0) {
                test=0.0;
            }
            else
                test=1.0;
            //mult and add digit[j] and finalw1l1[i][j]
            //printf("%f ",finalW1L1[i][j]);
            //printf("%d",(int)test);
            sum=sum+(test*finalW1L1[i][j]);
        }
        //store sum to finalw1l1timesdigit
        //printf("%f\n",sum);
        finalW1L1TimesDigit[i][0]=sum;
        sum=0.0;
    }
    for (i=0; i<200; i++) {
        z1[i][0]=1.0/(1.0+exp(-(finalW1L1TimesDigit[i][0]+finalB1L1[i][0])));
        //printf("%f\n",z1[i][0]);
    }
    //z2
    sum=0.0;
    for (i=0; i<200; i++) {//change back to 200!
        for (j=0; j<200; j++) {
            
            //mult and add digit[j] and finalw1l1[i][j]
            //printf("%f ",finalW1L1[i][j]);
            //printf("%d",(int)test);
            sum=sum+(z1[j][0]*finalW1L2[i][j]);
        }
        //store sum to finalw1l1timesdigit
        //printf("%f\n",sum);
        z2[i][0]=sum;
        sum=0.0;
    }
    for (i=0; i<200; i++) {
        z2[i][0]=1.0/(1.0+exp(-(z2[i][0]+finalB1L2[i][0])));
        //printf("%f\n",z2[i][0]);
    }
    //z3
    sum=0.0;
    for (i=0; i<10; i++) {//change back to 200!
        for (j=0; j<200; j++) {
            
            //mult and add digit[j] and finalw1l1[i][j]
            //printf("%f ",finalW1L1[i][j]);
            //printf("%d",(int)test);
            sum=sum+(z2[j][0]*finalSoftmaxTheta[i][j]);
        }
        //store sum to finalw1l1timesdigit
        //printf("%f\n",sum);
        z3[i][0]=sum;
        sum=0.0;
    }
    double max=-100000000.0;
    int number=0;
    for (i=0; i<10; i++) {
        //z2[i][0]=1.0/(1.0+exp(-(z2[i][0]+finalB1L2[i][0])));
        //printf("%f\n",z3[i][0]);
        if (z3[i][0]>max) {
            max=z3[i][0];
            number=i+1;
        }
    }
    printf("digit= %d\n",number);
}

int main(void){
    FILE *source;
    source=fopen("image128.bmp","rb");
    int cols=(int)getImageInfo(source, 18, 4);
    int rows=(int)getImageInfo(source, 22, 4);
    int bitsPerPixel=(int)getImageInfo(source, 28, 4);
    int colors=(int)getImageInfo(source, 46, 4);
    fseek(source,0,SEEK_SET);//begining of file
    fseek(source,54+256*4,SEEK_SET);//seek to start of pixels
    //printf("cols=%d rows=%d bits=%d colors=%d\n",cols, rows, bitsPerPixel, colors);
    int i,j;
    int printInt=0;
    int converted;
    unsigned char buffer[28];
    //works to get 2d array 
    for (i=28; i>0; i--) {
        fread(buffer,1,28,source);
        for (j=0; j<28; j++) {
            if (buffer[j]==255) {
                converted=1;
            }
            else
                converted=0;
            smallImage[i-1][j]=converted;
        }
    }
    /* semi works for reading 28 by 28 image
    for (i=28; i>0; i--) {
        printInt=28*i-28-1;
        fread(buffer,1,28,source);//read 28 bytes starting from bottom left
        for (j=0; j<28; j++) {//store 28 bytes into array, starting at highest position
            if (buffer[j]==255) {
                converted=1;
            }
            else
                converted=0;
            smallImage[printInt]=converted;
            printInt++;
        }
    }
     */
    printInt=0;
    int toPrint;
    reshape();
    readfinalB1L1();
    readfinalB1L2();
    readfinalSoftmaxTheta();
    readfinalW1L1();
    readfinalW1L2();
    findDigit();
    
    //finished until w1l1*digit, need lots more
    
    //print final weight arrays and binary image
    /*for (i=0; i<200; i++) {
        printf("%f\n",finalB1L2[i][0]);
    }*/
    /*
    for (i=0; i<28; i++) {
        for (j=0; j<28; j++) {
            
            printf("%d",smallImage[i][j]);
            printInt++;
        }
        printf("\n");
    }
     */
    /*for (i=0; i<28*28; i++) {
        if (smallImage[i]==255) {
            toPrint=1;
        }
        else
            toPrint=0;
        printf("%d",toPrint);
    }*/
    
    
    
}
