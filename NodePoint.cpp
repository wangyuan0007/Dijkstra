#include "NodePoint.h"

//deque<NodePoint* > NodePoint::PointPosition;
//Constraint NodePoint::constraint;

NodePoint::~NodePoint()
{
    clear();
}

//清理资源
void NodePoint::clear()
{
    for(uint i = 1; i < PointPosition.size(); i++)
    {
        if(PointPosition[i]->Index != constraint.StartPoint)
            delete PointPosition[i];
    }
    PointPosition.clear();
    constraint.ForbidLine.clear();
    constraint.ForbidPoint.clear();
    constraint.MustpassLine.clear();
    constraint.MustPassPoint.clear();
    constraint.PathPassPoint.clear();
    constraint.Forbidnum = 0;
    constraint.MustPointnum = 0;
    constraint.StartPoint = 0;
    constraint.EndPoint = 0;
}

void NodePoint::InitPoint(string filename)
{
    ifstream fin;
    fin.open(filename.c_str());
    string line;
    bool PointCreat = true;
    NodePoint* newpoint;
    uint MinLineLength = uint(-1);
    while(1)
    {
        getline(fin,line);
        if(line.length() > 0&&line.length() < 6)
            break;
        if(line.empty()&&fin.eof())
            break;
        if(line.substr(0,5) == "Point")//当前创建节点操作
        {
            PointCreat = true;
            continue;
        }
        else if(line.length() >10 && line.substr(0,10) == "Connection")//当前读取连接操作
        {
            PointCreat = false;
            continue;
        }
        if(PointCreat)//创建点
        {
            bool startread = false;//是否可以读取内容
            bool isname = true;//是否是节点标签
            string c;
            string newpointname;//新节点标签
            string newpointattrib;//新节点属性
            for(uint i = 0; i < line.length(); i++)
            {
                if(line[i] == '[')//开始读内容
                {
                    startread = true;
                    c = "";
                    continue;
                }
                if(line[i] == ']')//读取内容结束
                {
                    if(isname)
                    {
                        newpointname = c;
                        isname = false;
                    }
                    else
                    {
                        newpointattrib = c;
                        break;
                    }
                    startread = false;
                    continue;
                }
                if(startread)
                {
                    c = c + line[i];
                }
            }
            if(newpointattrib == "4")//判断是否是起点 ，起点为当前类实例
            {
                newpoint = this;
                newpoint->NodeName = newpointname;
                newpoint->PointAttribute = Begin;
                newpoint->Index = PointPosition.size();
                constraint.StartPoint = newpoint->Index;
            }
            else//非起点分配内存
                newpoint = new NodePoint(newpointname,PointPosition.size(),Attribute(atoi(newpointattrib.c_str())));
            PointPosition.push_back(newpoint);//保存节点位置
            switch(newpoint->PointAttribute)//根据节点属性不同进行操作
            {
            case 0:
                break;
            case 1:
                constraint.MustPassPoint.push_back(PointPosition.size() - 1);
                break;//保存必须经过节点位置
            case 2:
                constraint.ForbidPoint.push_back(PointPosition.size() - 1);
                break;//保存禁止经过节点位置
            case 3:
                constraint.EndPoint = newpoint->Index;
                break;//保存终点位置
            }
        }
        else//连接点
        {
            bool startread = false;//是否可以读取内容
            uint readcount = 0;//读取的第几个值
            string c;
            string newpointname1;//连接的第一个点名字
            string newpointname2;//连接的第二个点名字
            string newlinetattrib;//连接属性
            string linedistance;//连接距离
            uint firstPoint;//第一个点位置
            uint secondPoint;//第二个点位置
            //读取标签
            for(uint i = 0; i < line.length(); i++)
            {
                if(line[i] == '[')
                {
                    startread = true;
                    c = "";
                    continue;
                }
                if(line[i] == ']')
                {
                    switch(readcount)
                    {
                    case 0:
                        newpointname1 = c;
                        break;
                    case 1:
                        newpointname2 = c;
                        break;
                    case 2:
                        newlinetattrib = c;
                        break;
                    case 3:
                        linedistance = c;
                        break;
                    default:
                        break;
                    }
                    readcount++;
                    startread = false;
                }
                if(startread)
                {
                    c = c + line[i];
                }
            }
            MinLineLength = MinLineLength < atoi(linedistance.c_str())?MinLineLength : atoi(linedistance.c_str());//最小权值

            Line temp;
            for(uint i = 0; i < PointPosition.size(); i++) //根据名字找到位置
            {
                if(PointPosition[i]->NodeName == newpointname1)
                {
                    if(newlinetattrib > "0")
                        temp.f = i;
                    firstPoint = i;
                    newpointname1 = "";
                    if(newlinetattrib == "1")//必须经过线放在前面以提高性能
                    {
                        PointPosition[i]->Distance.push_front(atoi(linedistance.c_str()));//保存权值
                        PointPosition[i]->LineAttribute.push_front(Attribute(atoi(newlinetattrib.c_str())));//保存线属性
                    }
                    else
                    {
                        PointPosition[i]->Distance.push_back(atoi(linedistance.c_str()));
                        PointPosition[i]->LineAttribute.push_back(Attribute(atoi(newlinetattrib.c_str())));
                    }
                }
                else if(PointPosition[i]->NodeName == newpointname2)
                {
                    if(newlinetattrib > "0")
                        temp.c = i;
                    secondPoint = i;
                    newpointname2 = "";
                    if(newlinetattrib == "1")
                    {
                        PointPosition[i]->Distance.push_front(atoi(linedistance.c_str()));
                        PointPosition[i]->LineAttribute.push_front(Attribute(atoi(newlinetattrib.c_str())));
                    }
                    else
                    {
                        PointPosition[i]->Distance.push_back(atoi(linedistance.c_str()));
                        PointPosition[i]->LineAttribute.push_back(Attribute(atoi(newlinetattrib.c_str())));
                    }
                }
                if(newpointname2.empty()&&newpointname1.empty())
                    break;
            }
            switch(atoi(newlinetattrib.c_str()))
            {
            case 0:
                break;
            case 1:
            {
                constraint.MustpassLine.push_back(temp);
                constraint.MustpassLineDistance.push_back(atoi(linedistance.c_str()));
                break;
            }//保存必须经过线与距离
            case 2:
                constraint.ForbidLine.push_back(temp);
                break;//保存禁止经过线
            }
            if(newlinetattrib == "1")//创建连接关系
            {
                PointPosition[firstPoint]->NextPoint.push_front(secondPoint);
                PointPosition[secondPoint]->NextPoint.push_front(firstPoint);
            }
            else
            {
                PointPosition[firstPoint]->NextPoint.push_back(secondPoint);
                PointPosition[secondPoint]->NextPoint.push_back(firstPoint);
            }
        }
        if(fin.eof())
            break;
        line = "";
    }
    fin.close();
    constraint.MinLineLength = MinLineLength;
    constraint.MustPointnum = constraint.MustPassPoint.size();
    constraint.PathPassPoint = constraint.MustPassPoint;//保存从起点到终点包括必须经过线节点的所有位置
    constraint.PathPassPoint.push_front(constraint.StartPoint);
    for(uint i = 0; i < constraint.MustpassLine.size(); i++)
    {
        bool addflag1 = true;
        bool addflag2 = true;
        for(uint j = 0; j < constraint.MustPassPoint.size(); j++)
        {
            if(constraint.MustpassLine[i].c == constraint.MustPassPoint[j])
                addflag1 = false;
            if(constraint.MustpassLine[i].f == constraint.MustPassPoint[j])
                addflag2 = false;
        }
        if(addflag1)
        {
            constraint.MustPointnum++;
            constraint.PathPassPoint.push_back(constraint.MustpassLine[i].c);
        }
        if(addflag2)
        {
            constraint.MustPointnum++;
            constraint.PathPassPoint.push_back(constraint.MustpassLine[i].f);
        }
    }
    constraint.PathPassPoint.push_back(constraint.EndPoint);
    constraint.Forbidnum = constraint.ForbidPoint.size();//保存禁止经过点与线节点
    for(uint i = 0; i < constraint.ForbidLine.size(); i++)
    {
        bool addflag1 = true;
        bool addflag2 = true;
        for(uint j = 0; j < constraint.ForbidPoint.size(); j++)
        {
            if(constraint.ForbidLine[i].c == constraint.ForbidPoint[j])
                addflag1 = false;
            if(constraint.ForbidLine[i].f == constraint.ForbidPoint[j])
                addflag2 = false;
        }
        for(uint j = 0; j < constraint.MustPassPoint.size(); j++)
        {
            if(constraint.ForbidLine[i].c == constraint.MustPassPoint[j])
                addflag1 = false;
            if(constraint.ForbidLine[i].f == constraint.MustPassPoint[j])
                addflag2 = false;
        }
        for(uint j = 0; j < constraint.MustpassLine.size(); j++)
        {
            if(constraint.ForbidLine[i].c == constraint.MustpassLine[j].c||constraint.ForbidLine[i].c == constraint.MustpassLine[j].f)
                addflag1 = false;
            if(constraint.ForbidLine[i].f == constraint.MustpassLine[j].c||constraint.ForbidLine[i].f == constraint.MustpassLine[j].f)
                addflag2 = false;
        }
        if(addflag1)
            constraint.Forbidnum++;
        if(addflag2)
            constraint.Forbidnum++;
    }
}


