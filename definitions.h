typedef struct color
{
   float r, g, b;
}color;

typedef struct point
{
    float x, y, z;
}point;

typedef struct triangle
{
    point p1, p2, p3;
}triangle;


typedef struct angles
{
   float angleX, angleY, angleZ;
}angles;

typedef struct camera
{
   point pos;
   angles ang;
}camera;
