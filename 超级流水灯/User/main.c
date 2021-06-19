#include <LPC213X_LIB.h>
#include <string.h>

#define LED_0		GPIO_PIN_8
#define LED_1		GPIO_PIN_9
#define LED_2		GPIO_PIN_10
#define LED_3		GPIO_PIN_11
#define LED_4		GPIO_PIN_12
#define LED_5		GPIO_PIN_13
#define LED_6		GPIO_PIN_14
#define LED_7		GPIO_PIN_15
#define LED_8		GPIO_PIN_16
#define LED_9		GPIO_PIN_17
#define LED_10	GPIO_PIN_18
#define LED_11	GPIO_PIN_19
#define LED_12	GPIO_PIN_20
#define LED_13	GPIO_PIN_21
#define LED_14	GPIO_PIN_22
#define LED_15	GPIO_PIN_23
#define LED_ALL	(LED_0 | LED_1 | LED_2 | LED_3 | LED_4 | LED_5 | LED_6 | LED_7 | LED_8 | LED_9 | LED_10 | LED_11 | LED_12 | LED_13 | LED_14 | LED_15)

#define TX			GPIO_PIN_0
#define RX			GPIO_PIN_1

#define KEY_0		GPIO_PIN_3
#define KEY_1		GPIO_PIN_7

//中断服务函数
void EXTI1_CallBack(void) __irq;
void EXTI2_CallBack(void) __irq;
void UART0_CallBack(void) __irq;


//led列表
uint32_t led_list[16] = { LED_0,LED_1,LED_2,LED_3,LED_4,LED_5,LED_6,LED_7,LED_8,LED_9,LED_10,LED_11,LED_12,LED_13,LED_14,LED_15 };
uint8_t style;		//流水灯样式
uint16_t display;	//led显示

uint32_t timer = 0;
int n = 0;

int main()
{
	//GPIO配置
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_MODE_OUTPUT;	
	GPIO_InitStruct.GPIO_Pin = TX | LED_ALL;		
	GPIO_Init(P0, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.GPIO_Pin = RX;
	GPIO_Init(P0, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_MODE_EXTI;
	GPIO_InitStruct.GPIO_Pin = KEY_0 | KEY_1;
	GPIO_Init(P0, &GPIO_InitStruct);
	
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
	VIC_InitStruct.VIC_IRQChannel = EXTI1_IRQn;
	VIC_InitStruct.VIC_IRQChannelPriority = 0;
	VIC_InitStruct.pCallBack = EXTI1_CallBack;
	VIC_InitStruct.VIC_IRQChannelCmd = ENABLE;
	VIC_Init(&VIC_InitStruct);
	
	VIC_InitStruct.VIC_IRQChannel = EXTI2_IRQn;
	VIC_InitStruct.VIC_IRQChannelPriority = 1;
	VIC_InitStruct.pCallBack = EXTI2_CallBack;
	VIC_Init(&VIC_InitStruct);
	
	VIC_InitStruct.VIC_IRQChannel = UART0_IRQn;
	VIC_InitStruct.VIC_IRQChannelPriority = 2;
	VIC_InitStruct.pCallBack = UART0_CallBack;
	VIC_Init(&VIC_InitStruct);
	
	//外部中断配置
	EXTI_InitTypeDef EXTI_InitStruct;
	EXTI_InitStruct.EXTI_n = EXTI1 | EXTI2;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStruct);
	
	//输出开始信号
	char buff[30] = "程序开始运行,当前流水灯样式:0\r";
	int i = 0;
	while(buff[i])
		UART_SendData(UART0, buff[i++]);
	
	style = 0;
	display = 0x0000;
	while(1)
	{
		switch(style)
		{
			case 0:
			{
				//流水灯样式0控制
				timer %= 800;
				display = 0x0101 << (timer / 100);
			}break;
			case 1:
			{
				//流水灯样式1控制
				if(timer == (16 - n) * 50)
				{
					timer = 0;
					n++;
				}
				display = (0xFFFF >> (16 - n) << (16 - n)) | (0x0001 << (timer / 50));
				n %= 16;
			}break;
			default:
				display = 0x0000;
				break;
		}
		
		for(int i = 0; i < 16; i++)
		{
			GPIO_WritePin(P0, led_list[i], (display >> i) & 0x01);
		}
		timer++;
		delay(1);
	}
}


void EXTI1_CallBack(void) __irq
{
	EXTI_ClearITPendingBit(EXTI1);
	style = 0;
	timer = 0;
	n = 0;
	char buff[30] = "";
	sprintf(buff, "你选择了流水灯样式: %d\r", style);
	int i = 0;
	while(buff[i])
		UART_SendData(UART0, buff[i++]);
}

void EXTI2_CallBack(void) __irq
{
	EXTI_ClearITPendingBit(EXTI2);
	style = 1;
	timer = 0;
	n = 0;
	char buff[30] = "";
	sprintf(buff, "你选择了流水灯样式: %d\r", style);
	int i = 0;
	while(buff[i])
		UART_SendData(UART0, buff[i++]);
}

void UART0_CallBack(void) __irq
{
	if(SET == UART_GetITStatus(UART0, UART_IT_RX_Available))
	{
		uint8_t data = UART_ReceiveData(UART0);
		style = data - '0';
		timer = 0;
		n = 0;
		char buff[30] = "";
		sprintf(buff, "你选择了流水灯样式: %d\r", style);
		int i = 0;
		while(buff[i])
			UART_SendData(UART0, buff[i++]);
	}
}

