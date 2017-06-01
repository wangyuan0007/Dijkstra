#ifndef NodePoint_h
#include <iostream>
#include <string>
#include <deque>
#include <fstream>
#include <sstream>
#include <time.h>
#include "Support.h"
using namespace std;

//保存图结构及计算最优路径 
class NodePoint
{
private:
	string NodeName;//节点标签 
	uint Index;
	deque<uint> NextPoint;//指向的几个点 
	deque<uint> Distance;//与指向点之间的距离 
	Attribute PointAttribute;//点属性 
	deque<Attribute> LineAttribute;//线属性 
	deque<NodePoint* > PointPosition;//每个点的位置 
	Constraint constraint;//限制条件
public:
	NodePoint(string name = "",uint index = 0,Attribute attrib = Normal):
	  NodeName(name),Index(index),PointAttribute(attrib)
	{};
	~NodePoint();
	void InitPoint(string filename);
	deque<deque<uint> > SearchMinPath(uint minpointnum,uint mindistance);//起点到终点最短距离
private:
	deque<deque<uint> > SearchMinPointNumPath(uint BeginPointindex,deque<uint> endpointindex,deque<uint> &D,uint num = 1,const bool Constra = false);//两点之间最小节点数目路径
	deque<deque<uint> > SearchMinDistancePath(uint BeginPointindex,deque<uint> endpointindex,deque<uint> &D,uint num = 1,const bool Constra = false);//两点之间距离最小路径
	void clear();
};

#else
#define NodePoint_h
#endif
