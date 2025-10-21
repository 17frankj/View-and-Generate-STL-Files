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
#include "stl_reader.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))



vec4 vertices[1000000] =
{{ 0.0,  0.5,  0.0, 1.0},	// top
 {-0.5, -0.5,  0.0, 1.0},	// bottom left
 { 0.5, -0.5,  0.0, 1.0},	// bottom right
 };	

vec4 colors[1000000] =
{{1.0, 0.0, 0.0, 1.0},	// red   (for top)
 {0.0, 1.0, 0.0, 1.0},	// green (for bottom left)
 {0.0, 0.0, 1.0, 1.0},	// blue  (for bottom right)
 };	

int num_vertices = 100000;

mat4 my_ctm = {{1,0,0,0},{0,1,0,0}, {0,0,1,0}, {0,0,0,1}};
GLuint ctm_location;
GLuint program;
GLuint vbo;
int stl_value = 0;
float large_scale_value = 1.1;
float small_scale_value = 0.9;

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

    colors[0] = (vec4){1.0, 0.0, 0.0, 1.0}; // red
    colors[1] = (vec4){1.0, 0.0, 0.0, 1.0}; // red
    colors[2] = (vec4){1.0, 0.0, 0.0, 1.0}; // red
    colors[3] = (vec4){0.0, 0.0, 1.0, 1.0};	// blue
    colors[4] = (vec4){0.0, 0.0, 1.0, 1.0};	// blue
    colors[5] = (vec4){0.0, 0.0, 1.0, 1.0};	// blue
}

void make_inital_sphere_on_z(void)
{
    int vert = 0;
    vec4 base = (vec4){0, -1, 0, 1};   // start at bottom of sphere (on -y axis)

    int lat_steps = 36;  // full sphere from -90 to +90 in 10° increments
    int lon_steps = 36;  // full 360° in 10° increments

    // latitudes: -80 to +70 (15 steps of 10)
    for(int lat = 0; lat < lat_steps; lat++)
    {
        float x1 = -80 + lat * 10;   // current band
        float x2 = x1 + 10;          // next band

        // longitudes: 0 to 350 (36 steps of 10)
        for(int lon = 0; lon < lon_steps; lon++)
        {
            float y1 = lon * 10;
            float y2 = y1 + 10;

            // 4 corner points of the rectangular patch (quad)
            vec4 p1 = matrix_vector_multi(rotate_y(y1), matrix_vector_multi(rotate_x(x1), base));
            vec4 p2 = matrix_vector_multi(rotate_y(y1), matrix_vector_multi(rotate_x(x2), base));
            vec4 p3 = matrix_vector_multi(rotate_y(y2), matrix_vector_multi(rotate_x(x2), base));
            vec4 p4 = matrix_vector_multi(rotate_y(y2), matrix_vector_multi(rotate_x(x1), base));

            // 1st triangle p1, p2, p3
            vertices[vert]   = p1;
            vertices[vert+1] = p2;
            vertices[vert+2] = p3;

            // 2nd triangle p1, p3, p4
            vertices[vert+3] = p1;
            vertices[vert+4] = p3;
            vertices[vert+5] = p4;

            // random colors
            int random1 = 1 + rand() % 10;
            int random2 = 1 + rand() % 10;

            vec4 color1 = pick_color(random1);
            vec4 color2 = pick_color(random2);

            colors[vert]   = color1;
            colors[vert+1] = color1;
            colors[vert+2] = color1;
            colors[vert+3] = color2;
            colors[vert+4] = color2;
            colors[vert+5] = color2;

            vert += 6;
        }
    }
    num_vertices = vert; // update total number of vertices
}

void make_Sphere(void)
{
    int random = 1 + rand() %10; // make random number between 1 and 10
    vec4 random_color = pick_color(random);

    make_inital_sphere_on_z();
}

void make_donut(void)
{
    int random = 1 + rand() %10; // make random number between 1 and 10
    vec4 random_color = pick_color(random);

    int vert = 0;
    int u_steps = 36; // around main circle
    int v_steps = 24; // around tube
    float R = 0.6f;   // distance from center to tube center
    float r = 0.3f;   // radius of tube

    for(int i = 0; i < u_steps; i++)
    {
        float u1 = (2.0f * M_PI * i) / u_steps;
        float u2 = (2.0f * M_PI * (i + 1)) / u_steps;

        for(int j = 0; j < v_steps; j++)
        {
            float v1 = (2.0f * M_PI * j) / v_steps;
            float v2 = (2.0f * M_PI * (j + 1)) / v_steps;

            // 4 corners of quad on torus
            vec4 p1 = { (R + r * cos(v1)) * cos(u1), (R + r * cos(v1)) * sin(u1), r * sin(v1), 1.0 };
            vec4 p2 = { (R + r * cos(v2)) * cos(u1), (R + r * cos(v2)) * sin(u1), r * sin(v2), 1.0 };
            vec4 p3 = { (R + r * cos(v2)) * cos(u2), (R + r * cos(v2)) * sin(u2), r * sin(v2), 1.0 };
            vec4 p4 = { (R + r * cos(v1)) * cos(u2), (R + r * cos(v1)) * sin(u2), r * sin(v1), 1.0 };

            // first triangle
            vertices[vert]   = p1;
            vertices[vert+1] = p2;
            vertices[vert+2] = p3;

            // second triangle
            vertices[vert+3] = p1;
            vertices[vert+4] = p3;
            vertices[vert+5] = p4;

            int random1 = 1 + rand() % 10;
            int random2 = 1 + rand() % 10;

            vec4 color1 = pick_color(random1);
            vec4 color2 = pick_color(random2);

            colors[vert]   = color1;
            colors[vert+1] = color1;
            colors[vert+2] = color1;
            colors[vert+3] = color2;
            colors[vert+4] = color2;
            colors[vert+5] = color2;

            vert += 6;
        }
    }

    num_vertices = vert; // update total vertices
}

