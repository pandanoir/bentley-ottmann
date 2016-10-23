#include<iostream>
#include<vector>
#include<queue>
#include<utility>
#include "binary-search-node.h"


const int LEFT_END_POINT = 0;
const int RIGHT_END_POINT = 1;
const int INTERSECTION = 2;
const double HEIGHT = 1000;

using namespace std;

class Point {
public:
    double x;
    double y;
    Point(double _x, double _y) :x(_x), y(_y) {}
    Point() :x(0), y(0) {}
    bool operator> (const Point& p) const { return y > p.y; }/*{{{*/
    bool operator== (const Point& p) const { return y == p.y; }
    inline bool operator<(const Point& p) const { return !(*this > p || *this == p); }
    inline bool operator>=(const Point& p) const { return *this > p || *this == p; }
    inline bool operator<=(const Point& p) const { return !(*this > p); }
    inline bool operator!=(const Point& p) const { return !(*this == p); }/*}}}*/
};
ostream& operator<<(ostream& os, const Point& p) {/*{{{*/
    os << "(" << p.x << ", " << p.y << ")";
    return os;
}/*}}}*/
class Segment {
public:
    Point left_ep; // end point
    Point right_ep; // end point
    Segment(Point l, Point r) : left_ep(l), right_ep(r) {};
    Segment() : left_ep(Point(-1, 0)), right_ep(Point(-1, 0)) {};
    bool intersects(const Segment& s) const {/*{{{*/
        const Point &a = left_ep,
            &b = right_ep,
            &c = s.left_ep,
            &d = s.right_ep;
        const double ta = (c.x - d.x) * (a.y - c.y) + (c.y - d.y) * (c.x - a.x),
            tb = (c.x - d.x) * (b.y - c.y) + (c.y - d.y) * (c.x - b.x),
            tc = (a.x - b.x) * (c.y - a.y) + (a.y - b.y) * (a.x - c.x),
            td = (a.x - b.x) * (d.y - a.y) + (a.y - b.y) * (a.x - d.x);
        return tc * td < 0 && ta * tb < 0;
    }/*}}}*/
    Point intersection(const Segment& s) const {/*{{{*/
        Point p1 = left_ep, p2 = right_ep, p3 = s.left_ep, p4 = s.right_ep;

        if (p1.x == p2.x) {
            const double c = (p4.y - p3.y) / (p4.x - p3.x);
            const double d = p3.y - c * p3.x;
            return Point(p1.x, c * p1.x + d);
        }
        if (p3.x == p4.x) {
            const double a = (p2.y - p1.y) / (p2.x - p1.x);
            const double b = p1.y - a * p1.x;
            return Point(p3.x, a * p3.x + b);
        }

        const double a = (p2.y - p1.y) / (p2.x - p1.x),
            b = p1.y - a * p1.x,
            c = (p4.y - p3.y) / (p4.x - p3.x),
            d = p3.y - c * p3.x;
        return Point(-(b-d)/(a-c), a * (-(b-d)/(a-c)) + b);
    }/*}}}*/
    bool equals(const Segment& s) const {
    return left_ep.x == s.left_ep.x && left_ep.y == s.left_ep.y &&
        right_ep.x == s.right_ep.x && right_ep.y == s.right_ep.y;
    }
    friend bool operator> (const Segment& lhs, const Segment& rhs);/*{{{*/
    friend bool operator== (const Segment& lhs, const Segment& rhs);
    inline bool operator<(const Segment& s) const { return !(*this > s || *this == s); }
    inline bool operator>=(const Segment& s) const { return *this > s || *this == s; }
    inline bool operator<=(const Segment& s) const { return !(*this > s); }
    inline bool operator!=(const Segment& s) const { return !(*this == s); }/*}}}*/
};
Segment SL = Segment(Point(0, 0), Point(0, HEIGHT));
bool operator> (const Segment& lhs, const Segment& rhs) { return lhs.intersection(SL).y > rhs.intersection(SL).y; }
bool operator== (const Segment& lhs, const Segment& rhs) { return lhs.intersection(SL).y == rhs.intersection(SL).y; }
ostream& operator<<(ostream& os, const Segment& s) {/*{{{*/
    // os << "[ " << s.left_ep << ", " << s.right_ep << " ]";
    os << SL.intersection(s).y << "(" << s.left_ep << s.right_ep << ")";
    return os;
}/*}}}*/

class Event {
public:
    Segment s;
    Segment a;
    Segment b;
    int type;
    Event(Segment _s, int _type) : s(_s), a(), b(), type(_type) {};
    Event(Segment _s, int _type, Segment _a, Segment _b) : s(_s), a(_a), b(_b), type(_type) {};
    bool operator> (const Event& e) const {/*{{{*/
        double x = s.left_ep.x, _x = e.s.left_ep.x;
        if (type == RIGHT_END_POINT) x = s.right_ep.x;
        if (e.type == RIGHT_END_POINT) _x = e.s.right_ep.x;
        if (e.s.equals(s) && x == _x) return type == RIGHT_END_POINT;
        return (x == _x && e.type == LEFT_END_POINT && type == RIGHT_END_POINT) || x > _x;
    }
    bool operator== (const Event& e) const {
        double x = s.left_ep.x, _x = e.s.left_ep.x;
        if (type == RIGHT_END_POINT) x = s.right_ep.x;
        if (e.type == RIGHT_END_POINT) _x = e.s.right_ep.x;
        return x == _x;
    }
    inline bool operator<(const Event& e) const { return !(*this > e || *this == e); }
    inline bool operator>=(const Event& e) const { return *this > e || *this == e; }
    inline bool operator<=(const Event& e) const { return !(*this > e); }
    inline bool operator!=(const Event& e) const { return !(*this == e); }/*}}}*/
};

