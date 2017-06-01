#include "NodePoint.h"
#include <windows.h>

int main(int argc, char* argv[])
{ 
	NodePoint newexample;
	deque<deque<unsigned int> > path;
	string filename;//文件名 
	unsigned int minpointnum = 0;//最小点数量 
	unsigned int mindistance = 0;//最短距离 
	switch(argc)
	{
	case 1:
		{
			cout<<"输入文件名称"<<endl;
			cin>>filename;
			cout<<endl<<"输入路径的最小点数目"<<endl;
			cin>>minpointnum;
			cout<<endl<<"输入路径的最小距离"<<endl;
			cin>>mindistance;
			break;
		}
	case 2:
		{
			filename = argv[1];
			cout<<endl<<"输入路径的最小点数目"<<endl;
			cin>>minpointnum;
			cout<<endl<<"输入路径的最小距离"<<endl;
			cin>>mindistance;
			break;
		}
	case 3:
		{
			filename = argv[1];
			minpointnum = atoi(argv[2]);
			cout<<endl<<"输入路径的最小距离"<<endl;
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
	newexample.InitPoint(filename.c_str());//读入图 
	double begin = GetTickCount();
	path = newexample.SearchMinPath(minpointnum,mindistance);//查找最短路径 
	double end = GetTickCount();
	cout<<"运行时间"<<(end-begin)<<"ms"<<endl;
	system("pause");
}
