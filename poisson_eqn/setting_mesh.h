//
//  Untitled.h
//  Electrostat
//
//  Created by Sagar Kumar on 03/12/25.
//

#ifndef SETTING_MESH_H
#define SETTING_MESH_H

struct mesh{
    double** volt;
    double** rho;
};
extern struct mesh m1;

void rectangular_mesh( long thread_count,int enable_plot);
void circular_mesh( long thread_count,int enable_plot);
void writemeshtocsv(struct mesh* m1, long m, long n, const char *filename);





#endif

