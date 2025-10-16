#define _USE_MATH_DEFINES
#include <stdio.h>
#include "myLib.h"
#include <math.h>

int main(void) {
    vec4 v1 = {1, 2, 3, 4};
    vec4 v2 = {5, 6, 7, 8};

    mat4 m1 = {{1,-5,9,13}, {2,6,-10,14}, {3,7,11,15}, {4,8,12,-16}};
    mat4 m2 = {{4,8,12,16}, {3,7,11,15}, {2,6,10,14}, {1,5,9,13}};

    float scalor = 3.0;

    printf("Print x rotation: \n");
    print_mat4(rotate_x(90));

    printf("Print y rotation: \n");
    print_mat4(rotate_y(90));

    printf("Print z rotation: \n");
    print_mat4(rotate_z(90));
    
    return 0;
}
