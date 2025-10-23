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


// inital decelaration of vertices
//      -inital values for debugging
vec4 vertices[1500000] =
{{ 0.0,  0.5,  0.0, 1.0},	// top
 {-0.5, -0.5,  0.0, 1.0},	// bottom left
 { 0.5, -0.5,  0.0, 1.0},	// bottom right
 };	

vec4 colors[1500000] =
{{1.0, 0.0, 0.0, 1.0},	// red   (for top)
 {0.0, 1.0, 0.0, 1.0},	// green (for bottom left)
 {0.0, 0.0, 1.0, 1.0},	// blue  (for bottom right)
 };	

// ------------global variables- --------------------------------------------------------------------------------------// 

int num_vertices = 1500000;   // max number of verts needed for stl files
mat4 my_ctm = {{1,0,0,0},{0,1,0,0}, {0,0,1,0}, {0,0,0,1}};  // initial identity matrix for moving the objects

// converstions to open gl types
GLuint ctm_location;  
GLuint program;
GLuint vbo;

// msc global variables
int stl_value = 0;
float current_scale = 1;
float large_scale_value = 1.1;
float small_scale_value = 0.9;
float object_radius = 1.0f;

// mouse/ motion global variables
float lastX;
float lastY;
int leftDown = 0;
float touching = 0;

// -------------end glob vars  -----------------------------------------------------------------------------------------// 



// ----------------- object functions  ---------------------------------------------------------------------------------//

// Generate "random" numberes
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

// tester functon
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

