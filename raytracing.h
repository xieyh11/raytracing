#ifndef RAYTRACING_H_INCLUDED
#define RAYTRACING_H_INCLUDED

#include <cmath>
#include <vector>
using namespace std;

const double FAULT = 1e-8;

class _Point;
class _Vector{
public:
    double delx;
    double dely;
    double delz;
    _Vector();
    _Vector(double delx, double dely, double delz);
    _Vector(_Point &p0, _Point &p1);

    void setVector(_Point &p0, _Point &p1);

    void setVector(double delx, double dely, double delz);

    void vectorUnit();

    void vectorUnit(double lenver);

    void vectorLimit();

    _Vector timesNumber(double r);

    _Vector times(_Vector &v);

    _Vector oppositeVector();

    _Vector plusVector(_Vector &v);

    _Vector minusVector(_Vector &v);

    double timesVector(_Vector &v);

    double timesPoint(_Point &p);
};

class _Point{
public:
    double x;
    double y;
    double z;
    _Point(){
        this->x = this->y = this->z = 0;
    }
    _Point(double x, double y, double z){
        this->x = x;
        this->y = y;
        this->z = z;
    }

    void setPoint(double x, double y, double z){
        this->x = x;
        this->y = y;
        this->z = z;
    }
    _Point addVector(_Vector &v){
        _Point p;
        p.x = this->x + v.delx;
        p.y = this->y + v.dely;
        p.z = this->z + v.delz;
        return p;
    }
    _Vector toVector(){
        _Vector v;
        v.delx = this->x;
        v.dely = this->y;
        v.delz = this->z;
        return v;
    }
};


_Vector::_Vector(){
    this->delx = this->dely = this->delz = 0;
}
_Vector::_Vector(double delx, double dely, double delz){
    this->delx = delx;
    this->dely = dely;
    this->delz = delz;
}
_Vector::_Vector(_Point &p0, _Point &p1){
    this->delx = p0.x - p1.x;
    this->dely = p0.y - p1.y;
    this->delz = p0.z - p1.z;
}

void _Vector::setVector(_Point &p0, _Point &p1){
    this->delx = p0.x - p1.x;
    this->dely = p0.y - p1.y;
    this->delz = p0.z - p1.z;
}

void _Vector::setVector(double delx, double dely, double delz){
    this->delx = delx;
    this->dely = dely;
    this->delz = delz;
}

void _Vector::vectorUnit(){
    double lenver = 1/sqrt(this->delx*this->delx + this->dely*this->dely + this->delz*this->delz);
    this->delx = lenver * this->delx;
    this->dely = lenver * this->dely;
    this->delz = lenver * this->delz;
}

void _Vector::vectorUnit(double lenver){
    this->delx = lenver * this->delx;
    this->dely = lenver * this->dely;
    this->delz = lenver * this->delz;
}

void _Vector::vectorLimit(){
    if (this->delx > 1 || this->dely > 1 || this->delz > 1){
        double max = this->delx;
        if (this->dely > max){
            max = this->dely;
        }
        if (this->delz > max){
            max = this->delz;
        }
        max = 1/max;
        this->delx = max*this->delx;
        this->dely = max*this->dely;
        this->delz = max*this->delz;
    }
}

_Vector _Vector::timesNumber(double r){
    _Vector v;
    v.delx = this->delx * r;
    v.dely = this->dely * r;
    v.delz = this->delz * r;
    return v;
}

_Vector _Vector::times(_Vector &v){
    _Vector tmp;
    tmp.delx = this->delx * v.delx;
    tmp.dely = this->dely * v.dely;
    tmp.delz = this->delz * v.delz;
    return tmp;
}

_Vector _Vector::oppositeVector(){
    _Vector v;
    v.delx = -this->delx;
    v.dely = -this->dely;
    v.delz = -this->delz;
    return v;
}

_Vector _Vector::plusVector(_Vector &v){
    _Vector tmp;
    tmp.delx = this->delx + v.delx;
    tmp.dely = this->dely + v.dely;
    tmp.delz = this->delz + v.delz;
    return tmp;
}

_Vector _Vector::minusVector(_Vector &v){
    _Vector tmp;
    tmp.delx = this->delx - v.delx;
    tmp.dely = this->dely - v.dely;
    tmp.delz = this->delz - v.delz;
    return tmp;
}

double _Vector::timesVector(_Vector &v){
    return v.delx * this->delx + v.dely * this->dely + v.delz * this->delz;
}

double _Vector::timesPoint(_Point &p){
    return this->delx * p.x + this->dely * p.y + this->delz * p.z;
}

