#include "wireframe.h"
#include <stdio.h>
#define RRSTRINGLENGTH 512
#define FLOATSIZE 10

void saveWireframeDataToFile(char* filename, struct objPoint* points, struct objEdge* edges, struct objInfo info) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }
    fprintf(file, "WireframePrintToolData Points: %d Edges: %d\n", info.pointCount, info.edgeCount);
    // Write points to the file with fixed-width floats
    int sizeSoFar = 0;
    for (int i = 0; i < info.pointCount; i++) {
         
        fprintf(file, "%f %f %f ", points[i].x, points[i].y, points[i].z);
        sizeSoFar += FLOATSIZE * 3;
        if(sizeSoFar == 510) {
            fprintf(file, "\n"); 
            sizeSoFar = 0;
        }
    }
    if(sizeSoFar != 0) {
        fprintf(file, "\n");
    }

    // Write edges to the file
    sizeSoFar = 0;
    for (int i = 0; i < info.edgeCount; i++) {
        fprintf(file, "%d %d ", edges[i].indexA, edges[i].indexB);
        sizeSoFar += 20;
        if(sizeSoFar == 500) {
            fprintf(file, "\n");
            sizeSoFar = 0;
        }
    }

    fclose(file);
    printf("Data saved to %s successfully.\n", filename);
}



