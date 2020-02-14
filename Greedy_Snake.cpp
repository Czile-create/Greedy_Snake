#include <iostream>
#include <queue>
#include <vector>
#include <chrono>
#include <thread>
#include <queue>
#include <string>
#include <conio.h>
#include <time.h>
#include <stdlib.h>
#include <Windows.h>
#include <stdio.h>
#include <mutex>

#define up 72
#define down 80
#define left 75
#define right 77
#define random(x) rand()%x

using namespace std;

/*令光标停留在x,y*/
void gotoxy(HANDLE hOut1, int x, int y)
{
    COORD pos;
    pos.X = x;
    pos.Y = y;
    SetConsoleCursorPosition(hOut1, pos);
}
HANDLE hOut1 = GetStdHandle(STD_OUTPUT_HANDLE);

class point {
    public:
        int x,y;
};

/*设置*/
class set {
    public:
        int n,m; //size
        int speed;
        set()
        {
            n=30,m=58,speed=100;
            ExistLongestTime=0;
            LargestScore=0;
            LongestSnake=0;
            NearestTime=NearestScore=NearestSnake=0;
            Hitwall=0;
            TimeLimitedPoint=0;
            TimeLimitedPointUpdateGap=10000;
        }
        void readSetting(); //从文件中读取设置
        void changeSetting();   //更改设置
        void showSetting(); //显示设置
        void saveSetting(); //将保存保存到文件中
        void showHonor();   //显示荣誉
        double ExistLongestTime;    //存活最长时间
        double LargestScore;        //最高分
        unsigned int LongestSnake;  //蛇最大长度
        int Verify;                 //验证码(判断设置文件是否被修改)
        double NearestTime,NearestScore;    //最近一次的时间与分数
        int NearestSnake;           //最近一次的蛇长
        int Hitwall;               //允许撞墙
        int TimeLimitedPoint;       //限时的点
        int TimeLimitedPointUpdateGap;      //限时点更新间隔
} Setting;

void set::readSetting()
{
    FILE * fp=fopen("Greedy_Snake_v3.set","r");
    if (fp!=NULL) {
        fscanf(fp,"%d%d%d%lf%lf%d%d%lf%lf%d%d%d%d",&n,&m,&speed,&ExistLongestTime,&LargestScore,&LongestSnake,&Verify,&NearestTime,&NearestScore,&NearestSnake,&Hitwall,&TimeLimitedPoint,&TimeLimitedPointUpdateGap);
        if (!(int(n+m+speed+ExistLongestTime+LargestScore+LongestSnake+NearestTime+NearestScore+NearestSnake+Hitwall+TimeLimitedPoint+TimeLimitedPointUpdateGap)%127==Verify))
            *this=set();
    }
    fclose(fp);
}

void set::showHonor()
{
    cout<<"Here are your honors: \n>\tExist Longest Time: "<<ExistLongestTime<<"s";
    cout<<"\n>\tLargest Score: "<<LargestScore;
    cout<<"\n>\tLongest Snake: "<<LongestSnake;
    cout<<"\n\n>\tLast Time: "<<NearestTime;
    cout<<"\n>\tLast Score: "<<NearestScore;
    cout<<"\n>\tLast length of Snake: "<<NearestSnake<<"\n\n>";
}

void set::saveSetting()
{
    Verify=int(n+m+speed+ExistLongestTime+LargestScore+LongestSnake+NearestTime+NearestScore+NearestSnake+Hitwall+TimeLimitedPoint+TimeLimitedPointUpdateGap)%127;
    FILE * fp=fopen("Greedy_Snake_v3.set","w+");
    fprintf(fp, "%d %d %d %lf %lf %d %d %lf %lf %d %d %d %d",n,m,speed,ExistLongestTime,LargestScore,LongestSnake,Verify,NearestTime,NearestScore,NearestSnake,Hitwall,TimeLimitedPoint,TimeLimitedPointUpdateGap);
    fclose(fp);
}

