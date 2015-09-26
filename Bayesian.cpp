#include<string>
#include<vector>
#include<math.h>
//採用leave one out ，機制設置於predict()內
//而主要方法丟入兩個參數原本要區分學習和測試的資料檔，但最後使用leave one out的作法，在predict的方法內扣除實際類別值的機率值，
//但主要方法後來就沒有在更新，但簡化以後可以直接不需要切割資料檔。
//以下在使用bayesian()時皆在testSet和learningSet欄位丟入同一資料檔_data
//#############################################################################################################

int Bayesian(vector<vector<string> > trainSet,vector<vector<string> > testSet,vector<vector<double > >  &EofPrior, vector<int > &priorSize,vector<int> type);
void learningToHyposes(vector<vector<string> > trainSet , vector<vector<vector<int> > > &H,vector<int> &Hsize,vector<int> type);
int predict(vector<vector<string> > testSet ,int row ,int Ccol,vector<int> type,vector<vector<vector<int> > > H,vector<int> Hsize,vector<vector<double > >  EofPrior, vector<int> priorSize);
//EofPrior存放先驗樣本的期望值，二維陣列
//priorSize存放先驗樣本的大小，一維陣列
void initialPrior(vector<vector<double> > &EofPrior , vector<vector<vector<int> > > &GD,vector<int> &priorSize, vector<vector<string > > data, vector<int> type);

void findFitSize(vector<vector<double> > EofPrior,vector<int> &priorSize, vector<vector<string > > data, vector<int> type);
void findFitValue(vector<vector<double> > &EofPrior,vector<vector<vector<int> > > &GD,vector<int> priorSize, vector<vector<string > > data, vector<int> type);
void updateGD(vector<vector<vector<int> > > &GD,int att,int position);
void setEofPrior(vector<vector<double> > &EofPrior , vector<vector<vector<int> > > GD);