// creates the sphere object
void make_Sphere(void)
{
    int random = 1 + rand() %10; // make random number between 1 and 10
    vec4 random_color = pick_color(random);
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

// creates torus object
void make_donut(void)
{
    int random = 1 + rand() %10; // make random number between 1 and 10
    vec4 random_color = pick_color(random);

    int vert = 0;
    int u_steps = 36; // around main circle
    int v_steps = 24; // around tube
    float R = 0.6;   // distance from center to tube center
    float r = 0.3;   // radius of tube

    for(int i = 0; i < u_steps; i++)
    {
        float u1 = (2.0 * M_PI * i) / u_steps;
        float u2 = (2.0 * M_PI * (i + 1)) / u_steps;

        for(int j = 0; j < v_steps; j++)
        {
            float v1 = (2.0 * M_PI * j) / v_steps;
            float v2 = (2.0 * M_PI * (j + 1)) / v_steps;

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

// helper function for ends of spring object
int make_ends_spring(void)
{
    // make spring end circles

    float R = 0.4;   // radius of spring coil
    float r = 0.1;   // tube radius
    float height = 1.0; // total height of spring
    int vert = 10368;
    int circle_points = 36;
    vec4 p[36];
    
    // first use a base point on +x axis
    vec4 base_point = {1.0, 0.0, 0.0, 1.0};

    // Circle lies in XY plane, and centered at origin
    mat4 scale = matrix_scaling(r, r, 1.0);
    mat4 translate_bottom = matrix_translation(R, 0.0, 0.0);
    mat4 translate_top = matrix_translation(R, 0.0, height);

    mat4 align_to_z = rotate_x(270.0);  // rotate circle from XY to YZ plane

    // Create the circle outline
    for (int i = 0; i < circle_points; i++) 
    {
        float angle = (10 * i);
        mat4 rot = rotate_z(angle);
        //p[i] = matrix_vector_multi(rot, base_point);
        // rotate around Z to make the circle, then align to X
        p[i] = matrix_vector_multi(rot, base_point);
    }
    
    // bottom cap 
    for (int i = 0; i < circle_points; i++) 
    {
        int next = (i + 1) % circle_points;
        vec4 center = {0.0, 0.0, 0.0, 1.0};

        vec4 v1 = matrix_vector_multi(translate_bottom, matrix_vector_multi(align_to_z, matrix_vector_multi(scale, p[i])));
        vec4 v2 = matrix_vector_multi(translate_bottom, matrix_vector_multi(align_to_z, matrix_vector_multi(scale, p[next])));
        vec4 c  = matrix_vector_multi(translate_bottom, matrix_vector_multi(align_to_z, matrix_vector_multi(scale, center)));

        vertices[vert]   = c;
        vertices[vert+1] = v2;
        vertices[vert+2] = v1;

        int random1 = 1 + rand() % 10;
        int random2 = 1 + rand() % 10;
        int random3 = 1 + rand() % 10;
        vec4 color = pick_color(random1);
        vec4 color2 = pick_color(random2);
        vec4 color3 = pick_color(random3);

        colors[vert]   = color;
        colors[vert+1] = color2;
        colors[vert+2] = color3;
        vert += 3;
    }

    // top cap
    for (int i = 0; i < circle_points; i++) 
    {
        int next = (i + 1) % circle_points;
        vec4 center = {0.0, 0.0, 0.0, 1.0};

        vec4 v1 = matrix_vector_multi(translate_top, matrix_vector_multi(align_to_z, matrix_vector_multi(scale, p[i])));
        vec4 v2 = matrix_vector_multi(translate_top, matrix_vector_multi(align_to_z, matrix_vector_multi(scale, p[next])));
        vec4 c  = matrix_vector_multi(translate_top, matrix_vector_multi(align_to_z, matrix_vector_multi(scale, center)));

        vertices[vert]   = c;
        vertices[vert+1] = v1;  // <--- reverse the winding
        vertices[vert+2] = v2;

        int random1 = 1 + rand() % 10;
        int random2 = 1 + rand() % 10;
        int random3 = 1 + rand() % 10;
        vec4 color = pick_color(random1);
        vec4 color2 = pick_color(random2);
        vec4 color3 = pick_color(random3);

        colors[vert]   = color;
        colors[vert+1] = color2;
        colors[vert+2] = color3;
        vert += 3;
    }
    return vert;
}

// primary function to create spring object
void make_spring(void)
{
    int vert = 0;

    int u_steps = 72; // number of steps along the helix
    int v_steps = 24; // number of steps around the tube
    int turns = 3;    // number of full revolutions
    float R = 0.4;   // radius of spring coil
    float r = 0.1;   // tube radius
    float height = 1.0; // total height of spring

    // make spring body

    for(int i = 0; i < u_steps; i++)
    {
        float u1 = (2.0 * M_PI * turns * i) / u_steps;
        float u2 = (2.0 * M_PI * turns * (i + 1)) / u_steps;

        float z1 = height * i / u_steps;       // height along spring
        float z2 = height * (i + 1) / u_steps;

        for(int j = 0; j < v_steps; j++)
        {
            float v1 = (2.0 * M_PI * j) / v_steps;
            float v2 = (2.0 * M_PI * (j + 1)) / v_steps;

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
    num_vertices = make_ends_spring();
}

// Zoom out
void make_shape_larger(void)
{
    // increment the scale value to up the radious of the stored object
    current_scale += 0.1;
    my_ctm = matrix_multi(my_ctm, matrix_scaling(1.1, 1.1, 1.1));
    object_radius = current_scale;
}

// Zoom in
void make_shape_smaller(void)
{
    // decrease the scale value to up the radious of the stored object
    current_scale -= 0.1;
    my_ctm = matrix_multi(my_ctm, matrix_scaling(0.9, 0.9, 0.9));
    object_radius = current_scale;
}

// swaps between open gl buffers, one for basic objects, other for stl objects
void update_vertex_buffer()
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Stl
    if(stl_value == 1) 
    {
        Mesh mesh = read_stl_binary("Little-darth-vader.STL");
        normalize_mesh(&mesh);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vec4) * mesh.num_vertices * 2, NULL, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec4) * mesh.num_vertices, mesh.vertices);
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4) * mesh.num_vertices, sizeof(vec4) * mesh.num_vertices, mesh.colors);
    } 
    // basic objects
    else 
    {
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colors), NULL, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colors), colors);
    }
}


// -------------------------- end object funcitons  ---------------------------------------------------------------------------------//


// ---------------Open Gl Functions  --------------------------------------------------------------------------------- // 
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

    //my_ctm = identity();
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

    glutSwapBuffers();
}

void keyboard(unsigned char key, int mousex, int mousey)
{
    switch(key)
    {
        case 'e': // enlarge
            make_shape_larger();
            break;

        case 'r': // reduce
            make_shape_smaller();
            break;

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
    }
   
    glutPostRedisplay();
}

