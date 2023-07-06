#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "3d.h"

//Private helper methods

Coordinate3D* crd3D_create(double x, double y, double z)
{
    Coordinate3D* result = malloc(sizeof(Coordinate3D));
    result -> x = x;
    result -> y = y;
    result -> z = z;
    return result;
}

void crd3D_destroy(Coordinate3D* c3D)
{
    free(c3D);
}

//Note: Does NOT take a pointer
void crd3D_fprintf(FILE* fp, Coordinate3D c3D)
{
    fprintf(fp, "    vertex %.5f %.5f %.5f\n", c3D.x, c3D.y, c3D.z);
}

Triangle3D* tri3D_create(Coordinate3D* a, Coordinate3D* b, Coordinate3D* c)
{
    Triangle3D* result = malloc(sizeof(Triangle3D));
    result -> a = *a;
    result -> b = *b;
    result -> c = *c;
    return result;
}

void tri3D_destroy(Triangle3D* t3D)
{
    free(t3D);
}

//Note: Does NOT take a pointer
void tri3D_fprintf(FILE* fp, Triangle3D t3D)
{
    crd3D_fprintf(fp, t3D.a);
    crd3D_fprintf(fp, t3D.b);
    crd3D_fprintf(fp, t3D.c);
}

Triangle3DNode* triNode_create(Triangle3D* t, Triangle3DNode* tn)
{
    Triangle3DNode* result = malloc(sizeof(Triangle3DNode));
    result -> triangle = *t;
    result -> next = tn;
    return result;
}

void triNode_destroy(Triangle3DNode* tNode)
{
    free(tNode);
}

void triNode_destroy_complete(Triangle3DNode* tNode)
{
    if(tNode -> next == NULL)
    {
        free(tNode);
    }
    else
    {
        Triangle3DNode* temp = tNode->next;
        free(tNode);
        triNode_destroy_complete(temp);
    }
}

void write_stl_triNode(FILE* fp, Triangle3DNode* triNode)
{
    //Write the triangle
    fprintf(fp, "  facet normal 0.0 0.0 0.0\n");
    fprintf(fp, "    outer loop\n");
    tri3D_fprintf(fp, triNode -> triangle);
    fprintf(fp, "    endloop\n");
    fprintf(fp, "  endfacet\n");
    //If the node has more triangles continue to write
    if(triNode -> next != NULL)
    {
        write_stl_triNode(fp, triNode -> next);
    }
}

Object3D* obj3D_create(long count, Triangle3DNode* root)
{
    Object3D* result = malloc(sizeof(Object3D));
    result -> count = count;
    result -> root = root;
    return result;
}

void obj3D_destroy(Object3D* obj)
{
    triNode_destroy_complete(obj -> root);
    free(obj);
}

void expand_array(Scene3D* scene)
{
    scene -> size *= 2;
    scene -> objects = realloc(scene -> objects, (scene -> size * sizeof(Object3D*)));
}


//Public implemented header methods

Scene3D* Scene3D_create()
{
    Scene3D* result = malloc(sizeof(Scene3D));
    result -> count = 0;
    result -> size = 5;
    result -> objects = malloc(5 * sizeof(Object3D*));
    return result;
}

void Scene3D_destroy(Scene3D* scene)
{
    for(int i = 0; i < scene -> count; i++)
    {
        obj3D_destroy(scene -> objects[i]);
    }
    free(scene -> objects);
    free(scene);
}

void Scene3D_append(Scene3D* scene, Object3D* object)
{
    //First thing check if there's room in the array
    if(scene -> count == scene -> size)
    {
        //The array must be re-sized
        expand_array(scene);
    }
    //At this point we know there is space in the array
    //Add the object to the array
    scene -> objects[scene -> count] = object;
    //Increment the counter to match the number of objects in the array
    scene -> count += 1;
}

