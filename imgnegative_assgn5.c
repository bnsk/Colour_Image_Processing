//This program implements the image negative in HSI space...
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include "incl.h"
#include "cipl.h"


int HSI_Neg(THEADER *,THEADER *);

int HSI_Neg(THEADER *input,THEADER *output)
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

                        output->iarr[i][j]=255-input->iarr[i][j];     //iarr holds the I component in the HSI space...  
                        output->karr[i][j]=100-input->karr[i][j];     //karr holds the S component...                                         
                        output->jarr[i][j]=((input->jarr[i][j])+180) % 360;  //jarr holds the H component...
                
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

 CIPL_rgbtohsi(it,temp);    //We now have the image converted into the HSI space from the RGB space and now we need the negative of this image... 

int ret=HSI_Neg(temp,temp1);
 if(ret!=0)
 {
   printf("\nError performing Negative\n");
   exit(0);
 }  
  

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
