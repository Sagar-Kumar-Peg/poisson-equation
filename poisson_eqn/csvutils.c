//
//  csvutils.c
//  Electrostat
//
//  Created by Sagar Kumar on 06/12/25.
//

// csvutils.c
#include <stdio.h>
#include "setting_mesh.h"     // for struct mesh

void writemeshtocsv(struct mesh* m1, long m, long n, const char *filename)
{
    FILE *f = fopen(filename, "w");   // create / overwrite
    if (!f) {
        perror("fopen csv");
        return;
    }

    // header
    fprintf(f, "x,y,V\n");

    // i -> x index, j -> y index
    for (long i = 0; i < m; i++) {
        for (long j = 0; j < n; j++) {
            fprintf(f, "%ld,%ld,%lf\n", i, j, m1->volt[i][j]);
        }
    }

    fclose(f);
}