double _det(_Vector &a0, _Vector &a1, _Vector &a2){
    double tmp = a0.delx * (a1.dely * a2.delz - a1.delz * a2.dely);
    tmp -= a1.delx * (a0.dely * a2.delz - a0.delz * a2.dely);
    tmp += a2.delx * (a0.dely * a1.delz - a0.delz * a1.dely);
    return tmp;
}

class _Ray{
public:
    _Point r0;
    _Vector rd;
    _Ray(_Point& r0, _Vector& rd){
        this->r0 = r0;
        this->rd = rd;
    }
    _Point atTime(double t){
        _Vector tmp = rd.timesNumber(t);
        return r0.addVector(tmp);
    }
};

enum _GeometryType{
    PLANE = 0,
    TRIANGLE,
    POLYGON,
    SPHERE,
    CUBOID
};

class _Light{
public:
    _Point l0;
    _Vector i;
};

class _Geometry {
public:
    _GeometryType geometrytype;
    _Vector kd;
    _Vector ka;
    _Vector ks;
    _Vector kt;
    _Vector _ks;
    _Vector _kt;
    int ns;
    int nt;
    int ref;
    _Vector phong(vector<_Light>* ig, _Light &ie, _Point &p, _Ray &r, _Vector &N, double ref){
        vector<_Vector> L(ig->size());
        vector<_Vector> R(ig->size());
        vector<_Vector> T(ig->size());
        _Vector tmp;
        for (int i = 0; i < ig->size(); i++)
        {
            L[i].setVector(ig->at(i).l0, p);
            L[i].vectorUnit();
            R[i] = N.timesNumber(2*N.timesVector(L[i]));
            R[i] = R[i].minusVector(L[i]);
            tmp = r.rd.oppositeVector();
            double theta1 = N.timesVector(tmp);
            double theta2 = sqrt(1-(1-theta1*theta1)/(ref*ref));
            T[i] = tmp.timesNumber(-1/ref);
            tmp = N.timesNumber(theta1/ref - theta2);
            T[i] = T[i].plusVector(tmp);
        }
        _Vector ipl;
        _Vector ipr;
        _Vector ipt;
        for (int i = 0; i < ig->size(); i++)
        {
            double theta = L[i].timesVector(N);
            if (theta > 0)
            {
                tmp = ig->at(i).i.timesNumber(theta);
                ipl = ipl.plusVector(tmp);
            }
            tmp = r.rd.oppositeVector();
            theta = R[i].timesVector(tmp);
            if (theta > 0){
                tmp = ig->at(i).i.timesNumber(pow(theta, ns));
                ipr = ipr.plusVector(tmp);
            }
            tmp = r.rd.oppositeVector();
            theta = T[i].timesVector(tmp);
            if (theta > 0){
                tmp = ig->at(i).i.timesNumber(pow(theta, nt));
                ipt = ipt.plusVector(tmp);
            }
        }

        _Vector res;
        tmp = this->kd.times(ipl);
        res = res.plusVector(tmp);
        tmp = this->ks.times(ipr);
        res = res.plusVector(tmp);
        tmp = this->kt.times(ipt);
        res = res.plusVector(tmp);
        tmp = this->ka.times(ie.i);
        res = res.plusVector(tmp);
        return res;
    }
    virtual bool isIn(_Point &min, _Point &max){return false;}
    virtual bool intersect(_Ray &r, double* t){return false;}
    virtual _Vector getN(_Point &p){return _Vector();}
};

class _Plane : public _Geometry{
public:
    _Point p0;
    _Vector n;
    double d;

    _Plane(_Point &p0, _Vector &n){
        this->p0 = p0;
        this->n = n;
        this->d = -n.timesPoint(p0);
        this->geometrytype = PLANE;
    }
    _Plane(){
        this->d = 0;
        this->geometrytype = PLANE;
    }

    void setPlane(_Point &p0, _Vector &n){
        this->p0 = p0;
        this->n = n;
        this->d = -n.timesPoint(p0);
    }

    bool isOn(_Point &p){
        if ((n.timesPoint(p) + this->d) < FAULT && (n.timesPoint(p) + this->d) > -FAULT){
            return true;
        }
        return false;
    }

    bool intersect(_Ray &r, double* t){
        double tmp = -(this->d + this->n.timesPoint(r.r0));
        tmp = tmp/(this->n.timesVector(r.rd));
        if (tmp > 0)
        {
            *t = tmp;
            return true;
        }else{
            return false;
        }
    }