void set::showSetting()
{
    cout<<"Here are the Setting: \n";
    cout<<">\tNumber of row: "<<n<<"\n>\tNumber of column: "<<m<<'\n';
    cout<<">\tUpdate Gap: "<<speed<<"ms\n";
    cout<<">\tAllow hitting the wall: ";
    if (Hitwall)
        cout<<"True\n";
    else
        cout<<"False\n";
    cout<<">\tAllow Time Limited Food: ";
    if (TimeLimitedPoint)
        cout<<"True\n";
    else
        cout<<"False\n\n>";
    if (TimeLimitedPoint)
        cout<<">\tFood Update Gap: "<<TimeLimitedPointUpdateGap<<"s\n\n>";
}

void set::changeSetting()
{
    cout<<"Please enter 'row', 'column', 'UpdateGap', 'AllowHitWall',\n'AllowTimeLimitedFood', 'FoodUpdateGap'\n or 'save'to continue..\n\n>";
    string s;
    int temp;
sign1:
    s="";
    cin>>s;
    if (s=="row") {
        do {
            cout<<"Please enter new size of row, it must be a number from 10 to 38..\n\n>";
            cin>>temp;
            if (temp>=10&&temp<=38)
                n=temp;
        } while (temp<10 || temp>38);
        cout<<"\n>";
        goto sign1;
    }
    else if (s=="column") {
        do {
            cout<<"Please enter new size of column, it must be a number from 10 to 58..\n\n>";
            cin>>temp;
            if (temp>=10&&temp<=58)
                m=temp;
        } while (temp<10 || temp>58);
        cout<<"\n>";
        goto sign1;
    }
    else if (s=="Update_Gap") {
        do {
            cout<<"Please enter new Update Gap (ms) , it must be a number from 50 to 2000..\n\n>";
            cin>>temp;
            if (temp>=50&&temp<=2000)
                speed=temp;
        } while (temp<50 || temp>2000);
        cout<<"\n>";
        goto sign1;
    }
    else if (s=="AllowHitWall") {
        string t;
        do {
            cout<<"Please enter 'True' or 'False' to continue...\n\n>";
            cin>>t;
            if (t=="True")
                Hitwall=1;
            else if (t=="False")
                Hitwall=0;
        } while (t!="True"&&t!="False");
        cout<<"\n>";
        goto sign1;
    }
    else if (s=="AllowTimeLimitedFood") {
        string t;
        do {
            cout<<"Please enter 'True' or 'False' to continue...\n\n>";
            cin>>t;
            if (t=="True")
                TimeLimitedPoint=1;
            else if (t=="False")
                TimeLimitedPoint=0;
        } while (t!="True"&&t!="False");
        cout<<"\n>";
        goto sign1;
    }
    else if (TimeLimitedPoint&&s=="FoodUpdateGap") {
        do {
            cout<<"Please enter food update gap (ms), it must be a number from 5000 to 20000..\n\n>";
            cin>>temp;
            if (temp>=5000&&temp<=20000)
                TimeLimitedPointUpdateGap=temp;
        } while (temp<5000 || temp>20000);
        cout<<"\n>";
        goto sign1;
    }
    else if (s=="save")
        return;
    else {
        cout<<"No such command! Please try again! \n\n>";
        goto sign1;
    }
}

class map {
    public:
        void init(set);     //根据设置内容初始化
        void updateFood();  //更新食物坐标
        bool move(char);    //按照方向移动一步,返回蛇是否死亡
        bool check();       //判断是否死亡，更新live值
        void print();       //初始输出
        void printSth(int x,int y,char temp);   //在(x,y)坐标输出temp
        void removeSth(int x,int y);   //在(x,y)坐标输出‘ ’ 注意：此坐标经过转置
        void printFailed(double);       //输出死亡信息
        queue<point> snake; //蛇的坐标
        mutex mtx;          //互斥锁
        bool live;          //存活标志
    private:
        int n,m; //size
        char s[60][60];     //地图
        point food;         //食物坐标
} m;

void map::init(set Setting)
{
    CONSOLE_CURSOR_INFO cursor_info = {1, 0};
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);    //隐藏光标
    n=Setting.n;
    m=Setting.m;
    while (!snake.empty())
        snake.pop();        //清空蛇身
    for (int i=0; i<n; i++)
        for (int j=0; j<m; j++)
            s[i][j]=' ';    //清空地图
    srand(time(0));         //确定随机种子
    point start;            //确定蛇开始值
    start.x=random(n);
    start.y=random(m);
    s[start.x][start.y]='o';//画开始点
    printSth(start.y+1,start.x+1,'o');
    snake.push(start);      //加到蛇身
    live=1;                 //定义初始为存活
    updateFood();           //确定食物坐标
}

