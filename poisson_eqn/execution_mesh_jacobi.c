//
//  execution_mesh_jacobi.c
//  Electrostat
//
//  Created by Sagar Kumar on 05/12/25.
//
#include <stdlib.h>
#include <stdio.h>
#include"execution_mesh.h"
#define epsilon 8.854188e-12
#define itr_max 10000000
#include <math.h>
#include <omp.h>


/*Red points use older values of black points.
 
 Black points use updated values of red points.*/

void exe_rect_mesh_jacobi(struct mesh* m1,long m,long n,long thread_count,int enable_plot){
    
    printf("\n give the value of delta\n");
    double delta;
    scanf("%lf",&delta);
    double prev_volt=0.0;
    double residual=0.0;//defined here not in while loop to make it's scope available in condition of while loop
    long itr=0;
    double** mesh_prev;
    mesh_prev = malloc((m-2) * sizeof(double *));
#pragma omp parallel for num_threads(thread_count)
        for (long i = 0; i < m-2; ++i) {
            mesh_prev[i] = malloc((n-2) * sizeof(double));
        }
    
    double start_time12 = omp_get_wtime();
    
    do{
        residual=0.0;
       
#pragma omp parallel num_threads(thread_count)
        {
#pragma omp for collapse(2)
            //updating previous mesh
            for (int j = 1; j < n-1; ++j){
                for (int i = 1; i < m-1; ++i){
                    mesh_prev[i-1][j-1]=m1->volt[i][j];
                }
            }
       //use if and you can optimize the following cases:
#pragma omp for collapse(2)
            for (int j = 1; j < n-1; ++j){
                for (int i = 1; i < m-1; ++i){
                    if(i>1 && j>1 && i<m-2 && j<n-2){//as we don't need Boundary condition for interior points of mesh_prev case.
                        m1->volt[i][j] = 0.25*(mesh_prev[i][j-1]+mesh_prev[i-2][j-1]+mesh_prev[i-1][j-2]+mesh_prev[i-1][j]+(((delta*delta*m1->rho[i-1][j-1]))/epsilon));
                    }
                    else if (i==1 && j>1 && j<n-2 ){//leftmost column without corner
                        m1->volt[i][j] =  0.25*(mesh_prev[i][j-1]+m1->volt[i-1][j]+mesh_prev[i-1][j-2]+mesh_prev[i-1][j]+(((delta*delta*m1->rho[i-1][j-1]))/epsilon));
                    }
                    else if (i==(m-2) && j>1 && j<n-2 ){//rightmost column without corner
                        m1->volt[i][j] =  0.25*(m1->volt[i+1][j]+mesh_prev[i-2][j-1]+mesh_prev[i-1][j-2]+mesh_prev[i-1][j]+(((delta*delta*m1->rho[i-1][j-1]))/epsilon));
                    }
                    else if (i>1 && i<m-2 && j==1 ){//bottom most row without corner.
                        m1->volt[i][j] =  0.25*(mesh_prev[i][j-1]+mesh_prev[i-2][j-1]+m1->volt[i][j-1]+mesh_prev[i-1][j]+(((delta*delta*m1->rho[i-1][j-1]))/epsilon));
                    }
                    else if (i>1 && i<m-2 && j==(n-2) ){//top most row without corner.
                        m1->volt[i][j] =  0.25*(mesh_prev[i][j-1]+mesh_prev[i-2][j-1]+mesh_prev[i-1][j-2]+m1->volt[i][j+1]+(((delta*delta*m1->rho[i-1][j-1]))/epsilon));
                    }
                    else if (i==1 && j==1 ){//leftmost bottom corner
                        m1->volt[i][j] =  0.25*(mesh_prev[i][j-1]+m1->volt[i-1][j]+m1->volt[i][j-1]+mesh_prev[i-1][j]+(((delta*delta*m1->rho[i-1][j-1]))/epsilon));
                    }
                    else if (i==(m-2) && j==1 ){//rightmost bottom corner
                        m1->volt[i][j] =  0.25*(m1->volt[i+1][j]+mesh_prev[i-2][j-1]+m1->volt[i][j-1]+mesh_prev[i-1][j]+(((delta*delta*m1->rho[i-1][j-1]))/epsilon));
                    }
                    else if (i==(m-2) && j==(n-2) ){//rightmost top corner
                        m1->volt[i][j] =  0.25*(m1->volt[i+1][j]+mesh_prev[i-2][j-1]+mesh_prev[i-1][j-2]+m1->volt[i][j+1]+(((delta*delta*m1->rho[i-1][j-1]))/epsilon));
                    }
                    else if (i==1 && j==(n-2) ){//leftmost top corner
                        m1->volt[i][j] =  0.25*(mesh_prev[i][j-1]+m1->volt[i-1][j]+mesh_prev[i-1][j-2]+m1->volt[i][j+1]+(((delta*delta*m1->rho[i-1][j-1]))/epsilon));
                    }
                }
            }
            
#pragma omp for collapse(2) reduction(max:residual)
            for (int j = 1; j < n-1; ++j){
                for (int i = 1; i < m-1; ++i){
                    if(fabs(m1->volt[i][j]-mesh_prev[i-1][j-1])>residual){
                        residual=fabs(m1->volt[i][j]-mesh_prev[i-1][j-1]);
                    }
                }
            }
            
        }
        
        itr++;
    }while(residual>1e-7 && itr<itr_max);
    
    double end_time12 = omp_get_wtime();
    
    if((m+n)<=15){
        for (long j = (n-1); j >= 0; j--){
            for (long i = 0; i < m; ++i){
                printf("%lf ",m1->volt[i][j]);
            }
            printf("\n");
        }
    }
    
   
        writemeshtocsv(m1, m, n, "volt_mesh.csv");
    

    for (long i = 0; i < m-2 ; ++i) {
        free(mesh_prev[i]);
    }
    
    free(mesh_prev);
    double total_elapsed_time=end_time12-start_time12;
    printf("convergence margin is %lf \n itration(no. of sweeping) it takes for this convergence is %ld\n",residual,itr);
    printf("time to execute this iteration is %lf",total_elapsed_time);
  


}
