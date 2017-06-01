#include "NodePoint.h"
#include <windows.h>

int main(int argc, char* argv[])
{ 
	NodePoint newexample;
	deque<deque<unsigned int> > path;
	string filename;//�ļ��� 
	unsigned int minpointnum = 0;//��С������ 
	unsigned int mindistance = 0;//��̾��� 
	switch(argc)
	{
	case 1:
		{
			cout<<"�����ļ�����"<<endl;
			cin>>filename;
			cout<<endl<<"����·������С����Ŀ"<<endl;
			cin>>minpointnum;
			cout<<endl<<"����·������С����"<<endl;
			cin>>mindistance;
			break;
		}
	case 2:
		{
			filename = argv[1];
			cout<<endl<<"����·������С����Ŀ"<<endl;
			cin>>minpointnum;
			cout<<endl<<"����·������С����"<<endl;
			cin>>mindistance;
			break;
		}
	case 3:
		{
			filename = argv[1];
			minpointnum = atoi(argv[2]);
			cout<<endl<<"����·������С����"<<endl;
			cin>>mindistance;
			break;
		}
	case 4:
		{
			filename = argv[1];
			minpointnum = atoi(argv[2]);
			mindistance = atoi(argv[3]);
			break;
		}
	}
	newexample.InitPoint(filename.c_str());//����ͼ 
	double begin = GetTickCount();
	path = newexample.SearchMinPath(minpointnum,mindistance);//�������·�� 
	double end = GetTickCount();
	cout<<"����ʱ��"<<(end-begin)<<"ms"<<endl;
	system("pause");
}
