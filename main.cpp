//
//  main.cpp
//  Project_Management(cpm)
//
//  Created by Noor Fathima on 07/05/23.
//


#include <iostream>
#include <cstdio>
#include <iomanip>
#include <stack>
#include <vector>
#include <queue>

using namespace std;

typedef struct NODE{
    string actName;
    int duration;
    int earlystart, earlyfinish, lateststart, latestfinish, totalfloat;
}activity;


void topologic_sort(int i,stack<int> &s,vector<bool> &visit, vector<vector<int>> &adj_lst)
{
    visit[i]=true;
    vector<int>::iterator itr;
    for (itr=adj_lst[i].begin();itr!=adj_lst[i].end();itr++)
        if(!visit[*itr])
            topologic_sort(*itr,s,visit,adj_lst);
    s.push(i);
}

vector<int> read()
{
    vector<int> input ;
    do
    {
        int num ;
        if(cin>>num)
            input.push_back(num) ;
    }while(cin.peek() != '\n' && cin);
    return input ;
}

int main() {
    int top,actNo;
    vector<vector<int>> pre;
    vector<vector<int>> suc;
    
    cout<<"______________ PROJECT MANAGEMENT ______________\n";
    cout<<"          (using critical path method)          \n\n";
    cout<<"Enter the number of Activities: ";
    cin>>actNo;
    
    activity node[actNo+2];
    node[0].actName = "Start";
    node[0].duration = 0;
    node[actNo+1].actName = "Finish";
    node[actNo+1].duration = 0;

    
    for(int x=1;x<=actNo;x++) {
        cout<<"\nEnter Activity no."<<x<<"'s Name: ";
        cin>>node[x].actName;
        //getline(cin, node[i].actName);
        cout<<"Enter Activity no."<<x<<"'s Duration: ";
        cin>>node[x].duration;
    }
    cout<<"\n\nDetails entered successfully."<<endl;
    
    cout<<endl<<"Displaying Details:-"<<endl;
    cout<<endl<<left<<setw(5)<<"Sno."<<setw(15)<<"Activity Name"<<setw(10)<<"Duration\n"<<endl;
    for(int x=0;x<=actNo+1;x++) {
        cout<<left<<setw(5)<<x<<setw(15)<<node[x].actName<<setw(10)<<node[x].duration<<endl;
    }
    
    for(int x=0;x<=actNo;x++) {
        vector<int> tmp;
        suc.push_back(tmp);
        pre.push_back(tmp);
    }
    cout<<endl<<"Enter Dependency Details:-";
    cout<<endl<<"\n~Input all the tasks with no predecessors as the successor of \"Start\" ";
    cout<<endl<<"~Input all the tasks with no successor as the predecessor of \"Finish\" ";
    cout<<endl<<"\nStart=0, Finish="<<actNo+1<<"\n"<<endl;
    for(int x=0;x<=actNo;x++)
    {
        cout<<endl<<"Enter successors for activity "<<node[x].actName<<":";
        vector<int>tmp=read();
        if(tmp.size()==0){
            suc[x].push_back(actNo);
            pre[actNo].push_back(x);
        }
        else {
            for(int y=0;y<tmp.size();y++)
            {
                pre[tmp[y]].push_back(x);
            }
            for(int y=0;y<tmp.size();y++)
            {
                suc[x].push_back(tmp[y]);
            }
        }
    }
    
    // calculating earliest start and finish time
    vector<bool> visited1(actNo+2, false);
    stack<int> s1;
    
    topologic_sort(0,s1,visited1,suc);
    
    node[0].earlystart = 0;
    node[0].earlyfinish = 0;
    s1.pop();

    while(!s1.empty()) {
        int fmax=-1;
        top=s1.top();
        for(int x=0;x<pre[top].size();x++) {
            if(fmax<node[pre[top][x]].earlyfinish)
                fmax=node[pre[top][x]].earlyfinish;
        }
        node[top].earlystart=fmax;
        node[top].earlyfinish=fmax+node[top].duration;
        s1.pop();
    }

    
    // calculating latest start and finish time
    vector<bool> visited2(actNo+2, false);
    stack<int> s2;

    topologic_sort(actNo+1,s2,visited2,pre);

    node[actNo+1].lateststart=node[actNo+1].earlystart;
    node[actNo+1].latestfinish=node[actNo+1].earlyfinish;
    s2.pop();
    
    while(!s2.empty())
    {
        int smin=999;
        top=s2.top();
        for(int x=0;x<suc[top].size();x++) {
            if(smin>node[suc[top][x]].lateststart) {
                smin=node[suc[top][x]].lateststart;
            }
        }
        node[top].latestfinish=smin;
        node[top].lateststart=smin-node[top].duration;
        s2.pop();
    }


    cout<<"\nOUTPUT :"<<endl;
    cout<<endl<<left<<setw(5)<<"No."<<setw(11)<<"Activity"<<setw(10)<<"Duration"<<setw(5)<<"ES"<<setw(5)<<"EF"<<setw(5)<<"LS"<<setw(5)<<"LF"<<setw(5)<<"TF\n"<<endl;
    for(int x=0;x<actNo+2;x++)
    {
        node[x].totalfloat=node[x].latestfinish-node[x].earlystart-node[x].duration;
        cout<<endl<<left<<setw(5)<<x<<setw(11)<<node[x].actName<<setw(10)<<node[x].duration<<setw(5)<<node[x].earlystart<<setw(5)<<node[x].earlyfinish<<setw(5)<<node[x].lateststart<<setw(5)<<node[x].latestfinish<<setw(5)<<node[x].totalfloat;
    }
    cout<<endl<<"\n(ES: Early Start Time, EF: Early Finish Time)";
    cout<<endl<<"(LS: Latest Start Time, LF: Latest Finish Time)";
    cout<<endl<<"(TF: Total Float, FF: Free Float)";
    cout<<endl<<"\nNOTE:\n\tTotal Float: The time the completion of an activity be delayed without affecting the Project completion time"<<endl;

    // finding the critical path using BFS
    queue<int> q;
    vector<bool> criticalPath(actNo+2,false);
    vector<bool> visited3(actNo+2,false);
    
    q.push(0);
    criticalPath[0]=true;
    
    while(!q.empty())
    {
        top=q.front();
        q.pop();
        if(node[top].earlystart==node[top].lateststart)
        {
            criticalPath[top]=true;
        }
        for(int x=0;x<suc[top].size();x++)
        {
            //if(visited3[suc[top][x]]==false)
                q.push(suc[top][x]);
        }
    }

    int totalduration=0;
    cout<<endl<<"Critical Path : ";
    for(int x=0;x<criticalPath.size()-1;x++)
    {
        if(criticalPath[x]==true)
        {
            cout<<node[x].actName<<"->";
            totalduration+=node[x].duration;
        }
    }
    cout<<"Finish"<<endl;
    cout<<"Total Duration of Cirtical Path:"<<totalduration;
    cout<<endl<<"\nby:\n\tNoor Fathima (22pd26)\n\tDiravina E I (22pd12)"<<endl;
    return 0;
}