deque<deque<uint> > NodePoint::SearchMinPointNumPath(uint BeginPointindex,deque<uint> endpointindex,deque<uint> &D,uint num,const bool Constra)
{
    deque<deque<uint> > MinPath;//最短路径
    deque<deque<uint> > PointPath;//存储路径
    deque<uint> Pathdistance;//存储路径长度
    deque<uint> HasSearchPoint;//存储已经搜索过的点
    deque<uint> HasSearchEnd;//已经到达的终点
    deque<uint> TempHasSearchPoint;//一次迭代产生的已经搜索的点
    deque<uint> FinishEndPoint;//已经到达的终点
    deque<uint> TempFinishEndPoint;//一次迭代已经到达的终点
    Pathdistance.push_back(0);//初始路径长度为0
    PointPath.push_back(deque<uint>());
    PointPath[0].push_back(BeginPointindex);//初始路径为起点
    HasSearchPoint.push_back(BeginPointindex);//保存起点为已搜索点
    //D.clear();
    uint PathNum;//路径总数
    uint PointConectionNum;//连接到下一个点的数量
    uint HasSearchPointNum;//已经搜索过的点的数量
    uint EndPointIndexNum = endpointindex.size();//终点数量
    uint FinishEndPointNum;//已经到达的终点数量
    uint TempHasSearchPointNum;//一次循环产生的已搜索点数量
    uint PointToNextDistance;//到下一点的距离
    uint PointToNextIndex;//到下一点的索引
    num = num == 0?1:num;//搜索终点数量最小为1
    num = num > EndPointIndexNum?EndPointIndexNum:num;//最大为终点数量
    NodePoint* tempPoint;
    bool HasSearch;//已经搜索标志
    bool Finish = false;//结束标志
    bool isend;//终点标志
    for(uint i = 0; i < EndPointIndexNum; i++) //查找与起点相同的终点
    {
        if(BeginPointindex == endpointindex[i])
        {
            FinishEndPoint.push_back(BeginPointindex);
            num = num >= EndPointIndexNum?EndPointIndexNum:num+1;
        }
    }
    while(!Finish)
    {
        PathNum = PointPath.size();//本次循环初始路径总数
        HasSearchPointNum = HasSearchPoint.size();//本次循环已经搜索点初始总数
        TempHasSearchPoint.clear();
        TempFinishEndPoint.clear();
        for(uint i = 0; i < PathNum; i++) //搜索每一条路径
        {
            tempPoint = PointPosition[PointPath[i].back()];//指向路径最后一个点
            PointConectionNum = tempPoint->NextPoint.size();//最后一个点连接节点数量
            for(uint j = 0; j < PointConectionNum; j++)//遍历每一个连接点
            {
                PointToNextDistance = tempPoint->Distance[j];
                PointToNextIndex = tempPoint->NextPoint[j];
                HasSearch = false;
                isend = false;//终点标志
                if(Constra&&(tempPoint->LineAttribute[j] == 2||PointPosition[PointToNextIndex]->PointAttribute == 2))//跳过禁止经过点和线
                    continue;
                for(uint k = 0; k < HasSearchPointNum; k++) //搜索连接点是否在已搜索点中
                {
                    if(HasSearchPoint[k] == PointToNextIndex)
                    {
                        HasSearch = true;
                        break;
                    }
                }
                if(HasSearch)//已经搜索过点跳过
                    continue;
                for(uint k = 0; k < EndPointIndexNum; k ++) //判断是否是终点
                {
                    if(endpointindex[k] == PointToNextIndex)
                    {
                        isend = true;
                        //Finish = true;
                        break;
                    }
                }
                if(isend)//判断终点是否已经搜索过
                {
                    FinishEndPointNum = FinishEndPoint.size();
                    for(uint k = 0; k < FinishEndPointNum; k++)
                    {
                        if(FinishEndPoint[k] == PointToNextIndex)
                        {
                            isend = false;
                            break;
                        }
                    }

                }
                if(isend)//保存终点
                {
                    FinishEndPointNum = TempFinishEndPoint.size();
                    HasSearch = false;
                    for(uint k = 0; k < FinishEndPointNum; k++)
                    {
                        if(TempFinishEndPoint[k] == PointToNextIndex)
                        {
                            HasSearch = true;
                            break;
                        }
                    }
                    if(!HasSearch)
                    {
                        TempFinishEndPoint.push_back(PointToNextIndex);
                    }
                    else
                        HasSearch = false;
                }
                if(isend)
                {
                    MinPath.push_back(PointPath[i]);//保存最短路径
                    MinPath.back().push_back(PointToNextIndex);
                    D.push_back(Pathdistance[i] + PointToNextDistance);
                }

                TempHasSearchPointNum = TempHasSearchPoint.size();//保存已经搜索点
                if(TempHasSearchPointNum == 0)
                    TempHasSearchPoint.push_back(PointToNextIndex);
                else
                {
                    for(uint k = 0; k < TempHasSearchPointNum; k++)
                    {
                        if(TempHasSearchPoint[k] == PointToNextIndex)
                        {
                            HasSearch = true;
                            break;
                        }
                    }
                    if(!HasSearch)
                        TempHasSearchPoint.push_back(PointToNextIndex);
                    else
                        HasSearch = false;
                }
                PointPath.push_back(PointPath[i]);//保存路径
                PointPath.back().push_back(PointToNextIndex);
                Pathdistance.push_back(Pathdistance[i] + PointToNextDistance);

            }

            if(i > 2)//删除当前路径
            {
                PointPath[i] = PointPath[0];
                PointPath.pop_front();
                Pathdistance[i] = Pathdistance[0];
                Pathdistance.pop_front();
            }
            else
            {
                PointPath.erase(PointPath.begin() + i);
                Pathdistance.erase(Pathdistance.begin() + i);
            }
            PathNum--;
            i--;
        }
        HasSearchPoint.insert(HasSearchPoint.end(),TempHasSearchPoint.begin(),TempHasSearchPoint.end());//保存已经搜索点和终点
        FinishEndPoint.insert(FinishEndPoint.end(),TempFinishEndPoint.begin(),TempFinishEndPoint.end());
        if(PointPath.empty())
            break;
        if(FinishEndPoint.size() >= num)
            break;
    }
    return MinPath;
}

