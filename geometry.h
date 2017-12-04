#ifndef GEOMETRY_H
#define GEOMETRY_H

#define VAR_TYPE    float
#include<math.h>
#include <iostream>

class Vector2 {
public:
    Vector2(VAR_TYPE _x = 0, VAR_TYPE _y = 0) : x(_x), y(_y){}
    Vector2(const Vector2 & p):x(p.x), y(p.y){}

    float length() const{return float(sqrt(x*x + y*y));}
    static Vector2 abss(const Vector2 & other){ Vector2 r; r.x =abs(other.x); r.y=abs(other.y); return r; }
    friend std::ostream & operator << (std::ostream & out, const Vector2 & v) { return out << v.x << " " << v.y; }

    VAR_TYPE x, y;
};


inline static float dot(const Vector2 & u, const Vector2 & v) { return (u.x*v.x + u.y*v.y); }
inline Vector2 operator -(const Vector2 & v, const Vector2 & u){Vector2 res; res.x = v.x-u.x; res.y = v.y-u.y; return res;}
inline Vector2 operator +(const Vector2 & v, const Vector2 & u){Vector2 res; res.x = v.x+u.x; res.y = v.y+u.y; return res;}
inline Vector2 operator *(const Vector2 & v, const Vector2 & u){Vector2 res; res.x = v.x*u.x; res.y = v.y*u.y; return res;}
inline Vector2 operator *(const Vector2 & u, const float other){Vector2 r; r.x=other*u.x; r.y=other*u.y;  return r;}
inline Vector2 operator *(const float other, const Vector2 & u){Vector2 r; r.x=other*u.x; r.y=other*u.y;  return r;}
inline Vector2 operator /(const Vector2 & u, float other){Vector2 r; r.x=u.x/other; r.y=u.y/other;  return r;}

inline static Vector2 normalize(const Vector2 & v){Vector2 r = (v/v.length()); return r;}

class Vector3 {
public:
    Vector3(VAR_TYPE _x = 0, VAR_TYPE _y = 0, VAR_TYPE _z = 0) : x(_x), y(_y), z(_z) {}
    Vector3(const Vector3 & p):x(p.x), y(p.y), z(p.z){}

    float length() const{return float(sqrt(x*x + y*y + z*z));}
    static Vector3 abss(const Vector3 & other){ Vector3 r; r.x =abs(other.x); r.y=abs(other.y); r.z=abs(other.z); return r; }
    friend std::ostream & operator << (std::ostream & out, const Vector3 & v) { return out << v.x << " " << v.y << " " << v.z; }

    VAR_TYPE x, y, z;
};

inline static Vector3 cross(const Vector3 & u, const Vector3 & v) { return Vector3(u.y * v.z - u.z * v.y, u.z * v.x - u.x * v.z, u.x * v.y - u.y * v.x); }
inline static float dot(const Vector3 & u, const Vector3 & v) { return (u.x*v.x + u.y*v.y + u.z*v.z); }
inline Vector3 operator -(const Vector3 & v,const Vector3 & u){Vector3 res; res.x = v.x-u.x; res.y = v.y-u.y; res.z = v.z-u.z; return res;}
inline Vector3 operator +(const Vector3 & v, const Vector3 & u){Vector3 res; res.x = v.x+u.x; res.y = v.y+u.y; res.z = v.z+u.z; return res;}
inline Vector3 operator *(const Vector3 & v, const Vector3 & u){Vector3 res; res.x = v.x*u.x; res.y = v.y*u.y; res.z = v.z*u.z; return res;}
inline Vector3 operator *(const Vector3 & u, float other){Vector3 r; r.x=other*u.x; r.y=other*u.y; r.z=other*u.z;  return r;}
inline Vector3 operator *(float other, const Vector3 & u){Vector3 r; r.x=other*u.x; r.y=other*u.y; r.z=other*u.z;  return r;}
inline Vector3 operator /(const Vector3 & u, float other){Vector3 r; r.x=u.x/other; r.y=u.y/other; r.z=u.z/other;  return r;}

inline static Vector3 normalize(const Vector3 & v){Vector3 r = (v/v.length()); return r;}



class Box2{
public:
    Box2(Vector2 pmi, Vector2 pma): pmin(pmi), pmax(pma){}
    bool inBox(const Vector2 & p);

    Vector2 pmin;
    Vector2 pmax;
};

class Box3{
public:
    Box3(Vector3 pmi, Vector3 pma): pmin(pmi), pmax(pma){}
    bool inBox(const Vector3 & p);
    bool hitTest(const Box3 & b3);

    Vector3 pmin;
    Vector3 pmax;
};

#endif // GEOMETRY_H
