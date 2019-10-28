#include <iostream>
#include <vector>
#include <fstream>
using namespace std;
ifstream File_Input;
ofstream File_Output;

struct Sudoku_Data {  //一个宫格的内容
    unsigned char Data;  //数字1~9  0表示未完成
    bool May[9];  //假设数
};
struct Try_Point {  //假设链上的节点
    unsigned char Block_num;  //被假设的宫格编号
    unsigned char Point_num; //当前正在被假设的item编号
    vector <unsigned char> item;  //节点中的所有假设数
};

struct Sudoku_Data Base[81];  //全部81个宫格
struct Sudoku_Data Backup[81];  //进行假设的时候暂存使用
unsigned char Addr_Kind[3][9][9];  //行列宫对应的位置表
unsigned char Addr_Block[81][3];  //宫格所在行列宫的编号
vector <struct Try_Point> Try;  //假设链
struct Try_Point Empty_Point;  //空的假设节点
bool display;  //决定Solve_Output时是否输出

void Init();
void Analyse();
void Engine();
bool Check_Compete();
bool Check_Error();
void Create_New_Point();
void Solve_Output();
bool Try_Next();
void Data_Input();
unsigned int Calculate();
unsigned char Next_Empty_Block(unsigned char Start);
void Init_Point(unsigned char Block_num,unsigned char Point_num);

