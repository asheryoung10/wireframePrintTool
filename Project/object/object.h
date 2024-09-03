#pragma once
struct objInfo {
    int pointCount;
    int triangleCount;
    int edgeCount;
};
struct objPoint {
    float x;
    float y;
    float z;
};
struct objEdge {
    int indexA;
    int indexB;
};


void getObjInfo(char* filepath, struct objInfo *objInfo);

void getObjData(char* filepath, struct objPoint* points,struct objEdge* edges);

void removeDuplicateEdges(struct objEdge* edges, int* edgeCount);

void scalePointsToOne(struct objPoint* points, int pointCount);
