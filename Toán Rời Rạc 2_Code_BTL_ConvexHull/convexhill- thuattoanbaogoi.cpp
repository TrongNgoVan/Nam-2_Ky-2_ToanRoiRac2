#include <bits/stdc++.h>
using namespace std;

// Kiểu điểm
struct Point {
    int x, y;
    bool operator==(const Point &o) {
        return x == o.x && y == o.y;
    }
};

// Tích có hướng của AB và AC
long long cross(const Point &A, const Point &B, const Point &C) {
    return 1LL * (B.x - A.x) * (C.y - A.y) - 1LL * (C.x - A.x) * (B.y - A.y);
}

// Tích vô hướng của AB và AC
long long dot(const Point &A, const Point &B, const Point &C) {
    return 1LL * (B.x - A.x) * (C.x - A.x) + 1LL * (B.y - A.y) * (C.y - A.y);
}

// C nằm trên đoạn AB nếu ABxAC = 0 và CA.CB <= 0
bool onSegment(const Point &A, const Point &B, const Point &C) {
    return cross(A, B, C) == 0 && dot(C, A, B) <= 0;
}

// A -> B -> C đi theo thứ tự cùng chiều kim đồng hồ
bool cw(const Point &A, const Point &B, const Point &C) {
    return cross(A, B, C) < 0;
}

// A -> B -> C đi theo thứ tự ngược chiều kim đồng hồ
bool ccw(const Point &A, const Point &B, const Point &C) {
    return cross(A, B, C) > 0;
}

// Trả về bao lồi với thứ tự các điểm được liệt kê theo chiều kim đồng hồ
vector<Point> convexHull(vector<Point> p, int n) {
    // Sắp xếp các điểm theo tọa độ x, nếu bằng nhau sắp xếp theo y
    sort(p.begin(), p.end(), [](const Point &A, const Point &B) {
        if (A.x != B.x) return A.x < B.x;
        return A.y < B.y;
    });

    // Tập bao lồi
    vector<Point> hull;
    hull.push_back(p[0]);

    // Dựng bao trên
    for (int i = 1; i < n; ++i) {
        while (hull.size() >= 2 && ccw(hull[hull.size() - 2], hull.back(), p[i])) {
            hull.pop_back();
        }
        hull.push_back(p[i]);
    }

    // Tiếp tục dựng bao dưới
    for (int i = n - 2; i >= 0; --i) {
        while (hull.size() >= 2 && ccw(hull[hull.size() - 2], hull.back(), p[i])) {
            hull.pop_back();
        }
        hull.push_back(p[i]);
    }

    // Xoá đỉểm đầu được lặp lại ở cuối
    if (n > 1) hull.pop_back();

    return hull;
}

// Kiểm tra P có nằm trong bao lồi hull hay không
bool checkInHull(vector<Point> &hull, Point P) {
    int n = hull.size();

    // Xử lý trường hợp suy biến có diện tích bao lồi = 0
    if (n == 1) return (hull[0] == P);
    if (n == 2) return onSegment(hull[0], hull[1], P);

    // Nếu (hull[0], hull[1], P) ngược chiều kim đồng hồ thì P nằm ngoài bao lồi
    if (ccw(hull[0], hull[1], P)) return false;

    // Nếu (hull[n - 1], hull[0], P) không cùng chiều kim đồng hồ thì P chỉ thoả
    // nếu P nằm trên đoạn (hull[n - 1], hull[0])
    if (!cw(hull[n - 1], hull[0], P)) {
        return onSegment(hull[n - 1], hull[0], P);
    }

    // Tìm x thoả mãn tia (hull[0], hull[x]) là tia gần nhất ở phía bên phải của P
    int lo = 2, hi = n - 1, x = -1;
    while (lo <= hi) {
        int mid = (lo + hi) >> 1;
        // Nếu (hull[0], hull[mid], P) ngược chiều kim đồng hồ thì
        // tia (hull[0], hull[mid]) nằm ở phía bên phải của P
        if (ccw(hull[0], hull[mid], P)) {
            x  = mid;
            hi = mid - 1;
        }
        else lo = mid + 1;
    }

    // P nằm trong tam giác (hull[0], hull[x - 1], hull[x]) 
    // nếu (hull[x - 1], hull[x], P) không ngược chiều kim đồng hồ
    return !ccw(hull[x - 1], hull[x], P);
}

int main() {
    int n;
    cin >> n;
    vector<Point> p(n);
    for (Point &a : p) {
        cin >> a.x >> a.y;
    }
    vector<Point> hull = convexHull(p, n);

    int m;
    cin >> m;
    while (m--) {
        Point P;
        cin >> P.x >> P.y;
        cout << (checkInHull(hull, P) ? "YES\n" : "NO\n");
    }
}