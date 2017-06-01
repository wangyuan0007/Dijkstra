#ifndef Support_h
#include <deque>
using std::deque;
typedef unsigned int uint;
//点和线的状态，分别是普通点线，必须经过的点线，禁止经过的点线，起点，终点 
enum Attribute{Normal = 0,MustPass = 1,Forbid = 2,Begin = 4,End = 3};
//连接结构
struct Line
{
	uint f;
	uint c;
};

//存储图的条件点
struct Constraint
{
	deque<uint> MustPassPoint;//必须经过的点
	deque<uint> ForbidPoint;//禁止经过的点
	deque<Line> MustpassLine;//必须经过的线
	deque<Line> ForbidLine;//禁止通行的路径
	deque<uint> PathPassPoint;//从起点到终点所必须经过的点
	deque<uint> MustpassLineDistance;//必须经过的线
	uint MustPointnum;//必须经过的点与线的总数目
	uint Forbidnum;//禁止经过的点与线的总数目
	uint StartPoint;//起点
	uint EndPoint;//终点
	uint MinLineLength;//最小连接线长度
};

#else
#define Support_h
#endif