void printGD(vector<vector<vector<int> > > GD,vector<vector<double> > E);
//H表示各類別值的每個特徵的特徵值出現次數，三維
//Hsize表示每個類別值出現的次數，一維
//EofPrior表示先驗樣本的個期望值，特徵和特徵值，二維
//priorSize 表示先驗樣本的大小，一維
//#############################################################################################################
//依據LearningSet 預測TestSet, 回傳正確次數
int Bayesian(vector<vector<string> > trainSet,vector<vector<string> > testSet,vector<vector<double> > &EofPrior,vector<int > &priorSize, vector<int> type){
    int c ;          //類別值欄位
    for(int i=0;i<type.size();i++){
        if(type[i]==2)
            c=i;
    }
    int totalNum=0;   //測試資料總數
    int S=0;          //正確預測
    int F=0;          //錯誤預測
    vector<vector<vector<int> > > H;    //存放個別出現次數
    vector<int> Hsize;                  //存放個類別值出現次數
    //依據學習資料計算 H
    learningToHyposes(trainSet,H,Hsize,type);
    //設定prior 和prior長度
    
    //initialPrior(prior,H);
    //initialPriorSize(priorSize,H);
    
    //printVector(Hsize);
    
    //開始預測
    //myLog.push_back("開始預測");
    //myLog.push_back("==========================");
    for(int i=0;i<testSet.size();i++)
    {
        //myLog.push_back("**************************");
        //myLog.push_back("開始預測第"+int2str(i)+"筆測試資料");
        
        int original=str2int(testSet[i][c]);
        int result;
        result=predict(testSet,i,c,type,H,Hsize,EofPrior,priorSize);
        //myLog.push_back("實際為"+int2str(original));
        if(result==original){
            //myLog.push_back("預測正確");
            S++;}
        else if (result!=original){
            //myLog.push_back("預測錯誤");
            F++;}
        else
            myLog.push_back("出現例外情況,"+int2str(i)+"資料異常");
        totalNum++;
        //myLog.push_back(" ");

    }
    
    //myLog.push_back("==========================");
    //myLog.push_back("總測試資料筆數為："+int2str(totalNum));
    //myLog.push_back("正確預測資料筆數為："+int2str(S));
    //myLog.push_back("錯誤預測資料筆數為："+int2str(F));
    double rate=static_cast<double>(S)/static_cast<double>(totalNum);
    //myLog.push_back("預測正確率為："+dbl2str(rate));
    //cout<<"正確率為"<<rate<<endl;
    if(S>testSet.size())
        cout<<"ERROR"<<endl;
    return S;
}
//將學習資料轉為Hypoesses
void learningToHyposes(vector<vector<string> > trainSet , vector<vector<vector<int> > > &H,vector<int> &Hsize,vector<int> type){
    //取得類別值欄位
    int c;
    for(int i=0;i<type.size();i++){
        if(type[i]==2)
            c=i;
    }
    //判斷最大最小類別值
    int minC=100;
    int maxC=0;
    int numC=0;
    for(int i=0;i<trainSet.size();i++){
        
        if(str2int(trainSet[i][c])>maxC)
        {
            maxC=str2int(trainSet[i][c]);
        }
        else if(str2int(trainSet[i][c])<minC)
        {
            minC=str2int(trainSet[i][c]);

        }
    }
    numC=maxC-minC+1;
    
    //取得最後面的特徵值欄位
    int startAtt=-1;
    int numAtt=0;
    for(int i =0;i<type.size();i++){
        if(type[i]==1)
        {
            numAtt++;
            if(startAtt==-1)
                startAtt=i;
        }
        
    }
    //myLog.push_back("類別值個數為"+ int2str(numC));
    //myLog.push_back("特徵起始欄位為"+ int2str(startAtt));
    //myLog.push_back("特徵值個數為"+ int2str(numAtt));
    
    //比照類別值宣告，如：最大類別值為7，就需要宣告l=8的vector
    H.resize(maxC+1);
    //比照欄位index宣告，如：起始欄位為一，共有九個特徵，宣告l=10的vector
    for(int i=0;i<H.size();i++){
        H[i].resize(startAtt+numAtt);
        
    }
    for(int i=0;i<H.size();i++){
        for(int j=0;j<H[0].size();j++)
        {
            //特徵可能值上限0~9
            H[i][j].resize(10);
        }
    }
    //Hsize()存放個類別值的個數
    Hsize.resize(numC+1);
    //myLog.push_back("H初始化完畢"+int2str(maxC+1)+"by"+int2str(numAtt+startAtt)+"by10");
    //myLog.push_back(" ");
    
    printLog(myLog);
    

    //計算特徵值出現的次數
    for(int i =0;i<trainSet.size();i++)
    {
        for(int j = startAtt;j<numAtt+startAtt;j++)
        {
            //依序為       類別值、     特徵、         特徵值
            H[str2int(trainSet[i][c])][j][str2int(trainSet[i][j])]++;
            
            ////cout<<str2int(trainSet[i][c])<<endl;
        }
        //計算各類別的出現次數
        Hsize[str2int(trainSet[i][c])]++;
    }
    
    //myLog.push_back("H計算完畢");
/*
     //橫是特徵、縱是特徵值出現次數
    for(int i =0;i<8;i++){
        //cout<<"類別值為"+int2str(i)<<endl;
        printBigVector(H[i]);
        //cout<<"=================================="<<endl;}
 */
    
}
//依據 H預測類別值並回傳正確與否,testSet,H,prior,row
int predict(vector<vector<string> > testSet,int row , int c,  vector<int> type,vector<vector<vector<int> > > H,vector<int> Hsize,vector<vector<double> >  EofPrior, vector<int> priorSize){
    
    static double total=0;
    for(int i =0;i<Hsize.size();i++)
        total=total+Hsize[i];
    total=total-1;               //扣除自身，因為leave one out
    //存放個類別的機率值
    vector<double> p;
    
    for(int i =0;i<H.size();i++)//各類別
    {
        double tmp;
        //若測試資料的實際類別和計算類別相同，則需扣除自身
        if(str2int(testSet[row][c])==i)
        {
            tmp=log(((Hsize[i])-1)/total);
            for(int j =0;j<type.size();j++)
            {
                if(type[j]==1)//只有屬性才計算
                {
                    double nj  =(H[i][j][str2int(testSet[row][j])])-1;
                    double N   =(Hsize[i])-1;
                    double p   =EofPrior[j][str2int(testSet[row][j])]*priorSize[j];
                    double k   =priorSize[j];
                    tmp+=log(nj+p);
                    tmp-=log(N+k);
                }
            }
            
        }
        //其他類別則沒有差別
        else {
            tmp=log(Hsize[i]/total);
            for(int j =0;j<type.size();j++)
            {
                if(type[j]==1)//只有屬性才計算
                {
                    double nj  =H[i][j][str2int(testSet[row][j])];
                    double N   =Hsize[i];
                    double p   =EofPrior[j][str2int(testSet[row][j])]*priorSize[j];
                    double k   =priorSize[j];
                    tmp+=log(nj+p);
                    tmp-=log(N+k);
                }
            }
        }
        //myLog.push_back("類別為"+int2str(i)+"的機率取log是"+dbl2str(tmp));
        p.push_back(tmp);
    }
    
    int result = findMaxIndex(p);
    //myLog.push_back("預測為"+int2str(result));
    return result;
}
//排序特徵
void SNB(vector<int> &order,vector<vector<string> > data,vector<int> &type,bool mode){
    //宣告暫存的計算空間,及所需變數
    int attNum=0;                 //特徵總數
    for(int i=0;i<type.size();i++){
        if(type[i]==1)
            attNum++;
    }
    vector<int> col;            //特徵子集
    col.resize(attNum);
    for(int i =0;i<col.size();i++){
        col[i]=-1;}
    vector<bool> isAttJoin;     //表示特徵是否已經加入特徵子集
    isAttJoin.resize(attNum);
    
    vector<vector<double > >  EofPrior;    //分類器參數
    vector<int > priorSize;                     //
    vector<vector<vector<int> > > GD;
    myLog.push_back("#############################################");
    myLog.push_back("開始排序特徵");
    
    vector<int> allAcc;
    //排序特徵
    for(int i =0;i<attNum;i++)
    {
        //找出可使子集增加最佳正確率的特徵子集＆順序
        int att=0;            //表示當前最好的att加入子集
        int max=0;
        for(int j=0;j<type.size();j++)
        {
            if(type[j]==1&&isAttJoin[j]==0)
            {
                vector<vector<string> > tmp;
                
                col[i]=j;
                vector<int> newType;
                copyVector(data,tmp,col,type,newType);
                
                //設定參數
                initialPrior(EofPrior,GD,priorSize,tmp,newType);
                
                
                static int acc=0;
                acc=Bayesian(tmp,tmp,EofPrior,priorSize,newType);
                
                
                if(acc>max)
                {
                    max=acc;
                    att=j;
                }
            }
        }
        //找到當前最佳特徵
        col[i]=att;
        isAttJoin[att]=1;
        myLog.push_back("找到當前最佳特徵"+int2str(att));
        myLog.push_back("正確比數為"+int2str(max));
        allAcc.push_back(max);
            
    }
    myLog.push_back("特徵排序結束");
    order=col;
    int usefulAtt=0;  //用於判斷有幾個特徵有用
    for(int i=1;i<allAcc.size();i++)
    {
        if(allAcc[i]<=allAcc[i-1] && usefulAtt==0)
            usefulAtt=i;
    }
    if(mode==1)
        order.resize(usefulAtt);
    
    myLog.push_back("排序結果為"+v2str(order));
    
    myLog.push_back("#############################################");
    
}
//若prior未初始化，則始之。採laplace
//依據GD內分配設定期望值
//GD內維度依序是特徵、特徵值、alpha&beta(0&1)
void initialPrior(vector<vector<double> > &EofPrior,vector<vector<vector<int> > > &GD,vector<int> &priorSize, vector<vector<string > > data, vector<int> type){
    //宣告向量空間
    //特徵個數
    int attNum=0;
    for(int i=0;i<type.size();i++){
        if(type[i]==1)
            attNum=attNum+1;
    }
    EofPrior.resize((attNum));
    GD.resize((attNum));
    priorSize.resize(attNum);
    //特徵值個數
    for(int i=0;i<type.size();i++){
        
        if(type[i]==1)
        {
            //找到各特徵的最大最小值
            int minValue=100;
            int maxValue=0;
        
            for(int j=0;j<data.size();j++)
            {
                if(str2int(data[j][i])>maxValue)
                {
                    maxValue=str2int(data[j][i]);
                }
                else if(str2int(data[j][i])<minValue)
                {
                    minValue=str2int(data[j][i]);
                }
            }
            //cout<<minValue<<" "<<maxValue<<endl;
            //
            EofPrior[i].resize(maxValue+1);
            GD[i].resize(maxValue+1);
             
        }
    }
    //特徵值的alpha,beta變數
    for(int i=0;i<GD.size();i++){
        for(int j=0;j<GD[i].size();j++)
        {
            GD[i][j].resize(2);
        }
    }
    //********************************
    //採用laplace 設定初始值
    for(int i=0;i<EofPrior.size();i++)
    {
        for(int j=0;j<EofPrior[i].size();j++)
        {
            EofPrior[i][j]=1/static_cast<double>(EofPrior[i].size());
            //cout<<EofPrior[i][j]<<endl;
        }
    }
    for(int i =0;i<priorSize.size();i++)
    {
        priorSize[i]=EofPrior[i].size();
    }
    //GD初始值alpha均為1,beta另行計算
    for(int i=0;i<GD.size();i++)
    {
        for(int j=0;j<GD[i].size();j++)
        {
            GD[i][j][0]=1;
        }
    }
    //GD調整分配中的Beta
    //cout<<GD.size()<<endl;
    for(int i=0;i<GD.size();i++)
    {
        updateGD(GD,i,0);
    }
}

