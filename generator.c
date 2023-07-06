#include <stdlib.h>
#include <stdio.h>

#include "3d.h"

int main()
{
    /*
    Coordinate3D origin; origin.x = 0; origin.y = 0; origin.z = 0;

    Object3D* pyr_u = Object3D_create_pyramid(origin, 5, 5, "up");
    Object3D* pyr_d = Object3D_create_pyramid(origin, 5, 5, "down");
    Object3D* pyr_f = Object3D_create_pyramid(origin, 5, 5, "forward");
    Object3D* pyr_b = Object3D_create_pyramid(origin, 5, 5, "backward");
    Object3D* pyr_r = Object3D_create_pyramid(origin, 5, 5, "right");
    Object3D* pyr_l = Object3D_create_pyramid(origin, 5, 5, "left");

    Scene3D* scene1 = Scene3D_create();
    Scene3D* scene2 = Scene3D_create();
    Scene3D* scene3 = Scene3D_create();
    Scene3D* scene4 = Scene3D_create();
    Scene3D* scene5 = Scene3D_create();
    Scene3D* scene6 = Scene3D_create();

    Scene3D_append(scene1, pyr_u);
    Scene3D_append(scene2, pyr_d);
    Scene3D_append(scene3, pyr_f);
    Scene3D_append(scene4, pyr_b);
    Scene3D_append(scene5, pyr_r);
    Scene3D_append(scene6, pyr_l);

    Scene3D_write_stl_text(scene1, "pyr_u.stl");
    Scene3D_write_stl_text(scene2, "pyr_d.stl");
    Scene3D_write_stl_text(scene3, "pyr_f.stl");
    Scene3D_write_stl_text(scene4, "pyr_b.stl");
    Scene3D_write_stl_text(scene5, "pyr_r.stl");
    Scene3D_write_stl_text(scene6, "pyr_l.stl");

    Scene3D_destroy(scene1);
    Scene3D_destroy(scene2);
    Scene3D_destroy(scene3);
    Scene3D_destroy(scene4);
    Scene3D_destroy(scene5);
    Scene3D_destroy(scene6);

    */


    //Coordinates needed
    //Coordinate3D origin; origin.x = 0; origin.y = 0; origin.z = 0;
    Coordinate3D origin2; origin2.x = 0; origin2.y = 2.5; origin2.z = 0;
    Coordinate3D origin3; origin3.x = 0; origin3.y = 5; origin3.z = 0;
    Coordinate3D pp; pp.x = 25; pp.y = 10; pp.z = 20;
    Coordinate3D np; np.x = -25, np.y = 10; np.z = 20;
    Coordinate3D nn; nn.x = -25, nn.y = 10; nn.z = -20;
    Coordinate3D pn; pn.x = 25, pn.y = 10; pn.z = -20;

    //Create Objects
    Object3D* base = Object3D_create_cuboid(origin2, 60, 5, 50);

    Object3D* front_right_tower = Object3D_create_cuboid(pp, 10,10,10);
    Object3D* front_left_tower = Object3D_create_cuboid(np, 10,10,10);
    Object3D* back_left_tower = Object3D_create_cuboid(nn, 10,10,10);
    Object3D* back_right_tower = Object3D_create_cuboid(pn, 10,10,10);

    Object3D* pyramid_middle = Object3D_create_pyramid(origin3, 30, 70,"up");



    //Create Scene
    Scene3D* scene = Scene3D_create();
    //Add objects
    Scene3D_append(scene, base);
    Scene3D_append(scene, pyramid_middle);
    Scene3D_append(scene, front_right_tower);
    Scene3D_append(scene, front_left_tower);
    Scene3D_append(scene, back_left_tower);
    Scene3D_append(scene, back_right_tower);

    //Write file and destroy memory objects
    Scene3D_write_stl_text(scene, "generator.stl");
    Scene3D_destroy(scene);

    /*
    free(&origin);
    free(&origin2);
    free(&origin3);
    free(&pp);
    free(&np);
    free(&nn);
    free(&pn);
    */

    return 0;
}