#include "win32calls.h"
#include <stdio.h>
int main() {
    
    char filename[MAX_FILENAME_SIZE];
    win32callsOpenFileDialog(filename);
    printf("%s", filename);
}
