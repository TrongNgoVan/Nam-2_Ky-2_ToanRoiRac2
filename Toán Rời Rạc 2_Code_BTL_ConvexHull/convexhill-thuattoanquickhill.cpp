#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;

struct Point {
    double x, y, z;
};
 
struct Face {
    int a, b, c;
    bool visible;
};

vector<Point> points;
vector<Face> faces;
// tính khoảng cách AB;
double dist(Point a, Point b) {
    return sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y) + (a.z - b.z)*(a.z - b.z));
}
// tính khoảng cach từ điểm p đến mặt phẳng chứa a,b,c;
double dist(Face f, Point p) {
    Point a = points[f.a];
    Point b = points[f.b];
    Point c = points[f.c];
    double d = ((b.y - a.y)*(c.z - a.z) - (b.z - a.z)*(c.y - a.y))*p.x
        + ((b.z - a.z)*(c.x - a.x) - (b.x - a.x)*(c.z - a.z))*p.y
        + ((b.x - a.x)*(c.y - a.y) - (b.y - a.y)*(c.x - a.x))*p.z
        - (a.x*(b.y*c.z - c.y*b.z) + a.y*(b.z*c.x - c.z*b.x) + a.z*(b.x*c.y - c.x*b.y));
    return abs(d) / sqrt((b.y - a.y)*(b.y - a.y) + (b.z - a.z)*(b.z - a.z) + (c.y - b.y)*(c.y - b.y) + (c.z - b.z)*(c.z - b.z) + (a.y - c.y)*(a.y - c.y) + (a.z - c.z)*(a.z - c.z));
}

// thêm một mặt phẳng vào tập mặt phẳng
void add_face(int a, int b, int c) {
    Face f = { a, b, c, true };
    faces.push_back(f);
}

void build_hull(int a, int b, int c) {
    if (faces.size() > 1000) return; // giới hạn độ sâu đệ quy để ngăn vòng lặp vô hạn
    bool found_face = false;

    for (int i = 0; i < points.size(); i++) {
        if (i == a || i == b || i == c) continue;
        Point p = points[i];
        double d1 = dist(points[a], p);
        double d2 = dist(points[b], p);
        double d3 = dist(points[c], p);

        if (d1 >= 0 && d2 >= 0 && d3 >= 0) {
            found_face = true;
            add_face(a, b, i);
            add_face(b, c, i);
            add_face(c, a, i);
            build_hull(i, b, a);
            build_hull(a, c, i);
            build_hull(b, i, c);
            break;
        }
    }

    if (!found_face) {
        for (auto &f : faces) {
            if (f.visible && dist(f, points[a]) <= 0) {
                f.visible = false;
                build_hull(f.b, f.a, a);
                build_hull(f.c, f.b, a);
                build_hull(f.a, f.c, a);
            }
        }
    }
}

vector<Face> quickhull() {
    // tìm các điểm có tọa độ x nhỏ nhất và lớn nhất
    int min_x = 0, max_x = 0;
    for (int i = 1; i < points.size(); i++) {
        if (points[i].x < points[min_x].x) min_x = i;
        if (points[i].x > points[max_x].x) max_x = i;
    }

    // thêm tứ diện ban đầu
    add_face(min_x, max_x, -1); // thêm một điểm bổ sung với tọa độ z thấp nhất để ngăn chặn sự đồng phẳng
    for (int i = 0; i < points.size(); i++) {
        if (i == min_x || i == max_x) continue;
        Point p = points[i];
        double d1 = dist(points[min_x], p);
        double d2 = dist(points[max_x], p);
        double d3 = dist(points[faces[0].a], p);
        double d4 = dist(points[faces[0].b], p);
        double d5 = dist(points[faces[0].c], p);
        if (d1 > 0 && d2 > 0 && d3 > 0 && d4 > 0 && d5 > 0) {
            add_face(min_x, max_x, i);
            add_face(max_x, faces.back().b, i);
            add_face(faces.back().c, min_x, i);
            add_face(faces.back().a, faces.back().c, i);
        }
    }

    // đệ quy xây dựng  the hull
    for (auto &f : faces) {
        if (f.visible) {
            build_hull(f.a, f.b, f.c);
        }
    }

    // loại bỏ những khuôn mặt không phải là một phần của the hull
    vector<Face> hull_faces;
    for (auto &f : faces) {
        if (f.visible) {
            hull_faces.push_back(f);
        }
    }

    return hull_faces;
}

int main() {
    // input the points
    int n;
    cout << "Enter the number of points: ";
    cin >> n;
    for (int i = 0; i < n; i++) {
        Point p;
        cout << "Enter point " << i+1 << ": ";
        cin >> p.x >> p.y >> p.z;
        points.push_back(p);
    }

    // find the convex hull
    vector<Face> hull_faces = quickhull();

    // output the faces of the convex hull
    cout << "Faces of the convex hull:" << endl;
    for (auto &f : hull_faces) {
        cout << "(" << f.a << ", " << f.b << ", " << f.c << ")" << endl;
    }

    return 0;
}
