clang wireframePrinterTool.c -I./wireframe/ ./wireframe/wireframe.c ./windows/win32calls.c ./object/object.c -I./object/ -Iraylib -Iwindows ./raylib/arm64/raygui.a ./raylib/arm64/libraylib.a -lgdi32 -lopengl32 -lcomdlg32 -lwinmm -lole32  -O3 -o WireframePrinterTool.exe
WireframePrinterTool.exe

