#ifndef NodePoint_h
#include <iostream>
#include <string>
#include <deque>
#include <fstream>
#include <sstream>
#include <time.h>
#include "Support.h"
using namespace std;

//����ͼ�ṹ����������·�� 
class NodePoint
{
private:
	string NodeName;//�ڵ��ǩ 
	uint Index;
	deque<uint> NextPoint;//ָ��ļ����� 
	deque<uint> Distance;//��ָ���֮��ľ��� 
	Attribute PointAttribute;//������ 
	deque<Attribute> LineAttribute;//������ 
	deque<NodePoint* > PointPosition;//ÿ�����λ�� 
	Constraint constraint;//��������
public:
	NodePoint(string name = "",uint index = 0,Attribute attrib = Normal):
	  NodeName(name),Index(index),PointAttribute(attrib)
	{};
	~NodePoint();
	void InitPoint(string filename);
	deque<deque<uint> > SearchMinPath(uint minpointnum,uint mindistance);//��㵽�յ���̾���
private:
	deque<deque<uint> > SearchMinPointNumPath(uint BeginPointindex,deque<uint> endpointindex,deque<uint> &D,uint num = 1,const bool Constra = false);//����֮����С�ڵ���Ŀ·��
	deque<deque<uint> > SearchMinDistancePath(uint BeginPointindex,deque<uint> endpointindex,deque<uint> &D,uint num = 1,const bool Constra = false);//����֮�������С·��
	void clear();
};

#else
#define NodePoint_h
#endif
