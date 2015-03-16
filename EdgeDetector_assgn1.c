//This program implements an Edge Detector with a 3*3 neighbourhood...


#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include "incl.h"
#include "cipl.h"


 THEADER *it, *ot, *temp, *temp1;
  DATA *ID;

int *findrange(int,int);

int main(int argc, char *argv[]) {


 int i,j,threshold1,threshold2,threshold3;  
 int *ran;
 
  HIPL_Setmemory(4096);
  
  it=(THEADER *)malloc(sizeof(THEADER));
  ot=(THEADER *)malloc(sizeof(THEADER));
 
   ID=(DATA *)malloc(sizeof(DATA));

    HIPL_Initialise(it,argv[1],0,0,ID);
    HIPL_Initialise(ot,NULL,1,0,ID);

   HIPL_AllocateMem(it);
   HIPL_AllocateMem(ot);
 
  printf("\nPlease enter 3 scalars for the threshold: \n");
  scanf(" %d",&threshold1);

 int scalar;

   for (i=0; i<= it->img.ROWS-3; i++)  
   {
	for (j=0; j<= it->img.COLS-3; j++)
        {	
		if ((HIPL_Getpart(it)) < 0) 
                {
			printf("Sorry!!!Error in reading image...\n");
				exit(1);
		}


                //Now, we need to find the average colour for the particular image segment corresponding to the mask...
                //Let us pass the indices i and j to the function for calculating the avg for the  mask (i,j) to (i+3,j+3) 
                ran=findrange(i,j);

                //Now, we need to compare this with the user entered threshold...
               if(ran[0]>threshold1)
                      ot->iarr[i+3/2][j+3/2]=255;
               else
                      ot->iarr[i+3/2][j+3/2]=0;

               
               if(ran[1]>threshold2)
                      ot->jarr[i+3/2][j+3/2]=255;
               else
                      ot->jarr[i+3/2][j+3/2]=0;
               

                  if(ran[2]>threshold3)
                      ot->karr[i+3/2][j+3/2]=255;
                  else
                      ot->karr[i+3/2][j+3/2]=0;


               if (HIPL_Putpart(ot) < 0) 
               {
				printf("Sorry!!!Error in writing image...\n");
				exit(1);
		}

	}
   }
        
	HIPL_StoreResult(argv[2], ot);
	HIPL_FreeMem(it);
	HIPL_FreeMem(ot);
	HIPL_Closeall(it);
	HIPL_Closeall(ot);
	exit(0);

}






int *findrange(int x,int y)
{
  //int c1=x+3/2;
  //int c2=y+3/2;   //These give us the center of the mask...
  int i,j;
  int mask[3][3][3]; //This holds the values for the three components of each of the vectors... 
  int a=0;
  int b=0;
  int sum[3]={0,0,0};
  int avg[3]={0,0,0};

   for(i=0;i<=2;i++)
   {
   	for(j=0;j<=2;j++)
        {
        	 mask[a][b][0] += it->iarr[x+i][y+j];
                 mask[a][b][1] += it->jarr[x+i][y+j];
                 mask[a][b][2] += it->karr[x+i][y+j]; 


                 sum[0] += mask[a][b][0];
                 sum[1] += mask[a][b][1];
                 sum[2] += mask[a][b][2];

                 b++;  //increasing the column coeff....
         }
           a++;
           b=0;
  }
  
  avg[0]=sum[0]/9;
  avg[1]=sum[1]/9;
  avg[2]=sum[2]/9;

   //Now , we need to sort these based on their distances from the average...

 int max=0;
 int min=0;
 int dist,r1,r2,s1,s2;
 int *range;
 int x1,y1,z1;
 range=malloc(3*sizeof(int));

 for(i=0;i<=2;i++)
 {
 	for(j=0;j<=2;j++)
        {
             x1=mask[i][j][0]-avg[0];
             y1=mask[i][j][1]-avg[1];
             z1=mask[i][j][2]-avg[2];

             dist= x1*x1 + y1*y1 + z1*z1;

             if(dist < min)
             { 
        	min=dist;
                r1=i;
                r2=j;
             }
             if(dist > max)
             {
                 max=dist;
                 s1=i;
                 s2=j;
             }
         }
  } //end for i;

  //NOw we have the vectors i.e. colors with highest and lowest distances from the average and we have to find the range...
  
  range[0]=mask[s1][s2][0] - mask[r1][r2][0];
  range[1]=mask[s1][s2][1] - mask[r1][r2][1];
  range[2]=mask[s1][s2][2] - mask[r1][r2][2];

 return range;

}