void make_spring(void)
{
    int vert = 0;

    int u_steps = 72; // number of steps along the helix
    int v_steps = 24; // number of steps around the tube
    int turns = 3;    // number of full revolutions
    float R = 0.4f;   // radius of spring coil
    float r = 0.1f;   // tube radius
    float height = 1.0f; // total height of spring

    for(int i = 0; i < u_steps; i++)
    {
        float u1 = (2.0f * M_PI * turns * i) / u_steps;
        float u2 = (2.0f * M_PI * turns * (i + 1)) / u_steps;

        float z1 = height * i / u_steps;       // linear height along spring
        float z2 = height * (i + 1) / u_steps;

        for(int j = 0; j < v_steps; j++)
        {
            float v1 = (2.0f * M_PI * j) / v_steps;
            float v2 = (2.0f * M_PI * (j + 1)) / v_steps;

            // 4 corners of quad on spring tube
            vec4 p1 = { (R + r * cos(v1)) * cos(u1),
                        (R + r * cos(v1)) * sin(u1),
                        z1 + r * sin(v1),
                        1.0 };

            vec4 p2 = { (R + r * cos(v2)) * cos(u1),
                        (R + r * cos(v2)) * sin(u1),
                        z1 + r * sin(v2),
                        1.0 };

            vec4 p3 = { (R + r * cos(v2)) * cos(u2),
                        (R + r * cos(v2)) * sin(u2),
                        z2 + r * sin(v2),
                        1.0 };

            vec4 p4 = { (R + r * cos(v1)) * cos(u2),
                        (R + r * cos(v1)) * sin(u2),
                        z2 + r * sin(v1),
                        1.0 };

            // first triangle
            vertices[vert]   = p1;
            vertices[vert+1] = p2;
            vertices[vert+2] = p3;

            // second triangle
            vertices[vert+3] = p1;
            vertices[vert+4] = p3;
            vertices[vert+5] = p4;

            int random1 = 1 + rand() % 10;
            int random2 = 1 + rand() % 10;

            vec4 color1 = pick_color(random1);
            vec4 color2 = pick_color(random2);

            colors[vert]   = color1;
            colors[vert+1] = color1;
            colors[vert+2] = color1;
            colors[vert+3] = color2;
            colors[vert+4] = color2;
            colors[vert+5] = color2;

            vert += 6;
        }
    }

    num_vertices = vert;
}

void update_vertex_buffer()
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    if(stl_value == 1) 
    {
        Mesh mesh = read_stl_binary("Little-darth-vader.STL");
        normalize_mesh(&mesh);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vec4) * mesh.num_vertices * 2, NULL, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec4) * mesh.num_vertices, mesh.vertices);
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4) * mesh.num_vertices, sizeof(vec4) * mesh.num_vertices, mesh.colors);
    } 
    else 
    {
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colors), NULL, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colors), colors);
    }
}

void make_shape_larger(void)
{
    my_ctm = matrix_scaling(large_scale_value, large_scale_value, large_scale_value);
    small_scale_value = large_scale_value;
    large_scale_value += 0.1;
}

void make_shape_smaller(void)
{
    my_ctm = matrix_scaling(small_scale_value, small_scale_value, small_scale_value);
    large_scale_value = small_scale_value;
    small_scale_value -= 0.1;
    
}

void init(void)
{

    GLuint program = initShader("vshader.glsl", "fshader.glsl");
    glUseProgram(program);

    // Single VBO
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colors), NULL, GL_STATIC_DRAW);

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
    switch(key)
    {
        case 'q':
            glutLeaveMainLoop();
            break;

        case '1': // Sphere
            stl_value = 0;
            make_Sphere();
            update_vertex_buffer();
            break;

        case '2': // Donut
            stl_value = 0;
            make_donut();
            update_vertex_buffer();
            break;

        case '3': // Spring
            stl_value = 0;
            make_spring();
            update_vertex_buffer();
            break;

        case '4': // STL
            stl_value = 1;
            update_vertex_buffer();
            break;

        case 'e': // enlarge
            make_shape_larger();
            break;
        case 'r': // shrink
            make_shape_smaller();
            break;
    }
   
    glutPostRedisplay();
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

    glewInit();
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    //glutMouseFunc(mouse);
    //glutMotionFunc(motion);
    glutMainLoop();

    return 0;
}
