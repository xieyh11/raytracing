#ifndef TRACING_H
#define TRACING_H

#include "raytracing.h"
#include <vector>
using namespace std;

class _BspElement
{
public:
  vector<int> object;
  _Point minp;
  _Point maxp;
  _BspElement* child0;
  _BspElement* child1;

  _BspElement();

  _BspElement(_BspElement *child0,_BspElement *child1){
    this->child1 = child1;
    this->child0 = child0;
  }
  _BspElement(_Point &minp, _Point &maxp);
};

_BspElement* nil = new _BspElement();

_BspElement::_BspElement(){
  this->child0 = nil;
  this->child1 = nil;
}

_BspElement::_BspElement(_Point &minp, _Point &maxp){
  this->minp = minp;
  this->maxp = maxp;
  this->child0 = nil;
  this->child1 = nil;
}

class _Bsp{
public:
  _BspElement* node;
  const int maxdepth;
  const int minobject;

  _Bsp(int depth, int obj):maxdepth(depth), minobject(obj){
    node = nil;
  }

  void setUp(vector<_Geometry>& allobject, _Point& min, _Point& max){
    node = new _BspElement(min, max);
    for (int i = 0; i < allobject.size(); ++i)
    {
      node->object.push_back(i);
    }
    subdivide(allobject, node, 1, 'x');
  }

  void subdivide(vector<_Geometry>& allobject, _BspElement* nowat, int currentdepth, char currentaxis){
    if (nowat->object.size() > minobject && currentdepth <= maxdepth){
      node->child0 = new _BspElement(node->minp, node->maxp);
      node->child1 = new _BspElement(node->minp, node->maxp);
      char nextaxis;
      switch(currentaxis){
      case 'x':
        nowat->child1->minp.x = (nowat->child1->minp.x+nowat->child1->minp.x)/2;
        nowat->child0->maxp.x = nowat->child1->minp.x;
        nextaxis = 'y';
        break;
      case 'y':
        nowat->child1->minp.y = (nowat->child1->minp.y+nowat->child1->minp.y)/2;
        nowat->child0->maxp.y = nowat->child1->minp.y;
        nextaxis = 'z';
        break;
      case 'z':
        nowat->child1->minp.z = (nowat->child1->minp.z+nowat->child1->minp.z)/2;
        nowat->child0->maxp.z = nowat->child1->minp.z;
        nextaxis = 'x';
        break;
      default:
        return;
      }
      for(int i = 0; i < nowat->object.size(); i++){
        if (allobject[nowat->object[i]].isIn(nowat->child0->minp, nowat->child0->maxp)){
          nowat->child0->object.push_back(i);
        }
        if (allobject[nowat->object[i]].isIn(nowat->child1->minp, nowat->child1->maxp)){
          nowat->child1->object.push_back(i);
        }
      }
      subdivide(allobject, nowat->child0, currentdepth + 1, nextaxis);
      subdivide(allobject, nowat->child1, currentdepth + 1, nextaxis);
    }
  }
};

#endif