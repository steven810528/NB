#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<sstream>
#include<math.h>

using namespace std;
vector<string> myLog;
//基本方法
#include "toolbox.cpp"
#include "print.cpp"
//相關方法
#include "fs.cpp"
#include "Bayesian.cpp"

//主要資料集
vector<vector<string> > data;

int main()
{
    //依照輸入值選擇分析檔案，宣告陣列是因為方便設值
    string fileName;
    int type1     []={1,1,1,1,1,1,1,1,1,1,2};
    bool discrete1[]={0,0,0,0,0,0,0,0,0,0,1};
    int type2     []={1,1,1,1,1,1,1,1,2};
    bool discrete2[]={0,0,0,0,0,0,0,0,1};
    
    vector<bool> isDiscrete;        //是否為離散屬性
    vector<int> attType;            //-1表示無用索引值，1表示屬性值，0表示類別值，2表示切割群
    vector<int> attOrder;           //用於處理snb的屬性順續
    
    cout<<"Please choose a dataset to be analysis:"<<endl;
    cout<<"->1.Glass"<<endl<<"->2.Pima"<<endl<<endl;
     //選擇資料檔
    int f ;
    cin>>f;
    switch(f)
    {
        case 1://有索引值，九個屬性，一個類別值，
            fileName="data/glass.txt";
            arrayToVector(discrete1,11,&isDiscrete);
            arrayToVector(type1,11,&attType);
            break;
        case 2:
            fileName="data/pima.txt";
            arrayToVector(discrete2,9,&isDiscrete);
            arrayToVector(type2,9,&attType);
            break;
            /*
        case 3:
            fileName="data/hepatitis.txt";
            //int arr[]={1,0,0,0,0,0,0,1};
            break;
        case 4:
            fileName="data/";
            //int arr[]={1,0,0,0,0,0,0,1};
            break;
             */
    }

    //#############################################
    //開始將資料存入vector中
    //log.push_back("開始將資料存入vector中");
    readFile(fileName,data);
    myLog.push_back("將要分析的檔案是："+fileName);
    myLog.push_back("檔案讀取成功");
    //#############################################
    
    
    
    //#############################################
    myLog.push_back("將連續資料離散化");
    discretizate(data,1,10,isDiscrete);
    myLog.push_back("檔案離散化成功");
    //#############################################
    
    //printBigVector(data);
    //依據對類別值的預測正確率排序屬特徵
    
    //排序後的資料欄位order和型別向量newType
    vector<int > newType;
    vector <vector<string> > newData;
    vector<int> order;
    //#############################################
    SNB(order,data,attType,0);                              //0為不刪除；1為刪除
    copyVector(data,newData,order,attType,newType);         //依據結果修改data的特徵順序
    //#############################################
    
    //printBigVector(newData);
    
    vector<vector<double > > EofPrior;                      //先驗機率的期望值
    vector<int> priorSize;                                  //先驗樣本大小
    vector<vector<vector<int> > > GD;                       //先驗樣本的機率分配
    initialPrior(EofPrior,GD,priorSize,newData, newType);   //設定長度，數值採用laplace
    
    int a = 0;
    a= Bayesian(newData,newData,EofPrior,priorSize,newType);
    cout<<"總資料筆數："<<newData.size()<<endl;
    cout<<"原始正確筆數："<<a<<endl;
    double r=a;
    r=r/newData.size();
    cout<<"正確率："<<r<<endl;
    
    
    //#############################################
    //使用dirichlet取得適當的PriorSize大小
    findFitSize(EofPrior,priorSize,newData,newType);
    //#############################################
    
    //#############################################
    //使用general dirichlet 取得各個特徵適當的特徵值分配
    findFitValue(EofPrior,GD,priorSize,newData,newType);
    //#############################################
    
    findFitSize(EofPrior,priorSize,newData,newType);
    
    a= Bayesian(newData,newData,EofPrior,priorSize,newType);
    cout<<"結果正確筆數："<<a<<endl;
    r=a;
    r=r/newData.size();
    cout<<"正確率："<<r<<endl;
    
    
    printLog(myLog);
}



