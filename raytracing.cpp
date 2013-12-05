#include "raytracing.h"
#include "tracing.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>
#include <iostream>
#include <math.h>

using namespace cv;
using namespace std;

Mat image0;

class _points
{
public:
    int x;
    int y;
    _points() {x = y = 0;}
    _points(int _ox,int _oy) {x = _ox; y = _oy;}
};

void drawpixel(int x, int y, _Vector c)
{
    int height     = image0.rows;
    int width      = image0.cols;
    int step       = image0.step;
    int channels   = image0.channels();
    uchar* data    = (uchar *)image0.data;
    c.vectorLimit();
    data[y*step+x*channels+0] = (uchar)(c.delx*255 + .5);
    data[y*step+x*channels+1] = (uchar)(c.dely*255 + .5);
    data[y*step+x*channels+2] = (uchar)(c.delz*255 + .5);
}

int main( int argc, char** argv )
{

    image0.create(600,800,CV_8UC3);
    if( image0.empty() )
    {
        cout << "Image empty. \n";
        return 0;
    }
    namedWindow( "image", 0 );

    _Light ig1;
    ig1.l0.x = 0;
    ig1.l0.y = 0;
    ig1.l0.z = 400;
    ig1.i.delx = 1;
    ig1.i.dely = 1;
    ig1.i.delz = 1;
    vector<_Light> ig;
    ig.push_back(ig1);
    _Light ie;
    ie.i.delx = .5;
    ie.i.dely = .5;
    ie.i.delz = .5;
    _Point view(400, 300, -5000);
    _Vector background(0,0,0);
    _Point pc(400, 300, 500);
    _Sphere ball(pc, 250);
    ball.kd.setVector(.8, 0, 0);
    ball.ka.setVector(.3,0,0);
    ball.ks.setVector(1,1,1);
    //ball.kt.setVector(1,1,1);
    ball.nt = 7;
    ball.ns = 5;
    for(;;)
    {
        for (int i = 0; i < 800; i++)
        {
            for (int j = 0; j < 600; j++){
                _Vector tmp(i - view.x, j - view.y, 0 - view.z);
                tmp.vectorUnit();
                _Point rayr0(i,j,0);
                _Ray now(rayr0, tmp);
                double t;
                if (ball.intersect(now, &t)){
                    _Point tmp1 = now.atTime(t);
                    _Vector tmp2 = ball.getN(tmp1);
                    tmp2 = ball.phong(&ig, ie, tmp1, now, tmp2, 1.6);
                    drawpixel(i, j ,tmp2);
                }
                else{
                    drawpixel(i, j, background);
                }
            }
        }
        imshow("image",image0);

        int c = waitKey(0);
        if( (c & 255) == 27 )
        {
            cout << "Exiting ...\n";
            break;
        }
        switch( (char)c )
        {
        case 'c':

            break;
        }
    }
    return 0;
}