//可動的參數是priorSize
void findFitSize(vector<vector<double> > EofPrior,vector<int> &priorSize, vector<vector<string > > data, vector<int> type){
    
    myLog.push_back("#############################################");
    myLog.push_back("開始計算最佳先驗樣本大小");
    myLog.push_back("原始的priorSize大小依序為:");
    myLog.push_back(v2str(priorSize));
    
    int maxValue=0;
    for(int i=0;i<priorSize.size();i++)
    {
        int size=0;
        
        for(int j=1;j<=50;j++)
        {
            vector<int> S;
            S=priorSize;
            S[i]=S[i]*j;
            
            int tmp =Bayesian(data,data,EofPrior,S,type);
            if(tmp>maxValue)
            {
                maxValue=tmp;
                size=S[i];
            }
        }
        if(size!=0)
            priorSize[i]=size;
        myLog.push_back("當前的正確筆數為"+int2str(maxValue));
    }
    //cout<<endl;
    myLog.push_back("結束priorSize的最佳化，結果為:");
    myLog.push_back(v2str(priorSize));
    myLog.push_back("#############################################");
}
//調整各個特徵的特徵值分布，可動變數是EofPrior和GD
void findFitValue(vector<vector<double> > &EofPrior,vector<vector<vector<int> > > &GD,vector<int> priorSize, vector<vector<string > > data, vector<int> type){
    myLog.push_back("#############################################");
    myLog.push_back("開始各特徵適當的機率分配");
    
    int maxValue=Bayesian(data,data,EofPrior,priorSize,type);
    for(int i=0;i<GD.size();i++)
    {
        for(int j=0;j<GD[i].size();j++)
        {
            int fit=1;
            for(int k=1;k<50;k++){
                GD[i][j][0]=k;
                //updateGD(GD,i,j+1);
                setEofPrior(EofPrior,GD);
                
                int tmp = Bayesian(data,data,EofPrior,priorSize,type);
                if(tmp>maxValue)
                {
                    maxValue=tmp;
                    fit=k;
                    cout<<maxValue<<" !!"<<endl;
                    //myLog.push_back(int2str(i));
                    //myLog.push_back(int2str(j));
                    //myLog.push_back(int2str(k));
                }
                else
                {
                    //cout<<maxValue<<endl;
                }
            }
            GD[i][j][0]=fit;
            //updateGD(GD,i,j+1);
            setEofPrior(EofPrior,GD);
            //printGD(GD);
        }
        
    }
    myLog.push_back("結束各特徵適當的機率分配");
    myLog.push_back("結果為正確筆數為："+int2str(maxValue));
    myLog.push_back("#############################################");
    
    printGD(GD,EofPrior);
    

    
    int a = 0;
    a= Bayesian(data,data,EofPrior,priorSize,type);
    cout<<a<<endl;
}
//att欲調整的特徵prior
//從position開始調整
void updateGD(vector<vector<vector<int> > > &GD,int att,int position){
    
    int k=GD[att].size();
    
    for(int i=position;i<GD[att].size();i++)
    {
        GD[att][i][1]=(k-i-1)*GD[att][i][0];
    }
}
//依據GD分配決定EofPrior
void setEofPrior(vector<vector<double> > &EofPrior , vector<vector<vector<int> > > GD){
    for(int i=0;i<EofPrior.size();i++)
    {
        for(int j=0;j<EofPrior[i].size();j++)
        {
            EofPrior[i][j]=static_cast<double>(GD[i][j][0])/(GD[i][j][0]+GD[i][j][1]);
            for(int k=0;k<j;k++)
            {
                EofPrior[i][j]=EofPrior[i][j]*(static_cast<double>(GD[i][k][1])/(GD[i][k][0]+GD[i][k][1]));
            }
        }
    }
}
void printGD(vector<vector<vector<int> > > GD,vector<vector<double> > E)
{
    cout<<endl<<"************"<<endl;
    for(int i=0;i<GD.size();i++)
    {
        for(int j=0;j<GD[i].size();j++)
        {
            cout<<GD[i][j][0]<<"\t";
        }
        cout<<endl;
        for(int j=0;j<GD[i].size();j++)
        {
            cout<<GD[i][j][1]<<"\t";
        }
        cout<<endl;
        
        for(int j=0;j<E[i].size();j++)
        {
            cout<<E[i][j]<<"\t";
        }
        cout<<endl<<"************"<<endl;
        
    }
}
void printE(vector<vector<double> > E )
{
    for(int i=0;i<E.size();i++)
    {
        for(int j=0;j<E[i].size();j++)
        {
            
        }
    }
}













