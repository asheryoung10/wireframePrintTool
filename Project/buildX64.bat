x86_64-w64-mingw32-clang wireframePrinterTool.c -I./wireframe/ ./wireframe/wireframe.c ./windows/win32calls.c ./object/object.c -I./object/ -Iraylib -Iwindows ./raylib/x64/raygui.a ./raylib/x64/libraylib.a -lgdi32 -lopengl32 -lcomdlg32 -lwinmm -lole32 -mwindows -o WireframePrinterTool.exe
WireframePrinterTool.exe
