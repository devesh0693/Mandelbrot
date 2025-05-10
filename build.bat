@echo off
echo Building Mandelbrot Explorer (32-bit)...

:: Create lib directory if it doesn't exist
if not exist "C:\SDL2\lib\x86" mkdir "C:\SDL2\lib\x86"

:: Copy the 32-bit libraries if needed (uncomment and modify path if needed)
:: copy "path\to\your\SDL2\i686-w64-mingw32\lib\*.a" "C:\SDL2\lib\x86\"

g++ -std=c++17 -O3 -I"C:/SDL2/include" "parallel Mandelbrot.cpp" -o "parallel Mandelbrot.exe" -L"C:/SDL2/lib/x86" -lmingw32 -lSDL2main -lSDL2 -fopenmp -mwindows

if %ERRORLEVEL% EQU 0 (
    echo Build successful!
    echo Running program...
    "parallel Mandelbrot.exe"
) else (
    echo Build failed! Make sure SDL2.dll is in the same directory.
    pause
)