#ifndef Support_h
#include <deque>
using std::deque;
typedef unsigned int uint;
//����ߵ�״̬���ֱ�����ͨ���ߣ����뾭���ĵ��ߣ���ֹ�����ĵ��ߣ���㣬�յ� 
enum Attribute{Normal = 0,MustPass = 1,Forbid = 2,Begin = 4,End = 3};
//���ӽṹ
struct Line
{
	uint f;
	uint c;
};

//�洢ͼ��������
struct Constraint
{
	deque<uint> MustPassPoint;//���뾭���ĵ�
	deque<uint> ForbidPoint;//��ֹ�����ĵ�
	deque<Line> MustpassLine;//���뾭������
	deque<Line> ForbidLine;//��ֹͨ�е�·��
	deque<uint> PathPassPoint;//����㵽�յ������뾭���ĵ�
	deque<uint> MustpassLineDistance;//���뾭������
	uint MustPointnum;//���뾭���ĵ����ߵ�����Ŀ
	uint Forbidnum;//��ֹ�����ĵ����ߵ�����Ŀ
	uint StartPoint;//���
	uint EndPoint;//�յ�
	uint MinLineLength;//��С�����߳���
};

#else
#define Support_h
#endif

