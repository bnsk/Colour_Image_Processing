//This program simulates the appearance of an image under arbitary illuminant...
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include "incl.h"
#include "cipl.h"

int CIPL_rgbtoxyztoLAB(THEADER *, THEADER *);
int CIPL_LABtoxyztorgb(THEADER *, THEADER *,int ,int ,int);

int CIPL_rgbtoxyztoLAB(THEADER *it, THEADER *ot)
{
     int i, j, max;
     double R, G, B, X, Y, Z;
     ot->img.TYPE = XYZ;
     do {
	  max = HIPL_Getpart(it);
	  if (max < 0) {
	       HIPL_Error("Error in Getpart - incomplete read");
	       return(max);
	  }

	  for (i=0; i<max; i++) {
	       for (j=0; j<it->img.COLS; j++) {
		    R = it->iarr[i][j];
		    G = it->jarr[i][j];
		    B = it->karr[i][j];
		    /* iarr stores Y component scaled to range 0 - 255 */
		    Y = 0.212671 * R + 0.715160 * G + 0.072169 * B;
		    ot->iarr[i][j] = 116*(((int)((int)(Y+0.5)/100))^(1/3)) - 16;   //Gives us the corresponding L* value...                       
		    /* jarr stores X component scaled to range 0 - 255 */
		    X = 0.412453 * R + 0.357580 * G  + 0.180423 * B;
		     ot->jarr[i][j] = 500*((((int)((int)(X+0.5)/95.047))^(1/3))-(((int)(Y+0.5)/100)^(1/3))); //Gives the  a* value
		    /* karr stores Z component scaled to range 0 - 255 */
		     Z = 0.019334 * R  +0.119193 * G + 0.950227 * B;
		    ot->karr[i][j] = 200*((((int)((int)(Y+0.5)/100))^(1/3))-(((int)((int)(Z+0.5)/108.883))^(1/3)));; 
		//The D65 values are taken as X(0)=95.047,Y(0)=100,Z(0)=108.883...
       
                
	       }
	  }

	  max = HIPL_Putpart(ot);
	  if (max < 0) {
	       HIPL_Error("Error in Putpart - incomplete write");
	       return(max);
	  }
	  
	  if (ot->P.nleft == 0)
	       max = -1;
     } while (max >= 0);

     HIPL_Refresh(it, 0);
     HIPL_Refresh(ot, 0);

     return(0);     //Hence, we have now converted the image into the LAB space using the XYZ values of D65 light...
}


int CIPL_LABtoxyztorgb(THEADER *it, THEADER *ot,int X,int Y,int Z)
{
 
     int i, j, max;
     double L,A,B,X1,Z1,X2,Y2,Z2,tempR,tempG,tempB;
     ot->img.TYPE = XYZ;
     int Y1;
     do {
	  max = HIPL_Getpart(it);
	  if (max < 0) {
	       HIPL_Error("Error in Getpart - incomplete read");
	       return(max);
	  }

	  for (i=0; i<max; i++) {
	       for (j=0; j<it->img.COLS; j++) {

                  L = it->iarr[i][j];
		  A = it->jarr[i][j];
		  B = it->karr[i][j];
        //We need to convert this to the corresponding RGB space using the XYZ values passed as arguments...

            Y1=(((int)((L+16)/116))^3)*Y;
             
            X1=((int)(((A/500)+((L+16)/116)))^3)*X;
            
            Z1=((int)(((((L+16)/116)-B)/200))^3)*Z;

           
      tempR=1.055*((int)(((((int)(X1+0.5)/100)*(3.2406))+(((int)(Y1+0.5)/100)*(-1.5372))+(((int)(Z1+0.5)/100)*(-0.4986))))^(1/(int)(2.4))) - 0.055;
      tempG=1.055*((int)(((((int)(X1+0.5)/100)*(-0.9689))+(((int)(Y1+0.5)/100)*(1.8758))+(((int)(Z1+0.5)/100)*(0.0415))))^(1/(int)(2.4))) - 0.055;
      tempB=1.055*((int)(((((int)(X1+0.5)/100)*(0.0557))+(((int)(Y1+0.5)/100)*(-0.2040))+(((int)(Z1+0.5)/100)*(1.0570))))^(1/(int)(2.4))) - 0.055;

           ot->iarr[i][j]=tempR*255;
           ot->jarr[i][j]=tempG*255;
           ot->karr[i][j]=tempB*255;

        }
      }

	  max = HIPL_Putpart(ot);
	  if (max < 0) {
	       HIPL_Error("Error in Putpart - incomplete write");
	       return(max);
	  }
	  
	  if (ot->P.nleft == 0)
	       max = -1;
     } while (max >= 0);

     HIPL_Refresh(it, 0);
     HIPL_Refresh(ot, 0);

     return(0);   //Hence,we have converted back the image from LAB to rgb...   

}

main(int argc, char *argv[]) {
  int i, j, max;
  int arr[2]={1200,2400};
  THEADER *it, *ot, *temp;
  DATA *ID;
  
  HIPL_Setmemory(4096);
  
  it=(THEADER *)malloc(sizeof(THEADER));
  ot=(THEADER *)malloc(sizeof(THEADER));
  temp=(THEADER *)malloc(sizeof(THEADER));
  //temp1=(THEADER *)malloc(sizeof(THEADER));
  //temp2=(THEADER *)malloc(sizeof(THEADER));
  

 ID=(DATA *)malloc(sizeof(DATA));
  HIPL_Initialise(it,argv[1],0,0,ID);
  HIPL_Initialise(ot,NULL,1,0,ID);
  HIPL_Initialise(temp,NULL,1,0,ID);
  //HIPL_Initialise(temp1,NULL,1,0,ID);
 
 HIPL_AllocateMem(it);
  HIPL_AllocateMem(ot);
  HIPL_AllocateMem(temp);
 //HIPL_AllocateMem(temp1);

 int R=0;
 int G=0;
 int B=0;

  printf("\nPlease enter the R,G,B values of the illuminant: \n");
  scanf("\n%d,%d,%d",&R,&G,&B);

 //Now, we need to convert these R,G,B values to the corresponding X,Y,Z values...
 int X=(int)((0.412453 * R + 0.357580 * G  + 0.180423 * B)+0.5);
 int Y=(int)((0.212671 * R + 0.715160 * G + 0.072169 * B)+0.5);
 int Z=(int)(( 0.019334 * R  +0.119193 * G + 0.950227 * B)+0.5);

 //So, now we have the XYZ tri-stimulus values...

CIPL_rgbtoxyztoLAB(it,temp);  //We have the image in LAB space...

CIPL_LABtoxyztorgb(temp,ot,X,Y,Z);

 HIPL_StoreResult(argv[2], ot);


  HIPL_FreeMem(it);
  HIPL_FreeMem(ot);
  HIPL_FreeMem(temp);
  //HIPL_FreeMem(temp1);
  //HIPL_FreeMem(temp2);
  
  HIPL_Closeall(it);
  HIPL_Closeall(ot);
  HIPL_Closeall(temp);
  //HIPL_Closeall(temp1);
  //HIPL_Closeall(temp2);
  
  free(ID);
  free(it);
  free(ot);
  free(temp);
  //free(temp1);
  //free(temp2);
}