int main() {
    unsigned int Solve_num, dat;
    Init();
    cout<<"Welcome to Sudoku-Calculator by Dnomd343"<<endl<<endl;
Reinput:;
    Data_Input();
    cout<<"The Input Data:"<<endl<<endl;
    for (int i = 0; i < 81; i++) {
        if (Base[i].Data == 0) {
            cout<<"- ";
        } else {
            cout<<int(Base[i].Data)<<" ";
        }
        if (i % 3 == 2) {cout<<" ";}
        if (i % 9 == 8) {cout<<endl;}
        if (i % 27 == 26) {cout<<endl;}
    }
    cout<<endl;
    cout<<"0. Go Back and Edit the Data"<<endl;
    cout<<"1. Just Search all the Cases"<<endl;
    cout<<"2. Show all Solutions One by One"<<endl;
    cout<<endl<<"Select a mode and then Continue : ";
    cin>>dat;
    cin.get();
    if (dat == 1) {
        display=false;
    } else if (dat == 2) {
        display=true;
    } else {
        goto Reinput;
    }
    cout<<"Please Wait..."<<endl<<endl;
    Solve_num = Calculate();
    if (Solve_num == 0) {
        cout<<"Error or No solve!!!"<<endl;
    } else {
        cout<<"Found "<<Solve_num<<" Solution"<<endl;
    }
    cout<<"Press ENTER to Exit...";
    cin.get();
    return 0;
}
void Data_Input() {  //命令行模式下输入数独内容
    unsigned int num = 0;
    char dat;
Next:;
    cout<<endl<<endl<<endl<<endl<<endl;
    cout<<"You can Input the Command and then Press the ENTER"<<endl;
    cout<<"'+' : Next Block    '-' : Last Block"<<endl;
    cout<<"'*' : Next Line     '/' : Last Line"<<endl;
    cout<<"'=' : Compete       '0' : Empty Block"<<endl;
    cout<<"'1' to '9' : The number"<<endl<<endl;
    for (int i = 0; i < 81; i++) {
        if (i == num) {
            cout<<"_"<<" ";
        } else {
            if (Base[i].Data == 48) {
                cout<<"?"<<" ";
            } else {
                cout<<int(Base[i].Data)<<" ";
            }
        }
        if (i % 3 == 2) {cout<<" ";}
        if (i % 9 == 8) {cout<<endl;}
        if (i % 27 == 26) {cout<<endl;}
    }
    cin>>dat;
    if (dat == 43) {  // '+'
        if (num <= 79) {num++;}
        goto Next;
    } else if (dat == 45) { // '-'
        if (num >= 1) {num--;}
        goto Next;
    } else if (dat == 42) { // '*'
        if (num <= 71) {num = num + 9;}
        goto Next;
    } else if (dat == 47) { // '/'
        if (num >= 9) {num = num - 9;}
        goto Next;
    } else if (dat == 61) {// '='
    } else if ((dat >= 48) && (dat <= 57)) {  // 0~9
        Base[num].Data = int(dat - 48);
        if (num <= 79) {num++;}
        goto Next;
    } else {  //其他
        goto Next;
    }
}
void Solve_Output() {  //命令行模式下显示数独答案
    if (display == true) {  //确认是否处于输出模式
        cout<<endl;
        for (int i = 0; i < 81; i++) {
            cout<<int(Base[i].Data)<<" ";
            if (i % 3 == 2) {cout<<" ";}
            if (i % 9 == 8) {cout<<endl;}
            if (i % 27 == 26) {cout<<endl;}
        }
        cout<<"Press ENTER to Show Next Solution...";
        cin.get();
        cout<<endl;
    }
    char dat[82];
    for (int i = 0; i < 81; i++) {dat[i] = Base[i].Data + 48;}
    File_Output<<dat<<endl;  //将结果保存到文件中
}
unsigned int Calculate() {  //主计算函数
    unsigned char i;
    int Solve_num = 0;
    File_Output.open("Sudoku_Output.txt");
    Engine();  //初步排除运算
    if (Check_Error() == true) {  //输入的数独有误
        File_Output.close();
        return 0;
    }
    if (Check_Compete() == true) {  //输入的数独已完成
        Solve_Output();
        File_Output.close();
        return 1;
    }
    for (i = 0; i < 81; i++) {Backup[i] = Base[i];}  //备份数据 假设时暂存
    Create_New_Point();  //创建假设链根节点
    while (1 == 1) {
        for (i = 0; i < Try.size(); i++) {  //装载假设链
            Base[Try[i].Block_num].Data = Try[i].item[Try[i].Point_num];
        }
        Engine();  //进行排除运算
        if (Check_Error() == 1) {  //当前假设错误
            if(Try_Next() == false) {break;}  //没有下一个假设，则该数独无解，退出
        } else {  //当前假设可能正确
            if (Check_Compete() == false) {  //若仍未完成，则创建新的假设节点
                Create_New_Point();
            } else {  //若已完成
                Solve_Output();
                Solve_num++;  //答案数目+1
                if (Try_Next() == false) {break;}  //没有下一个假设，求解完成，退出
            }
        }
    }
    File_Output.close();
    return Solve_num;
}
bool Try_Next() {  //当前假设错误，装载下一个假设
    unsigned char i;
    while (Try[Try.size()-1].Point_num >= (Try[Try.size()-1].item.size()-1)) {
        if (Try.size() == 1) {return false;}
        Try.resize(Try.size()-1);
    }
    Try[Try.size()-1].Point_num++;  //假设链末节点指向下一个item
    for (i = 0; i < 81; i++) {Base[i] = Backup[i];}  //重新装载备份数据
    return true;
}
void Create_New_Point() {
    Try.push_back(Empty_Point);
    if (Try.size() == 1) {  //若为根节点
        Init_Point(Next_Empty_Block(0),0);  //从第一格开始搜索未完成宫格
    } else {  //若不是根节点
        Init_Point(Next_Empty_Block(Try[Try.size()-2].Block_num+1),Try.size()-1);  //从上一节点指向的宫格开始搜索未完成宫格
    }
}
void Init_Point(unsigned char Block_num,unsigned char Point_num) {  //初始化假设链节点  Block_num->被假设宫格的编号  Point_num->节点的编号
    unsigned char i;
    Try[Point_num].Block_num = Block_num;
    for (i = 0; i < 9; i++) {
        if (Base[Try[Point_num].Block_num].May[i] == true) {  //遍历目标宫格的所有假设数并加入到该节点的item中
            Try[Point_num].item.push_back(i + 1);
        }
    }
    Try[Point_num].Point_num = 0;  //指向item中的第一个假设数
}
unsigned char Next_Empty_Block(unsigned char Start) {  //找到下一个未确定答案的宫格并返回其编号
    unsigned char i;
    for (i = Start; i < 81; i++) {
        if (Base[i].Data == 0) {return i;}
    }
    return 0;  //没有未确定宫格
}
bool Check_Error() {  //检查数独是否存在错误
    unsigned char kind, num, add, item;
    for (kind = 0; kind < 3; kind++) {  //分别扫描行列宫
        for (num = 0; num < 9; num++) {
            for (item = 1; item <= 9; item++) {
                add = 0;
                for (unsigned char k = 0; k < 9; k++) {
                    if (Base[Addr_Kind[kind][num][k]].Data == item) {add++;}
                }
                if (add >= 2) {return true;}  //若一组行列宫中存在两个相同的数 -> 错误退出
            }
        }
    }
    for (num = 0; num < 81; num++) {  //扫描全部宫格
        if (Base[num].Data == 0) {  //若未完成
            add = 0;
            for (unsigned char k = 0; k < 9; k++) {  //遍历其所有假设数
                if (Base[num].May[k] == true) {add++;}
            }
            if (add == 0) {return true;}  //没有可能的数 -> 错误退出
        }
    }
    return false;  //暂时未发现错误
}
bool Check_Compete() {  //判断数独是否完成
    unsigned char i;
    for (i = 0; i < 81; i++) {
        if (Base[i].Data == 0) {return false;}
    }
    return true;
}
void Engine() {  //使用排除法消去不可能的数
    unsigned char kind, num, item, add, dat;
    bool Could_Solve;
Again:;
    Analyse();  //每次排除前应先消去假设数
    Could_Solve = false;
    for (kind = 0; kind < 3; kind++) {  //分别扫描行列宫
        for (num = 0; num < 9; num++) {
            for (item = 0; item < 9; item++) {
                add = 0;
                for(unsigned char k = 0; k < 9; k++) {  //记录一组行列宫中的可能数
                    if ((Base[Addr_Kind[kind][num][k]].Data == 0) && (Base[Addr_Kind[kind][num][k]].May[item] == true)) {
                        add++;
                        dat = k;
                    }
                }
                if (add == 1) {  //若仅有唯一可能数，则该宫格答案确定
                    Base[Addr_Kind[kind][num][dat]].Data = item + 1;
                    Could_Solve = true;
                }
            }
        }
        if (Could_Solve == true) {goto Again;}  //一直循环直到没有排除对象
    }
}
void Analyse() {  //消去假设数
    unsigned char num, kind, item;
    for (num = 0; num < 81; num++) {  //遍历所有宫格
        if (Base[num].Data != 0) {  //若该宫格已完成
            for (kind = 0; kind < 3; kind++) {  //分别扫描行列宫
                for (item = 0; item < 9; item++) {
                    Base[Addr_Kind[kind][Addr_Block[num][kind]][item]].May[Base[num].Data - 1] = false;  //消去同行同列同宫的假设数
                }
            }
        }
    }
}
void Init() {  //初始化容器
    unsigned char i, j, x, y;
    for (i = 0; i < 9; i++) {  //初始化行与列的位置表
        for (j = 0; j < 9; j++) {
            Addr_Kind[0][i][j] = i * 9 + j;
            Addr_Kind[1][i][j] = j * 9 + i;
        }
    }
    for (x = 0; x < 3; x++) {  //初始化九宫格的位置表
        for (y = 0; y < 3; y++) {
            for (i = 0; i < 3; i++) {
                for (j = 0; j < 3; j++) {
                    Addr_Kind[2][y*3 + x][j*3 + i] = (y*3 + j) * 9 + (x*3 + i);
                }
            }
        }
    }
    for (i = 0; i < 3; i++) {  //初始化宫格所在行列宫的编号
        for (x = 0; x < 9; x++) {
            for (y = 0; y < 9; y++) {
                Addr_Block[Addr_Kind[i][x][y]][i] = x;
            }
        }
    }
    for (i = 0; i < 81; i++) {  //清空所有假设数
        for (j = 0; j < 9; j++) {
            Base[i].May[j] = true;
        }
    }
    for (i = 0; i < 81; i++) {Base[i].Data = 0;}  //清空所有数字
}
