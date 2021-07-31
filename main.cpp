#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <deque>
#include <ctime>
#pragma warning(disable:4996)
using namespace std;
HANDLE hOut;
COORD pos;

//1.實現gotoxy函式
void gotoxy(short x,short y)
{
 hOut = GetStdHandle(STD_OUTPUT_HANDLE); //獲取控制代碼
 pos = { x,y };
 SetConsoleCursorPosition(hOut,pos);  //移動游標到指定位置
}
void HideCursor() //隱藏游標
{
 HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
 CONSOLE_CURSOR_INFO CursorInfo;
 GetConsoleCursorInfo(handle,&CursorInfo);//獲取控制檯游標資訊
 CursorInfo.bVisible = false;    //隱藏控制檯游標
 SetConsoleCursorInfo(handle,&CursorInfo);//設定控制檯游標狀態
}

//2.蛇的結構體
struct Snake
{
 char body;
 short position_x,position_y;    //蛇的座標
};

//3.遊戲執行類
class Game
{
private:
 char image;
 enum mapSize { width = 60,height = 30 }; //遊戲地圖
 deque<Snake> snake;      //定義一個佇列，裝蛇的身體
 int score = 0;        //遊戲分數
 char hit = 'w';       //按鍵輸入
 bool eat_Food = false;      //是否吃到食物
 short food_x,food_y;      //食物座標
 int speed = 400;       //蛇的速度
 bool snake_state = true;     //蛇的狀態
 int level = 1;        //遊戲關卡
public:
 Game();
 void draw_Frame()  //畫邊框
 {
 for (int i = 0; i < height; i++)
 {
 gotoxy(0,i);
 cout << "■";
 gotoxy(width,i);
 cout << "■";
 }
 for (int i = 0; i <= width; i += 2) //■ 這個符號佔兩個字元位置，所以是+2
 {
 gotoxy(i,0);
 cout << "■";
 gotoxy(i,height);
 cout << "■";
 }
 }
 void init_snake()  //初始化蛇
 {
 snake.push_back({ '#',width / 2,static_cast<short>(height / 2) }); //新增蛇頭
 for (int i = 0; i < 3; i++) //蛇的初始身體節數，可自定
 snake.push_back({ char('o'),static_cast<short>((height / 2) + 1 + i) });
 snake.push_back({ ' ',static_cast<short>((height / 2) + 4) }); //新增蛇尾，先放空，以便於後面新增節數時操作
 }
 void draw_Snake() //畫蛇
 {
 for (int k = 0; k < snake.size(); k++)
 {
 gotoxy(snake[k].position_x,snake[k].position_y);
 cout << snake[k].body;
 }
 }
 void clear_Tail() //清除蛇尾，不建議使用清屏函式，避免屏閃
 {
 int k = snake.size() - 1;
 gotoxy(snake[k].position_x,snake[k].position_y);
 cout << " "; //蛇每移動一次（即一格），就把上一次畫出來的蛇尾擦掉
 }
 void draw_Food() //畫食物
 {
 while (1)
 {
 food_x = rand() % (width - 4) + 2; //食物要在地圖中，不能再地圖邊框上，地圖的符號在x方向佔兩個字元位置所以+2，而-4則是減去邊框
 food_y = rand() % (height - 2) + 1; //與上同理
 if (wrong_Location() && food_x % 2 == 0)
 break;
 }
 gotoxy(food_x,food_y);
 cout << "O";
 }
 bool wrong_Location() //判斷食物的座標是否合理
 {
 for (auto i : snake) //c++11的基於範圍的迴圈
 {
 if (food_x == i.position_x && food_y == i.position_y) //食物不能出現在蛇的身體上
 return 0;
 }
 return 1;
 }
 void judge_eatFood() //判斷是否吃到食物
 {
 if (food_x == snake[0].position_x && food_y == snake[0].position_y)
 eat_Food = true;
 }
 void judge_state() //判斷蛇是否撞牆或撞身體
 {
 if (snake.size() >= 2) 
 {
 deque<Snake>::iterator iter = snake.begin() + 1; //實際就是把snake容器裡第一個（即蛇頭）存在iter裡
 int x = (iter - 1)->position_x,y = (iter - 1)->position_y;
 for (; iter != snake.end(); ++iter) 
 {
 if (iter->position_x == x && iter->position_y == y) //蛇頭不能撞自身
  snake_state = false;
 }
 }
 if(snake[0].position_x == 1 ||
 snake[0].position_x == 59 ||
 snake[0].position_y == 0 ||
 snake[0].position_y == 30) //蛇頭不能撞邊框
 snake_state = false;
 }
 void key_Down() //按鍵響應
 {
 char key = hit;
 if (_kbhit()) //接受按鍵
 hit = _getch(); 
 for (int i = snake.size() - 1; i > 0; i--) //蛇的移動方法，每移動一次，後面一節的身體到了它的前一節身體上
 {
 snake[i].position_x = snake[i - 1].position_x;
 snake[i].position_y = snake[i - 1].position_y;
 }
 if ((hit == 'a' || hit == 'A') && hit != 'd')
 {
 hit = 'a'; snake[0].position_x--;
 }
 else if ((hit == 'd' || hit == 'D') && hit != 'a')
 {
 hit = 'd'; snake[0].position_x++;
 }
 else if ((hit == 'w' || hit == 'W') && hit != 's')
 {
 hit = 'w'; snake[0].position_y--;
 }
 else if ((hit == 's' || hit == 'S') && hit != 'w')
 {
 hit = 's'; snake[0].position_y++;
 }
 }
 void show()
 {
 gotoxy(65,0);
 cout << "你的得分是:";
 gotoxy(71,1);
 cout << score;
 gotoxy(69,2);
 cout << "關卡:" << level;
 }
};
Game::Game()
{
 HideCursor();
 srand(static_cast<unsigned int>(time(NULL))); //隨機數種子
 init_snake();
 draw_Food();
 Snake tail; //蛇尾
 while (1)
 {
 draw_Frame();
 tail = snake.back();
 if (eat_Food)
 {
 snake.back().body = 'o'; //把初始化蛇的空尾顯示化為o，看到的效果就是加了一節
 snake.push_back(tail); //再新增一節空尾，便於下次操作
 gotoxy(food_x,food_y);
 cout << " "; //食物被吃後要在原來的位置畫空，否則游標會退位，導致邊框錯位
 draw_Food();
 score++;
 if (score % 5 == 0)
 {
 speed *= 0.8;
 level++;
 }
 eat_Food = false;
 }
 if (level == 10)
 break;
 key_Down();
 draw_Snake();
 judge_state();
 if (!snake_state)
 break;
 judge_eatFood(); 
 Sleep(speed);
 clear_Tail();
 show();
 }
}
int main()
{
 system("mode con cols=100 lines=40"); //設定開啟視窗大小
 system("color 7C"); //設定背景色和前景色
 system("title 貪吃蛇 v1.0"); //設定視窗標題
 Game game;
 gotoxy(0,32);
 cout << "Game over!" << endl;
}
