#ifndef LIB_H 
#define LIB_H

#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <limits>
#include <math.h>
#include <numbers>

using namespace cv;
using namespace std;

struct MyPoint {
	double x;
	double y;
	double z;
};

struct MyPoint2d {
	double x;
	double y;
};

const Vec3b BLACK  = { 0, 0, 0 };
const Vec3b WHITE  = { 255, 255, 255 };   
const Vec3b BLUE   = { 255, 0, 0 };
const Vec3b MAGENTA   = { 255, 0, 255 }; 
const Vec3b CYAN      = { 255, 255, 0 };
const Vec3b PURPLE    = { 128, 0, 128 };
const Vec3b PINK      = { 203, 192, 255 };
const Vec3b SILVER    = { 192, 192, 192 };

void onePointTransform(vector<vector<MyPoint>> faces, double r, Mat& image);

void XYProjection(vector<vector<MyPoint>> faces, Mat& image);

void setPixel(int x, int y, Mat& image, Vec3b color);

void fillBackground(Mat& image, Vec3b color);

vector<vector<MyPoint>> resize(vector<vector<MyPoint>> faces, double M);

vector<vector<MyPoint>> translate(vector<vector<MyPoint>> faces, double tx, double ty, double tz);

vector<vector<MyPoint>> deleteFaces(vector<vector<MyPoint>> faces);

vector<vector<MyPoint>> rotateFaces(vector<vector<MyPoint>> faces, MyPoint point, double phi);

#endif