    _Vector getN(_Point &p){
        return this->n;
    }
};

class _Triangle : public _Geometry{
public:
    _Point p0;
    _Point p1;
    _Point p2;

    _Triangle(){
        this->geometrytype = TRIANGLE;
    }
    _Triangle(_Point &p0, _Point &p1, _Point &p2){
        this->geometrytype = TRIANGLE;
        this->p0 = p0;
        this->p1 = p1;
        this->p2 = p2;
    }

    bool intersect(_Ray &r, double* t){
        _Vector e1(this->p0, this->p1);
        _Vector e2(this->p0, this->p2);
        _Vector s(this->p0, r.r0);

        _Vector res(_det(s, e1, e2), _det(r.rd, s, e2), _det(r.rd, e1, s));
        res = res.timesNumber(1/_det(r.rd, e1, e2));

        if (res.delx > 0 && res.dely >= 0 && res.delz >= 0
            && res.dely <= 1 && res.delz <= 1 && (res.dely + res.delz) <= 1)
        {
            *t = res.delx;
            return true;
        }else{
            return false;
        }
    }

    _Vector getN(_Point &p){
        _Vector e1(this->p0, this->p1), e2(this->p0, this->p2);
        return _Vector(e1.dely * e2.delz - e1.delz * e2.dely, e1.delz * e2.delx - e1.delx * e2.delz, e1.delx * e2.dely - e1.dely * e2.delx);
    }

    bool isIn(_Point &min, _Point &max){
        return min.x <= p0.x && min.y <= p0.y && min.z <= p0.z
                && max.x >= p0.x && max.y >= p0.y && max.z >= p0.z
                && min.x <= p1.x && min.y <= p1.y && min.z <= p1.z
                && max.x >= p1.x && max.y >= p1.y && max.z >= p1.z
                && min.x <= p2.x && min.y <= p2.y && min.z <= p2.z
                && max.x >= p2.x && max.y >= p2.y && max.z >= p2.z;
    }
};

class _Sphere : public _Geometry{
public:
    _Point pc;
    double r;

    _Sphere(){
        this->r = 0;
        this->geometrytype = SPHERE;
    }
    _Sphere(_Point &pc, double r){
        this->pc = pc;
        this->r = r;
        this->geometrytype = SPHERE;
    }

    bool intersect(_Ray &r, double* t){
        _Vector l(this->pc, r.r0);
        double letp = l.timesVector(r.rd);
        double lentp = letp * letp;
        double lenl = l.timesVector(l);
        double lenr = this->r * this->r;
        if ((lenl - lenr) > -FAULT && (lenl - lenr) < FAULT){
            *t = letp;
            return true;
        }else{
            if (lentp < 0)
            {
                return false;
            }
            double lend = lenl - lentp;
            if (lend > lenr)
            {
                return false;
            }
            double lent = lenr - lend;
            if (lenl > lenr)
            {
                *t = letp - sqrt(lent);
                return true;
            }else{
                *t = letp + sqrt(lent);
                return true;
            }
        }
    }

    _Vector getN(_Point &p){
        _Vector v(p, this->pc);
        v.vectorUnit(1/this->r);
        return v;
    }

    bool isIn(_Point &min, _Point &max){
        _Vector tmp;
        double lenr = this->r * this->r;
        _Point tmpo;
        tmp.setVector(min, this->pc);
        if(tmp.timesVector(tmp) < lenr){
            tmp.setVector(max, this->pc);
            if (tmp.timesVector(tmp) >= lenr){
                return true;
            }
        }else{
            tmp.setVector(max, this->pc);
            if (tmp.timesVector(tmp) < lenr){
                return true;
            }
        }
        tmpo.setPoint(min.x, max.y, max.z);
        tmp.setVector(tmpo, this->pc);
        if(tmp.timesVector(tmp) < lenr){
            tmpo.setPoint(max.x, min.y, min.z);
            tmp.setVector(tmpo, this->pc);
            if (tmp.timesVector(tmp) >= lenr){
                return true;
            }
        }else{
            tmpo.setPoint(max.x, min.y, min.z);
            tmp.setVector(tmpo, this->pc);
            if (tmp.timesVector(tmp) < lenr){
                return true;
            }
        }
        tmpo.setPoint(max.x, min.y, max.z);
        tmp.setVector(tmpo, this->pc);
        if(tmp.timesVector(tmp) < lenr){
            tmpo.setPoint(min.x, max.y, min.z);
            tmp.setVector(tmpo, this->pc);
            if (tmp.timesVector(tmp) >= lenr){
                return true;
            }
        }else{
            tmpo.setPoint(min.x, max.y, min.z);
            tmp.setVector(tmpo, this->pc);
            if (tmp.timesVector(tmp) < lenr){
                return true;
            }
        }
        tmpo.setPoint(max.x, max.y, min.z);
        tmp.setVector(tmpo, this->pc);
        if(tmp.timesVector(tmp) < lenr){
            tmpo.setPoint(min.x, min.y, max.z);
            tmp.setVector(tmpo, this->pc);
            if (tmp.timesVector(tmp) >= lenr){
                return true;
            }
        }else{
            tmpo.setPoint(min.x, min.y, max.z);
            tmp.setVector(tmpo, this->pc);
            if (tmp.timesVector(tmp) < lenr){
                return true;
            }
        }
        if (tmp.timesVector(tmp) >= lenr){
            if (min.x <= this->pc.x && min.y <= this->pc.y && min.z <= this->pc.z
                && max.x >= this->pc.x && max.y >= this->pc.y && max.z >= this->pc.z)
                return true;
        }
        return false;
    }
};

