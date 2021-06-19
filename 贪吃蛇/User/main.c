#include <LPC213X_LIB.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RAND_INT(a, b) rand() % (b + 1 - a) + a

#define SPI0_SCK	GPIO_PIN_4
#define SPI0_MISO	GPIO_PIN_5
#define SPI0_MOSI	GPIO_PIN_6
#define SPI0_SSEL	GPIO_PIN_7

#define TX			GPIO_PIN_0
#define RX			GPIO_PIN_1

#define SEL_0			GPIO_PIN_8
#define SEL_1			GPIO_PIN_9
#define SEL_2			GPIO_PIN_10
#define SEL_3			GPIO_PIN_11
#define SEL_4			GPIO_PIN_12
#define SEL_5			GPIO_PIN_13
#define SEL_6			GPIO_PIN_14
#define SEL_7			GPIO_PIN_15
#define SEL_ALL		(SEL_0 | SEL_1 | SEL_2 | SEL_3 | SEL_4 | SEL_5 | SEL_6 | SEL_7)

#define KEY_UP		GPIO_PIN_16
#define KEY_DOWM	GPIO_PIN_16
#define KEY_LEFT	GPIO_PIN_16
#define KEY_RIGHT	GPIO_PIN_16

//封面图像
uint32_t img_cover[32] = {
	0x00000000,0x0FA00010,0x11410028,0x24AF8054,
	0x2B0040BA,0x2807BCA2,0x29C42410,0x2422247C,
	0x12213C54,0x09A08054,0x14204010,0x27C04028,
	0x28084044,0x27878082,0x10400000,0x0F808000,
	0x00041000,0x08014008,0x80800041,0x4900002A,
	0x0007E000,0x3E081008,0x41100814,0x512E1C14,
	0x8EB12272,0x88B52A82,0xBEB1227A,0xAAAE1C82,
	0xAAA0647A,0x4927A482,0x4128247C,0x3E27C400

};
//游戏结束图像
uint32_t img_gameover[32] = {
	0x00000000,0x07910870,0x009B1408,0x07951C68,
	0x00912248,0x07912270,0x00000000,0x239E2278,
	0x24822248,0x239E1448,0x02821448,0x249E0878,
	0x00000000,0x01400018,0x02A07014,0x01538FCA,
	0x00FC0026,0x00500290,0x00218110,0x00227A90,
	0x00110420,0x01E883D0,0x0214400E,0x040A2008,
	0x08402088,0x08B02040,0x090E20E0,0x0901C490,
	0x09000590,0x04800610,0x024003E0,0x01A00000
};

/*************************游戏相关变量及函数******************************/
#define Snack_Init_Length 4							//蛇的初始长度
#define Map_Width					31						//地图宽度
#define Map_Height				31						//地图高度
#define Game_Delay				5							//游戏更新间隔
#define Start_X						10						//蛇的起始x坐标
#define Start_Y						20						//蛇的起始y坐标

//蛇的结点结构体
typedef struct
{
	int x;
	int y;
	char dir_now;
}snack_node;

snack_node snack[1024];									//蛇
int snack_length = Snack_Init_Length;		//蛇长度
int game_state = 0;											//游戏状态（判断是否游戏结束和显示信息）

int food_x, food_y;											//食物的x,y坐标

int score;															//得分
							
void Game_Init(void);										//游戏初始化
void Game_Update(void);									//游戏数据更新
void Game_Draw(void);										//游戏画面绘制
						
void Snack_Move(void);									//蛇移动
int Collision_Chack(void);							//碰撞检测
void Food_Update(void);									//食物更新
void New_Food(void);										//生成新食物

/******************************************************************/

uint32_t sel_list[8] = { SEL_0,SEL_1,SEL_2,SEL_3,SEL_4,SEL_5,SEL_6,SEL_7 };
uint32_t screen[32] = { 0x00000000 };		//屏幕缓冲区

uint32_t timer = 0;											//计时器
uint8_t key = 0;												//当前按键值

void UART0_CallBack(void) __irq;

void display(void);											//绘制屏幕
void SetPixel(int x, int y);						//绘制像素点
void ResetPixel(int x, int y);					//清除像素点
void ClearScreen(void);									//清屏

void ShowPicture(uint32_t picture[32]);	//显示图片