void map::updateFood()
{
    if (s[food.x][food.y]=='$') {
        s[food.x][food.y]=' ';
        removeSth(food.y+1,food.x+1);
    }
    do {
        food.x=random(n);
        food.y=random(m);
    } while (s[food.x][food.y]!=' '); //确定食物坐标，使他不得与蛇身相冲突
    s[food.x][food.y]='$';
    printSth(food.y+1,food.x+1,'$');  //画出食物
}

bool map::move(char ch)
{
    lock_guard<mutex> guard(mtx);
    if (ch==0)   //还没开始就不动
        return true;
    if (!check())//死了就不动
        return false;
    point temp=snake.front();   //获取蛇尾坐标
    point tail=temp;
    s[temp.x][temp.y]=' ';      //将蛇尾坐标设为空白地图
    removeSth(temp.y+1,temp.x+1);
    if (snake.size()>1) {
        temp=snake.back();
        s[temp.x][temp.y]='*';
        printSth(temp.y+1,temp.x+1,'*');
    }                           //将蛇头坐标设为第二个坐标
    if (ch==left)
        temp.y--;
    else if (ch==right)
        temp.y++;
    else if (ch==up)
        temp.x--;
    else if (ch==down)
        temp.x++;
    //移动，获取新蛇头坐标
    if (Setting.Hitwall) {
        temp.x+=n;
        temp.x%=n;
        temp.y+=m;
        temp.y%=m;
    }
    if (temp.x==food.x&&temp.y==food.y) {
        s[tail.x][tail.y]='*';
        s[temp.x][temp.y]='o';
        printSth(tail.y+1,tail.x+1,'*');
        printSth(temp.y+1,temp.x+1,'o');
        snake.push(temp);
        updateFood();
    }                           //假如吃到食物就把蛇尾坐标还原为蛇，并且确定新事物坐标
    else {
        snake.pop();
        snake.push(temp);
        live=check();
        if (live) {
            s[temp.x][temp.y]='o';
            printSth(temp.y+1,temp.x+1,'o');
        }
    }                           //假如没吃到事物弹出蛇尾坐标，并判断有没有死亡
    return live;
}

bool map::check()
{
    point temp=snake.back();
    if (temp.x>=n||temp.y>=m||temp.x<0||temp.y<0)   //假如蛇头为#
        return false;
    if (s[temp.x][temp.y]=='*') //假如蛇头为*
        return false;
    return true;
}

void map::print()               //最开始输出
{
    system("cls");
    for (int i=0; i<m+2; i++)
        cout<<'#';
    cout<<'\n';
    for (int i=0; i<n; i++) {
        cout<<'#';
        for (int j=0; j<m; j++)
            cout<<s[i][j];
        cout<<"#\n";
    }
    for (int i=0; i<m+2; i++)
        cout<<'#';
    cout<<'\n';
}

void map::printFailed(double score)
{
    system("cls");
    cout<<"************************************************************\n\n";
    cout<<"                          Game Over                         \n\n";
    cout<<"************************************************************\n\n";
    cout<<">\tExist Longest Time: "<<Setting.ExistLongestTime<<"s";
    cout<<"\n>\tLargest Score: "<<Setting.LargestScore;
    cout<<"\n>\tLongest Snake: "<<Setting.LongestSnake;
    cout<<"\n\n>\tYour score is: "<<score<<"\n>\tThe length of snake is: "<<snake.size()<<"\n>\tPress any key to exit...\n\n";
    cout<<"************************************************************\n\n>";
    CONSOLE_CURSOR_INFO cursor_info = {1, 1};
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);    //还原光标
    _getch();
}

void map::printSth(int x,int y,char temp)
{
    gotoxy(hOut1,x,y);
    cout<<temp;
}
void map::removeSth(int x,int y)
{
    gotoxy(hOut1,x,y);
    cout<<' ';
}

void updatePosition(char & ch)
{
    while (true) {
        if (!m.move(ch))
            break;
        this_thread::sleep_for(chrono::milliseconds(Setting.speed));
    }
}
void UpdateFood()
{
    if (!Setting.TimeLimitedPoint)
        return;
    while (m.live) {
        m.mtx.lock();
        m.updateFood();
        m.mtx.unlock();
        if (!m.live)
            break;
        this_thread::sleep_for(chrono::milliseconds(Setting.TimeLimitedPointUpdateGap));
    }
}

