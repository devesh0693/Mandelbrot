CXX = g++
CXXFLAGS = -std=c++11 -O3 -fopenmp -Wall -Wextra -I"C:/SDL3-3.2.12/include" -I.
LDFLAGS = -L"C:/SDL3-3.2.12/lib/x64" -lmingw32 -lsdl3 -lm -fopenmp

TARGET = mandelbrot_explorer
SOURCE = mandelbrot_renderer.cpp

all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

parallel_mandelbrot: parallel\ Mandelbrot.cpp
	$(CXX) $(CXXFLAGS) -o parallel_mandelbrot.exe "parallel Mandelbrot.cpp" $(LDFLAGS)

clean:
	rm -f $(TARGET) parallel_mandelbrot.exe

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run parallel_mandelbrot
