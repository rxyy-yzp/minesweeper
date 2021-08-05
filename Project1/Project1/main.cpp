#include"stdio.h"
#include"stdlib.h"
#include"time.h"
#include"graphics.h"
#include"conio.h"
#include "windows.h"
#include "mmsystem.h"
#pragma comment(lib, "winmm.lib")

#define ROW 10
#define COL 10
#define MINE 10
#define SIZE 50

int map[ROW][COL];//全局变量自动初始化未0

//定义图片
IMAGE img[12];
IMAGE img_fm;

//旗子个数标志
int standard = 0;

void loadimg();
void GameInit();
void GameDraw();
int MouseControl();
void openNull(int row, int col);
void openNull(int row, int col);
void show();

//加载图片
void loadimg()
{
	loadimage(&img[0], "0.jpg", SIZE, SIZE);
	loadimage(&img[1], "1.jpg", SIZE, SIZE);
	loadimage(&img[2], "2.jpg", SIZE, SIZE);
	loadimage(&img[3], "3.jpg", SIZE, SIZE);
	loadimage(&img[4], "4.jpg", SIZE, SIZE);
	loadimage(&img[5], "5.jpg", SIZE, SIZE);
	loadimage(&img[6], "6.jpg", SIZE, SIZE);
	loadimage(&img[7], "7.jpg", SIZE, SIZE);
	loadimage(&img[8], "8.jpg", SIZE, SIZE);
	loadimage(&img[9], "9.jpg", SIZE, SIZE);
	loadimage(&img[10], "10.jpg", SIZE, SIZE);
	loadimage(&img[11], "11.jpg", SIZE, SIZE);
}

//游戏初始化
void GameInit()
{
	int i, j;
	//mciSendString("open sf.mp3 alias BGM", 0, 0, 0);
	//mciSendString("play BGM repeat", 0, 0, 0);
	//设置随机数种子
	srand((unsigned)time(NULL));
	//初始化数组
	for (i = 0;i < ROW;i++)
	{
		for (j = 0;j < COL;j++)
		{
			map[i][j] = 0;
		}
	}
	//埋雷，雷：-1，10个雷
	for (i = 0;i < MINE;)
	{
		int r = rand() % 10;//0~9
		int c = rand() % 10;//0~9
		if (map[r][c] == 0)
		{
			map[r][c] = -1;
			i++;
		}
	}
	int m, n;
	for (i = 0;i < ROW;i++)
	{
		for (j = 0;j < COL;j++)
		{
			if (map[i][j] == -1)
			{
				//遍历雷所在九宫格
				for (m = i - 1;m <= i + 1;m++)
				{
					for (n = j - 1;n <= j + 1;n++)
					{
						if (m >= 0 && n >= 0 && m < ROW && n < COL && map[m][n] != -1)
						{
							map[m][n] += 1;
						}
					}
				}
			}
		}
	}
	
	//加密格子
	for (i = 0;i < ROW;i++)
	{
		for (j = 0;j < COL;j++)
		{
			map[i][j] += 20;
		}
	}
}

//绘制界面
void GameDraw()
{
	int i, j;
	cleardevice();
	for (i = 0;i < ROW;i++)
	{
		for (j = 0;j < COL;j++)
		{
			if (map[i][j] == -1)
			{
				putimage(j * SIZE, i * SIZE, &img[9]);//绘制雷
			}
			else if (map[i][j] >= 0 && map[i][j] <= 8)
			{
				putimage(j * SIZE, i * SIZE, &img[map[i][j]]);
			}
			else if (map[i][j] >= 19 && map[i][j] <= 28)
			{
				putimage(j * SIZE, i * SIZE, &img[10]);
			}
			else if (map[i][j] > 30)
			{
				putimage(j * SIZE, i * SIZE, &img[11]);
			}
		}
	}
}

