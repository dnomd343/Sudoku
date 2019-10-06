#include<iostream>
#include<vector>
#include<fstream>
using namespace std;
ifstream File_Input;
ofstream File_Output;

struct Sudoku_Class{  //一个宫格的内容
    unsigned char Data;  //数字1~9，0表示未完成
    bool May[9];  //假设数
};
struct Try_Point_Class{  //假设链上的节点
    unsigned char Block_Num;  //被假设的宫格编号
    unsigned char Point_Num;  //当前正在被假设的Item编号
    vector <unsigned char> Item;  //节点中的所有假设数
};

struct Sudoku_Class Base[81];  //全部81个宫格
struct Sudoku_Class Backup[81];  //进行假设的时候暂存使用
unsigned char Addr_Kind[3][9][9];  //行列宫对应的位置表
unsigned char Addr_Block[81][3];  //宫格所在行列宫的编号
vector <struct Try_Point_Class> Try;  //假设链
struct Try_Point_Class Empty_Point;  //空的假设节点

void Init();
void Analyse();
void Engine();
bool Check_Compete();
bool Check_Error();
void Create_New_Point();
void Solve_Output();
bool Try_Next();
bool Data_Input();
unsigned int Calculate();
unsigned char Next_Empty_Block(unsigned char Start);
void Init_Point(unsigned char Block_Num,unsigned char Point_Num);