class _Cuboid : public _Geometry{
public:
    _Point p;
    _Vector a;
    _Vector b;
    _Vector c;
    _Plane ab;
    _Plane ac;
    _Plane bc;

    _Cuboid(){
        this->geometrytype = CUBOID;
    }
    _Cuboid(_Point &p, _Vector &a, _Vector &b, _Vector &c){
        this->a = a;
        this->b = b;
        this->c = c;
        this->p = p;
        _Vector tmp = this->c;
        tmp = tmp.oppositeVector();
        tmp.vectorUnit();
        ab.setPlane(p, tmp);
        tmp = this->b;
        tmp = tmp.oppositeVector();
        tmp.vectorUnit();
        ac.setPlane(p, tmp);
        tmp = this->a;
        tmp = tmp.oppositeVector();
        tmp.vectorUnit();
        bc.setPlane(p, tmp);
        this->geometrytype = CUBOID;
    }

    bool intersect(_Ray &r, double* t){
        _Point tmp=ab.p0.addVector(this->c);
        _Plane opposite(tmp, ab.n);
        double t1, t2, tmin, tmax;
        if (ab.intersect(r, &t1))
        {
            if (ab.intersect(r, &t2))
            {
                if (t1 > t2)
                {
                    double tmp = t1;
                    t1 = t2;
                    t2 = tmp;
                }
                tmin = t1;
                tmax = t2;
            }else{
                return false;
            }
        }else{
            return false;
        }
        tmp = ac.p0.addVector(this->b);
        opposite.setPlane(tmp, ac.n);
        if (ac.intersect(r, &t1))
        {
            if (opposite.intersect(r, &t2))
            {
                if (t1 > t2)
                {
                    double tmp = t1;
                    t1 = t2;
                    t2 = tmp;
                }
                if (t1 > tmin){
                    tmin = t1;
                }
                if (t2 < tmax){
                    tmax = t2;
                }
            }else{
                return false;
            }
        }else{
            return false;
        }
        tmp = bc.p0.addVector(this->a);
        opposite.setPlane(tmp, bc.n);
        if (bc.intersect(r, &t1))
        {
            if (opposite.intersect(r, &t2))
            {
                if (t1 > t2)
                {
                    double tmp = t1;
                    t1 = t2;
                    t2 = tmp;
                }
                if (t1 > tmin){
                    tmin = t1;
                }
                if (t2 < tmax){
                    tmax = t2;
                }
            }else{
                return false;
            }
        }else{
            return false;
        }
        if (tmin < tmax){
            *t = tmin;
            return true;
        }
        return false;
    }

    _Vector getN(_Point &p){
        _Point tmp;
        if (ab.isOn(p)){
            return ab.n;
        }
        if (ac.isOn(p)){
            return ac.n;
        }
        if (bc.isOn(p)){
            return bc.n;
        }
        tmp = ab.p0.addVector(this->c);
        _Plane opposite(tmp, ab.n);
        if (opposite.isOn(p))
        {
            return opposite.n.oppositeVector();
        }
        tmp = ac.p0.addVector(this->c);
        opposite.setPlane(tmp, ac.n);
        if (opposite.isOn(p))
        {
            return opposite.n.oppositeVector();
        }
        tmp = bc.p0.addVector(this->c);
        opposite.setPlane(tmp, bc.n);
        if (opposite.isOn(p))
        {
            return opposite.n.oppositeVector();
        }
    }
};

#endif // RAYTRACING_H_INCLUDED