int MouseControl()
{
	if (MouseHit())//判断有没有鼠标消息
	{
		MOUSEMSG msg = GetMouseMsg();
		int row = msg.y / SIZE;
		int col = msg.x / SIZE;//转换成数组下标
		//分发鼠标消息
		switch (msg.uMsg)
		{
		case WM_LBUTTONDOWN://鼠标左键
			if (map[row][col] > 8)//如果没有打开就打开
			{
				mciSendString("close yx", 0, 0, 0);
				mciSendString("open yx.mp3 alias yx", 0, 0, 0);
				mciSendString("play yx", 0, 0, 0);
				map[row][col] -= 20;
				openNull(row, col);
			}
			break;
		case WM_RBUTTONDOWN:
			if (map[row][col] > 8 && map[row][col] <= 28)//如果未打开且未被标记
			{
				mciSendString("close yx", 0, 0, 0);
				mciSendString("open yx.mp3 alias yx", 0, 0, 0);
				mciSendString("play yx", 0, 0, 0);
				map[row][col] += 20;
				standard++;
			}
			else if (map[row][col] > 28)//只能对已有标记取消标记
			{
				mciSendString("close yx", 0, 0, 0);
				mciSendString("open yx.mp3 alias yx", 0, 0, 0);
				mciSendString("play yx", 0, 0, 0);
				map[row][col] -= 20;
				standard--;
			}
			break;
		}
		return map[row][col];
	}
}

//打开所有空白为中心的九宫格
void openNull(int row, int col)
{
	int m, n;
	if (map[row][col] == 0)
	{
		for (m = row - 1;m <= row + 1;m++)
		{
			for (n = col - 1;n <= col + 1;n++)
			{
				if ((m >= 0 && n >= 0 && m < ROW && n < COL) && 
					(map[m][n] == 0||map[m][n]!=19) &&
					map[m][n]>8)//如果没有越界，为空或为数字，且未被打开
				{
					map[m][n] -= 20;
					openNull(m, n);
				}
			}
		}
	}
}

int judge(int n)
{
	int i, j;
	int flag = 1;
	int flag2 = 1;
	int z;
	if (n == -1)
	{
		mciSendString("close bomb", 0, 0, 0);
		mciSendString("open bomb.mp3 alias bomb", 0, 0, 0);
		mciSendString("play bomb", 0, 0, 0);
		Sleep(300);
		//结束
		int isok = MessageBox(GetHWnd(), "你输了!是否再玩一次", "提示", MB_OKCANCEL);
		if (IDOK == isok)
		{
			GameInit();
			standard = 0;
		}
		else
		{
			return 2;
		}
	}
	for (i = 0;i < ROW;i++)
	{
		for (j = 0;j < COL;j++)
		{
			if (map[i][j] > 30)
			{
				z = map[i][j] - 40;
				if (z != -1)
				{
					flag = 0;
				}
			}
		}
	}
	for (i = 0;i < ROW;i++)
	{
		for (j = 0;j < COL;j++)
		{
			if (map[i][j] > 18 && map[i][j] <= 30)
				flag2 = 0;
		}
	}
	if (flag==1 && standard == MINE && flag2 == 1)
	{
		mciSendString("close vic", 0, 0, 0);
		mciSendString("open vic.mp3 alias vic", 0, 0, 0);
		mciSendString("play vic", 0, 0, 0);
		Sleep(300);
		//胜利
		int isok = MessageBox(GetHWnd(), "你赢了!是否再玩一次", "提示", MB_OKCANCEL);
		if (IDOK == isok)
		{
			GameInit();
			standard = 0;
		}
		else
		{
			return 2;
		}
	}
	return 1;
}

void show()
{
	int i, j;
	for (i = 0;i < ROW;i++)
	{
		for (j = 0;j < COL;j++)
		{
			printf("%2d  ", map[i][j]);
		}
		printf("\n");
	}
	//system("cls");
}

int main()
{
	int n;
	MOUSEMSG m;
	HWND sl = initgraph(ROW * SIZE, COL * SIZE);
	SetWindowTextA(sl, "扫雷");
	loadimg();
	loadimage(&img_fm, "bk.jpg", ROW * SIZE, COL * SIZE);
	do {
		cleardevice();
		putimage(0, 0, &img_fm);
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN)
		{
			if (m.x >= 120 && m.x <= 370 && m.y >= 395 && m.y <= 470)
			{
				cleardevice();
				GameInit();
				BeginBatchDraw();
				while (1)
				{
					n = MouseControl();
					//show();
					GameDraw();
					FlushBatchDraw();
					if (judge(n) == 2)
					{
						break;
					}
				}
				EndBatchDraw();
			}
		}
	} while (1);
	
	return 0;
}