#include <raylib.h>
#include <raygui.h>
#include <win32calls.h>
#include <stdio.h>
#include <object.h>
#include <stdlib.h>
#include <raymath.h>
#include "controlsPNG.h"
#include  <wireframe.h>
//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
#define SCREENWIDTH 800
#define SCREENHEIGHT 400
int screenWidth = SCREENWIDTH;
int screenHeight = SCREENHEIGHT;
    
enum Menu {
    MENUMAIN,
    MENUSETTINGS,
    MENUCONVERT,
    MENUIMPORT,
    MENUMOUSEBOT
};
void convertToScreenSpace(Rectangle* rectangle) {
    int originX = screenWidth/2;
    int originY = screenHeight/2;
    float xAxis = screenWidth/4.0;
    float yAxis = screenHeight/2.0;
    if(xAxis < yAxis) {
       // yAxis = xAxis; 
    }else {
        //xAxis = yAxis;
    }
    rectangle->width*=xAxis;
    rectangle->height*=yAxis;
    rectangle->x=rectangle->x*xAxis+originX-rectangle->width/2;
    rectangle->y=rectangle->y*yAxis+originY-rectangle->height/2;
     
}
bool drawBackButton(enum Menu* menu) {
    Rectangle bounds = {-1.9,-0.9,0.1,0.1}; 
    convertToScreenSpace(&bounds);
    if(GuiButton(bounds, GuiIconText(ICON_HOUSE, ""))) {
         (*menu)=MENUMAIN;
        return true;
    }
    return false;
}
int main(void)
{
    struct objPoint* points;
    struct objEdge* edges;
    struct objInfo info;
    // Initialization
    //--------------------------------------------------------------------------------------
    SetConfigFlags(FLAG_WINDOW_HIGHDPI);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "WireframeTool");
    SetWindowSize(screenWidth, screenWidth);
    enum Menu currentMenu = MENUMAIN;
    bool forceAspectRatio = true; 
    bool aspectRatioToggled = false;
    bool modelLoaded = false;

    Camera camera = { 0 };
    camera.position = (Vector3){ 0.0f, 0.0f, 10.0f };
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    QuaternionIdentity();
    Quaternion rotation = QuaternionIdentity();

    SetTargetFPS(300);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    Image image;
    image.data = (void*)CONTROLSPNG_DATA;
    image.width = CONTROLSPNG_WIDTH;
    image.height = CONTROLSPNG_HEIGHT;
    image.format = CONTROLSPNG_FORMAT;
    image.mipmaps =1;
    Texture controlsTexture = LoadTextureFromImage(image);
    char filename[MAX_FILENAME_SIZE];
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        
        if(aspectRatioToggled || (IsWindowResized() && forceAspectRatio)) {
            aspectRatioToggled = false;
            static int oldWidth = SCREENWIDTH;
            static int oldHeight = SCREENHEIGHT;
            int xFactor = GetScreenWidth() - oldWidth;
            int yFactor = GetScreenHeight() - oldHeight;
            if(xFactor+yFactor<0) {
                if(xFactor<yFactor) {
                screenWidth = GetScreenWidth()%2==0 ? GetScreenWidth() : GetScreenWidth()-1;
                SetWindowSize(screenWidth, screenWidth/2);
                }else{

                SetWindowSize(GetScreenHeight()*2, GetScreenHeight());
                }
            }else if(xFactor>yFactor) {
                printf("CHANGIN WIDTH!");
                screenWidth = GetScreenWidth()%2==0 ? GetScreenWidth() : GetScreenWidth()+1;
                SetWindowSize(screenWidth, screenWidth/2);
            }else {
                SetWindowSize(GetScreenHeight()*2, GetScreenHeight());
            }
            oldWidth = GetScreenWidth();
            oldHeight = GetScreenHeight();
        }
        screenWidth = GetScreenWidth()/GetWindowScaleDPI().x;
        screenHeight = GetScreenHeight()/GetWindowScaleDPI().y;
        GuiSetStyle(LABEL, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
        BeginDrawing();
            ClearBackground(BLACK);
            Rectangle backdrop = {0,0,4,2};
            convertToScreenSpace(&backdrop);
            DrawRectangleRec(backdrop, DARKBROWN);
            switch(currentMenu) {
                case(MENUMAIN):
                    {
                        Rectangle bounds = {-1.9,-0.9,0.1,0.1}; 
                        convertToScreenSpace(&bounds);
                        if(GuiButton(bounds, GuiIconText(ICON_EXIT, ""))) {
                            CloseWindow();
                            break;
                        }
                    }  
                    {
                        Rectangle bounds = {0,-0.9,2,1}; 
                        convertToScreenSpace(&bounds);
                        GuiLabel(bounds, "Wireframe Print Tool");  
                        
                    }   
                    {
                        Rectangle bounds = {1.9,-0.9,0.1,0.1}; 
                        convertToScreenSpace(&bounds);
                        if(GuiButton(bounds, GuiIconText(ICON_GEAR, ""))) {
                            currentMenu = MENUSETTINGS;
                        break;
                        }
                    }
                    {
                        Rectangle bounds = {-1,0,0.8,0.8}; 
                        convertToScreenSpace(&bounds);
                        if(GuiButton(bounds, "Step 1\nConvert Obj")) {
                            currentMenu = MENUCONVERT;  
                            break;
                        }
                    }
                    {
                        Rectangle bounds = {0,0,0.8,0.8}; 
                        convertToScreenSpace(&bounds);
                        if(GuiButton(bounds, "Step 2\nImport Generated\nData")) {
                            currentMenu = MENUIMPORT;
                            break;
                        }
                    }
                    {
                        Rectangle bounds = {1,0,0.8,0.8}; 
                        convertToScreenSpace(&bounds);
                        if(GuiButton(bounds, "Step 3\nRun \nAuto Clicker")) {
                            currentMenu = MENUMOUSEBOT; 
                        }
                    }
                    break;
                 
                case(MENUSETTINGS):   
                    if(drawBackButton(&currentMenu)) {
                        break;
                    }
                    {
                        Rectangle bounds = {0,-0.9,2,1}; 
                        convertToScreenSpace(&bounds);
                        GuiLabel(bounds, "Settings");  
                        
                    }  
                    {
                        Rectangle bounds = {0,-0.5,2,1}; 
                        convertToScreenSpace(&bounds);
                        GuiLabel(bounds, "Force Aspect Ratio");  
                    }  
                    {
                        Rectangle bounds = {0,-0.4,0.1,0.1}; 
                        convertToScreenSpace(&bounds);
                        bool toggled = forceAspectRatio;
                        
                        GuiCheckBox(bounds, "", &forceAspectRatio);
                         
                        if(toggled != forceAspectRatio) aspectRatioToggled = true;
                    }
                break;
                case(MENUCONVERT):
                if(drawBackButton(&currentMenu)) {
                    break;
                }
                {
                        Rectangle bounds = {-1.5,-0.7,0.5,0.5}; 
                        convertToScreenSpace(&bounds);
                        if(GuiButton(bounds, GuiIconText(ICON_FILE_OPEN, "Select\nOBJ\nFile"))) {
                            char fileLocation[MAX_FILENAME_SIZE];
                            if(win32callsOpenFileDialogOBJ(fileLocation)) {
                                
                                printf("reading obj file: %s", fileLocation);
                                getObjInfo(fileLocation, &info);
                                points = malloc(sizeof(struct objPoint) * info.pointCount);
                                edges = malloc(sizeof(struct objEdge) * info.edgeCount);
                                getObjData(fileLocation, points, edges);
                                removeDuplicateEdges(edges, &(info.edgeCount));
                                scalePointsToOne(points, info.pointCount);
                                modelLoaded = true;
                            }else {
                                printf("failed to get file?");
                            }
                             
                            break;
                        }
                 
                }
                {
                      
                    if(modelLoaded) {
                        {
                            Rectangle bounds = {1.5, -0.7, 0.5, 0.5};
                            convertToScreenSpace(&bounds);
                            if(GuiButton(bounds, GuiIconText(ICON_FILE_SAVE, "Compile\nTo\nText\nData"))) {
                                char fileLocation[MAX_FILENAME_SIZE];
                                if(win32callsSaveFileDialog(fileLocation)) {
                                    
                                    printf("writing wireframe text file: %s", fileLocation);
                                    saveWireframeDataToFile(fileLocation, points, edges, info);
                                }else {
                                    printf("failed to get file?");
                                }
                                 
                                break;
                            }
                        }
 
                        {
                            Rectangle bounds = {-1.5, -0.3, 0.1, 0.1};
                            convertToScreenSpace(&bounds);
                            if(GuiButton(bounds, GuiIconText(ICON_REDO, ""))){
                                camera.position = (Vector3){ 0.0f, 0.0f, 10.0f };
                                camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
                                camera.fovy = 45.0f;
                                camera.projection = CAMERA_PERSPECTIVE;
                                rotation = QuaternionIdentity();
                            }
                        }
                        Rectangle scissorRect = {0, 0, 1.9, 1.9};
                        convertToScreenSpace(&scissorRect);
                        Vector3 up = {0,1,0};
                        Vector3 down = {0,-1,};
                        Vector3 left = {-1,0,0};
                        Vector3 right = {1,0,0};
                        Vector3 forward = {0,0,-1};
                        Vector3 backward = {0,0,1};
                        static float speed = 3;
                        {
                            Rectangle bounds = {-1.5, 0.9, 0.5, 0.1};
                            convertToScreenSpace(&bounds);
                            GuiSlider(bounds, "Slow", "Fast", &speed, 0.1, 10);
                        }
                        if(IsKeyDown(KEY_LEFT)) {
                           rotation = QuaternionMultiply(rotation, QuaternionFromAxisAngle(up,GetFrameTime()));
                        }   
                        if(IsKeyDown(KEY_RIGHT)) {
                           rotation = QuaternionMultiply(rotation, QuaternionFromAxisAngle(down, GetFrameTime()));
                        } 
                        if(IsKeyDown(KEY_UP)) {
                           rotation = QuaternionMultiply(rotation, QuaternionFromAxisAngle(right, GetFrameTime()));
                        }   
                        if(IsKeyDown(KEY_DOWN)) {
                           rotation = QuaternionMultiply(rotation, QuaternionFromAxisAngle(left, GetFrameTime()));
                        }
                        if(IsKeyDown(KEY_E)) {
                           rotation = QuaternionMultiply(rotation, QuaternionFromAxisAngle(forward, GetFrameTime()));
                        }   
                        if(IsKeyDown(KEY_Q)) {
                           rotation = QuaternionMultiply(rotation, QuaternionFromAxisAngle(backward, GetFrameTime()));
                        }
                        if(IsKeyDown(KEY_W)) {
                            camera.position  = Vector3Add(camera.position, Vector3Scale(Vector3RotateByQuaternion(forward, rotation),speed * GetFrameTime()));
                        }
                        if(IsKeyDown(KEY_S)) {
                            camera.position  = Vector3Add(camera.position, Vector3Scale(Vector3RotateByQuaternion(backward, rotation),speed * GetFrameTime()));
                        }
                        
                        if(IsKeyDown(KEY_A)) {
                            camera.position  = Vector3Add(camera.position, Vector3Scale(Vector3RotateByQuaternion(left, rotation),speed * GetFrameTime()));
                        }
                        if(IsKeyDown(KEY_D)) {
                            camera.position  = Vector3Add(camera.position, Vector3Scale(Vector3RotateByQuaternion(right, rotation),speed * GetFrameTime()));
                        } 
                        if(IsKeyDown(KEY_SPACE)) {
                            camera.position  = Vector3Add(camera.position, Vector3Scale(Vector3RotateByQuaternion(up, rotation),speed * GetFrameTime()));
                        }
                        if(IsKeyDown(KEY_LEFT_CONTROL)) {
                            camera.position  = Vector3Add(camera.position, Vector3Scale(Vector3RotateByQuaternion(down, rotation),speed * GetFrameTime()));
                        } 

                        
                        camera.up = Vector3RotateByQuaternion(up, rotation);
                        camera.target = Vector3Add(camera.position, Vector3RotateByQuaternion(forward, rotation));
                        
                        {
                            Rectangle bounds = {-1.5, 0, 1, 0.2};
                            convertToScreenSpace(&bounds);
                            GuiLabel(bounds, TextFormat("Camera Position:\n %.3f, %.3f, %.3f\nCamera Target:\n %.3f, %.3f, %.3f",
                                                        camera.position.x, camera.position.y, camera.position.z,
                                                        camera.target.x, camera.target.y, camera.target.z));
                        }
                        {
                            Rectangle source = {0,0,controlsTexture.width,controlsTexture.height};
                            Rectangle destination = {-1.5, 0.5, 0.5, 0.75};
                            Vector2 origin = {0,0};
                            convertToScreenSpace(&destination);
                            DrawTexturePro(controlsTexture,source, destination, origin, 0, WHITE);
                        }
                        
                        DrawRectangleRec(scissorRect, GRAY);
                        BeginScissorMode(scissorRect.x, scissorRect.y, scissorRect.width, scissorRect.height);
                        BeginMode3D(camera);
                        DrawPoint3D((Vector3){0,0,0}, GREEN);
                        for(int i = 0; i < info.edgeCount; i++) {
                            struct objEdge edge = edges[i];
                            struct objPoint a = points[edge.indexA];
                            struct objPoint b = points[edge.indexB];
                            DrawLine3D((Vector3){a.x, a.y, a.z},(Vector3){b.x, b.y, b.z},RED);
                        }

                        EndMode3D();
                        EndScissorMode();

                    }
                }
                
                break;
                case(MENUIMPORT):
                if(drawBackButton(&currentMenu)) {
                    break;
                }
                break;
                case(MENUMOUSEBOT):
                
                static bool mouseBotEnabled = false;
                static bool setMouseCoords = false;
                static int xPos = -1;
                static int yPos = -1;
                if(drawBackButton(&currentMenu)) {
                    mouseBotEnabled = false;
                    break;
                }
                {
                    Rectangle bounds = {0,0,2,1};
                    convertToScreenSpace(&bounds);
                    struct win32callsColor color;
                    int x, y;
                    win32callsGetCursorPos(&x, &y);
                    win32callsGetPixelColor(xPos, yPos, &color);
                    DrawRectangleRec(bounds, (Color){color.r, color.g,color.b,255});
                    
                }
                {
                    Rectangle bounds = {-1.5, -0.7, 0.2, 0.2};
                    convertToScreenSpace(&bounds);
                    if(GuiCheckBox(bounds, "Enable\nAuto\nClicker", &mouseBotEnabled)){
                        if(xPos == -1 || yPos == -1) {
                            DrawRectangleRec(bounds, RED);
                            mouseBotEnabled = false;
                        }
                        break;
                    }
                }
                {

                    Rectangle bounds = {0, .75, 1, 0.2};
                    convertToScreenSpace(&bounds);
                    GuiCheckBox(bounds, "", &setMouseCoords);
                    Rectangle bounds2 = {0,0.75, 1, 0.2};
                    convertToScreenSpace(&bounds2);
                    GuiLabel(bounds2, "Set Position To Cursor On Click");
                    if(setMouseCoords) {
                        win32callsGetCursorPos(&xPos, &yPos);
                        if(win32callsGetLeftMouseDown()) {
                            setMouseCoords = false;
                        }
                    }

                }

                {

                    Rectangle bounds = {-1, .75, 0.4, 0.2};
                    convertToScreenSpace(&bounds);
                    GuiValueBox(bounds, "X Cord", &xPos, -1, GetMonitorWidth(GetCurrentMonitor()), false);
                }
                {
                    Rectangle bounds = {1, .75, 0.4, 0.2};
                    convertToScreenSpace(&bounds);
                    GuiValueBox(bounds, "Y Cord", &yPos, -1, GetMonitorWidth(GetCurrentMonitor()), false);

                }
                {
                        struct win32callsColor color;
                        win32callsGetPixelColor(xPos, yPos, &color);
                    if(mouseBotEnabled) {
                        if(color.r == 13 && color.g==13 && color.b==13) {
                            win32callsSetLeftMouseButtonUp();
                            printf("Up");
                        }else if( (color.r == 240) && ((color.g == 240) && (color.b == 240)) ){
                            win32callsSetLeftMouseButtonDown();
                            printf("down");
                        }
                    }
                        Rectangle bounds = {0,-0.75,1,0.4};
                        convertToScreenSpace(&bounds);
                        GuiLabel(bounds, TextFormat("I See: %d %d %d\nRefresh Rate: %f", color.r, color.g, color.b,
                                                    GetFrameTime()));
                        
                
                }
                break;
            }
        EndDrawing();
        screenWidth = GetScreenWidth();
        screenHeight = GetScreenHeight();
        //----------------------------------------------------------------------------------
    }
    free(points);
    free(edges);
    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