// return 1 if pointer at (glx,gly) is over the object
// i.e the mouse is "out of bounds"
int is_pointer_on_object(float glx, float gly)
{
    // where mouse is pointing
    vec4 p_screen = (vec4){ glx, gly, 0.0f, 1.0f };

    // transform pointer into the object space: p_obj = inverse(my_ctm) * p_screen
    mat4 inv = matrix_inverse(my_ctm);
    vec4 p_obj = matrix_vector_multi(inv, p_screen);

    // distance from origin in object space 
    // create a vec4 direction with w = 0 to compute magnitude
    vec4 p_obj_dir = (vec4){ p_obj.x, p_obj.y, p_obj.z, 0.0f };
    float dist = vec_Magnitude(p_obj_dir);

    return (dist <= object_radius);
}

// make a "virtual sphere" to aid in mouse location and motion
vec4 project_to_sphere(float x, float y)
{
    float z;
    float d = x * x + y * y;
    if (d <= 1.0f)
        z = sqrtf(1.0f - d);
    else
        z = 0.0f;
    return (vec4){x, y, z, 0.0f};
}

// get the mouse location and input from user
// then return a state based on the mouse pointer's location
void mouse(int button, int state, int x, int y)
{
    float glx = (x / 400.0f) - 1.0f;
    float gly = 1.0f - (y / 400.0f);

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        leftDown = 1;
        touching = is_pointer_on_object(glx, gly);  // check if pointer is on object's radius
        lastX = glx;
        lastY = gly;
    }
    else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {
        leftDown = 0;
    }

    glutPostRedisplay();
}

// find a matrix from an arbitrary angle
mat4 axis_angle_rotation(vec4 axis, float angle)
{
    float c = cosf(angle);  // cos
    float s = sinf(angle);  // sin
    float t = 1.0f - c;     // tan

    float x = axis.x;
    float y = axis.y;
    float z = axis.z;

    mat4 m;  // generic mat4

    // calculate the arbitrary matrix coordinates with cos, sin, tan of provided angle
    m.x = (vec4){t*x*x + c,     t*x*y - s*z,   t*x*z + s*y,   0.0f}; 
    m.y = (vec4){t*x*y + s*z,   t*y*y + c,     t*y*z - s*x,   0.0f};
    m.z = (vec4){t*x*z - s*y,   t*y*z + s*x,   t*z*z + c,     0.0f};
    m.w = (vec4){0.0f,          0.0f,          0.0f,          1.0f}; // just assigns this struct as a mat4 (0) instead of a point(1)

    return m; // returns the arbitrary matrix 
}

// calculates object location based on the users actions
void motion(int x, int y)
{
    // make sure the user is pressing their keys and the pointer location is in bounds
    if (!leftDown)
        return;
    if (!touching)
        return;

    // get loaction of mouse pointer 
    // based on a 400 / 400 pixel display
    float glx = (x / 400.0f) - 1.0f;
    float gly = 1.0f - (y / 400.0f);

    // if pointer has moved off the object, stop rotating
    // this is checked every frame!
    if (!is_pointer_on_object(glx, gly))
    {
        touching = 0;    // stop the rotation session
        return;
    }

    // keep previous and current points on the "virtual sphere"
    vec4 v1 = project_to_sphere(lastX, lastY);
    vec4 v2 = project_to_sphere(glx, gly);

    // rotation axis = cross product(v2, v1)
    // cross product of the previous and current virtual spheres 
    vec4 axis = vec_Cross_Product(v2, v1);  // make this v1, v2 for inverse controls!!!!
    float axis_len = vec_Magnitude(axis);
    axis = vec_Normalized(axis);  

    // gets the angle of the two vectors using the inverse 
    // angle = arccos(dot(v1,v2))
    float dot = vec_Dot_product(v1, v2);
    if (dot > 1.0f) 
    {
        dot = 1.0f;
    }
    if (dot < -1.0f) 
    {
        dot = -1.0f;
    }
    float angle = acosf(dot);

    // rotation matrix about arbitrary axis
    mat4 rotation = axis_angle_rotation(axis, angle);

    // apply rotation about origin
    //my_ctm = matrix_multi(rotation, my_ctm);
    my_ctm = matrix_multi(my_ctm, rotation);
   
    lastX = glx;
    lastY = gly;
    glutPostRedisplay();
}

// ------------------------- end open Gl Functions --------------------------------------------------------------------------------- //


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
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutMainLoop();

    return 0;
}