deque<deque<uint> > NodePoint::SearchMinDistancePath(uint BeginPointindex,deque<uint> endpointindex,deque<uint> &D,uint num,const bool Constra)
{
    deque<deque<uint> > MinPath;//最短路径
    deque<deque<uint> > PointPath;//存储路径
    deque<uint> PathToNext;//距离下一个点的距离
    deque<uint> Pathdistance;//路径距离
    deque<uint> HasSearchPoint;//已经搜索过的点
    deque<uint> TempHasSearchPoint;
    deque<uint> FinishEndPoint;//已经到达的终点
    deque<uint> TempFinishEndPoint;//一次迭代已经到达的终点
    Pathdistance.push_back(0);//初始距离为0
    PointPath.push_back(deque<uint>());
    PointPath[0].push_back(BeginPointindex);//初始路径为起点
    HasSearchPoint.push_back(BeginPointindex);//起点为已搜索点
    PathToNext.push_back(uint(1e7));//距离下一点设置极大值
    //D.clear();
    uint PathNum;//路径总数
    uint PointConectionNum;//连接总数
    uint HasSearchPointNum;//已经搜索点总数
    uint EndPointIndexNum = endpointindex.size();//终点总数
    uint PathPointNum;// 单条路径节点数量
    uint MinLineLength = 1e7;//最小步长设置极大值
    uint tempMinLineLength;//最短线权值
    uint PointToNextDistance;//距离下一点距离
    uint PointToNextIndex;//下一点位置
    uint FinishEndPointNum;//已经到达的终点数量
    uint LastPointIndex;//最后一节点位置
    NodePoint* tempPoint;
    bool HasSearch;//已经搜索标志
    bool Finish = false;//完成标志
    bool isend;//终点标志
    for(uint i = 0; i < EndPointIndexNum; i++) //检查终点是否与起点相同
        if(BeginPointindex == endpointindex[i])
        {
            FinishEndPoint.push_back(BeginPointindex);
            num = num >= EndPointIndexNum?EndPointIndexNum:num+1;
        }
    while(!Finish)
    {
        PathNum = PointPath.size();//本次循环初始路径数量
        HasSearchPointNum = HasSearchPoint.size();//本次循环已经搜索的点数量
        TempHasSearchPoint.clear();
        TempFinishEndPoint.clear();
        tempMinLineLength = uint(-1);
        for(uint i = 0; i < PathNum; i++) //搜索每一条路径最后一个点的连接
        {
            PathPointNum = PointPath[i].size();
            tempPoint = PointPosition[PointPath[i].back()];
            PointConectionNum = tempPoint->NextPoint.size();
            HasSearch = false;
            LastPointIndex = PointPath[i][PathPointNum - 1];
            if(HasSearchPointNum > 1)//已经搜索过点，搜索当前点是否已经搜索过
            {
                for(uint j = 0; j < HasSearchPointNum; j++)
                {
                    if(LastPointIndex == HasSearchPoint[j])//已经搜索过的点删除
                    {
                        HasSearch = true;
                        if(i > 2)
                        {
                            PointPath[i] = PointPath[0];
                            PointPath.pop_front();
                            Pathdistance[i] = Pathdistance[0];
                            Pathdistance.pop_front();
                            PathToNext[i] = PathToNext[0];
                            PathToNext.pop_front();
                        }
                        else
                        {
                            PointPath.erase(PointPath.begin() + i);
                            Pathdistance.erase(Pathdistance.begin() + i);
                            PathToNext.erase(PathToNext.begin() + i);
                        }
                        PathNum--;
                        i--;
                        break;
                    }
                }
                if(HasSearch)
                    continue;
            }
            isend = false;
            PathToNext[i] -= MinLineLength;//前进一步
            tempMinLineLength = PathToNext[i] <= 0||tempMinLineLength == 1?tempMinLineLength:(tempMinLineLength > PathToNext[i]?PathToNext[i]:tempMinLineLength);
            if(PathToNext[i] <= 0)//到达下一节点
            {
                uint TempHasSearchPointNum = TempHasSearchPoint.size();
                if(TempHasSearchPointNum == 0)//保存节点到已搜索节点
                    TempHasSearchPoint.push_back(LastPointIndex);
                else
                {
                    for(uint k = 0; k < TempHasSearchPointNum; k++)
                    {
                        if(TempHasSearchPoint[k] == LastPointIndex)
                        {
                            HasSearch = true;
                            break;
                        }
                    }
                    if(!HasSearch)
                        TempHasSearchPoint.push_back(LastPointIndex);
                }

                for(uint k = 0; k < EndPointIndexNum; k ++) //检测是否是终点
                {
                    if(endpointindex[k] == LastPointIndex)
                    {
                        isend = true;
                        //Finish = true;
                        break;
                    }
                }
                /*if(isend)//检查终点是否已搜索过
                {
                    FinishEndPointNum = FinishEndPoint.size();
                    for(uint k = 0; k < FinishEndPointNum; k++)
                    {
                        if(FinishEndPoint[k] == LastPointIndex)
                        {
                            isend = false;
                            break;
                        }
                    }

                }*/
                if(isend)//保存终点到已搜索终点
                {
                    FinishEndPointNum = TempFinishEndPoint.size();
                    HasSearch = false;
                    for(uint k = 0; k < FinishEndPointNum; k++)
                    {
                        if(TempFinishEndPoint[k] == LastPointIndex)
                        {
                            HasSearch = true;
                            break;
                        }
                    }
                    if(!HasSearch)
                        TempFinishEndPoint.push_back(LastPointIndex);
                    else
                        HasSearch = false;
                }
                if(isend)//保存最小路径与长度
                {
                    MinPath.push_back(PointPath[i]);
                    D.push_back(Pathdistance[i]);
                    //continue;
                }

                for(uint j = 0; j < PointConectionNum; j++)//遍历每一个连接点
                {
                    PointToNextDistance = tempPoint->Distance[j];
                    PointToNextIndex = tempPoint->NextPoint[j];
                    HasSearch = false;
                    if(Constra&&(tempPoint->LineAttribute[j] == 2||PointPosition[PointToNextIndex]->PointAttribute == 2))
                        continue;
                    for(uint k = 0; k < HasSearchPointNum; k++) //搜索连接点是否在已搜索点中
                    {
                        if(HasSearchPoint[k] == PointToNextIndex)
                        {
                            HasSearch = true;
                            break;
                        }
                    }
                    if(HasSearch)//已搜索点跳过
                        continue;
                    tempMinLineLength = tempMinLineLength < PointToNextDistance?tempMinLineLength:PointToNextDistance;
                    PointPath.push_back(PointPath[i]);
                    PointPath.back().push_back(PointToNextIndex);
                    Pathdistance.push_back(Pathdistance[i] + PointToNextDistance);
                    PathToNext.push_back(PointToNextDistance);
                }
                if(i > 2)//删除本路径
                {
                    PointPath[i] = PointPath[0];
                    PointPath.pop_front();
                    Pathdistance[i] = Pathdistance[0];
                    Pathdistance.pop_front();
                    PathToNext[i] = PathToNext[0];
                    PathToNext.pop_front();
                }
                else
                {
                    PointPath.erase(PointPath.begin() + i);
                    Pathdistance.erase(Pathdistance.begin() + i);
                    PathToNext.erase(PathToNext.begin() + i);
                }
                PathNum--;
                i--;
            }
            /**/
        }
        MinLineLength = (tempMinLineLength != -1)?tempMinLineLength:MinLineLength;//更新最小步长
        HasSearchPoint.insert(HasSearchPoint.end(),TempHasSearchPoint.begin(),TempHasSearchPoint.end());//更新已搜索点与终点
        FinishEndPoint.insert(FinishEndPoint.end(),TempFinishEndPoint.begin(),TempFinishEndPoint.end());
        if(PointPath.size() == 0)
            break;
        if(FinishEndPoint.size() >= num)
            break;
        //uint TempHasSearchPointNum = TempHasSearchPoint.size();
        /*for(int k = 0;k < TempHasSearchPointNum;k++)
        HasSearchPoint.push_back(TempHasSearchPoint[k]);*/
    }
    return MinPath;
}


