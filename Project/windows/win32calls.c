#include <windows.h>
#include <stdbool.h>
#include <stdio.h>
#include "win32calls.h"
int win32callsOpenFileDialogPNG(char filenameBuffer[]) {
    static bool dpiAware = false;
    if(!dpiAware) {
        SetProcessDPIAware();
        dpiAware = true;
    }
    OPENFILENAME ofn;       // common dialog box structure
    ZeroMemory(filenameBuffer, MAX_FILENAME_SIZE);
    // Initialize OPENFILENAME
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = filenameBuffer;
    ofn.nMaxFile = MAX_FILENAME_SIZE;
    ofn.lpstrFilter = "PNG Image Files (*.png)\0*.PNG\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    return GetOpenFileName(&ofn);
}
int win32callsOpenFileDialogOBJ(char filenameBuffer[]) {
    static bool dpiAware = false;
    if(!dpiAware) {
        SetProcessDPIAware();
        dpiAware = true;
    }
    OPENFILENAME ofn;       // common dialog box structure

    ZeroMemory(filenameBuffer, MAX_FILENAME_SIZE);
    // Initialize OPENFILENAME
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = filenameBuffer;
    ofn.nMaxFile = MAX_FILENAME_SIZE;
    ofn.lpstrFilter = "OBJ Object Files (*.obj)\0*.OBJ\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    return GetOpenFileName(&ofn);
}


void win32callsSetClipboardText(const char* text) {
    // Open the clipboard
    if (!OpenClipboard(NULL)) {
        return;
    }

    // Empty the clipboard
    EmptyClipboard();

    // Get the length of the text
    size_t len = strlen(text) + 1;

    // Allocate global memory for the text
    HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len);
    if (!hMem) {
        CloseClipboard();
        return;
    }

    // Copy the text to the global memory
    memcpy(GlobalLock(hMem), text, len);
    GlobalUnlock(hMem);

    // Set the clipboard data
    SetClipboardData(CF_TEXT, hMem);

    // Close the clipboard
    CloseClipboard();
}

int win32callsSaveFileDialog(char* filePath) {
    OPENFILENAME ofn;
    ZeroMemory(&ofn, sizeof(ofn));
    ZeroMemory(filePath, MAX_FILENAME_SIZE);
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = filePath;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = MAX_FILENAME_SIZE;
    ofn.lpstrFilter = "Wireframe Text File\0*.wireframe\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    if (GetSaveFileName(&ofn)) {
        // Check if the file path already has the desired extension
        const char* dot = strrchr(filePath, '.');
        if (!dot || strcmp(dot, ".wireframe") != 0) {
            // Append the desired extension
            strcat(filePath, ".wireframe");
        }
        return TRUE;
    }
    return FALSE;
    return GetSaveFileName(&ofn);
}

void win32callsGetPixelColor(int x, int y, struct win32callsColor* color) {
    // Get the device context of the screen
    HDC hdc = GetDC(NULL);
    if (hdc == NULL) {
        return;
    }

    // Get the color of the pixel at (x, y)
    COLORREF pixelColor = GetPixel(hdc, x, y);

    // Release the device context
    ReleaseDC(NULL, hdc);

    // Extract the RGB values from the COLORREF
    color->r = GetRValue(pixelColor);
    color->g = GetGValue(pixelColor);
    color->b = GetBValue(pixelColor);
}
void win32callsGetCursorPos(int* x, int*y) {
    POINT p;
    GetCursorPos(&p);
    (*x)=p.x;
    (*y)=p.y;
}
bool win32callsGetLeftMouseDown() {
    return (GetAsyncKeyState(VK_LBUTTON) & 0x8000);
}
// Function to simulate a left mouse button down event
void win32callsSetLeftMouseButtonDown() {
    INPUT input = {0};
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

    // Send the input event
    SendInput(1, &input, sizeof(INPUT));
}

// Function to simulate a left mouse button up event
void win32callsSetLeftMouseButtonUp() {
    INPUT input = {0};
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_LEFTUP;

    // Send the input event
    SendInput(1, &input, sizeof(INPUT));
}
