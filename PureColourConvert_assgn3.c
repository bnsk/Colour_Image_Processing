//This program converts a given image into an image with 8 pure colours...
#define THRESHOLD 50

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include "incl.h"
#include "cipl.h"


int Convert_to_PureColour(THEADER *,THEADER *);

int Convert_to_PureColour(THEADER *input,THEADER *output)
{
    int i, j, max;

     do {
	  max = HIPL_Getpart(input);
	  if (max < 0) {
	       HIPL_Error("Error in Getpart - incomplete read");
	       return(max);
	  }

	  for (i=0; i<max; i++) {
	       for (j=0; j<input->img.COLS; j++) {

                        //output->iarr[i][j]=255-input->iarr[i][j];     //iarr holds the I component in the HSI space...  
                        //output->karr[i][j]=100-input->karr[i][j];     //karr holds the S component...                                         
                        //output->jarr[i][j]=((input->jarr[i][j])+180) % 360;  //jarr holds the H component...
                     
                        //The karr holds the S component...Lets take the threshold value as 50
                      int hvalue=0;
                      int a=abs(input->karr[i][j]-0);
                      int b=abs(input->karr[i][j]-60);
                      int c=abs(input->karr[i][j]-120);
                      int d=abs(input->karr[i][j]-180);
                      int e=abs(input->karr[i][j]-240);
                      int f=abs(input->karr[i][j]-300);
                    



                      if(input->karr[i][j] > THRESHOLD)
                      {

                      	if(a<b && a<c && a<d && a<e && a<f)
                      	    output->jarr[i][j]=0;
                      	else if(b<a && b<c && b<d && b<e && b<f)
                      	    output->jarr[i][j]=60;
                      	else if(c<a && c<b && c<d && c<e && c<f)
                      	    output->jarr[i][j]=120;
                      	else if(d<a && d<b && d<c && d<e && d<f)
                      	    output->jarr[i][j]=180;
                      	else if(e<a && e<b && e<c && e<d && e<f)
                      	     output->jarr[i][j]=240;
                      	else if(f<a && f<b && f<c && f<d && f<e)
                      	     output->jarr[i][j]=300;

                     
                           output->karr[i][j]=100;
                           output->iarr[i][j]=255;
                      }
                      else
                      { 
                         if((input->iarr[i][j]) > 128)
                               output->iarr[i][j]=255;
                         else
                              output->iarr[i][j]=0;
                      }
                    

                
               }
          }
    max = HIPL_Putpart(output);
	  if (max < 0) {
	       HIPL_Error("Error in Putpart - incomplete write");
	       return(max);
	  }

	  if (output->P.nleft == 0)
	       max = -1;
     } while (max >= 0);

     HIPL_Refresh(input, 0);
     HIPL_Refresh(output, 0);

     return(0);
}


main(int argc, char *argv[]) {
  int i, j, max;
  int arr[2]={1200,2400};
  THEADER *it, *ot, *temp, *temp1;
  DATA *ID;
  
  HIPL_Setmemory(4096);
  
  it=(THEADER *)malloc(sizeof(THEADER));
  ot=(THEADER *)malloc(sizeof(THEADER));
  temp=(THEADER *)malloc(sizeof(THEADER));
  temp1=(THEADER *)malloc(sizeof(THEADER));
  //temp2=(THEADER *)malloc(sizeof(THEADER));
  

 ID=(DATA *)malloc(sizeof(DATA));
  HIPL_Initialise(it,argv[1],0,0,ID);
  HIPL_Initialise(ot,NULL,1,0,ID);
  HIPL_Initialise(temp,NULL,1,0,ID);
  HIPL_Initialise(temp1,NULL,1,0,ID);
 
 HIPL_AllocateMem(it);
  HIPL_AllocateMem(ot);
  HIPL_AllocateMem(temp);
 HIPL_AllocateMem(temp1);

CIPL_rgbtohsi(it,temp);   //Now, we have the image in HSI space...

Convert_to_PureColour(temp,temp1);

CIPL_hsitorgb(temp1,ot);

 HIPL_StoreResult(argv[2], ot);

  HIPL_FreeMem(it);
  HIPL_FreeMem(ot);
  HIPL_FreeMem(temp);
  HIPL_FreeMem(temp1);
  //HIPL_FreeMem(temp2);
  
  HIPL_Closeall(it);
  HIPL_Closeall(ot);
  HIPL_Closeall(temp);
  HIPL_Closeall(temp1);
  //HIPL_Closeall(temp2);
  
  free(ID);
  free(it);
  free(ot);
  free(temp);
  free(temp1);
  //free(temp2);
}
