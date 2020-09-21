LIBRARIES = -framework OpenGL -framework GLUT -lpng

EXE = output

all:
	clang++ -arch x86_64 -std=c++17 -mmacosx-version-min=10.15 -Wall -Wno-nonportable-include-path -Wno-missing-braces $(LIBRARIES) *.cpp -o $(EXE)
clean:
	-rm $(EXE)