#include "NodePoint.h"

//deque<NodePoint* > NodePoint::PointPosition;
//Constraint NodePoint::constraint;

NodePoint::~NodePoint()
{
    clear();
}

//������Դ
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
        if(line.substr(0,5) == "Point")//��ǰ�����ڵ����
        {
            PointCreat = true;
            continue;
        }
        else if(line.length() >10 && line.substr(0,10) == "Connection")//��ǰ��ȡ���Ӳ���
        {
            PointCreat = false;
            continue;
        }
        if(PointCreat)//������
        {
            bool startread = false;//�Ƿ���Զ�ȡ����
            bool isname = true;//�Ƿ��ǽڵ��ǩ
            string c;
            string newpointname;//�½ڵ��ǩ
            string newpointattrib;//�½ڵ�����
            for(uint i = 0; i < line.length(); i++)
            {
                if(line[i] == '[')//��ʼ������
                {
                    startread = true;
                    c = "";
                    continue;
                }
                if(line[i] == ']')//��ȡ���ݽ���
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
            if(newpointattrib == "4")//�ж��Ƿ������ �����Ϊ��ǰ��ʵ��
            {
                newpoint = this;
                newpoint->NodeName = newpointname;
                newpoint->PointAttribute = Begin;
                newpoint->Index = PointPosition.size();
                constraint.StartPoint = newpoint->Index;
            }
            else//���������ڴ�
                newpoint = new NodePoint(newpointname,PointPosition.size(),Attribute(atoi(newpointattrib.c_str())));
            PointPosition.push_back(newpoint);//����ڵ�λ��
            switch(newpoint->PointAttribute)//���ݽڵ����Բ�ͬ���в���
            {
            case 0:
                break;
            case 1:
                constraint.MustPassPoint.push_back(PointPosition.size() - 1);
                break;//������뾭���ڵ�λ��
            case 2:
                constraint.ForbidPoint.push_back(PointPosition.size() - 1);
                break;//�����ֹ�����ڵ�λ��
            case 3:
                constraint.EndPoint = newpoint->Index;
                break;//�����յ�λ��
            }
        }
        else//���ӵ�
        {
            bool startread = false;//�Ƿ���Զ�ȡ����
            uint readcount = 0;//��ȡ�ĵڼ���ֵ
            string c;
            string newpointname1;//���ӵĵ�һ��������
            string newpointname2;//���ӵĵڶ���������
            string newlinetattrib;//��������
            string linedistance;//���Ӿ���
            uint firstPoint;//��һ����λ��
            uint secondPoint;//�ڶ�����λ��
            //��ȡ��ǩ
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
            MinLineLength = MinLineLength < atoi(linedistance.c_str())?MinLineLength : atoi(linedistance.c_str());//��СȨֵ

            Line temp;
            for(uint i = 0; i < PointPosition.size(); i++) //���������ҵ�λ��
            {
                if(PointPosition[i]->NodeName == newpointname1)
                {
                    if(newlinetattrib > "0")
                        temp.f = i;
                    firstPoint = i;
                    newpointname1 = "";
                    if(newlinetattrib == "1")//���뾭���߷���ǰ�����������
                    {
                        PointPosition[i]->Distance.push_front(atoi(linedistance.c_str()));//����Ȩֵ
                        PointPosition[i]->LineAttribute.push_front(Attribute(atoi(newlinetattrib.c_str())));//����������
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
            }//������뾭���������
            case 2:
                constraint.ForbidLine.push_back(temp);
                break;//�����ֹ������
            }
            if(newlinetattrib == "1")//�������ӹ�ϵ
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
    constraint.PathPassPoint = constraint.MustPassPoint;//�������㵽�յ�������뾭���߽ڵ������λ��
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
    constraint.Forbidnum = constraint.ForbidPoint.size();//�����ֹ���������߽ڵ�
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
    deque<deque<uint> > MinPath;//���·��
    deque<deque<uint> > PointPath;//�洢·��
    deque<uint> Pathdistance;//�洢·������
    deque<uint> HasSearchPoint;//�洢�Ѿ��������ĵ�
    deque<uint> HasSearchEnd;//�Ѿ�������յ�
    deque<uint> TempHasSearchPoint;//һ�ε����������Ѿ������ĵ�
    deque<uint> FinishEndPoint;//�Ѿ�������յ�
    deque<uint> TempFinishEndPoint;//һ�ε����Ѿ�������յ�
    Pathdistance.push_back(0);//��ʼ·������Ϊ0
    PointPath.push_back(deque<uint>());
    PointPath[0].push_back(BeginPointindex);//��ʼ·��Ϊ���
    HasSearchPoint.push_back(BeginPointindex);//�������Ϊ��������
    //D.clear();
    uint PathNum;//·������
    uint PointConectionNum;//���ӵ���һ���������
    uint HasSearchPointNum;//�Ѿ��������ĵ������
    uint EndPointIndexNum = endpointindex.size();//�յ�����
    uint FinishEndPointNum;//�Ѿ�������յ�����
    uint TempHasSearchPointNum;//һ��ѭ��������������������
    uint PointToNextDistance;//����һ��ľ���
    uint PointToNextIndex;//����һ�������
    num = num == 0?1:num;//�����յ�������СΪ1
    num = num > EndPointIndexNum?EndPointIndexNum:num;//���Ϊ�յ�����
    NodePoint* tempPoint;
    bool HasSearch;//�Ѿ�������־
    bool Finish = false;//������־
    bool isend;//�յ��־
    for(uint i = 0; i < EndPointIndexNum; i++) //�����������ͬ���յ�
    {
        if(BeginPointindex == endpointindex[i])
        {
            FinishEndPoint.push_back(BeginPointindex);
            num = num >= EndPointIndexNum?EndPointIndexNum:num+1;
        }
    }
    while(!Finish)
    {
        PathNum = PointPath.size();//����ѭ����ʼ·������
        HasSearchPointNum = HasSearchPoint.size();//����ѭ���Ѿ��������ʼ����
        TempHasSearchPoint.clear();
        TempFinishEndPoint.clear();
        for(uint i = 0; i < PathNum; i++) //����ÿһ��·��
        {
            tempPoint = PointPosition[PointPath[i].back()];//ָ��·�����һ����
            PointConectionNum = tempPoint->NextPoint.size();//���һ�������ӽڵ�����
            for(uint j = 0; j < PointConectionNum; j++)//����ÿһ�����ӵ�
            {
                PointToNextDistance = tempPoint->Distance[j];
                PointToNextIndex = tempPoint->NextPoint[j];
                HasSearch = false;
                isend = false;//�յ��־
                if(Constra&&(tempPoint->LineAttribute[j] == 2||PointPosition[PointToNextIndex]->PointAttribute == 2))//������ֹ���������
                    continue;
                for(uint k = 0; k < HasSearchPointNum; k++) //�������ӵ��Ƿ�������������
                {
                    if(HasSearchPoint[k] == PointToNextIndex)
                    {
                        HasSearch = true;
                        break;
                    }
                }
                if(HasSearch)//�Ѿ�������������
                    continue;
                for(uint k = 0; k < EndPointIndexNum; k ++) //�ж��Ƿ����յ�
                {
                    if(endpointindex[k] == PointToNextIndex)
                    {
                        isend = true;
                        //Finish = true;
                        break;
                    }
                }
                if(isend)//�ж��յ��Ƿ��Ѿ�������
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
                if(isend)//�����յ�
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
                    MinPath.push_back(PointPath[i]);//�������·��
                    MinPath.back().push_back(PointToNextIndex);
                    D.push_back(Pathdistance[i] + PointToNextDistance);
                }

                TempHasSearchPointNum = TempHasSearchPoint.size();//�����Ѿ�������
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
                PointPath.push_back(PointPath[i]);//����·��
                PointPath.back().push_back(PointToNextIndex);
                Pathdistance.push_back(Pathdistance[i] + PointToNextDistance);

            }

            if(i > 2)//ɾ����ǰ·��
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
        HasSearchPoint.insert(HasSearchPoint.end(),TempHasSearchPoint.begin(),TempHasSearchPoint.end());//�����Ѿ���������յ�
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
    deque<deque<uint> > MinPath;//���·��
    deque<deque<uint> > PointPath;//�洢·��
    deque<uint> PathToNext;//������һ����ľ���
    deque<uint> Pathdistance;//·������
    deque<uint> HasSearchPoint;//�Ѿ��������ĵ�
    deque<uint> TempHasSearchPoint;
    deque<uint> FinishEndPoint;//�Ѿ�������յ�
    deque<uint> TempFinishEndPoint;//һ�ε����Ѿ�������յ�
    Pathdistance.push_back(0);//��ʼ����Ϊ0
    PointPath.push_back(deque<uint>());
    PointPath[0].push_back(BeginPointindex);//��ʼ·��Ϊ���
    HasSearchPoint.push_back(BeginPointindex);//���Ϊ��������
    PathToNext.push_back(uint(1e7));//������һ�����ü���ֵ
    //D.clear();
    uint PathNum;//·������
    uint PointConectionNum;//��������
    uint HasSearchPointNum;//�Ѿ�����������
    uint EndPointIndexNum = endpointindex.size();//�յ�����
    uint PathPointNum;// ����·���ڵ�����
    uint MinLineLength = 1e7;//��С�������ü���ֵ
    uint tempMinLineLength;//�����Ȩֵ
    uint PointToNextDistance;//������һ�����
    uint PointToNextIndex;//��һ��λ��
    uint FinishEndPointNum;//�Ѿ�������յ�����
    uint LastPointIndex;//���һ�ڵ�λ��
    NodePoint* tempPoint;
    bool HasSearch;//�Ѿ�������־
    bool Finish = false;//��ɱ�־
    bool isend;//�յ��־
    for(uint i = 0; i < EndPointIndexNum; i++) //����յ��Ƿ��������ͬ
        if(BeginPointindex == endpointindex[i])
        {
            FinishEndPoint.push_back(BeginPointindex);
            num = num >= EndPointIndexNum?EndPointIndexNum:num+1;
        }
    while(!Finish)
    {
        PathNum = PointPath.size();//����ѭ����ʼ·������
        HasSearchPointNum = HasSearchPoint.size();//����ѭ���Ѿ������ĵ�����
        TempHasSearchPoint.clear();
        TempFinishEndPoint.clear();
        tempMinLineLength = uint(-1);
        for(uint i = 0; i < PathNum; i++) //����ÿһ��·�����һ���������
        {
            PathPointNum = PointPath[i].size();
            tempPoint = PointPosition[PointPath[i].back()];
            PointConectionNum = tempPoint->NextPoint.size();
            HasSearch = false;
            LastPointIndex = PointPath[i][PathPointNum - 1];
            if(HasSearchPointNum > 1)//�Ѿ��������㣬������ǰ���Ƿ��Ѿ�������
            {
                for(uint j = 0; j < HasSearchPointNum; j++)
                {
                    if(LastPointIndex == HasSearchPoint[j])//�Ѿ��������ĵ�ɾ��
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
            PathToNext[i] -= MinLineLength;//ǰ��һ��
            tempMinLineLength = PathToNext[i] <= 0||tempMinLineLength == 1?tempMinLineLength:(tempMinLineLength > PathToNext[i]?PathToNext[i]:tempMinLineLength);
            if(PathToNext[i] <= 0)//������һ�ڵ�
            {
                uint TempHasSearchPointNum = TempHasSearchPoint.size();
                if(TempHasSearchPointNum == 0)//����ڵ㵽�������ڵ�
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

                for(uint k = 0; k < EndPointIndexNum; k ++) //����Ƿ����յ�
                {
                    if(endpointindex[k] == LastPointIndex)
                    {
                        isend = true;
                        //Finish = true;
                        break;
                    }
                }
                /*if(isend)//����յ��Ƿ���������
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
                if(isend)//�����յ㵽�������յ�
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
                if(isend)//������С·���볤��
                {
                    MinPath.push_back(PointPath[i]);
                    D.push_back(Pathdistance[i]);
                    //continue;
                }

                for(uint j = 0; j < PointConectionNum; j++)//����ÿһ�����ӵ�
                {
                    PointToNextDistance = tempPoint->Distance[j];
                    PointToNextIndex = tempPoint->NextPoint[j];
                    HasSearch = false;
                    if(Constra&&(tempPoint->LineAttribute[j] == 2||PointPosition[PointToNextIndex]->PointAttribute == 2))
                        continue;
                    for(uint k = 0; k < HasSearchPointNum; k++) //�������ӵ��Ƿ�������������
                    {
                        if(HasSearchPoint[k] == PointToNextIndex)
                        {
                            HasSearch = true;
                            break;
                        }
                    }
                    if(HasSearch)//������������
                        continue;
                    tempMinLineLength = tempMinLineLength < PointToNextDistance?tempMinLineLength:PointToNextDistance;
                    PointPath.push_back(PointPath[i]);
                    PointPath.back().push_back(PointToNextIndex);
                    Pathdistance.push_back(Pathdistance[i] + PointToNextDistance);
                    PathToNext.push_back(PointToNextDistance);
                }
                if(i > 2)//ɾ����·��
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
        MinLineLength = (tempMinLineLength != -1)?tempMinLineLength:MinLineLength;//������С����
        HasSearchPoint.insert(HasSearchPoint.end(),TempHasSearchPoint.begin(),TempHasSearchPoint.end());//���������������յ�
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
    if(constraint.MustPointnum < 1)//û�б��뾭����
    {
        endpointindex.push_back(constraint.EndPoint);
        MinPathPointNum = SearchMinPointNumPath(constraint.StartPoint,endpointindex,Distance,true);//���Ҿ������ٵ��·��
        uint MinPathLength = MinPathPointNum[0].size();//���·������
        uint MinDistance = 1e7;//·����̾���
        deque <uint> MinDistanceIndex;//��̾���·������
        deque <uint> MinPointNumIndex;
        for(uint i = 0; i < MinPathPointNum.size(); i++) //���ٻ��ѵ�·��
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
            cout<<"�ҵ���������·�������ٵ㵽���յ㻨�����ٵ�·��Ϊ��"<<endl;
            for(uint i = 0; i < ReturnPath.size(); i++)
            {
                for(uint j = 0; j < MinPathLength; j++)
                {
                    cout<<PointPosition[MinPathPointNum[MinDistanceIndex[i]][j]]->NodeName<<"  ";
                }
                cout<<endl;
            }
            cout<<"����"<<MinPathLength<<"���㣬����Ϊ"<<MinDistance<<endl;
            return ReturnPath;
        }
        else if(MinDistance <= mindistance)
        {
            cout<<"�Ҳ�����������ͨ����Ҫ��·��������̻���Ҫ�������ٵ㵽���յ��·��Ϊ��"<<endl;
            for(uint i = 0; i < ReturnPath.size(); i++)
            {
                for(uint j = 0; j < MinPathLength; j++)
                {
                    cout<<PointPosition[MinPathPointNum[MinDistanceIndex[i]][j]]->NodeName<<"  ";
                }
                cout<<endl;
            }
            cout<<"����"<<MinPathLength<<"���㣬����Ϊ"<<MinDistance<<endl;
            return ReturnPath;
        }
        else
        {
            Distance.clear();
            MinPathDistance = SearchMinDistancePath(constraint.StartPoint,endpointindex,Distance,true);
            if(Distance[0] <= mindistance)
            {

                cout<<"�Ҳ�����������ͨ����Ҫ��·���������ٻ���Ҫ�������ٵ㵽���յ��·��Ϊ��"<<endl;
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
                cout<<"����"<<MinPathLength<<"���㣬����Ϊ"<<MinDistance<<endl;
            }
            else
            {
                cout<<"�Ҳ�������Ҫ���·����·��������̻���Ҫ�������ٵ㵽���յ��·��Ϊ��"<<endl;
                for(uint i = 0; i < ReturnPath.size(); i++)
                {
                    for(uint j = 0; j < MinPathLength; j++)
                    {
                        cout<<PointPosition[MinPathPointNum[MinDistanceIndex[i]][j]]->NodeName<<"  ";
                    }
                    cout<<endl;
                }
                cout<<"����"<<MinPathLength<<"���㣬����Ϊ"<<MinDistance<<endl;
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
        PointPath = SearchMinPointNumPath(endPoint[i],endPoint,Point2PointDistance,endPoint.size()/2>10?10:endPoint.size()/2,true);
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
    uint MinPointNum = 1e7;
	uint MaxPointNum = 0;

    for(uint i = 0; i < PointPathNum; i++)
    {
        HasSearchPoint.push_back(deque<uint>());
        HasSearchPoint[i].push_back(PointPath[i].back());
    }
    for(int i = 0; i < SearchNum - 1; i ++)
    {
        PointPathNum = PointPath.size();
        //cout<<PointPathNum<<endl;
        if(PointPathNum > 8000)//·������ʱɾ�����ȹ���·��
		{
			uint meanPointNum = (MinPointNum + MaxPointNum)/2;
			for(uint j = 0; j < PointPathNum; j++)
			{
				if(PointPath[j].size() > meanPointNum)
				{
					PointPath[j] = PointPath[0];
					PointPath.pop_front();
					HasSearchPoint[j] = HasSearchPoint[0];
					HasSearchPoint.pop_front();
					PathDistance[j] = PathDistance[0];
					PathDistance.pop_front();
					j--;
					PointPathNum--;
				}
			}
		}
		MinPointNum = 1e7;
		MaxPointNum = 0;
		
        for(uint j = 0; j < PointPathNum; j++)//ѭ������ÿ��·��
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
				uint next,dist;
				for(uint k = 0; k < MustPassLineNum ; k++)
				{
					if(LastPoint == constraint.MustpassLine[k].c)
					{
						next = constraint.MustpassLine[k].f;
						dist = constraint.MustpassLineDistance[k];
						//PointPath[i].push_back(constraint.MustpassLine[k].f);
						//PathDistance[i] = PathDistance[i] + constraint.MustpassLineDistance[k];
						break;
					}
					if(LastPoint == constraint.MustpassLine[k].f)
					{
						next = constraint.MustpassLine[k].c;
						dist = constraint.MustpassLineDistance[k];
						//PointPath[i].push_back(constraint.MustpassLine[k].c);
						//PathDistance[i] = PathDistance[i] + constraint.MustpassLineDistance[k];
						break;
					}

				}
				hasSearch = false;
				for(uint s = 0; s < HasSearchNum; s++)
				{
					if(HasSearchPoint[j][s] == next)
					{
						hasSearch = true;
						break;
					}
				}
				if(hasSearch)
					hasSearch = false;
				else
				{
					addPath = PointPath[j];
					addPath.push_back(next);
					MaxPointNum = MaxPointNum < addPath.size()?addPath.size():MaxPointNum;
					MinPointNum = MinPointNum > addPath.size()?addPath.size():MinPointNum;
					PointPath.push_back(addPath);
					addPath = HasSearchPoint[j];
					addPath.push_back(next);
					HasSearchPoint.push_back(addPath);
					PathDistance.push_back(PathDistance[j] + dist);
					continue;
				}

			}
            for(int k = 0; k < SearchNum; k++)//�������һ������·���е����λ��
            {
                if(endPoint[k] == LastPoint)
                {
                    PointStartIndex = k + 1;
                    break;
                }
            }
            isSearch = PointPath[j];
            for(uint k = Point2PointStartIndex[PointStartIndex]; k < Point2PointStartIndex[PointStartIndex + 1]; k++)//�������һ���㵽�������·��
            {

                for(uint s = 0; s < HasSearchNum; s++)//���ҵ㵽����յ��Ƿ�������������
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
                MaxPointNum = MaxPointNum < addPath.size()?addPath.size():MaxPointNum;
				MinPointNum = MinPointNum > addPath.size()?addPath.size():MinPointNum;
                addPath = HasSearchPoint[j];
                addPath.push_back(Point2Point[k].back());
                HasSearchPoint.push_back(addPath);
                PathDistance.push_back(PathDistance[j] + Point2PointDistance[k]);
                //				Point2PointDistance.push_back();
                //HasSearchPoint.back().push_back(Point2Point[k].back());
            }

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
			if(TempPoint2Point.empty())
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
			}
			
			for(uint j = 0; j < PointPathNum; j++)
			{
				LastPoint = PointPath[j].back();
				HasSearchNum = HasSearchPoint[j].size();
				for(int k = 0; k < SearchNum; k++)//�������һ������·���е����λ��
				{
					if(endPoint[k] == LastPoint)
					{
						PointStartIndex = k + 1;
						break;
					}
				}
				isSearch = PointPath[j];
				for(uint k = TempPoint2PointStartIndex[PointStartIndex]; k < TempPoint2PointStartIndex[PointStartIndex + 1]; k++)//�������һ���㵽�������·��
				{

					for(uint s = 0; s < HasSearchNum; s++)//���ҵ㵽����յ��Ƿ�������������
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
					MaxPointNum = MaxPointNum < addPath.size()?addPath.size():MaxPointNum;
					MinPointNum = MinPointNum > addPath.size()?addPath.size():MinPointNum;
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
        cout<<"�ҵ�����������·����·��Ϊ:"<<endl;
    }
    else if(MinPathLength <= minpointnum)
    {
        cout<<"�Ҳ����������ٻ��ѵ�·�����������پ����㻨�����ٵ�·��Ϊ:"<<endl;
    }
    else if(MinDistance <= mindistance)
    {
        cout<<"�Ҳ������Ͼ������ٵ�������·�������پ�����·��Ϊ:"<<endl;
    }
    else
    {
        cout<<"�Ҳ�������������·�������پ�����·��Ϊ:"<<endl;
    }
    for(uint i = 0; i < ReturnPath.size(); i++)
    {
        for(uint j = 0; j < MinPathLength; j++)
        {
            cout<<PointPosition[ReturnPath[i][j]]->NodeName<<"  ";
        }
        cout<<endl;
    }
    cout<<"����"<<MinPathLength<<"���㣬����Ϊ"<<MinDistance<<endl;
    return ReturnPath;
}