void Scene3D_write_stl_text(Scene3D* scene, char* file_name)
{
    FILE* fp = fopen(file_name, "w");
    fprintf(fp, "solid scene\n");
    for(int i = 0; i < scene -> count; i++)
    {
        //This loop handles each object
        write_stl_triNode(fp, scene -> objects[i] -> root);
    }
    fprintf(fp, "endsolid scene");
    fclose(fp);
}

Object3D* Object3D_create_pyramid(Coordinate3D origin, double width, double height, char* orientation)
{
    double x0 = origin.x;
    double y0 = origin.y;
    double z0 = origin.z;

    double hwidth = width/2;

    char f[] = "forward";
    char b[] = "backward";
    char u[] = "up";
    char d[] = "down";
    char l[] = "left";
    char r[] = "right";

    int forward = strcmp(orientation, f);
    int backward = strcmp(orientation, b);
    int up = strcmp(orientation, u);
    int down = strcmp(orientation, d);
    int left = strcmp(orientation, l);
    int right = strcmp(orientation, r);

    Coordinate3D* p0p;
    Coordinate3D* n0p;
    Coordinate3D* n0n;
    Coordinate3D* p0n;
    Coordinate3D* pt;

    Triangle3D* base_front;
    Triangle3D* base_back;
    Triangle3D* side_front;
    Triangle3D* side_left;
    Triangle3D* side_back;
    Triangle3D* side_right;

    Triangle3DNode* bfn;
    Triangle3DNode* bbn;
    Triangle3DNode* sfn;
    Triangle3DNode* sln;
    Triangle3DNode* sbn;
    Triangle3DNode* srn;

    Object3D* result;

    if(forward == 0 || backward == 0)
    {
        p0p = crd3D_create(x0 + hwidth, y0 - hwidth, z0);
        n0p = crd3D_create(x0 - hwidth, y0 - hwidth, z0);
        n0n = crd3D_create(x0 - hwidth, y0 + hwidth, z0);
        p0n = crd3D_create(x0 + hwidth, y0 + hwidth, z0);

        if(forward == 0)
        {
            pt = crd3D_create(x0, y0, z0 + height);

            base_front = tri3D_create(n0p,p0p,p0n);
            base_back = tri3D_create(n0p,n0n,p0n);
            side_front = tri3D_create(p0p,n0p,pt);
            side_left = tri3D_create(n0p,n0n,pt);
            side_back = tri3D_create(n0n,p0n,pt);
            side_right = tri3D_create(p0n,p0p,pt);


        }
        else
        {
            pt = crd3D_create(x0, y0, z0 - height);

            base_front = tri3D_create(n0p,p0p,p0n);
            base_back = tri3D_create(n0p,n0n,p0n);
            side_front = tri3D_create(p0p,n0p,pt);
            side_left = tri3D_create(n0p,n0n,pt);
            side_back = tri3D_create(n0n,p0n,pt);
            side_right = tri3D_create(p0n,p0p,pt);
        }
    }
    else if(up ==0 || down == 0)
    {
        p0p = crd3D_create(x0 + hwidth, y0, z0 + hwidth);
        n0p = crd3D_create(x0 - hwidth, y0, z0 + hwidth);
        n0n = crd3D_create(x0 - hwidth, y0, z0 - hwidth);
        p0n = crd3D_create(x0 + hwidth, y0, z0 - hwidth);

        if(up == 0)
        {
            pt = crd3D_create(x0, y0 + height, z0);
        }
        else
        {
            pt = crd3D_create(x0, y0 - height, z0);
        }
        base_front = tri3D_create(n0p,p0p,p0n);
        base_back = tri3D_create(n0p,n0n,p0n);
        side_front = tri3D_create(p0p,n0p,pt);
        side_left = tri3D_create(n0p,n0n,pt);
        side_back = tri3D_create(n0n,p0n,pt);
        side_right = tri3D_create(p0n,p0p,pt);
    }
    else if(left == 0 || right == 0)
    {
        p0p = crd3D_create(x0, y0 - hwidth, z0 - hwidth);
        n0p = crd3D_create(x0, y0 - hwidth, z0 + hwidth);
        n0n = crd3D_create(x0, y0 + hwidth, z0 + hwidth);
        p0n = crd3D_create(x0, y0 + hwidth, z0 - hwidth);

        if(right == 0)
        {
            pt = crd3D_create(x0 + height, y0, z0);
        }
        else
        {
            pt = crd3D_create(x0 - height, y0, z0);
        }
        base_front = tri3D_create(p0p,n0p,n0n);
        base_back = tri3D_create(p0p,p0n,n0n);
        side_front = tri3D_create(p0p,n0p,pt);
        side_left = tri3D_create(n0p,n0n,pt);
        side_back = tri3D_create(n0n,p0n,pt);
        side_right = tri3D_create(p0n,p0p,pt);
    }

    bfn = triNode_create(base_front, NULL);
    bbn = triNode_create(base_back, bfn);
    sfn = triNode_create(side_front, bbn);
    sln = triNode_create(side_left, sfn);
    sbn = triNode_create(side_back, sln);
    srn = triNode_create(side_right, sbn);
    //srn is the head

    result = obj3D_create(6, srn);

    crd3D_destroy(p0p);
    crd3D_destroy(n0p);
    crd3D_destroy(n0n);
    crd3D_destroy(p0n);
    crd3D_destroy(pt);

    tri3D_destroy(base_front);
    tri3D_destroy(base_back);
    tri3D_destroy(side_front);
    tri3D_destroy(side_left);
    tri3D_destroy(side_back);
    tri3D_destroy(side_right);

    return result;
}

