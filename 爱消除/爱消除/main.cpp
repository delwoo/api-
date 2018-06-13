#include<graphics.h>
#include<conio.h>
#include<time.h>
#include<stdio.h>

typedef struct position
{
	int x;
	int y;
} posType;

#define KEY_DOWN(vk_c) (GetAsyncKeyState(vk_c)&0x8000)

void init(void);  // ��ʼ������
void gamebegin(void);  //��Ϸ��ʼ����
void gameplay(void);   //����Ϸ�ľ������
void close(void);   //�ͷ���Դ
void drawcursor(posType, COLORREF); //�ػ���
void drawtime(int);  //�ػ�ʱ��
void drawscore(int); //�ػ����
void getsamecolorballs(posType, COLORREF); //��ȡͬɫС��
int isValid(posType cur, COLORREF cl); //�ж��Ƿ�Ϸ�
void ballsfall(); //С������

COLORREF colorArr[6] = { RGB(200,0,0),RGB(0,200,0),RGB(0,0,200),
RGB(200,200,0),RGB(200,0,200),RGB(0,200,200) };

posType cur; //��ǰ�������λ��
posType ballsArr[180]; //ͬɫС������
int index;  //ͬɫС��ĸ���

void main()
{
	init();
	gamebegin();
	gameplay();
	close();
}

void init()
{
	initgraph(1000, 700);
}

void gamebegin()
{
	//���Ʊ߿�
	setlinecolor(RGB(50, 50, 50));
	setlinestyle(PS_SOLID | PS_JOIN_ROUND, 10);
	rectangle(255, 45, 745, 655);
	//����С��		
	setlinestyle(PS_SOLID);
	srand((unsigned)time(NULL));  //�ṩ���������
	for (int x = 280; x<740; x += 40)
	{
		for (int y = 70; y<650; y += 40)
		{
			COLORREF cl = colorArr[rand() % 6];
			setlinecolor(cl);
			setfillcolor(cl);
			fillcircle(x, y, 18);
		}
	}
	//���ƹ��
	cur.x = 480;
	cur.y = 390;
	drawcursor(cur, RGB(255, 255, 255));
	//����ʱ��
	drawtime(30);
	//���Ʒ���
	drawscore(0);
}

void gameplay()
{
	int score = 0;
	for (int i = 50; i>-1; i--)
	{
		if (i % 10 == 0)
		{
			drawtime(i / 10);
		}
		if (KEY_DOWN(VK_UP) && cur.y>70)
		{
			drawcursor(cur, RGB(0, 0, 0));
			cur.y -= 40;
			drawcursor(cur, RGB(255, 255, 255));
		}
		else if (KEY_DOWN(VK_DOWN) && cur.y<630)
		{
			drawcursor(cur, RGB(0, 0, 0));
			cur.y += 40;
			drawcursor(cur, RGB(255, 255, 255));
		}
		else if (KEY_DOWN(VK_LEFT) && cur.x>280)
		{
			drawcursor(cur, RGB(0, 0, 0));
			cur.x -= 40;
			drawcursor(cur, RGB(255, 255, 255));
		}
		else if (KEY_DOWN(VK_RIGHT) && cur.x<720)
		{
			drawcursor(cur, RGB(0, 0, 0));
			cur.x += 40;
			drawcursor(cur, RGB(255, 255, 255));
		}
		else if (KEY_DOWN(VK_RETURN) || KEY_DOWN(VK_SPACE))
		{
			//��ù������λ����Χ��ͬɫС�����꣬�������飬����¼����
			getsamecolorballs(cur, getpixel(cur.x, cur.y));

			//��������Ԫ�������ú�һ��ʱ��
			if (index>1)
			{
				for (int k = 0; k<index; k++)
				{
					setlinecolor(RGB(0, 0, 0));
					setfillcolor(RGB(0, 0, 0));
					fillcircle(ballsArr[k].x, ballsArr[k].y, 18);
				}
				Sleep(500);
				//�Ϸ���С������
				ballsfall();
				//ˢ�·���
				score += index;
				drawscore(score);
			}

			index = 0;
		}

		Sleep(100);
	}
	//��Ϸ����
	cleardevice();
	settextcolor(RGB(255, 0, 0));
	settextstyle(25, 0, _T("����"));
	outtextxy(300, 300, _T("Game Over"));
	drawscore(score);
}

void close()
{
	getch();
	closegraph();
}

void drawcursor(posType cur, COLORREF cl)
{
	setlinecolor(cl);
	rectangle(cur.x - 20, cur.y - 20, cur.x + 20, cur.y + 20);
}

void drawtime(int sec)
{
	TCHAR str[30];
	settextcolor(RGB(255, 255, 0));
	settextstyle(25, 0, _T("����"));
	_stprintf(str, _T("ʣ��ʱ�䣺%2d s"), sec);
	outtextxy(30, 50, str);
}

void drawscore(int score)
{
	TCHAR str[30];
	settextcolor(RGB(255, 0, 0));
	settextstyle(25, 0, _T("����"));
	_stprintf(str, _T("������%d"), score);
	outtextxy(30, 600, str);
}

void getsamecolorballs(posType cur, COLORREF cl) //�����ݽṹ��---�Թ����
{
	ballsArr[index].x = cur.x;
	ballsArr[index].y = cur.y;
	index++;

	posType tmpPos;
	for (int k = 0; k<4; k++)
	{
		switch (k)
		{
		case 0:tmpPos.x = cur.x; tmpPos.y = cur.y - 40; break; //��
		case 1:tmpPos.x = cur.x; tmpPos.y = cur.y + 40; break; //��
		case 2:tmpPos.x = cur.x - 40; tmpPos.y = cur.y; break; //��
		case 3:tmpPos.x = cur.x + 40; tmpPos.y = cur.y; break; //��
		}

		if (isValid(tmpPos, cl))
		{
			getsamecolorballs(tmpPos, cl);
		}
	}
}

int isValid(posType cur, COLORREF cl)
{
	if (getpixel(cur.x, cur.y) != cl)
	{
		return 0;
	}
	else
	{
		for (int i = 0; i<index; i++)  //�ж�������֮ǰ�Ƿ��Ѵ���
		{
			if (cur.x == ballsArr[i].x&&cur.y == ballsArr[i].y)
			{
				return 0;
			}
		}
		return 1;
	}
}

void turn()
{
	int i, j; posType temp;
	for (j = 0; j<index - 1; j++)
		for (i = 0; i<index - 1 - j; i++)
		{
			if (ballsArr[i].x>ballsArr[i + 1].x)
			{
				temp = ballsArr[i];
				ballsArr[i] = ballsArr[i + 1];
				ballsArr[i + 1] = temp;
			}
			if (ballsArr[i].y>ballsArr[i + 1].y)
			{
				temp = ballsArr[i];
				ballsArr[i] = ballsArr[i + 1];
				ballsArr[i + 1] = temp;
			}
		}
}

void ballsfall()
{
	turn();
	for (int i = 0; i<index; i++)
	{
		for (int k = ballsArr[i].y; k>70; k -= 40)
		{
			COLORREF cl = getpixel(ballsArr[i].x, k - 40);
			setlinecolor(cl);
			setfillcolor(cl);
			fillcircle(ballsArr[i].x, k, 18);
		}
		COLORREF cl = colorArr[rand() % 6];
		setlinecolor(cl);
		setfillcolor(cl);
		fillcircle(ballsArr[i].x, 70, 18);
	}
}
