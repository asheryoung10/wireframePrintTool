#pragma once
#define MAX_FILENAME_SIZE 512

struct win32callsColor {
    int r, g, b;
};
int win32callsOpenFileDialogPNG(char filenameBuffer[]);

int win32callsOpenFileDialogOBJ(char filenameBuffer[]);
void win32callsSetClipboardText(const char* text);
int win32callsSaveFileDialog(char* filePath);

void win32callsGetPixelColor(int x, int y, struct win32callsColor* color);

void win32callsGetCursorPos(int* x, int*y);

bool win32callsGetLeftMouseDown();

void win32callsSetLeftMouseButtonDown();

void win32callsSetLeftMouseButtonUp();