int main(){
    cout<<"Welcome to Sudoku-Calculator by Dnomd343"<<endl;
    cout<<"Please wait..."<<endl;
    Init();
    if(Data_Input()==false){cout<<"Something Wrong!"<<endl;return 0;}
    unsigned int i;
    i=Calculate();
    cout<<"Calculate Compete"<<endl;
    File_Output.open("Compete");
    if(i==0){File_Output<<"Error!!!"<<endl;}
    File_Output.close();
    return 0;
}
bool Data_Input(){
    unsigned char i;
    char dat[81];
    File_Input.open("Data_Input.txt");
    File_Input>>dat;
    File_Input.close();
    for(i=0;i<=80;i++){
        Base[i].Data=dat[i]-48;
        if(Base[i].Data>9){return false;}
    }
    return true;
}
void Solve_Output(){
    unsigned char i;
    char Data[82];
    for(i=0;i<=80;i++){
        Data[i]=Base[i].Data+48;
    }
    Data[81]=0;
    File_Output<<Data<<endl;
}
unsigned int Calculate(){  //主计算函数
    unsigned char i;
    int Solve_Num=0;
    File_Output.open("Data_Output.txt");
    Engine();  //初步排除运算
    if(Check_Error()==true){File_Output.close();return 0;}  //输入的数独有误
    if(Check_Compete()==true){Solve_Output();File_Output.close();return 1;}  //输入的数独已完成
    for(i=0;i<=80;i++){Backup[i]=Base[i];}  //备份数据 假设时暂存
    Create_New_Point();  //创建假设链根节点
    while(1){
        for(i=0;i<=(Try.size()-1);i++){  //装载假设链
            Base[Try[i].Block_Num].Data=Try[i].Item[Try[i].Point_Num];
        }
        Engine();  //进行排除运算
        if(Check_Error()==true){  //当前假设错误
            if(Try_Next()==false){break;}  //没有下一个假设，则该数独无解，退出
        }
        else{  //当前假设可能正确
            if(Check_Compete()==false){  //若仍未完成，则创建新的假设节点
                Create_New_Point();
            }
            else{  //已完成
                Solve_Output();
                Solve_Num++;  //答案数目+1
                if(Try_Next()==false){break;}  //没有下一个假设，求解完成，退出
            }
        }
    }
    File_Output.close();
    return Solve_Num;
}
bool Try_Next(){  //当前假设错误，装载下一个假设
    unsigned char i;
    while((Try[Try.size()-1].Point_Num>=(Try[Try.size()-1].Item.size()-1))){
        if(Try.size()==1){return false;}
        Try.resize(Try.size()-1);
    }
    Try[Try.size()-1].Point_Num++;  //假设链末节点指向下一个Item
    for(i=0;i<=80;i++){  //将备份数据重新装载
        Base[i]=Backup[i];
    }
    return true;  //成功
}
void Create_New_Point(){  //创建新的假设链节点
    Try.push_back(Empty_Point);
    if(Try.size()==1){  //若为根节点
        Init_Point(Next_Empty_Block(0),0);  //从第一格开始搜索未完成宫格
    }
    else{  //若不是根节点
        Init_Point(Next_Empty_Block(Try[Try.size()-2].Block_Num+1),Try.size()-1);  //从上一节点指向的宫格开始搜索未完成宫格
    }
}
void Init_Point(unsigned char Block_Num,unsigned char Point_Num){  //初始化假设链节点  Block_Num->被假设宫格的编号  Point_Num->节点的编号
    unsigned char i;
    Try[Point_Num].Block_Num=Block_Num;
    for(i=0;i<=8;i++){
        if(Base[Try[Point_Num].Block_Num].May[i]==true){  //遍历目标宫格的所有假设数并加入到该节点的Item中
            Try[Point_Num].Item.push_back(i+1);
        }
    }
    Try[Point_Num].Point_Num=0;  //指向Item中的第一个假设数
}
unsigned char Next_Empty_Block(unsigned char Start){  //找到下一个未确定答案的宫格并返回其编号
    unsigned char i;
    for(i=Start;i<=80;i++){
        if(Base[i].Data==0){return i;}
    }
    return 0;  //没有未确定宫格
}
bool Check_Error(){  //检查数独是否存在错误
    unsigned char kind,num,add,item;
    for(kind=0;kind<=2;kind++){  //分别扫描行列宫
        for(num=0;num<=8;num++){
            for(item=1;item<=9;item++){
                add=0;
                for(unsigned char k=0;k<=8;k++){
                    if(Base[Addr_Kind[kind][num][k]].Data==item){add++;}
                }
                if(add>=2){return true;}  //若一组行列宫中存在两个相同的数 -> 错误退出
            }
        }
    }
    for(num=0;num<=80;num++){  //扫描全部宫格
        if(Base[num].Data==0){  //若未完成
            add=0;
            for(unsigned char k=0;k<=8;k++){  //遍历其所有假设数
                if(Base[num].May[k]==true){add++;}
            }
            if(add==0){return true;}  //没有可能的数 -> 错误退出
        }
    }
    return false;  //暂时未发现错误
}
bool Check_Compete(){  //判断数独是否完成
    unsigned char i;
    for(i=0;i<=80;i++){
        if(Base[i].Data==0){return false;}
    }
    return true;
}
void Engine(){  //使用排除法
    unsigned char kind,num,item,add,dat;
    bool Could_Solve;
    Again:;
    Analyse();  //每次排除前应先消去假设数
    Could_Solve=false;
    for(kind=0;kind<=2;kind++){  //分别扫描行列宫
        for(num=0;num<=8;num++){
            for(item=0;item<=8;item++){
                add=0;
                for(unsigned char k=0;k<=8;k++){
                    if((Base[Addr_Kind[kind][num][k]].Data==0)&&(Base[Addr_Kind[kind][num][k]].May[item]==true)){add++;dat=k;}  //记录一组行列宫中的可能数
                }
                if(add==1){Base[Addr_Kind[kind][num][dat]].Data=item+1;Could_Solve=true;}  //若仅有唯一可能数，则该宫格答案确定
            }
        }
        if(Could_Solve==true){goto Again;}  //一直循环直到没有排除对象
    }
}
void Analyse(){  //消去假设数
    unsigned char num,kind,item;
    for(num=0;num<=80;num++){  //遍历所有宫格
        if(Base[num].Data!=0){  //若该宫格已完成
            for(kind=0;kind<=2;kind++){  //分别对行列宫操作
                for(item=0;item<=8;item++){
                    Base[Addr_Kind[kind][Addr_Block[num][kind]][item]].May[Base[num].Data-1]=false;  //消去同行同列同宫的假设数
                }
            }
        }
    }
}
void Init(){  //初始化容器
    unsigned char i,j,x,y;
    for(i=0;i<=8;i++){  //初始化行与列的位置表
        for(j=0;j<=8;j++){
            Addr_Kind[0][i][j]=i*9+j;
            Addr_Kind[1][i][j]=j*9+i;
        }
    }
    for(x=0;x<=2;x++){  //初始化九宫格的位置表
        for(y=0;y<=2;y++){
            for(i=0;i<=2;i++){
                for(j=0;j<=2;j++){
                    Addr_Kind[2][y*3+x][j*3+i]=(y*3+j)*9+(x*3+i);
                }
            }
        }
    }
    for(i=0;i<=2;i++){  //初始化宫格所在行列宫的编号
        for(x=0;x<=8;x++){
            for(y=0;y<=8;y++){
                Addr_Block[Addr_Kind[i][x][y]][i]=x;
            }
        }
    }
    for(i=0;i<=80;i++){  //清空所有假设数
        for(j=0;j<=8;j++){
            Base[i].May[j]=true;
        }
    }
    for(i=0;i<=80;i++){  //清空所有数字
        Base[i].Data=0;
    }
}
