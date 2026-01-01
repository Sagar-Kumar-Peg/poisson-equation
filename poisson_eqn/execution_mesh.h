//
//  execution_mesh.h
//  Electrostat
//
//  Created by Sagar Kumar on 03/12/25.
//

#ifndef EXECUTION_MESH_H
#define EXECUTION_MESH_H
#include"setting_mesh.h"

void exe_rect_mesh_rbgs(struct mesh* m1,long m,long n,long thread_count,int enable_plot);
void exe_rect_mesh_jacobi(struct mesh* m1,long m,long n,long thread_count,int enable_plot);
void exe_cir_mesh(void);

#endif
