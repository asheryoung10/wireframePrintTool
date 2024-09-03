
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "object.h"
#include <math.h>



void getObjInfo(char* filepath, struct objInfo *objInfo) {
    FILE *fp = fopen(filepath, "r");
    if (fp == NULL) {
        perror("Error opening file");
        return;
    }
    
    char line[256];
    objInfo->pointCount = 0;
    objInfo->triangleCount = 0;
    objInfo->edgeCount = 0;

    while (fgets(line, sizeof(line), fp)) {
        if (strncmp(line, "v ", 2) == 0) {
            objInfo->pointCount++;
        } else if (strncmp(line, "f ", 2) == 0) {
            objInfo->triangleCount++;
            // Count edges in the face
            int vertexCount = 0;
            char *token = strtok(line, " ");
            bool first = true;
            while (token != NULL) {
                if (token[0] != 'f') {
                    if(first) {
                        first = false;
                    }else {
                    vertexCount++;
                    }
                }
                token = strtok(NULL, " ");
            }
            objInfo->edgeCount += vertexCount+1;
        }
    }

    fclose(fp);
}
void getObjData(char* filepath, struct objPoint* points,struct objEdge* edges) {
    FILE *fp = fopen(filepath, "r");
    if (fp == NULL) {
        perror("Error opening file");
        return;
    }
    int currentPoint = 0;
    int currentEdge = 0;
    char line[256];
    struct objPoint point;
    while (fgets(line, sizeof(line), fp)) {
        if (strncmp(line, "v ", 2) == 0) {
            char *token = strtok(line, " ");
            int i = 0;
            
            while (token != NULL) {
                if (token[0] != 'v') {
                    float value = atof(token);
                    switch(i) {
                        case(0):
                            point.x = value;
                        break;
                        case(1):
                            point.y = value;
                        break;
                        case(2):
                            point.z = value;
                        break;
                    }
                    i++;
                }
                token = strtok(NULL, " ");
            }
            points[currentPoint] = point;
            currentPoint++;

        } else if (strncmp(line, "f ", 2) == 0) {
            // Count edges in the face
            char *token = strtok(line, " ");
            bool firstValue = true;
            int firstVal;
            int lastVal;
            int indexA;
            int indexB;
            while (token != NULL) {
                if (token[0] != 'f') {
                    int value = atoi(token) -1;
                    if(firstValue) {
                        indexA = value;
                        firstVal = value; 
                        firstValue = false;
                    }else {
                        indexB = value;
                        struct objEdge edge;
                        edge.indexA = indexA;
                        edge.indexB = indexB;
                        edges[currentEdge] = edge; 
                        indexA = indexB;
                        currentEdge++;
                        lastVal = value;
                    }
                }
                token = strtok(NULL, " ");
            }
            struct objEdge edge;
            edge.indexA = firstVal;
            edge.indexB = lastVal;
            edges[currentEdge] =  edge;
            currentEdge++;
        }
    }

    fclose(fp);
  
}
float magnitude(struct objPoint point) {
    return(sqrt(point.x*point.x+point.y*point.y+point.z+point.z));
}
void scalePoint(struct objPoint* point, float scale) {
    point->x*=scale;
    point->y*=scale;
    point->z*=scale;
}
void scalePointsToOne(struct objPoint* points, int pointCount) {
    float biggestSoFar = 0;
    int bigIndex;
    for(int i = 0; i < pointCount; i++) {
        if(magnitude(points[i]) > biggestSoFar) {
            bigIndex = i;
            biggestSoFar = magnitude(points[i]);
        }
    }
    biggestSoFar = 1.0 / biggestSoFar;
    for(int i = 0; i < pointCount; i++) {
        scalePoint(&(points[i]), biggestSoFar);     
    }

}


void removeDuplicateEdges(struct objEdge* edges, int* edgeCount) {
    for(int i = (*edgeCount)-1; i >= 0; i--) {
        struct objEdge currentEdge = edges[i];
        for(int j = i-1; j >= 0; j--) {
            if((edges[j].indexA == currentEdge.indexA && edges[j].indexB == currentEdge.indexB)
            || (edges[j].indexB == currentEdge.indexA && edges[j].indexA == currentEdge.indexB)) {
                //Remove current edge and shift higher indexed elements down;
                for(int k = i; k < (*edgeCount)-1; k++) {
                    edges[k] = edges[k+1];
                }
                (*edgeCount)--;
                break;
            }
        }
    }
}



