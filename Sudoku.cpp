#include <iostream>
#include <vector>
#include <fstream>
using namespace std;
ifstream File_Input;
ofstream File_Output;

struct Sudoku_Data {  //һ�����������
    unsigned char Data;  //����1~9  0��ʾδ���
    bool May[9];  //������
};
struct Try_Point {  //�������ϵĽڵ�
    unsigned char Block_num;  //������Ĺ�����
    unsigned char Point_num; //��ǰ���ڱ������item���
    vector <unsigned char> item;  //�ڵ��е����м�����
};

struct Sudoku_Data Base[81];  //ȫ��81������
struct Sudoku_Data Backup[81];  //���м����ʱ���ݴ�ʹ��
unsigned char Addr_Kind[3][9][9];  //���й���Ӧ��λ�ñ�
unsigned char Addr_Block[81][3];  //�����������й��ı��
vector <struct Try_Point> Try;  //������
struct Try_Point Empty_Point;  //�յļ���ڵ�
bool display;  //����Solve_Outputʱ�Ƿ����

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
void Data_Input() {  //������ģʽ��������������
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
    } else {  //����
        goto Next;
    }
}
void Solve_Output() {  //������ģʽ����ʾ������
    if (display == true) {  //ȷ���Ƿ������ģʽ
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
    File_Output<<dat<<endl;  //��������浽�ļ���
}
unsigned int Calculate() {  //�����㺯��
    unsigned char i;
    int Solve_num = 0;
    File_Output.open("Sudoku_Output.txt");
    Engine();  //�����ų�����
    if (Check_Error() == true) {  //�������������
        File_Output.close();
        return 0;
    }
    if (Check_Compete() == true) {  //��������������
        Solve_Output();
        File_Output.close();
        return 1;
    }
    for (i = 0; i < 81; i++) {Backup[i] = Base[i];}  //�������� ����ʱ�ݴ�
    Create_New_Point();  //�������������ڵ�
    while (1 == 1) {
        for (i = 0; i < Try.size(); i++) {  //װ�ؼ�����
            Base[Try[i].Block_num].Data = Try[i].item[Try[i].Point_num];
        }
        Engine();  //�����ų�����
        if (Check_Error() == 1) {  //��ǰ�������
            if(Try_Next() == false) {break;}  //û����һ�����裬��������޽⣬�˳�
        } else {  //��ǰ���������ȷ
            if (Check_Compete() == false) {  //����δ��ɣ��򴴽��µļ���ڵ�
                Create_New_Point();
            } else {  //�������
                Solve_Output();
                Solve_num++;  //����Ŀ+1
                if (Try_Next() == false) {break;}  //û����һ�����裬�����ɣ��˳�
            }
        }
    }
    File_Output.close();
    return Solve_num;
}
bool Try_Next() {  //��ǰ�������װ����һ������
    unsigned char i;
    while (Try[Try.size()-1].Point_num >= (Try[Try.size()-1].item.size()-1)) {
        if (Try.size() == 1) {return false;}
        Try.resize(Try.size()-1);
    }
    Try[Try.size()-1].Point_num++;  //������ĩ�ڵ�ָ����һ��item
    for (i = 0; i < 81; i++) {Base[i] = Backup[i];}  //����װ�ر�������
    return true;
}
void Create_New_Point() {
    Try.push_back(Empty_Point);
    if (Try.size() == 1) {  //��Ϊ���ڵ�
        Init_Point(Next_Empty_Block(0),0);  //�ӵ�һ��ʼ����δ��ɹ���
    } else {  //�����Ǹ��ڵ�
        Init_Point(Next_Empty_Block(Try[Try.size()-2].Block_num+1),Try.size()-1);  //����һ�ڵ�ָ��Ĺ���ʼ����δ��ɹ���
    }
}
void Init_Point(unsigned char Block_num,unsigned char Point_num) {  //��ʼ���������ڵ�  Block_num->�����蹬��ı��  Point_num->�ڵ�ı��
    unsigned char i;
    Try[Point_num].Block_num = Block_num;
    for (i = 0; i < 9; i++) {
        if (Base[Try[Point_num].Block_num].May[i] == true) {  //����Ŀ�깬������м����������뵽�ýڵ��item��
            Try[Point_num].item.push_back(i + 1);
        }
    }
    Try[Point_num].Point_num = 0;  //ָ��item�еĵ�һ��������
}
unsigned char Next_Empty_Block(unsigned char Start) {  //�ҵ���һ��δȷ���𰸵Ĺ��񲢷�������
    unsigned char i;
    for (i = Start; i < 81; i++) {
        if (Base[i].Data == 0) {return i;}
    }
    return 0;  //û��δȷ������
}
bool Check_Error() {  //��������Ƿ���ڴ���
    unsigned char kind, num, add, item;
    for (kind = 0; kind < 3; kind++) {  //�ֱ�ɨ�����й�
        for (num = 0; num < 9; num++) {
            for (item = 1; item <= 9; item++) {
                add = 0;
                for (unsigned char k = 0; k < 9; k++) {
                    if (Base[Addr_Kind[kind][num][k]].Data == item) {add++;}
                }
                if (add >= 2) {return true;}  //��һ�����й��д���������ͬ���� -> �����˳�
            }
        }
    }
    for (num = 0; num < 81; num++) {  //ɨ��ȫ������
        if (Base[num].Data == 0) {  //��δ���
            add = 0;
            for (unsigned char k = 0; k < 9; k++) {  //���������м�����
                if (Base[num].May[k] == true) {add++;}
            }
            if (add == 0) {return true;}  //û�п��ܵ��� -> �����˳�
        }
    }
    return false;  //��ʱδ���ִ���
}
bool Check_Compete() {  //�ж������Ƿ����
    unsigned char i;
    for (i = 0; i < 81; i++) {
        if (Base[i].Data == 0) {return false;}
    }
    return true;
}
void Engine() {  //ʹ���ų�����ȥ�����ܵ���
    unsigned char kind, num, item, add, dat;
    bool Could_Solve;
Again:;
    Analyse();  //ÿ���ų�ǰӦ����ȥ������
    Could_Solve = false;
    for (kind = 0; kind < 3; kind++) {  //�ֱ�ɨ�����й�
        for (num = 0; num < 9; num++) {
            for (item = 0; item < 9; item++) {
                add = 0;
                for(unsigned char k = 0; k < 9; k++) {  //��¼һ�����й��еĿ�����
                    if ((Base[Addr_Kind[kind][num][k]].Data == 0) && (Base[Addr_Kind[kind][num][k]].May[item] == true)) {
                        add++;
                        dat = k;
                    }
                }
                if (add == 1) {  //������Ψһ����������ù����ȷ��
                    Base[Addr_Kind[kind][num][dat]].Data = item + 1;
                    Could_Solve = true;
                }
            }
        }
        if (Could_Solve == true) {goto Again;}  //һֱѭ��ֱ��û���ų�����
    }
}
void Analyse() {  //��ȥ������
    unsigned char num, kind, item;
    for (num = 0; num < 81; num++) {  //�������й���
        if (Base[num].Data != 0) {  //���ù��������
            for (kind = 0; kind < 3; kind++) {  //�ֱ�ɨ�����й�
                for (item = 0; item < 9; item++) {
                    Base[Addr_Kind[kind][Addr_Block[num][kind]][item]].May[Base[num].Data - 1] = false;  //��ȥͬ��ͬ��ͬ���ļ�����
                }
            }
        }
    }
}
void Init() {  //��ʼ������
    unsigned char i, j, x, y;
    for (i = 0; i < 9; i++) {  //��ʼ�������е�λ�ñ�
        for (j = 0; j < 9; j++) {
            Addr_Kind[0][i][j] = i * 9 + j;
            Addr_Kind[1][i][j] = j * 9 + i;
        }
    }
    for (x = 0; x < 3; x++) {  //��ʼ���Ź����λ�ñ�
        for (y = 0; y < 3; y++) {
            for (i = 0; i < 3; i++) {
                for (j = 0; j < 3; j++) {
                    Addr_Kind[2][y*3 + x][j*3 + i] = (y*3 + j) * 9 + (x*3 + i);
                }
            }
        }
    }
    for (i = 0; i < 3; i++) {  //��ʼ�������������й��ı��
        for (x = 0; x < 9; x++) {
            for (y = 0; y < 9; y++) {
                Addr_Block[Addr_Kind[i][x][y]][i] = x;
            }
        }
    }
    for (i = 0; i < 81; i++) {  //������м�����
        for (j = 0; j < 9; j++) {
            Base[i].May[j] = true;
        }
    }
    for (i = 0; i < 81; i++) {Base[i].Data = 0;}  //�����������
}
