#include<iostream>
#include<vector>
string int2str(int i) {
      string s;
      stringstream ss(s);
      ss << i;
    
      return ss.str();
    }
string dbl2str(double i) {
    string s;
    stringstream ss(s);
    ss << i;
    
    return ss.str();
}
int str2int(string s){
    stringstream ss(s);
    int a ;
    ss>>a;
    return a;
}
double str2dbl(string s){
    stringstream ss(s);
    double a ;
    ss>>a;
    return a;
}
string v2str(vector<int> v){
    stringstream ss;
    for(int i=0;i<v.size();i++)
    {
        ss<<v[i]<<" ";
    }
    return ss.str();
}
void arrayToVector(bool a[] ,int l,vector<bool> *v){
    for(int i=0;i<l;i++)
    {
        v->push_back(a[i]);
    }
}
void arrayToVector(int a[] ,int l,vector<int> *v){
    for(int i=0;i<l;i++)
    {
        v->push_back(a[i]);
    }
}
int findMaxIndex(vector<double> v){
    int index=-1;
    double value=-1000;
    for (int i=0;i<v.size();i++)
    {
        if(v[i]>value)
        {
            index=i;
            value=v[i];
        }
    }
    return index;
}
void printLog(vector<string> v){
    ofstream file("log");
    
    if (!file) return;
    
    for(int i=0;i<v.size();i++)
    {
        file<<v[i]<<"\n";
        //if(v[i]==1)
        //file<<"\n\n";
    }
    //file<<endl;
    file.close();
}
void lineToVector(string *s,vector<string> *v,int start ,int end){
    if(start>=end)
    {
        //cout<<"done"<<endl;
    }
    else
    {
        string tmp;
        int k = s->find(' ',start);
        if(k==-1)
        {
            k=end;
        }
        tmp.append(s[0],start,k-start);
        v->push_back(tmp);
        lineToVector(s,v,k+1,end);
    }
}
void readFile(string fileName,vector<vector<string> > &v){
    fstream data;
    string line;
    data.open(fileName,ios::in);
    if(!data)
    {
        cerr << "Can't open the file!\n";
        exit(1);
    }
    //將文件存入vector
    while (getline(data,line))
    {
        vector<string>vv;
        int end = line.length();
        
        lineToVector(&line,&vv,0,end);
        v.push_back(vv);
    }
    
}
//for SNB only
//將特定欄位複製至新vector,並產生新的type向量（分類器的參數）
//col是特徵子集，有順序特性，不包含類別
void copyVector(vector<vector<string> > data, vector<vector<string> > &newData, vector<int> col,vector<int> type,vector<int> &newType )
{
    int c ;
    for(int i =0;i<type.size();i++)
    {
        if(type[i]==2)
            c=i;
    }
    //cout<<c<<endl;
    newData.resize(data.size());
    
    for(int i=0;i<data.size();i++)
    {
        for(int j=0;j<col.size();j++)
        {
            //-1表示未加入特徵的空位
            if(col[j]!=-1)
            {
                newData[i].push_back(data[i][col[j]]);
                //type[col[j]]=1;
            }
        }
        //插入類別值
        newData[i].push_back(data[i][c]);
        //cout<<data[i][c]<<endl;
        //cout<<data[i][c]<<endl;
        //type[col[col.size()]]=2;
    }
    
    //
    int l =0;
    for(int i=0;i<col.size();i++)
    {
        if(col[i]!=-1)
            l=l+1;
    }
    newType.resize(l+1);
    for(int i=0;i<newType.size();i++)
    {
        newType[i]=1;
    }
    newType[l]=2;
    //cout<<newType.size()<<endl;
   

}


