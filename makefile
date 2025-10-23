LIBS = -lXi -lXmu -lglut -lGLEW -lGLU -lm -lGL

shapes: make_shapes.c initShader.o myLib.o stl_reader.o
	gcc -o shapes make_shapes.c initShader.o myLib.o stl_reader.o $(LIBS)

initShader.o: initShader.c initShader.h
	gcc -c initShader.c

myLib.o: myLib.c myLib.h
	gcc -c myLib.c

stl_reader.o: stl_reader.c stl_reader.h
	gcc -c stl_reader.c
