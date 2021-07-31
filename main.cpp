#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <deque>
#include <ctime>
#pragma warning(disable:4996)
using namespace std;
HANDLE hOut;
COORD pos;

//1.��{gotoxy�禡
void gotoxy(short x,short y)
{
 hOut = GetStdHandle(STD_OUTPUT_HANDLE); //�������N�X
 pos = { x,y };
 SetConsoleCursorPosition(hOut,pos);  //���ʴ�Ш���w��m
}
void HideCursor() //���ô��
{
 HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
 CONSOLE_CURSOR_INFO CursorInfo;
 GetConsoleCursorInfo(handle,&CursorInfo);//��������i��и�T
 CursorInfo.bVisible = false;    //���ñ����i���
 SetConsoleCursorInfo(handle,&CursorInfo);//�]�w�����i��Ъ��A
}

//2.�D�����c��
struct Snake
{
 char body;
 short position_x,position_y;    //�D���y��
};

//3.�C��������
class Game
{
private:
 char image;
 enum mapSize { width = 60,height = 30 }; //�C���a��
 deque<Snake> snake;      //�w�q�@�Ӧ�C�A�˳D������
 int score = 0;        //�C������
 char hit = 'w';       //�����J
 bool eat_Food = false;      //�O�_�Y�쭹��
 short food_x,food_y;      //�����y��
 int speed = 400;       //�D���t��
 bool snake_state = true;     //�D�����A
 int level = 1;        //�C�����d
public:
 Game();
 void draw_Frame()  //�e���
 {
 for (int i = 0; i < height; i++)
 {
 gotoxy(0,i);
 cout << "��";
 gotoxy(width,i);
 cout << "��";
 }
 for (int i = 0; i <= width; i += 2) //�� �o�ӲŸ�����Ӧr����m�A�ҥH�O+2
 {
 gotoxy(i,0);
 cout << "��";
 gotoxy(i,height);
 cout << "��";
 }
 }
 void init_snake()  //��l�ƳD
 {
 snake.push_back({ '#',width / 2,static_cast<short>(height / 2) }); //�s�W�D�Y
 for (int i = 0; i < 3; i++) //�D����l����`�ơA�i�۩w
 snake.push_back({ char('o'),static_cast<short>((height / 2) + 1 + i) });
 snake.push_back({ ' ',static_cast<short>((height / 2) + 4) }); //�s�W�D���A����šA�H�K��᭱�s�W�`�Ʈɾާ@
 }
 void draw_Snake() //�e�D
 {
 for (int k = 0; k < snake.size(); k++)
 {
 gotoxy(snake[k].position_x,snake[k].position_y);
 cout << snake[k].body;
 }
 }
 void clear_Tail() //�M���D���A����ĳ�ϥβM�̨禡�A�קK�̰{
 {
 int k = snake.size() - 1;
 gotoxy(snake[k].position_x,snake[k].position_y);
 cout << " "; //�D�C���ʤ@���]�Y�@��^�A�N��W�@���e�X�Ӫ��D������
 }
 void draw_Food() //�e����
 {
 while (1)
 {
 food_x = rand() % (width - 4) + 2; //�����n�b�a�Ϥ��A����A�a����ؤW�A�a�Ϫ��Ÿ��bx��V����Ӧr����m�ҥH+2�A��-4�h�O��h���
 food_y = rand() % (height - 2) + 1; //�P�W�P�z
 if (wrong_Location() && food_x % 2 == 0)
 break;
 }
 gotoxy(food_x,food_y);
 cout << "O";
 }
 bool wrong_Location() //�P�_�������y�ЬO�_�X�z
 {
 for (auto i : snake) //c++11�����d�򪺰j��
 {
 if (food_x == i.position_x && food_y == i.position_y) //��������X�{�b�D������W
 return 0;
 }
 return 1;
 }
 void judge_eatFood() //�P�_�O�_�Y�쭹��
 {
 if (food_x == snake[0].position_x && food_y == snake[0].position_y)
 eat_Food = true;
 }
 void judge_state() //�P�_�D�O�_����μ�����
 {
 if (snake.size() >= 2) 
 {
 deque<Snake>::iterator iter = snake.begin() + 1; //��ڴN�O��snake�e���̲Ĥ@�ӡ]�Y�D�Y�^�s�biter��
 int x = (iter - 1)->position_x,y = (iter - 1)->position_y;
 for (; iter != snake.end(); ++iter) 
 {
 if (iter->position_x == x && iter->position_y == y) //�D�Y���༲�ۨ�
  snake_state = false;
 }
 }
 if(snake[0].position_x == 1 ||
 snake[0].position_x == 59 ||
 snake[0].position_y == 0 ||
 snake[0].position_y == 30) //�D�Y���༲���
 snake_state = false;
 }
 void key_Down() //�����T��
 {
 char key = hit;
 if (_kbhit()) //��������
 hit = _getch(); 
 for (int i = snake.size() - 1; i > 0; i--) //�D�����ʤ�k�A�C���ʤ@���A�᭱�@�`�������F�����e�@�`����W
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
 cout << "�A���o���O:";
 gotoxy(71,1);
 cout << score;
 gotoxy(69,2);
 cout << "���d:" << level;
 }
};
Game::Game()
{
 HideCursor();
 srand(static_cast<unsigned int>(time(NULL))); //�H���ƺؤl
 init_snake();
 draw_Food();
 Snake tail; //�D��
 while (1)
 {
 draw_Frame();
 tail = snake.back();
 if (eat_Food)
 {
 snake.back().body = 'o'; //���l�ƳD���ŧ���ܤƬ�o�A�ݨ쪺�ĪG�N�O�[�F�@�`
 snake.push_back(tail); //�A�s�W�@�`�ŧ��A�K��U���ާ@
 gotoxy(food_x,food_y);
 cout << " "; //�����Q�Y��n�b��Ӫ���m�e�šA�_�h��з|�h��A�ɭP��ؿ���
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
 system("mode con cols=100 lines=40"); //�]�w�}�ҵ����j�p
 system("color 7C"); //�]�w�I����M�e����
 system("title �g�Y�D v1.0"); //�]�w�������D
 Game game;
 gotoxy(0,32);
 cout << "Game over!" << endl;
}