Object3D* Object3D_create_cuboid(Coordinate3D origin, double width, double height, double depth)
{
    double hwidth = width/2;
    double hheight = height/2;
    double hdepth = depth/2;

    //Top corners starting from front right (all cords pos) clockwise
    Coordinate3D* ppp = crd3D_create(origin.x + hwidth, origin.y + hheight, origin.z + hdepth);
    Coordinate3D* npp = crd3D_create(origin.x - hwidth, origin.y + hheight, origin.z + hdepth);
    Coordinate3D* npn = crd3D_create(origin.x - hwidth, origin.y + hheight, origin.z - hdepth);
    Coordinate3D* ppn = crd3D_create(origin.x + hwidth, origin.y + hheight, origin.z - hdepth);

    //Bottom corners starting from front right (y neg, x and z pos) clockwise
    Coordinate3D* pnp = crd3D_create(origin.x + hwidth, origin.y - hheight, origin.z + hdepth);
    Coordinate3D* nnp = crd3D_create(origin.x - hwidth, origin.y - hheight, origin.z + hdepth);
    Coordinate3D* nnn = crd3D_create(origin.x - hwidth, origin.y - hheight, origin.z - hdepth);
    Coordinate3D* pnn = crd3D_create(origin.x + hwidth, origin.y - hheight, origin.z - hdepth);

    //Top and bottom
    Triangle3D* top_front = tri3D_create(ppp,npp,ppn);
    Triangle3D* top_back = tri3D_create(npp,npn,ppn);
    Triangle3D* bottom_front = tri3D_create(pnp,nnp,pnn);
    Triangle3D* bottom_back = tri3D_create(nnp,nnn,pnn);

    //Front and back
    Triangle3D* front_top = tri3D_create(npp,ppp,pnp);
    Triangle3D* front_bottom = tri3D_create(pnp,nnp,npp);
    Triangle3D* back_top = tri3D_create(pnn,npn,ppn);
    Triangle3D* back_bottom = tri3D_create(pnn,nnn,npn);

    //Right and left
    Triangle3D* right_top = tri3D_create(ppp,ppn,pnp);
    Triangle3D* right_bottom = tri3D_create(pnp,pnn,ppn);
    Triangle3D* left_top = tri3D_create(nnp,npp,npn);
    Triangle3D* left_bottom = tri3D_create(nnp,nnn,npn);

    Triangle3DNode* tfn = triNode_create(top_front, NULL);
    Triangle3DNode* tbn = triNode_create(top_back, tfn);
    Triangle3DNode* bfn = triNode_create(bottom_front, tbn);
    Triangle3DNode* bbn = triNode_create(bottom_back, bfn);
    Triangle3DNode* ftn = triNode_create(front_top, bbn);
    Triangle3DNode* fbn = triNode_create(front_bottom, ftn);
    Triangle3DNode* btn = triNode_create(back_top, fbn);
    Triangle3DNode* bbn2 = triNode_create(back_bottom, btn);
    Triangle3DNode* rtn = triNode_create(right_top, bbn2);
    Triangle3DNode* rbn = triNode_create(right_bottom, rtn);
    Triangle3DNode* ltn = triNode_create(left_top, rbn);
    Triangle3DNode* lbn = triNode_create(left_bottom, ltn);
    //lbn is the head

    //Create the object
    Object3D* result = obj3D_create(12, lbn);

    //Free coords
    crd3D_destroy(ppp);
    crd3D_destroy(npp);
    crd3D_destroy(npn);
    crd3D_destroy(ppn);
    crd3D_destroy(pnp);
    crd3D_destroy(nnp);
    crd3D_destroy(nnn);
    crd3D_destroy(pnn);

    tri3D_destroy(top_front);
    tri3D_destroy(top_back);
    tri3D_destroy(bottom_front);
    tri3D_destroy(bottom_back);
    tri3D_destroy(front_top);
    tri3D_destroy(front_bottom);
    tri3D_destroy(back_top);
    tri3D_destroy(back_bottom);
    tri3D_destroy(right_top);
    tri3D_destroy(right_bottom);
    tri3D_destroy(left_top);
    tri3D_destroy(left_bottom);

    //Objects necessary have been destroyed
    return result;
}

