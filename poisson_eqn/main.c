//
//  main.c
//  Electrostat
//
//  Created by Sagar Kumar on 03/12/25.
//

#include <stdlib.h>
#include <stdio.h>
#include "setting_mesh.h"
#include <string.h>
int main(int argc, const char * argv[]) {
    int enable_plot = 0;
    long thread_count=1;
    if (argc==2){
        thread_count=strtol(argv[1], NULL, 10);
    }
    
    else if(argc>2){
        thread_count=strtol(argv[1], NULL, 10);
        
        // Default: plotting disabled unless user requests it
      
        
        if (strcmp(argv[2], "--plot") == 0) {
              enable_plot = 1;
          }
          else if (strcmp(argv[2], "--noplot") == 0) {
              enable_plot = 0;
          }
          else {
              printf("Unknown option: %s\n", argv[2]);
              return 1;
          }
        
    }
    
    else{
        printf("more than 1 argument is required");
        return 1;
    }
    
    int opt;
    printf("first determine the system\n 1.Rectangular \n 2.Circular\n");
    scanf("%d", &opt);
    switch (opt){
        case 1:
             rectangular_mesh(thread_count,enable_plot);
            break;
        case 2:
             circular_mesh(thread_count, enable_plot);
            break;
        default:
             rectangular_mesh(thread_count, enable_plot);
    }
    
    
    
    return EXIT_SUCCESS;
}