void start()
{
    m.init(Setting);    //初始化地图
    clock_t start,end;  //初始化时间
    char ch=0;          //初始化移动方向
    bool _move=0;       //是否已经进行了第一次移动(用来确定计时起点)
    char temp;          //记录输入当前方向，用来判断输入冲突
    m.print();          //画出初始地图
    thread automove(updatePosition,ref(ch));    //自动更新
    thread autoupdatefood(UpdateFood);          //自动更新食物
    while (m.live) {
        if ((_getch())==224) {
            temp=_getch();
            if (!m.live)
                break;
            if (!((temp==left&&ch==right)||(temp==right&&ch==left)||(temp==up&&ch==down)||(temp==down&&ch==up)||(temp==ch))) {
                ch=temp;    //判断输入错误
                if (!_move)
                    start=clock();
                _move=1;    //判断开始计时
                if (m.live)
                    m.move(ch); //移动一步
            }
        }   //正常输入
        else if (_getch()==' ') {
            if (!m.live)
                break;
            system("pause");
            m.print();
        }   //暂停游戏
        else {
            if (!m.live)
                break;
            _getch();
        }
    }
    end=clock();
    autoupdatefood.detach();
    automove.join();
    double time=double(end-start)/CLOCKS_PER_SEC;       //计算时间
    double score=100000000*(m.snake.size()-1)/Setting.n/Setting.m/Setting.speed/time;
    //计算分数
    if (time>Setting.ExistLongestTime)
        Setting.ExistLongestTime=time;
    if (m.snake.size()>Setting.LongestSnake)
        Setting.LongestSnake=m.snake.size();
    if (score>Setting.LargestScore)
        Setting.LargestScore=score;
    //更新最高纪录
    Setting.NearestTime=time;
    Setting.NearestSnake=m.snake.size();
    Setting.NearestScore=score;
    //更新最近记录
    Setting.saveSetting();
    m.printFailed(score);                               //输出游戏结束标志
}

void shell()
{
    system("mode con cols=60 lines=41");
    system("color F0");
    Setting.readSetting();
    string s;
sign2:
    system("cls");
    cout<<"************************************************************\n\n";
    cout<<"                        Greedy Snake\n\n";
    cout<<"************************************************************\n\n";
    cout<<"       Welcome to Greedy Snake! Wish you a happy time.\n";
    cout<<"    Greedy Snake Copyright (c) 2020 Purelight.Chan Zee Lok.\n\n";
    cout<<"            >>>  Enter 'start' to start!  <<<\n\n";
    cout<<"         Any Questions? Enter 'help' to get help!\n\n";
    cout<<"************************************************************\n\n>";//欢迎界面
sign:
    cin>>s;
    if (s=="start") {
        start();
        goto sign2;
    }
    else if (s=="showSetting") {
        Setting.showSetting();
        goto sign;
    }
    else if (s=="changeSetting") {
        Setting.changeSetting();
        Setting.saveSetting();
        goto sign2;
    }
    else if (s=="about") {
        cout<<"\tGreedy Snake v3.0.0\n\tCopyright (c) 2020 Purelight.Chan Zee Lok.\n";
        cout<<"\tGithub: https://github.com/Czile-create\n";
        cout<<"\tEmail: Czile@foxmail.com\n\n>";
        goto sign;
    }
    else if (s=="help") {
        cout<<"\nHere are commands you can use:\n>\tstart: To start the game.\n>\tshowSetting: To show Setting.\n";
        cout<<">\tchangeSetting: To change Setting.\n>\thonor: To show your honor.\n>\tabout: To show the info of author.\n>\texit: To exit the game\n\n";
        cout<<"How to control?\n>\tTo use the Function Key 'up', 'down',\n>\t'left' or 'right' to control the snake\n\n>";
        goto sign;
    }
    else if (s=="exit") {
        Setting.saveSetting();
        return;
    }
    else if (s=="honor") {
        Setting.showHonor();
        goto sign;
    }
    else {
        cout<<"No such command! Please try again. \n\n>";
        goto sign;
    }
}

int main()
{
    shell();
    return 0;
}
