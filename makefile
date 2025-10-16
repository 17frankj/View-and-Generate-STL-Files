LIBS = -lXi -lXmu -lglut -lGLEW -lGLU -lm -lGL

main: main.c myLib.o
	gcc -o main main.c myLib.o -lm

triangle: triangle.c initShader.o myLib.o
	gcc -o triangle triangle.c initShader.o myLib.o $(LIBS)

triangle_ctm: triangle_ctm.c initShader.o
	gcc -o triangle_ctm triangle_ctm.c initShader.o $(LIBS)

initShader.o: initShader.c initShader.h
	gcc -c initShader.c

myLib.o: myLib.c myLib.h
	gcc -c myLib.c