int main()
{
	//GPIO配置
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_MODE_OUTPUT;	
	GPIO_InitStruct.GPIO_Pin = SEL_ALL | SPI0_SCK | SPI0_MOSI | SPI0_SSEL | TX;		
	GPIO_Init(P0, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.GPIO_Pin = SPI0_MISO | KEY_UP | KEY_DOWM | KEY_LEFT | KEY_RIGHT | RX;
	GPIO_Init(P0, &GPIO_InitStruct);
	
	//SPI相关配置
	SPI_InitTypeDef SPI_InitStruct;
	SPI_InitStruct.SPI_BaudRate = 4800;
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
	SPI_InitStruct.SPI_IT_Cmd = DISABLE;
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;
	SPI_Init(SPI0, &SPI_InitStruct);
	
	//UART0配置
	UART_InitTypeDef UART_InitStruct;
	UART_InitStruct.BaudRate = 9600;
	UART_InitStruct.Parity = UART_Parity_None;
	UART_InitStruct.StopBits = UART_StopBits_1;
	UART_InitStruct.WordLength = UART_WordLength_8b;
	UART_Init(UART0, &UART_InitStruct);
	UART_Cmd(UART0, ENABLE);
	
	UART_ITConfig(UART0, UART_IT_RX_1Byte, ENABLE);
	
	UART_Cmd(UART0, ENABLE);
	
	//中断向量控制
	VIC_InitTypeDef VIC_InitStruct;
	VIC_InitStruct.VIC_IRQChannel = UART0_IRQn;
	VIC_InitStruct.VIC_IRQChannelPriority = 0;
	VIC_InitStruct.pCallBack = UART0_CallBack;
	VIC_InitStruct.VIC_IRQChannelCmd = ENABLE;
	VIC_Init(&VIC_InitStruct);
	
	//外设初始化
	GPIO_WritePin(P0, SEL_ALL, GPIO_PIN_RESET);
	
	GPIO_WritePin(P0, KEY_UP | KEY_DOWM | KEY_LEFT | KEY_RIGHT, GPIO_PIN_SET);
	
	SPI_WriteByte(SPI0, 0xFF);
	GPIO_WritePin(P0, SEL_ALL, GPIO_PIN_SET);
	GPIO_WritePin(P0, SEL_ALL, GPIO_PIN_RESET);
	
	//清屏
	ClearScreen();

	//输出提示信息
	char msg[50] = "按 'r' 投币开始游戏\r";
	int msg_n = 0;
	while(msg[msg_n])
		UART_SendData(UART0, msg[msg_n++]);
	while(key != 'r')
	{
		ShowPicture(img_cover);	//显示游戏封面
		display();
		timer++;
		delay(1);
	}
	
	srand(timer);	//初始化随机数种子
	
	Game_Init();	//游戏初始化
	
	while(1)
	{
		Game_Update();	//游戏数据更新
		Game_Draw();		//游戏画面绘制
		timer++;
		delay(1);
	}
}


void display(void)
{
	for(int i = 0; i < 32; i++)
	{
		//行选择
		SPI_WriteByte(SPI0, ~(0x01 << (i % 8)));
		GPIO_WritePin(P0, sel_list[i / 8 + 4], GPIO_PIN_SET);
		GPIO_WritePin(P0, sel_list[i / 8 + 4], GPIO_PIN_RESET);
		//列选择
		for(int j = 0; j < 4; j++)
		{
			SPI_WriteByte(SPI0, (screen[i] >> j * 8) & 0xFF);
			GPIO_WritePin(P0, sel_list[j], GPIO_PIN_SET);
			GPIO_WritePin(P0, sel_list[j], GPIO_PIN_RESET);
		}
		delay_us(50);
		//消影
		SPI_WriteByte(SPI0, 0xFF);
		GPIO_WritePin(P0, sel_list[i / 8 + 4], GPIO_PIN_SET);
		GPIO_WritePin(P0, sel_list[i / 8 + 4], GPIO_PIN_RESET);
		for(int j = 0; j < 4; j++)
		{
			SPI_WriteByte(SPI0, 0x00);
			GPIO_WritePin(P0, sel_list[j], GPIO_PIN_SET);
			GPIO_WritePin(P0, sel_list[j], GPIO_PIN_RESET);
		}
	}
}

void SetPixel(int x, int y)
{
	if(x < 0 || x > 31 || y < 0 || y > 31)return;	//判断输入的合法性
	screen[y] |= 0x00000001 << x;									//设置屏幕缓冲区
}
void ResetPixel(int x, int y)
{
	if(x < 0 || x > 31 || y < 0 || y > 31)return;	//判断输入的合法性
	screen[y] &= ~(0x00000001 << x);              //设置屏幕缓冲区
}

void ClearScreen(void)
{
	for(int i = 0; i < 32; i++)
	{
		screen[i] = 0x00000000;	//缓冲区清零
	}
}

void ShowPicture(uint32_t picture[32])
{
	for(int i = 0; i < 32; i++)
	{
		screen[i] = picture[i];	//缓冲区赋值
	}
}


void UART0_CallBack(void) __irq
{
	if(SET == UART_GetITStatus(UART0, UART_IT_RX_Available))
	{
		uint8_t data = UART_ReceiveData(UART0);
		key = data;
	}
}


void Game_Init(void)
{
	//初始化相关变量
	snack_length = Snack_Init_Length;
	timer = 0;
	score = 0;
	game_state = 1;
	key = 'w';
	//生成蛇并设置初始位置
	for(int i = 0; i < snack_length; i++)
	{
		snack[i].x = Start_X;
		snack[i].y = Start_Y + i;
		snack[i].dir_now = 'w';
	}
	//生成一个新的食物
	New_Food();
	//发送开始游戏信息
	char msg[30] = "游戏开始! 祝你好运!\r";
	int msg_n = 0;
	while(msg[msg_n])
		UART_SendData(UART0, msg[msg_n++]);
}

void Game_Update(void)
{
	if(timer == Game_Delay)		//控制刷新间隔
	{
		timer = 0;
		if(game_state)					//游戏开始
		{
			Snack_Move();					//更新蛇的位置信息
			if(Collision_Chack())	//碰撞检测
			{
				game_state = 0;			//撞墙或者撞到自己，设置游戏状态为游戏结束
				//发送游戏结束信息
				char msg[60] = "";
				int msg_n = 0;
				sprintf(msg, "游戏结束! 你获得的分数是: %d \r按 'r' 投币开始新游戏!\r", score);
				while(msg[msg_n])
					UART_SendData(UART0, msg[msg_n++]);
				return;
			}
			//更新食物信息，包括蛇吃到食物后的处理
			Food_Update();
		}
		else
		{
			//游戏结束后按'r'重新开始
			if(key == 'r' || key == 'R')
			{
				Game_Init();
			}
		}
	}
}

void Game_Draw(void)
{
	ClearScreen();	//首先清屏
	if(game_state)	//游戏还在进行则绘制蛇和食物的图像
	{
		for(int i = 0; i < snack_length; i++)
		{
			SetPixel(snack[i].x, snack[i].y);
		}
		SetPixel(food_x, food_y);
	}
	else						//游戏结束则绘制结束图像
	{
		ShowPicture(img_gameover);
	}
	display();
}


void Snack_Move(void)
{
	//首先更新蛇头的运动方向
	if(key == 'w' || key == 's' || key == 'a' || key == 'd' || key == 'W' || key == 'S' || key == 'A' || key == 'D')
	{
		switch(snack[0].dir_now)
		{
			case 'w':
			case 'W':
			{
				if(key != 's' && key != 'S')snack[0].dir_now = key;
			}break;
			case 's':
			case 'S':
			{
				if(key != 'w' && key != 'W')snack[0].dir_now = key;
			}break;
			case 'a':
			case 'A':
			{
				if(key != 'd' && key != 'D')snack[0].dir_now = key;
			}break;
			case 'd':
			case 'D':
			{
				if(key != 'a' && key != 'A')snack[0].dir_now = key;
			}break;
			default:
				break;
		}
	}
	//更新蛇的每个结点位置
	for(int i = snack_length - 1; i >= 0; i--)
	{
		switch(snack[i].dir_now)
		{
			case 'w':
			case 'W':
			{
				snack[i].y--;
			}break;
			case 's':
			case 'S':
			{
				snack[i].y++;
			}break;
			case 'a':
			case 'A':
			{
				snack[i].x--;
			}break;
			case 'd':
			case 'D':
			{
				snack[i].x++;
			}break;
			default:
				break;
		}
	}
	//更新每个结点下次的移动方向
	for(int i = snack_length - 1; i > 0; i--)
		snack[i].dir_now = snack[i - 1].dir_now;
	
}

int Collision_Chack(void)
{
	//判断是否撞墙
	if(snack[0].x < 0 || snack[0].x > Map_Width || snack[0].y < 0 || snack[0].y > Map_Height)return 1;
	
	//判断是否撞到自己
	for(int i = 1; i < snack_length; i++)
	{
		if(snack[0].x == snack[i].x && snack[0].y == snack[i].y)return 1;
	}
	return 0;
}

void Food_Update(void)
{
	//判断是否吃到食物
	if(food_x == snack[0].x && food_y == snack[0].y)
	{
		//吃到食物后增加分数和蛇的长度，并生成新的食物
		score++;
		snack[snack_length]= snack[snack_length - 1];
		switch(snack[snack_length - 1].dir_now)
		{
			case 'w':
			case 'W':
			{
				snack[snack_length].y++;
			}break;
			case 's':
			case 'S':
			{
				snack[snack_length].y--;
			}break;
			case 'a':
			case 'A':
			{
				snack[snack_length].x++;
			}break;
			case 'd':
			case 'D':
			{
				snack[snack_length].x--;
			}break;
			default:
				break;
		}
		snack_length++;
		
		New_Food();
		
		//显示当前得分
		char msg[50] = "";
		int msg_n = 0;
		sprintf(msg, "当前分数:%d\r", score);
		while(msg[msg_n])
			UART_SendData(UART0, msg[msg_n++]);
	}
}

void New_Food(void)
{
	int flag = 1;
	while(flag)
	{
		//随机生成一个食物
		food_x = RAND_INT(0, 31);
		food_y = RAND_INT(0, 31);
		flag = 0;
		//保证食物不在蛇的身上
		for(int i = 0; i < snack_length; i++)
		{
			if(food_x == snack[i].x && food_y == snack[i].y)
			{
				flag = 1;
				break;
			}
		}
	}
}

