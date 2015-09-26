#include<iostream>
#include<vector>
//using namespace std;
void discretizate(vector<vector<string> > &data,int type,int arg1,vector<bool>isDiscrete);
void discretizate_Equal_Width(vector<vector <string> > &,int,int,vector<bool>);

//初始化變數
static int row=690;
static int attNum=14;
//static bool isDiscrete[15]={1,0,0,0,0,0,0,1,1,0,1,1,0,0,1};



//function
void discretizate(vector<vector<string> > &data,int type,int arg1,vector<bool>isDiscrete)
{
    //將除了類別值外的特徵離散化
    for(int i=0;i<data[0].size()-1;i++)
    {
        if(isDiscrete[i]==0)
        {
            switch (type)
            {
                case 1://Equal_Width
                    discretizate_Equal_Width(data,i,arg1,isDiscrete);
                    break;
                case 2://Equal_Freq
                    break;
                case 3://Entropy
                    break;
                
            }
        }
    }
}
//col是特徵欄位，k是區間數
void discretizate_Equal_Width(vector<vector <string> > &data,int col,int k,vector<bool>isDiscrete)
{
    if(data[0].size()!=isDiscrete.size())
        cout<<"isDiscrete isn't fit to data"<<endl;
    else{
        double max=0;
        double min=100;
        //將該col截出,找出最大最小值
        vector<double> dv;
        //vector<int> iv;
        //vector<string> sv;
        for(int i =0;i<data.size();i++)
        {
            double tmp = atof(data[i][col].c_str());
            //cout<<tmp<<endl;
            dv.push_back(tmp);
            if(tmp>max)
                max=tmp;
            if(tmp<min)
                min=tmp;
        }
        double interval=(max-min)/k;
        //將連續值轉為離散值，並存回data
        for (int i=0;i<data.size();i++)
        {
            int a=((dv[i]-min)/interval);
            if(a==k)
                a=k-1;
            //cout<<a<<endl;
            //sv.push_back(a.toString());
            data[i][col]=to_string(a);
        }

    }
    
}
void discretizate_Entropy();
//feature selection
/*
double getH(int c1)
{
    //int row = v.size();
    double result=0;
    //計算出現次數
    for(int i = 0;i<row;i++)
    {
        //stringstream tmp ;
        //tmp<<1;
        //cout<<v[i][c1];
        //cout<<tmp<<endl;
        int a =stoi(v[i][c1]);
        //a<<tmp;
        calArea1[ a ]+=1;
        //cout<<a<<endl;
    }
    
    
    //依照出現次數計算entropy值
    for(int i = 0;i<attNum+1;i++)
    {
        double x = (calArea1[i]);
        x=x/row;
        if(x!=0)
        {
            //cout<<x<<endl;
            result+=x*log(x)/log(2);
        }
    }
    //cout<<result<<endl;
    return result*(-1);
}
double get2H(int c1,int c2);*/