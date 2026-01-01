//  execution_2D_mesh.c
//  Electrostat
//  Created by Sagar Kumar on 03/12/25.

#include <stdlib.h>
#include <stdio.h>
#include"execution_mesh.h"
#define epsilon 8.854188e-12
#define itr_max 10000000
#include <math.h>
#include <omp.h>
#include<time.h>


/*Red points use older values of black points.
 
 Black points use updated values of red points.*/

void exe_rect_mesh_rbgs(struct mesh* m1,long m,long n,long thread_count,int enable_plot){
    
    printf("\n give the value of delta\n");
    double delta;
    scanf("%lf",&delta);
    double prev_volt=0.0;
    double maxdiff=0.0;//defined here not in while loop to make it's scope available in condition of while loop
    double maxdiff_red=0.0,maxdiff_black=0.0;
    long itr=0;
    
    double start_time1 = omp_get_wtime();
    do{
        maxdiff=0.0;/*if not redefine after every iteration,the once maxdiff becomes large in an early iteration it stays large forever and the while condition                   never becomes false.*/
        maxdiff_red = 0.0;
        maxdiff_black = 0.0;
        
#pragma omp parallel num_threads(thread_count) private(prev_volt)
{
    double local_diff=0.0;
  //  double /*local_maxdiff_per_thread*/=0.0;

#pragma omp for collapse(2) reduction(max:maxdiff_red)
    for (int j = 1; j < n-1; ++j){
        for (int i = 1; i < m-1; ++i){
            
            if((i+j)%2==0){
                
                prev_volt= m1->volt[i][j];
                m1->volt[i][j] = 0.25*(m1->volt[i+1][j]+m1->volt[i-1][j]+m1->volt[i][j-1]+m1->volt[i][j+1]+(((delta*delta*m1->rho[i-1][j-1]))/epsilon));
                local_diff=fabs(m1->volt[i][j]-prev_volt);
                
                if(local_diff>/*local_maxdiff_per_thread*/maxdiff_red){
                    
                    /*local_maxdiff_per_thread*/maxdiff_red=local_diff;
                }
            }
        }
    } //if I used #pragma omp parallel for ..., then i have to give this brace after critical,(increases time complexity),so that each thread runs below if statement --> an advantage of using #pragma omp parallel num_threads... struct(without for).
    
 //   maxdiff_red=/*local_maxdiff_per_thread*/;
    /*
#pragma omp critical
{
                    if(local_maxdiff_per_thread>maxdiff_red){

                            maxdiff_red=local_maxdiff_per_thread;//by this way we find maximum deviation of voltage, out of (m-1)X(n-1) point,from its previous value
                        }
}
     */
//#pragma omp barrier
    
    local_diff=0.0;
  //  /*local_maxdiff_per_thread*/=0.0;
    
#pragma omp for collapse(2) reduction(max:maxdiff_black)
    for (int j = 1; j < n-1; ++j){
        
        for (int i = 1; i < m-1; ++i){
            
            if((i+j)%2==1){
                
                prev_volt= m1->volt[i][j];
                m1->volt[i][j] = 0.25*(m1->volt[i+1][j]+m1->volt[i-1][j]+m1->volt[i][j-1]+m1->volt[i][j+1]+(((delta*delta*m1->rho[i-1][j-1]))/epsilon));
                local_diff=fabs(m1->volt[i][j]-prev_volt);
                
                if(local_diff>/*local_maxdiff_per_thread*/maxdiff_black){
                    
                    /*local_maxdiff_per_thread*/maxdiff_black=local_diff;
                }
            }
        }
    }
 //   maxdiff_black=/*local_maxdiff_per_thread*/;
  /*
#pragma omp critical
{
    if(local_maxdiff_per_thread>maxdiff_black){
        maxdiff_black=local_maxdiff_per_thread;//by this way we find maximum deviation of voltage, out of (m-1)X(n-1) point,from its previous value
    }
}
    */
}
        if(maxdiff_black < maxdiff_red){
            maxdiff=maxdiff_red;
        }
        else{
            maxdiff=maxdiff_black;
        }
        
        itr++;
    }while(maxdiff>1e-7 && itr<itr_max);
    
    double end_time1 = omp_get_wtime();
    
    if((m+n)<=15){
        for (long j = (n-1); j >= 0; j--){
            for (long i = 0; i < m; ++i){
                printf("%lf ",m1->volt[i][j]);
            }
            printf("\n");
        }
    }
    printf("convergence margin is %lf \n itration(no. of sweeping) it takes for this convergence is %ld\n",maxdiff,itr);
    printf("time to execute this iteration is %lf",end_time1-start_time1);
    
   
        writemeshtocsv(m1, m, n, "volt_mesh.csv");
    

}