deque<deque<uint> > NodePoint::SearchMinPath(uint minpointnum,uint mindistance)
{
    minpointnum = (minpointnum == 0)?uint(-1):minpointnum;
    mindistance = (mindistance == 0)?uint(-1):mindistance;
    deque<deque<uint> > MinPathPointNum;
    deque<deque<uint> > MinPathDistance;
    deque<deque<uint> > ReturnPath;
    deque<uint> Distance;
    deque<uint> endpointindex;
    bool FillConstraint = false;
    if(constraint.MustPointnum < 1)//没有必须经过点
    {
        endpointindex.push_back(constraint.EndPoint);
        MinPathPointNum = SearchMinPointNumPath(constraint.StartPoint,endpointindex,Distance,true);//查找经过最少点的路径
        uint MinPathLength = MinPathPointNum[0].size();//最短路径距离
        uint MinDistance = 1e7;//路径最短距离
        deque <uint> MinDistanceIndex;//最短距离路径索引
        deque <uint> MinPointNumIndex;
        for(uint i = 0; i < MinPathPointNum.size(); i++) //最少花费的路径
        {
            if(Distance[i] <= mindistance)
                FillConstraint = true;
            MinDistance = MinDistance < Distance[i]?MinDistance:Distance[i];
        }
        for(uint i = 0; i < MinPathPointNum.size(); i++)
        {
            if(Distance[i] == MinDistance)
            {
                ReturnPath.push_back(MinPathPointNum[i]);
                MinDistanceIndex.push_back(i);
            }
        }
        if(FillConstraint&&MinPathLength <= minpointnum)
        {
            cout<<"找到符合条件路径，最少点到达终点花费最少的路径为："<<endl;
            for(uint i = 0; i < ReturnPath.size(); i++)
            {
                for(uint j = 0; j < MinPathLength; j++)
                {
                    cout<<PointPosition[MinPathPointNum[MinDistanceIndex[i]][j]]->NodeName<<"  ";
                }
                cout<<endl;
            }
            cout<<"经过"<<MinPathLength<<"个点，花费为"<<MinDistance<<endl;
            return ReturnPath;
        }
        else if(MinDistance <= mindistance)
        {
            cout<<"找不到符合最少通过点要求，路径符合最短花费要求并以最少点到达终点的路径为："<<endl;
            for(uint i = 0; i < ReturnPath.size(); i++)
            {
                for(uint j = 0; j < MinPathLength; j++)
                {
                    cout<<PointPosition[MinPathPointNum[MinDistanceIndex[i]][j]]->NodeName<<"  ";
                }
                cout<<endl;
            }
            cout<<"经过"<<MinPathLength<<"个点，花费为"<<MinDistance<<endl;
            return ReturnPath;
        }
        else
        {
            Distance.clear();
            MinPathDistance = SearchMinDistancePath(constraint.StartPoint,endpointindex,Distance,true);
            if(Distance[0] <= mindistance)
            {

                cout<<"找不到符合最少通过点要求，路径符合最少花费要求并以最少点到达终点的路径为："<<endl;
                for(uint i = 0; i < MinPathDistance.size(); i++)
                {
                    MinPathLength = MinPathLength < MinPathDistance[i].size()?MinPathLength:MinPathDistance[i].size();
                }
                ReturnPath.clear();
                for(uint i = 0; i < MinPathDistance.size(); i++)
                {
                    if(MinPathLength == MinPathDistance[i].size())
                    {
                        ReturnPath.push_back(MinPathDistance[i]);
                        MinPointNumIndex.push_back(i);
                    }
                }
                for(uint i = 0; i < ReturnPath.size(); i++)
                {
                    for(uint j = 0; j < MinPathLength; j++)
                    {
                        cout<<PointPosition[MinPathPointNum[MinDistanceIndex[i]][j]]->NodeName<<"  ";
                    }
                    cout<<endl;
                }
                cout<<"经过"<<MinPathLength<<"个点，花费为"<<MinDistance<<endl;
            }
            else
            {
                cout<<"找不到符合要求的路径，路径符合最短花费要求并以最少点到达终点的路径为："<<endl;
                for(uint i = 0; i < ReturnPath.size(); i++)
                {
                    for(uint j = 0; j < MinPathLength; j++)
                    {
                        cout<<PointPosition[MinPathPointNum[MinDistanceIndex[i]][j]]->NodeName<<"  ";
                    }
                    cout<<endl;
                }
                cout<<"经过"<<MinPathLength<<"个点，花费为"<<MinDistance<<endl;
                return ReturnPath;
            }

        }

    }

    deque<deque<uint> > Point2Point;
    deque<uint> Point2PointDistance;
    deque<uint> Point2PointStartIndex;
    deque<uint> PathDistance;
    deque<deque<uint> > TempPoint2Point;
	deque<uint> TempPoint2PointDistance;
	deque<uint> TempPoint2PointStartIndex;
	deque<deque<uint> > TempPointPath;
    deque<deque<uint> > PointPath;
    deque<uint> endPoint;
    deque<uint> isSearch;
    deque<uint> addPath;
    deque<deque<uint> > HasSearchPoint;
    uint PointPathNum;
    Point2PointStartIndex.push_back(0);
    bool hasSearch = false;
    endPoint.insert(endPoint.end(),constraint.PathPassPoint.begin() + 1,constraint.PathPassPoint.end() - 1);

    PointPath = SearchMinPointNumPath(constraint.StartPoint,endPoint,Point2PointDistance,endPoint.size()/2+2,true);
    Point2Point.insert(Point2Point.end(),PointPath.begin(),PointPath.end());
    Point2PointStartIndex.push_back(Point2Point.size());
    for(uint i = 0; i < endPoint.size(); i++)
    {
        PointPath = SearchMinPointNumPath(endPoint[i],endPoint,Point2PointDistance,endPoint.size()/2,true);
       // if(PointPath.size() < 2)
         //   PointPath = SearchMinPointNumPath(endPoint[i],endPoint,Point2PointDistance,endPoint.size()/2 + 1,true);
        Point2Point.insert(Point2Point.end(),PointPath.begin(),PointPath.end());
        Point2PointStartIndex.push_back(Point2Point.size());
    }
    PointPath = SearchMinPointNumPath(constraint.EndPoint,endPoint,Point2PointDistance,endPoint.size(),true);
    Point2Point.insert(Point2Point.end(),PointPath.begin(),PointPath.end());
    Point2PointStartIndex.push_back(Point2Point.size());

    PointPath.clear();
    PointPath.insert(PointPath.end(),Point2Point.begin(),Point2Point.begin() + Point2PointStartIndex[1]);
    PathDistance.insert(PathDistance.end(),Point2PointDistance.begin(),Point2PointDistance.begin() + Point2PointStartIndex[1]);
    PointPathNum = PointPath.size();
    uint PointStartIndex;
    int SearchNum = endPoint.size();
    uint LastPoint;
    uint HasSearchNum;
    uint MustPassLineNum = constraint.MustpassLine.size();

    for(uint i = 0; i < PointPathNum; i++)
    {
        HasSearchPoint.push_back(deque<uint>());
        HasSearchPoint[i].push_back(PointPath[i].back());
    }
    for(int i = 0; i < SearchNum - 1; i ++)
    {
        PointPathNum = PointPath.size();
        for(uint j = 0; j < PointPathNum; j++)//循环处理每个路径
        {
            LastPoint = PointPath[j].back();
            HasSearchNum = HasSearchPoint[j].size();
            if(HasSearchNum != i+1)
            {
                PointPath.pop_front();
                HasSearchPoint.pop_front();
                j--;
                PointPathNum--;
                continue;
            }
            if(PointPosition[LastPoint]->PointAttribute != MustPass)
            {
                hasSearch = false;
                for(uint s = 0; s < HasSearchNum; s++)
                {
                    if(HasSearchPoint[j][s] == LastPoint)
                    {
                        hasSearch = true;
                        break;
                    }
                }
                if(hasSearch)
                    hasSearch = false;
                else
                {
                    for(uint k = 0; k < MustPassLineNum ; k++)
                    {
                        if(LastPoint == constraint.MustpassLine[k].c)
                        {
                            PointPath[i].push_back(constraint.MustpassLine[k].f);
                            PathDistance[i] = PathDistance[i] + constraint.MustpassLineDistance[k];
                            break;
                        }
                        if(LastPoint == constraint.MustpassLine[k].f)
                        {
                            PointPath[i].push_back(constraint.MustpassLine[k].c);
                            PathDistance[i] = PathDistance[i] + constraint.MustpassLineDistance[k];
                            break;
                        }

                    }
                    continue;
                }

            }
            for(int k = 0; k < SearchNum; k++)//查找最后一个点在路径中的起点位置
            {
                if(endPoint[k] == LastPoint)
                {
                    PointStartIndex = k + 1;
                    break;
                }
            }
            isSearch = PointPath[j];
            for(uint k = Point2PointStartIndex[PointStartIndex]; k < Point2PointStartIndex[PointStartIndex + 1]; k++)//连接最后一个点到其他点的路径
            {

                for(uint s = 0; s < HasSearchNum; s++)//查找点到点的终点是否在已搜索点中
                {
                    if(HasSearchPoint[j][s] == Point2Point[k].back())
                    {
                        hasSearch = true;
                        break;
                    }
                }
                if(hasSearch)
                {
                    hasSearch = false;
                    continue;
                }
                addPath = isSearch;
                addPath.insert(addPath.end(),Point2Point[k].begin() + 1,Point2Point[k].end());
                PointPath.push_back(addPath);
                addPath = HasSearchPoint[j];
                addPath.push_back(Point2Point[k].back());
                HasSearchPoint.push_back(addPath);
                PathDistance.push_back(PathDistance[j] + Point2PointDistance[k]);
                //				Point2PointDistance.push_back();
                //HasSearchPoint.back().push_back(Point2Point[k].back());
            }
            
            //j--;
//            PointPathNum--;
        }
        if(PointPathNum != PointPath.size())
        {
        	for(uint j = 0; j < PointPathNum; j++)
        	{
        		PointPath.pop_front();
            	HasSearchPoint.pop_front();
            	PathDistance.pop_front();
        	}
        }
        else
		{
			TempPoint2Point.push_back(deque<uint>());
			TempPoint2PointStartIndex.push_back(0);
			TempPoint2PointStartIndex.push_back(1);
			for(uint f = 0; f < endPoint.size(); f++)
			{
				TempPointPath = SearchMinPointNumPath(endPoint[f],endPoint,TempPoint2PointDistance,endPoint.size(),true);
				TempPoint2Point.insert(TempPoint2Point.end(),TempPointPath.begin(),TempPointPath.end());
				TempPoint2PointStartIndex.push_back(TempPoint2Point.size());
			}
			for(uint j = 0; j < PointPathNum; j++)
			{
				LastPoint = PointPath[j].back();
				HasSearchNum = HasSearchPoint[j].size();
				for(int k = 0; k < SearchNum; k++)//查找最后一个点在路径中的起点位置
				{
					if(endPoint[k] == LastPoint)
					{
						PointStartIndex = k + 1;
						break;
					}
				}
				isSearch = PointPath[j];
				for(uint k = TempPoint2PointStartIndex[PointStartIndex]; k < TempPoint2PointStartIndex[PointStartIndex + 1]; k++)//连接最后一个点到其他点的路径
				{

					for(uint s = 0; s < HasSearchNum; s++)//查找点到点的终点是否在已搜索点中
					{
						if(HasSearchPoint[j][s] == TempPoint2Point[k].back())
						{
							hasSearch = true;
							break;
						}
					}
					if(hasSearch)
					{
						hasSearch = false;
						continue;
					}
					addPath = isSearch;
					addPath.insert(addPath.end(),TempPoint2Point[k].begin() + 1,TempPoint2Point[k].end());
					PointPath.push_back(addPath);
					addPath = HasSearchPoint[j];
					addPath.push_back(TempPoint2Point[k].back());
					HasSearchPoint.push_back(addPath);
					PathDistance.push_back(PathDistance[j] + TempPoint2PointDistance[k]);
				}
			}
        }
        
    }
    
    int end2othernum = Point2PointStartIndex.back();
    deque<deque<int> > end2pointindex(SearchNum);
    for(int i = Point2PointStartIndex[Point2PointStartIndex.size() - 2]; i < end2othernum; i++)
    {
        for(uint j = 0; j < SearchNum; j++)
        {
            if(Point2Point[i].back() == endPoint[j])
            {
                end2pointindex[j].push_back(i);
                break;
            }
        }
    }
    PointPathNum = PointPath.size();
    for(uint i = 0; i < PointPathNum; i++)
    {
        for(int j = 0; j < SearchNum; j++)
        {
            if(PointPath[i].back() == endPoint[j])
            {
                int n = end2pointindex[j].size();
                for(int k = 0; k < n; k++)
                {
                    if(k == n-1)
                    {
                        PointPath[i].insert(PointPath[i].end(),Point2Point[end2pointindex[j][k]].rbegin() + 1,Point2Point[end2pointindex[j][k]].rend());
                        PathDistance[i] = PathDistance[i] + Point2PointDistance[end2pointindex[j][k]];
                    }
                    else
                    {
                        PointPath.push_back(PointPath[i]);
                        PointPath.back().insert(PointPath.back().end(),Point2Point[end2pointindex[j][k]].rbegin() + 1,Point2Point[end2pointindex[j][k]].rend());
                        PathDistance.push_back(PathDistance[j] + Point2PointDistance[end2pointindex[j][k]]);
                    }
                }
            }
        }
    }
    PointPathNum = PointPath.size();
    uint MinPathLength = uint(-1), MinDistance = uint(-1);
    deque<uint> MinPathLengthIndex;
    for(uint i = 0; i < PointPathNum; i++)
    {
        MinPathLength = MinPathLength < PointPath[i].size()?MinPathLength:PointPath[i].size();
        //MinDistance = MinDistance < PathDistance[i]?MinDistance:PathDistance[i];
    }
    for(uint i = 0; i < PointPathNum; i++)
    {
        if(MinPathLength == PointPath[i].size())
        {
            MinPathLengthIndex.push_back(i);
            MinDistance = MinDistance < PathDistance[i]?MinDistance:PathDistance[i];
        }
    }
    for(uint i = 0; i < MinPathLengthIndex.size(); i++)
    {
        if(MinDistance == PathDistance[MinPathLengthIndex[i]])
            ReturnPath.push_back(PointPath[MinPathLengthIndex[i]]);
    }
    if(MinPathLength <= minpointnum&&MinDistance <= mindistance)
    {
        cout<<"找到符合条件的路径，路径为:"<<endl;
    }
    else if(MinPathLength <= minpointnum)
    {
        cout<<"找不到符合最少花费的路径，满足最少经过点花费最少的路径为:"<<endl;
    }
    else if(MinDistance <= mindistance)
    {
        cout<<"找不到符合经过最少点条件的路径，最少经过点路径为:"<<endl;
    }
    else
    {
        cout<<"找不到符合条件的路径，最少经过点路径为:"<<endl;
    }
    for(uint i = 0; i < ReturnPath.size(); i++)
    {
        for(uint j = 0; j < MinPathLength; j++)
        {
            cout<<PointPosition[ReturnPath[i][j]]->NodeName<<"  ";
        }
        cout<<endl;
    }
    cout<<"经过"<<MinPathLength<<"个点，花费为"<<MinDistance<<endl;
    return ReturnPath;
}
