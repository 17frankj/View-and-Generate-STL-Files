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



vec4 vertices[1000] =
{{ 0.0,  0.5,  0.0, 1.0},	// top
 {-0.5, -0.5,  0.0, 1.0},	// bottom left
 { 0.5, -0.5,  0.0, 1.0},	// bottom right
 };	

vec4 colors[1000] =
{{1.0, 0.0, 0.0, 1.0},	// red   (for top)
 {0.0, 1.0, 0.0, 1.0},	// green (for bottom left)
 {0.0, 0.0, 1.0, 1.0},	// blue  (for bottom right)
 };	

int num_vertices = 1000;

mat4 my_ctm = {{1,0,0,0},{0,1,0,0}, {0,0,1,0}, {0,0,0,1}};
GLuint ctm_location;

vec4 pick_color(int random_num)
{

    // red
    if(random_num <= 2)
    {
        return (vec4){1.0, 0.0, 0.0, 1.0};
        
    }
    // green
    else if(random_num <= 5)
    {
        return (vec4){0.0, 1.0, 0.0, 1.0};
    }
    // blue
    else if(random_num <= 7)
    {
        return (vec4){0.0, 0.0, 1.0, 1.0};
    }
    // yellow
    else if(random_num <= 9)
    {
        return (vec4){1.0, 1.0, 0.0, 1.0};
    }
    // cyan
    else if(random_num <= 10)
    {
        return (vec4){0.0, 1.0, 1.0, 0.5};
    }
}

void make_first_triangle_sphere(void)
{
    vertices[0] = (vec4){-0.0625,0.0625,0.0,1.0};  // top
    vertices[1] = (vec4){-0.0625,-0.0625,0.0,1.0};  // bottom left
    vertices[2] = (vec4){0.0625,-0.0625,0.0,1.0};  // bottom right
    vertices[3] = (vec4){0.0625,0.0625,0.0,1.0};  // top right
    vertices[4] = (vec4){-0.0625,0.0625,0.0,1.0};  // top left
    vertices[5] = (vec4){0.0625,-0.0625,0.0,1.0};  // bottom right
    mat4 S = matrix_scaling(1.0, 1.7, 1.0);
    for (int i = 0; i < 6; i++)
    {
        vertices[i] = matrix_vector_multi(S, vertices[i]);
    }
    colors[0] = (vec4){1.0, 0.0, 0.0, 1.0}; // red
    colors[1] = (vec4){1.0, 0.0, 0.0, 1.0}; // red
    colors[2] = (vec4){1.0, 0.0, 0.0, 1.0}; // red
    colors[3] = (vec4){0.0, 0.0, 1.0, 1.0};	// blue
    colors[4] = (vec4){0.0, 0.0, 1.0, 1.0};	// blue
    colors[5] = (vec4){0.0, 0.0, 1.0, 1.0};	// blue
}

void rotate_z_sphere(void)
{
    // base triangle (copy original vertices)
    vec4 base0 = vertices[0];
    vec4 base1 = vertices[1];
    vec4 base2 = vertices[2];
    vec4 base3 = vertices[3];
    vec4 base4 = vertices[4];
    vec4 base5 = vertices[5];

    float translate1 = 0.125;
    int vert = 0;
    for(int i = 0; i < 32; i++)
    {
        mat4 m = matrix_translation(1, 0.0, 0.0);
        mat4 m_r = rotate_z(11.25*i);
        mat4 m_d = matrix_multi(m_r, m);

        //vertices[vert] = matrix_vector_multi(m, vertices[0]);
        vertices[vert] = matrix_vector_multi(m_d, base0);

        //vertices[vert+1] = matrix_vector_multi(m, vertices[1]);
        vertices[vert+1] = matrix_vector_multi(m_d, base1);

        //vertices[vert+2] = matrix_vector_multi(m, vertices[2]);
        vertices[vert+2] = matrix_vector_multi(m_d, base2);

        //vertices[vert+3] = matrix_vector_multi(m, vertices[3]);
        vertices[vert+3] = matrix_vector_multi(m_d, base3);

        //vertices[vert+4] = matrix_vector_multi(m, vertices[4]);
        vertices[vert+4] = matrix_vector_multi(m_d, base4);

        //vertices[vert+5] = matrix_vector_multi(m, vertices[5]);
        vertices[vert+5] = matrix_vector_multi(m_d, base5);

        int random1 = 1 + rand() %10; // make random number between 1 and 10
        int random2 = 1 + rand() %10; // make random number between 1 and 10

        vec4 random_color1 = pick_color(random1);
        vec4 random_color2 = pick_color(random2);

        colors[vert] = random_color1;
        colors[vert+1] = random_color1;
        colors[vert+2] = random_color1;
        colors[vert+3] = random_color2;
        colors[vert+4] = random_color2;
        colors[vert+5] = random_color2;

        vert = vert + 6;
        translate1 = translate1 + 0.125;
    }
}

void make_Sphere(void)
{
    int random = 1 + rand() %10; // make random number between 1 and 10
    vec4 random_color = pick_color(random);

    make_first_triangle_sphere();
    rotate_z_sphere();
}

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

    glDrawArrays(GL_TRIANGLES, 0, num_vertices);
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
    glutCreateWindow("Project 1");

    make_Sphere();

    glewInit();
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    //glutMouseFunc(mouse);
    //glutMotionFunc(motion);
    glutMainLoop();

    return 0;
}
