#include "my_lib.h" 

#include <iostream>

const double PI = 3.14;

void showImage(const Mat image) {
    imshow("Display window", image);
}

void saveImage(Mat* image, string name) {
    imwrite("/Users/hisoka/Downloads/graphics5/result/" + name, *image);
}

vector<vector<MyPoint>> getDefaultCube() {
    vector<vector<MyPoint>> faces = {
        { {1, 0, 1}, {1, 1, 1},  {0, 1, 1}, {0, 0, 1}, }, // верхняя грань
        { {0, 0, 0}, {0, 1, 0}, {1, 1, 0}, {1, 0, 0} }, // нижняя грань
        { {1, 0, 1}, {0, 0, 1}, {0, 0, 0}, {1, 0, 0}, }, // левая грань
        { {1, 1, 0},  {0, 1, 0},  {0, 1, 1}, {1, 1, 1}, }, // правая грань
        { {0, 0, 0}, {0, 0, 1}, {0, 1, 1}, {0, 1, 0}, }, // задняя грань
        { {1, 1, 1}, {1, 0, 1}, {1, 0, 0}, {1, 1, 0} }, // передняя грань
    };

    faces = resize(faces, 200);

    faces = translate(faces, 200, 200, 200);

    return faces;
}

void XYProjectionTest() {
    Mat image = Mat::zeros({ 1000, 1000 }, CV_8UC3);

    fillBackground(image, BLACK);

    vector<vector<MyPoint>> faces = getDefaultCube();

    faces = rotateFaces(faces, { 10, 10, 10 }, PI / 4);

    XYProjection(faces, image);

    saveImage(&image, "XYProjection.png");
}

void transformPoint(double r) {
    Mat image = Mat::zeros({ 1000, 1000 }, CV_8UC3);

    fillBackground(image, BLACK);

    vector<vector<MyPoint>> faces = getDefaultCube();

    onePointTransform(faces, r, image);

    saveImage(&image, "point_transform.png");
}

void deleteFaces() {
    Mat image = Mat::zeros({ 1000, 1000 }, CV_8UC3);

    fillBackground(image, BLACK);

    vector<vector<MyPoint>> faces = getDefaultCube();

    faces = rotateFaces(faces, { 10, 10, 10 }, PI / 4);

    faces = deleteFaces(faces);

    XYProjection(faces, image);

    saveImage(&image, "without_faces.png");
}

void rotateXYProjection() {
    double angle = 0;

    while (angle < PI * 2) {
        Mat image = Mat::zeros({ 1000, 1000 }, CV_8UC3);

        fillBackground(image, BLACK);

        vector<vector<MyPoint>> faces = getDefaultCube();

        faces = rotateFaces(faces, { 10, 10, 10 }, angle);

        XYProjection(faces, image);

        saveImage(&image, "default_rotation/r" + to_string(angle) + ".png");

        angle += PI / 16;
    }
}

void rotatePointProjection(double r) {
    double angle = 0;

    while (angle < PI * 2) {
        Mat image = Mat::zeros({ 1000, 1000 }, CV_8UC3);

        fillBackground(image, BLACK);

        vector<vector<MyPoint>> faces = getDefaultCube();

        faces = rotateFaces(faces, { 10, 10, 10 }, angle);

        onePointTransform(faces, r, image);

        saveImage(&image, "point_rotation/r" + to_string(angle) + ".png");

        angle += PI / 16;
    }
}

int main()
{
    XYProjectionTest();
    transformPoint(-0.0005);
    deleteFaces();
    rotateXYProjection();
    rotatePointProjection(-0.0008);
}
