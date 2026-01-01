//
//  Execution.c
//  Electrostat
//
//  Created by Sagar Kumar on 03/12/25.
//
#include <stdlib.h>
#include <stdio.h>
#include"execution_mesh.h"
#include"setting_mesh.h"
#include<omp.h>

struct mesh m1;
struct mesh* point=&m1;

void rectangular_mesh( long thread_count,int enable_plot){
    
    long m,n;
    printf("enter the size of 2D mesh, first number of column(no. of elements in x) then rows\n");
    scanf("%ld %ld",&m,&n);
    if (m < 3 || n < 3) {
            printf("mesh too small (m=%ld, n=%ld)\n", m, n);
            return;
        }
    printf("provide all four voltage boundary condition in clockwise starting from left boudary\n");
    double bound_left;
    double bound_bottom;
    double bound_right;
    double bound_top;
    scanf("%lf %lf %lf %lf",&bound_left,&bound_bottom,&bound_right,&bound_top);
    
    printf("\n note:-corner vales have same values as horizantal trend.\n");
    
    
    //assigning boundary conditions
    
    m1.volt = malloc(m * sizeof(double *));
    m1.rho = malloc((m-2) * sizeof(double *));
#pragma omp parallel num_threads(thread_count)
    {
#pragma omp for
        for (long i = 0; i < m; ++i) {
            m1.volt[i] = malloc(n * sizeof(double));
        }
        
#pragma omp for
        for (long i = 0; i < m-2; ++i) {
            m1.rho[i] = malloc((n-2) * sizeof(double));
        }
        
#pragma omp for
        for(long i=0;i<m;i++){
            m1.volt[i][0]=bound_bottom;
            m1.volt[i][n-1]=bound_top;//first select cursor at begginging of word you wanna change if all words gets marked press                                         //Control+Command+E
        }
        
#pragma omp for
        for(long i=1;i<n-1;i++){
            m1.volt[0][i]=bound_left;
            m1.volt[m-1][i]=bound_right;
        }
        
    }
    
    printf("do you wanna provide guess values for V for interior points (for yes type 1 else 0 to initialise all interior points to 0 )\n");
    int f1;
    scanf("%d",&f1);
    if(f1==0){
        
#pragma omp parallel for collapse(2) num_threads(thread_count)
        for (long i = 1; i < m-1; ++i){
            for (long j = 1; j < n-1; ++j){
                m1.volt[i][j] = 0.0;
            }
        }
    }
    else{
        
        for (long i = 1; i < m-1; ++i){
            for (long j = 1; j < n-1; ++j){
                scanf("%lf",&m1.volt[i][j]);
            }
        }
    }
    
    printf("\n provide charge distribution \n 1. no charge anywhere\n 2.point charge at center\n 3.uniform charge in a rectangular region\n");
    int flag;
    scanf("%d",&flag);
    
    switch (flag) {
        case 1:
            
#pragma omp parallel for collapse(2) num_threads(thread_count)
            for (long i = 0; i < m-2; ++i){
                for (long j = 0; j < n-2; ++j){
                    m1.rho[i][j] = 0.0;
                }
            }
            break;
            
        case 2:
            printf("\n give the magnitude of point charge\n");
            double Qo;
            scanf("%lf",&Qo);
            
#pragma omp parallel for num_threads(thread_count)
            for (long i = 0; i < m-2; ++i){
                for (long j = 0; j < n-2; ++j){
                    if (i == (m-2)/2 && j == (n-2)/2){
                        m1.rho[i][j] = Qo;
                    }
                    else{
                        m1.rho[i][j] = 0.0;
                    }
                }
            }
            break;
            
        case 3:
            printf("\n give the magnitude of uniform charge\n");
            double Q;
            scanf("%lf",&Q);
            
#pragma omp parallel for collapse(2) num_threads(thread_count)
            for (long i = 0; i < m-2; ++i)
                for (long j = 0; j < n-2; ++j)
                    m1.rho[i][j] = Q;
            break;
            
        default:
            
#pragma omp parallel for collapse(2) num_threads(thread_count)
            for (long i = 0; i < m-2; ++i)
                for (long j = 0; j < n-2; ++j)
                    m1.rho[i][j] = 0.0;
            break;
    }
    printf("\n wanna do in\n 1.RBGS \n 2.in jacobi(but required two mesh to store V_old of interior points.\n)");
    int f2;
    scanf("%d",&f2);
    if(f2==1){
        exe_rect_mesh_rbgs(point,m,n,thread_count, enable_plot);
    }
    else{
        exe_rect_mesh_jacobi(point,m,n,thread_count, enable_plot);
    }
    
    // Auto-run Python script
    if(enable_plot){
        printf("\nRunning plot_jacobi.py...\n");
        int status=0;
            status = system("python3 plot.py");
        if (status == -1) {
            printf("Failed to run python script!\n");
        }
    }
    
    //Freeing is extremely fast — no need to parallelize.
    
//#pragma omp parallel num_threads(thread_count)
   // {
//#pragma omp for
        for (long i = 0; i < m; ++i) {
            free(m1.volt[i]);
        }
        
//#pragma omp for
        for (long i = 0; i < m-2; ++i) {
            free(m1.rho[i]) ;
        }
 //   }
    free(m1.rho);//not wanna run by each thread but only once
    free(m1.volt);
}

void circular_mesh( long thread_count,int enable_plot){
    ///code
    //
    
    
}
