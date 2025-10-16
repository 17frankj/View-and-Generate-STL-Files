/*
 * triangle.c
 *
 *  Created on: Aug 28, 2017
 *      Author: Thumrongsak Kosiyatrakul
 */


#ifdef __APPLE__  // include Mac OS X verions of headers

#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>

#else // non-Mac OS X operating systems

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>

#endif  // __APPLE__

#include "initShader.h"
#include "myLib.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))



vec4 vertices[6] =
{{ 0.0,  0.5,  0.0, 1.0},	// top
 {-0.5, -0.5,  0.0, 1.0},	// bottom left
 { 0.5, -0.5,  0.0, 1.0},	// bottom right
 { 0.5,  0.8,  -0.5, 1.0},	// top
 {0.9, 0.0,  -0.5, 1.0},	// bottom left
 { 0.1, 0.0,  -0.5, 1.0},	// bottom right
 };	

vec4 colors[6] =
{{1.0, 0.0, 0.0, 1.0},	// red   (for top)
 {0.0, 1.0, 0.0, 1.0},	// green (for bottom left)
 {0.0, 0.0, 1.0, 1.0},	// blue  (for bottom right)
 {0.0, 1.0, 0.0, 1.0},	// blue
 {0.0, 1.0, 0.0, 1.0},	// blue
 {0.0, 1.0, 0.0, 1.0},	// blue  
 };	

int num_vertices = 6;

mat4 my_ctm = {{1,0,0,0},{0,1,0,0}, {0,0,1,0}, {0,0,0,1}};
GLuint ctm_location;


void init(void)
{
    /*     example
    mat4 m = matrix_multi(matrix_translation(0.2, -0.4, 0.0), matrix_scaling(1.5, 1.5, 1.5));
    vertices[0] = matrix_vector_multi(m, vertices[0]);
    vertices[1] = matrix_vector_multi(m, vertices[1]);
    vertices[2] = matrix_vector_multi(m, vertices[2]);
    */ 

    GLuint program = initShader("vshader.glsl", "fshader.glsl");
    glUseProgram(program);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colors), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colors), colors);

    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *) 0);

    GLuint vColor = glGetAttribLocation(program, "vColor");
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *) sizeof(vertices));

    ctm_location = glGetUniformLocation(program, "ctm");

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glDepthRange(1,0);
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_LINE);

    glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &my_ctm);

    glDrawArrays(GL_TRIANGLES, 0, 3);
    /*
    my_ctm = identity();
    glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &my_ctm);
    glDrawArrays(GL_TRIANGLES, 3, 3);
    */
 

    glutSwapBuffers();
}

void keyboard(unsigned char key, int mousex, int mousey)
{
    if(key == 'q')
    	glutLeaveMainLoop();

    //glutPostRedisplay();
}

int mouseDown = 0;
float lastX;
float lastY;
int leftDown = 0;
int rightDown = 0;

void mouse(int button, int state, int x, int y)
{
    //printf("%d %d %d %d\n", button, state, x, y);
    float glx = (x / 400.0) - 1;
    float gly = 1 - (y / 400.0);

    // left button, move with mouse
    if(button == 0 && state == 0)
    {
        leftDown = 1;
        my_ctm = matrix_translation(glx, gly, 0.0);  // snap triangle to pointer position

        mouseDown = 1;
        lastX = glx;
        lastY = gly;

        //my_ctm = matrix_translation(glx, gly, 0.0);
    }
    // right pressed, scale to 0.5 (get larger)
    if(button == 2 && state == 0)
    {
        rightDown = 1;
        //printf("right button pressed\n");
        my_ctm = matrix_scaling(0.5, 0.5, 0.5);

        mouseDown = 1;
        lastX = glx;
        lastY = gly;

    }
    // no button, go back to center
    if(button == 0 && state == 1)
    {
        mouseDown = 0;
        leftDown = 0;
        //my_ctm = identity();
    }
    if(button == 2 && state == 1)
    {
        mouseDown = 0;
        rightDown = 0;
        my_ctm = identity();
    }
    if(leftDown && rightDown)
    {
        //printf("Both left and right buttons are pressed!\n");
        my_ctm = matrix_multi(matrix_translation(glx, gly, 0.0), matrix_scaling(0.5,0.5,0.5));
    }
    glutPostRedisplay();
}

void motion(int x, int y)
{
    if(mouseDown)
    {
        float glx = (x / 400.0f) - 1.0f;
        float gly = 1.0f - (y / 400.0f);

        float currentx = glx - lastX;
        float currenty = gly - lastY;
        if(currentx != 0 || currenty != 0)
        {
            my_ctm = matrix_multi(matrix_translation(currentx, currenty, 0.0), my_ctm);
            glutPostRedisplay();
        }
        lastX = glx;
        lastY = gly;
    }
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(100,100);
    glutCreateWindow("Triangle");
    glewInit();
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutMainLoop();

    return 0;
}