int main() {
    binarySearchNode<Segment> T;
    priority_queue<Event, vector<Event>, greater<Event>> Q;
    // 14
    vector<Segment> segments = {/*{{{*/
        Segment(Point(29, 30), Point(58, 63)),
        Segment(Point(37, 84), Point(46, 11)),
        Segment(Point(50, 84), Point(55, 53)),
        Segment(Point(2, 77), Point(14, 97)),
        Segment(Point(22, 45), Point(31, 43)),
        // Segment(Point(8, 87), Point(70, 43)),
        Segment(Point(1, 51), Point(96, 2)),
        Segment(Point(80, 43), Point(92, 15)),
        Segment(Point(26, 42), Point(95, 5)),
        /*Segment(Point(26, 10), Point(76, 28)),
          Segment(Point(25, 57), Point(80, 64)),
          Segment(Point(4, 57), Point(28, 88)),
          Segment(Point(46, 54), Point(58, 67)),
          Segment(Point(4, 29), Point(40, 18)),
          Segment(Point(53, 74), Point(98, 86)),
          Segment(Point(45, 8), Point(62, 7)),
          Segment(Point(7, 1), Point(47, 80)),
          Segment(Point(72, 51), Point(90, 64)),
          Segment(Point(4, 60), Point(56, 3)),
          Segment(Point(57, 48), Point(79, 28)),
          Segment(Point(8, 56), Point(44, 45)),
          Segment(Point(31, 51), Point(53, 74)),
          Segment(Point(16, 23), Point(72, 39)),*/
        Segment(Point(9, 95), Point(50, 34))
    };/*}}}*/
    vector<Segment>::iterator itr = segments.begin();
    vector<Point> res;
    while (itr != segments.end()) {
        Q.push(Event(*itr, LEFT_END_POINT));
        Q.push(Event(*itr, RIGHT_END_POINT));
        itr++;
    }
    auto intersection = [&Q, &res](Segment a, Segment b) {
        if (!a.equals(Segment()) && !b.equals(Segment()) && a.intersects(b)) {
            cout << "INTERSECTION" << endl;
            Point p = a.intersection(b);
            if (a < b) Q.push(Event(Segment(p, p), INTERSECTION, a, b));
            else Q.push(Event(Segment(p, p), INTERSECTION, b, a));
            res.push_back(p);
        }
    };
    const vector<string> type = {"LEFT", "RIGHT", "INTER"};
    while (!Q.empty()) {
        Event e = Q.top();
        Q.pop();
        cout << "==========";
        cout << (e.type == LEFT_END_POINT ? "LEFT" : e.type == RIGHT_END_POINT ? "RIGHT" : "INTER");
        cout << " step==========" << endl;
        if (e.type == LEFT_END_POINT) {
            SL = Segment(Point(e.s.left_ep.x, 0), Point(e.s.left_ep.x, HEIGHT));
            cout << T << endl;
            Segment r, t;
            T.insert(e.s);
            T.above_s(e.s, r);
            T.below_s(e.s, t);

            intersection(r, e.s);
            intersection(t, e.s);
            if (!r.equals(Segment()) && !t.equals(Segment()) && r.intersects(t)) {
                Point p = r.intersection(t);
                vector<Event> _Q;
                while (!Q.empty() && !Q.top().s.equals(Segment(p, p))) {
                    _Q.push_back(Q.top());
                    Q.pop();
                }
                Q.pop();
                vector<Event>::iterator itr = _Q.begin();
                while (itr != _Q.end()) {
                    Q.push(*itr);
                    ++itr;
                }
            }
        } else if (e.type == RIGHT_END_POINT) {
            SL = Segment(Point(e.s.right_ep.x, 0), Point(e.s.right_ep.x, HEIGHT));
            cout << T << endl;
            Segment r, t;
            T.above_s(e.s, r);
            T.below_s(e.s, t);

            intersection(r, t);

            cout << "====================" << endl;
            cout << T << endl;
            cout << (e.s) << endl;
            cout << "====================" << endl;
            T.remove(e.s);
        } else if (e.type == INTERSECTION) {
            Segment r, t, s, u;
            cout << T << endl;
            cout << e.a.left_ep << e.a.right_ep << endl;
            s = e.a;
            cout << e.b.left_ep << e.b.right_ep << endl;
            t = e.b;
            cout << "swapstart"<<endl;
            T.swap(s, t);
            cout << "swap"<<endl;
            T.above_s(t, r);
            T.below_s(s, u);
            cout << "above";
            cout << "below";
            intersection(s, r);
            intersection(t, u);
            SL = Segment(Point(e.s.left_ep.x, 0), Point(e.s.left_ep.x, HEIGHT));
            cout << T << endl;
        }
        cout << T << endl;
    }
    {
        vector<Point>::iterator itr = res.begin();
        while (itr != res.end()) {
            cout << *itr << endl;
            itr++;
        }
    }

    return 0;
}