void test()
{
    //Make coords
    double d = 2.0;
    Coordinate3D* c1 = crd3D_create(d,0,0);
    Coordinate3D* c2 = crd3D_create(0,d,0);
    Coordinate3D* c3 = crd3D_create(0,0,d);
    d = 3.0;
    Coordinate3D* c4 = crd3D_create(d,0,0);
    Coordinate3D* c5 = crd3D_create(0,d,0);
    Coordinate3D* c6 = crd3D_create(0,0,d);
    //Make triangles from coords
    Triangle3D* t1 = tri3D_create(c1,c2,c3);
    Triangle3D* t2 = tri3D_create(c4,c5,c6);
    //Free coords since they've been de-referenced in the triangles
    crd3D_destroy(c1);
    crd3D_destroy(c2);
    crd3D_destroy(c3);
    crd3D_destroy(c4);
    crd3D_destroy(c5);
    crd3D_destroy(c6);
    //Create triangle nodes with the triangles we've just made
    Triangle3DNode* tn_lea4 = triNode_create(t2, NULL);
    Triangle3DNode* tn_lea3 = triNode_create(t1, tn_lea4);
    Triangle3DNode* tn_leaf = triNode_create(t2, NULL);
    Triangle3DNode* tn_head = triNode_create(t1, tn_leaf);
    //Free triangles since they've been de-referenced in the nodes
    tri3D_destroy(t1);
    tri3D_destroy(t2);

    //triNode_destroy_complete(tn_head);

    Object3D* obj = obj3D_create(2, tn_head);
    Object3D* obj2 = obj3D_create(2, tn_lea3);
    //obj3D_destroy(obj);

    Scene3D* scene = Scene3D_create();
    Scene3D_append(scene, obj);
    Scene3D_append(scene, obj2);

    Scene3D_write_stl_text(scene, "testfile.stl");

    Scene3D_destroy(scene);

    /*
    //Create an object with the triangle node linked list we've made
    Object3D* obj = obj3D_create(4, tn_head);

    Scene3D* scene = Scene3D_create();
    scene -> objects[0] = obj;

    //Scene3D_destroy(scene);
    */

}