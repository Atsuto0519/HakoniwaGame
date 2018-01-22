#include "DxLib.h"
#include "Object.h"
#include "Garden.h"
#include <math.h>

#define WIDTH 640
#define HEIGHT 480
#define GARDENSIZE 9

#define ITEMSIZE 80

int distance(int x1, int y1, int x2, int y2);
void setBaseGarden(Garden garden[], int x, int y);
void printGarden(Garden garden[]);
int selectGarden(Garden garden[]);

int baseImage;
int flowerImage;
// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	ChangeWindowMode(TRUE);
	SetDrawScreen(DX_SCREEN_BACK);
	SetBackgroundColor(174,209,122);
	SetGraphMode(640, 480, 32);
	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}


	baseImage = LoadGraph(".\\img\\base.png");
	flowerImage = LoadGraph(".\\img\\flower.png");

	Garden garden[GARDENSIZE*GARDENSIZE];
	for (int i = 0; i < GARDENSIZE*GARDENSIZE; i++)
	{
		garden[i].object.ID = 0;
	}

	int x; int y;
	x = y = 0;
	int count = 0;
	char key[256];


	while (true)
	{
		for (int i = 0; i < GARDENSIZE*GARDENSIZE; i++)
		{
			garden[i].object.ID = 0;
		}

		setBaseGarden(garden,x,y);
		garden[selectGarden(garden)].object.ID = 1;

		GetHitKeyStateAll(key);
		if (key[KEY_INPUT_W])y--;
		if (key[KEY_INPUT_A])x--;
		if (key[KEY_INPUT_S])y++;
		if (key[KEY_INPUT_D])x++;

		if (count++ % 10 == 0)
		{
			ClearDrawScreen();
			printGarden(garden);
			ScreenFlip();

		}
	}


	WaitKey();				// キー入力待ち
	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}

void setBaseGarden(Garden garden[], int x, int y)
{
	int count = 0;
	int pic_x = WIDTH / 2 - ITEMSIZE / 2, pic_y = 50;
	for (int i = 0; i < GARDENSIZE; i++)
	{
		for (int j = 0; j <= i; j++)
		{
			garden[count].x = x + pic_x + j * ITEMSIZE;
			garden[count].y = y + pic_y;
			count++;
		}
		pic_x -= ITEMSIZE / 2;
		pic_y += 48 / 2;
	}
	pic_x += ITEMSIZE;
	for (int i = GARDENSIZE - 2; i >= 0; i--)
	{
		for (int j = 0; j <= i; j++)
		{
			garden[count].x = x + pic_x + j * ITEMSIZE;
			garden[count].y = y + pic_y;
			count++;
		}
		pic_x += ITEMSIZE / 2;
		pic_y += 48 / 2;
	}
}



void printGarden(Garden garden[])
{
	for (int i = 0; i < GARDENSIZE*GARDENSIZE; i++)
	{
		switch (garden[i].object.ID)
		{
		case 0:
			DrawGraph(garden[i].x, garden[i].y, baseImage, TRUE); // 画像を描画する
			break;
		case 1:
			DrawGraph(garden[i].x, garden[i].y, flowerImage, TRUE); // 画像を描画する
			break;
		}
	}
}
int distance(int x1, int y1, int x2, int y2)
{
	return (int)sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}
int selectGarden(Garden garden[])
{
	int mouseX, mouseY;
	GetMousePoint(&mouseX, &mouseY);
	int dist = 0;
	int bestNum = 0;

	dist = distance(garden[0].x + ITEMSIZE / 2, garden[0].y + (32 + 48 / 2), mouseX, mouseY);
	bestNum = 0;
	for (int i = 1; i < GARDENSIZE*GARDENSIZE; i++)
	{
		if (dist > distance(garden[i].x + ITEMSIZE / 2, garden[i].y + (32 + 48 / 2), mouseX, mouseY))
		{
			bestNum = i;
			dist = distance(garden[i].x + ITEMSIZE / 2, garden[i].y + (32 + 48 / 2), mouseX, mouseY);
		}
	}
	if (dist < 20)return bestNum;

	return -1;
}