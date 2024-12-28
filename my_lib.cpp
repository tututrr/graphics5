#include "my_lib.h" 

using namespace std;
using namespace cv;

void setPixel(int x, int y, Mat& image, Vec3b color) {
    if (x >= 0 && x < image.cols && y >= 0 && y < image.rows) {
        image.at<Vec3b>(y, x) = color;
    }
}

void fillBackground(Mat& image, Vec3b color) {
    for (int y = 0; y < image.rows; ++y) {
        for (int x = 0; x < image.cols; ++x) {
            setPixel(x, y, image, color);
        }
    }
}

void drawLine(int x1, int y1, int x2, int y2, Mat& image, Vec3b color) {
    int x = x1, y = y1;
    int dx = x2 - x1, dy = y2 - y1;
    int e, i;
    int ix = (dx > 0) - (dx < 0);  // -1, 0, or 1
    int iy = (dy > 0) - (dy < 0);  // -1, 0, or 1
    dx = abs(dx);
    dy = abs(dy);

    if (dx >= dy) {  // Основной алгоритм для наклонов с меньшим dx
        e = 2 * dy - dx;
        if (iy >= 0) {
            for (i = 0; i <= dx; i++) {
                setPixel(x, y, image, color);
                if (e >= 0) {
                    y += iy;
                    e -= 2 * dx;
                }
                x += ix;
                e += 2 * dy;
            }
        } else {
            for (i = 0; i <= dx; i++) {
                setPixel(x, y, image, color);
                if (e > 0) {
                    y += iy;
                    e -= 2 * dx;
                }
                x += ix;
                e += 2 * dy;
            }
        }
    } else {  // Основной алгоритм для наклонов с меньшим dy
        e = 2 * dx - dy;
        if (ix >= 0) {
            for (i = 0; i <= dy; i++) {
                setPixel(x, y, image, color);
                if (e >= 0) {
                    x += ix;
                    e -= 2 * dy;
                }
                y += iy;
                e += 2 * dx;
            }
        } else {
            for (i = 0; i <= dy; i++) {
                setPixel(x, y, image, color);
                if (e > 0) {
                    x += ix;
                    e -= 2 * dy;
                }
                y += iy;
                e += 2 * dx;
            }
        }
    }
}


void drawPolygon(const vector<MyPoint2d>& points, Mat& image, Vec3b borderColor, int n) {
    for (int i = 0; i < points.size(); ++i) {
        drawLine(points[i].x, points[i].y, points[(i + 1) % n].x, points[(i + 1) % n].y, image, borderColor);
    }
}

vector<vector<MyPoint>> transform(vector<vector<MyPoint>> faces, vector<vector<double>> T) {
    vector<vector<MyPoint>> result = {};

    for (int i = 0; i < faces.size(); ++i) {

        vector<MyPoint> face = {};

        for (int j = 0; j < 4; ++j) {

            MyPoint p = faces[i][j];
          
            
            double x_new = p.x * T[0][0] + p.y * T[1][0] + p.z * T[2][0] + T[3][0];
            double y_new = p.x * T[0][1] + p.y * T[1][1] + p.z * T[2][1] + T[3][1];
            double z_new = p.x * T[0][2] + p.y * T[1][2] + p.z * T[2][2] + T[3][2];
            double H     = p.x * T[0][3] + p.y * T[1][3] + p.z * T[2][3] + T[3][3];

            if (H == 0) {
                H = 1;
            }

            MyPoint p_new = { x_new / H, y_new / H, z_new / H };

            face.push_back(p_new);
        }

        result.push_back(face);
    }

	return result;
}

vector<vector<MyPoint2d>> get2d(vector<vector<MyPoint>> faces) {
    vector<vector<MyPoint2d>> result = {};

    for (int i = 0; i < faces.size(); ++i) {
        vector<MyPoint2d> face = {};

        for (int j = 0; j < 4; ++j) {
            MyPoint p = faces[i][j];
            face.push_back({ p.x, p.y });
        }

        result.push_back(face);
    }

    return result;
}

void drawCub(vector<vector<MyPoint>>& faces, Mat& image, Vec3b borderColor) {
    vector<vector<MyPoint2d>> faces2d = get2d(faces);

    for (int i = 0; i < faces.size(); ++i) {
        drawPolygon(faces2d[i], image, borderColor, faces2d[i].size());
    }
}

void XYProjection(vector<vector<MyPoint>> faces, Mat& image) {
    vector<vector<double>> T = {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 1},
    };

    vector<vector<MyPoint>> new_faces = transform(faces, T);

    drawCub(new_faces, image, MAGENTA);
}

void onePointTransform(vector<vector<MyPoint>> faces, double r, Mat& image) {
    vector<vector<double>> T = {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, r},
        {0, 0, 0, 1},
    };

    vector<vector<MyPoint>> new_faces = transform(faces, T);

    drawCub(new_faces, image, MAGENTA);
}

vector<vector<MyPoint>> resize(vector<vector<MyPoint>> faces, double M) {
    vector<vector<double>> T = {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1/M},
    };

    return transform(faces, T);
}

vector<vector<MyPoint>> translate(vector<vector<MyPoint>> faces, double tx, double ty, double tz) {
    vector<vector<double>> T = {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {tx, ty, tz, 1},
    };

    return transform(faces, T);
}

vector<vector<MyPoint>> deleteFaces(vector<vector<MyPoint>> faces) {
    vector<vector<MyPoint>> result = {};

    for (int i = 0; i < faces.size(); ++i) {
        MyPoint v1 = faces[i][0];
        MyPoint v2 = faces[i][1];
        MyPoint v3 = faces[i][2];

        MyPoint u = { v2.x - v1.x, v2.y - v1.y, v2.z - v1.z };
        MyPoint w = { v3.x - v1.x, v3.y - v1.y, v3.z - v1.z };

        MyPoint n = {
            u.y * w.z - u.z * w.y,
            u.z * w.x - u.x * w.z,
            u.x * w.y - u.y * w.x
        };

        if (n.z * -1 > 0) {
            result.push_back(faces[i]);
        }
    }

    return result;
}

vector<vector<MyPoint>> rotateFaces(vector<vector<MyPoint>> faces, MyPoint point, double phi) {
    double dist = sqrt(point.x * point.x + point.y * point.y + point.z * point.z);

    double nx = point.x / dist;
    double ny = point.y / dist;
    double nz = point.z / dist;

    vector<vector<double>> T = {
        {cos(phi) + nx * nx * (1 - cos(phi)), nx * ny * (1 - cos(phi)) + nz * sin(phi), nx * nz * (1 - cos(phi)) - ny * sin(phi), 0},
        {nx * ny * (1 - cos(phi)) - nz * sin(phi), cos(phi) + ny * ny * (1 - cos(phi)), ny * nz * (1 - cos(phi)) + nx * sin(phi), 0},
        {nx*nz*(1-cos(phi))+ny*sin(phi), ny*nz*(1-cos(phi))-nx*sin(phi), cos(phi)+nz*nz*(1-cos(phi)), 0},
        {0, 0, 0, 1}
    };

    return transform(faces, T);
}