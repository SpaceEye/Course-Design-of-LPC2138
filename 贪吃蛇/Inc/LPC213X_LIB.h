/**********************LPC213x_LIB.h************************/
/*	LPC213x系列相关寄存器、结构体和函数声明							 	 */
/*	版本：v5.0.0																					 */
/*	版本改动：增加PWM、ADC、看门狗的相关功能，修复已知bug  */
/*	最后修改日期：2020.6.2																 */
/*	作者：SpacePioneer																		 */
/***********************************************************/
#include <stdint.h>
#include <LPC213X.H>

#ifndef _LPC213X_LIB_H_
#define _LPC213X_LIB_H_

#define Fpclk 12000000					//晶振周期/Hz		//使用11.0592MHz晶振请改为11059200

void delay(uint16_t ms);				//毫秒延时函数
void delay_us(uint16_t us);			//微妙延时函数

typedef enum 
{
  DISABLE = 0, 
  ENABLE = !DISABLE
} FunctionalState;

typedef enum 
{
  RESET = 0, 
  SET = !RESET
} FlagStatus, ITStatus, Status;

/****************************************************GPIO****************************************************/

//引脚状态
typedef enum
{
  GPIO_PIN_RESET = 0,
  GPIO_PIN_SET
} GPIO_PinState;

typedef struct
{
	volatile uint32_t IOPIN;
	volatile uint32_t IOSET;
	volatile uint32_t IODIR;
	volatile uint32_t IOCLR;
} GPIO_TypeDef;

//GPIO初始化结构体定义
typedef struct
{
	//GPIO引脚
	uint32_t GPIO_Pin;
	//GPIO模式
	uint32_t GPIO_Mode;
	
}GPIO_InitTypeDef;


//所有引脚定义
#define GPIO_PIN_0                 ((uint32_t)0x00000001)  /* Pin 0			*/
#define GPIO_PIN_1                 ((uint32_t)0x00000002)  /* Pin 1     */
#define GPIO_PIN_2                 ((uint32_t)0x00000004)  /* Pin 2     */
#define GPIO_PIN_3                 ((uint32_t)0x00000008)  /* Pin 3     */
#define GPIO_PIN_4                 ((uint32_t)0x00000010)  /* Pin 4     */
#define GPIO_PIN_5                 ((uint32_t)0x00000020)  /* Pin 5     */
#define GPIO_PIN_6                 ((uint32_t)0x00000040)  /* Pin 6     */
#define GPIO_PIN_7                 ((uint32_t)0x00000080)  /* Pin 7     */
#define GPIO_PIN_8                 ((uint32_t)0x00000100)  /* Pin 8     */
#define GPIO_PIN_9                 ((uint32_t)0x00000200)  /* Pin 9     */
#define GPIO_PIN_10                ((uint32_t)0x00000400)  /* Pin 10    */
#define GPIO_PIN_11                ((uint32_t)0x00000800)  /* Pin 11    */
#define GPIO_PIN_12                ((uint32_t)0x00001000)  /* Pin 12    */
#define GPIO_PIN_13                ((uint32_t)0x00002000)  /* Pin 13    */
#define GPIO_PIN_14                ((uint32_t)0x00004000)  /* Pin 14    */
#define GPIO_PIN_15                ((uint32_t)0x00008000)  /* Pin 15    */
#define GPIO_PIN_16                ((uint32_t)0x00010000)  /* Pin 16		*/
#define GPIO_PIN_17                ((uint32_t)0x00020000)  /* Pin 17    */
#define GPIO_PIN_18                ((uint32_t)0x00040000)  /* Pin 18    */
#define GPIO_PIN_19                ((uint32_t)0x00080000)  /* Pin 19    */
#define GPIO_PIN_20                ((uint32_t)0x00100000)  /* Pin 20    */
#define GPIO_PIN_21                ((uint32_t)0x00200000)  /* Pin 21    */
#define GPIO_PIN_22                ((uint32_t)0x00400000)  /* Pin 22    */
#define GPIO_PIN_23                ((uint32_t)0x00800000)  /* Pin 23    */
#define GPIO_PIN_24                ((uint32_t)0x01000000)  /* Pin 24    */
#define GPIO_PIN_25                ((uint32_t)0x02000000)  /* Pin 25    */
#define GPIO_PIN_26                ((uint32_t)0x04000000)  /* Pin 26    */
#define GPIO_PIN_27                ((uint32_t)0x08000000)  /* Pin 27    */
#define GPIO_PIN_28                ((uint32_t)0x10000000)  /* Pin 28    */
#define GPIO_PIN_29                ((uint32_t)0x20000000)  /* Pin 29    */
#define GPIO_PIN_30                ((uint32_t)0x40000000)  /* Pin 30    */
#define GPIO_PIN_31                ((uint32_t)0x80000000)  /* Pin 31    */
#define GPIO_PIN_All               ((uint32_t)0xFFFFFFFF)  /* All pins  */

#define GPIO_MODE_INPUT			0
#define GPIO_MODE_OUTPUT		1
#define GPIO_MODE_EXTI			2

#define GPIO_MODE_IT_RISING        
#define GPIO_MODE_IT_FALLING
#define GPIO_MODE_IT_HIGH       
#define GPIO_MODE_IT_LOW

#define P0	((GPIO_TypeDef *)0xE0028000)
#define P1	((GPIO_TypeDef *)0xE0028010)

//GPIO初始化
void GPIO_Init(GPIO_TypeDef *Px, GPIO_InitTypeDef *GPIO_InitStruct);
//读引脚
GPIO_PinState GPIO_ReadPin(GPIO_TypeDef *Px, uint32_t GPIO_Pin);
//写引脚
void GPIO_WritePin(GPIO_TypeDef *Px, uint32_t GPIO_Pin, GPIO_PinState PinState);
//反转引脚状态
void GPIO_TogglePin(GPIO_TypeDef *Px, uint32_t GPIO_Pin);


/************************************************外部中断***********************************************************/


//EINT初始化结构体定义
typedef struct
{
	uint8_t EXTI_n;
	uint8_t EXTI_Trigger;
	
}EXTI_InitTypeDef;


#define EXTI0	0x01
#define EXTI1	0x02
#define EXTI2	0x04
#define EXTI3	0x08

#define EXTI_Trigger_Rising		0xFF
#define EXTI_Trigger_Falling	0x0F
#define EXTI_Trigger_High			0xF0
#define EXTI_Trigger_Low			0x00

#define VICVectAddr_BASE ((uint32_t*)0xFFFFF100)
#define VICVectCnt_BASE ((uint32_t*)0xFFFFF200)

//外部中断初始化
void EXTI_Init(EXTI_InitTypeDef *EXTI_InitStruct);
//清除中断标志位
void EXTI_ClearITPendingBit(uint8_t EXTIn);
//中断命令控制
void EXTI_Cmd(uint32_t EXTIn, FunctionalState Cmd);


/****************************************************向量中断控制****************************************************/

//向量中断通道
#define WDT_IRQn		0
#define TIM0_IRQn		4
#define TIM1_IRQn		5
#define UART0_IRQn	6
#define UART1_IRQn	7
#define PWM0_IRQn		8
#define I2C0_IRQn		9
#define SPI0_IRQn		10
#define SSP_IRQn		11
#define PLL_IRQn		12
#define RTC_IRQn		13
#define EXTI0_IRQn	14
#define EXTI1_IRQn	15
#define EXTI2_IRQn	16
#define EXTI3_IRQn	17
#define ADC0_IRQn		18
#define I2C1_IRQn		19
#define BOD_IRQn		20
#define ADC1_IRQn		21


typedef struct
{
	//中断向量通道
	uint32_t VIC_IRQChannel;
	//中断优先级
	uint32_t VIC_IRQChannelPriority;
	//控制
	FunctionalState VIC_IRQChannelCmd;
	//中断服务函数
	void (*pCallBack)(void)__irq;
	
}VIC_InitTypeDef;

//VIC初始化配置
void VIC_Init(VIC_InitTypeDef *VIC_InitStruct);

/****************************************************定时/计数器****************************************************/

typedef struct
{
	//中断寄存器
	volatile uint32_t IR;
	//定时器控制寄存器
	volatile uint32_t TCR;
	//定时器计数器
	volatile uint32_t TC;
	//预分频寄存器
	volatile uint32_t PR;
	//预分频计数器
	volatile uint32_t PC;
	//匹配控制寄存器
	volatile uint32_t MCR;
	//匹配寄存器0-3
	volatile uint32_t MR0;
	volatile uint32_t MR1;
	volatile uint32_t MR2;
	volatile uint32_t MR3;
	//捕获控制寄存器
	volatile uint32_t TxCCR;
	//捕获寄存器0-3
	volatile uint32_t CR0;
	volatile uint32_t CR1;
	volatile uint32_t CR2;
	volatile uint32_t CR3;
	//外部匹配寄存器
	volatile uint32_t EMR;
	volatile uint32_t other[12];
	//计数控制寄存器
	volatile uint32_t CTCR;
	
} TIM_TypeDef;

#define TIM0 ((TIM_TypeDef *)0xE0004000)
#define TIM1 ((TIM_TypeDef *)0xE0008000)

#define TIM_Mode_Timer										0x00
#define TIM_Mode_Conter_Rising						0x01
#define TIM_Mode_Conter_Falling						0x02
#define TIM_Mode_Conter_Rising__Falling		0x03

#define TIM_MC_Channel0	  								0x00
#define TIM_MC_Channel1	  								0x01
#define TIM_MC_Channel2	  								0x02
#define TIM_MC_Channel3	  								0x03

#define TIM_ConterMode_NONE								0x00
#define TIM_ConterMode_ITEnable						0x01
#define TIM_ConterMode_Period							0x02
#define TIM_ConterMode_Stop								0x04

//定时器基础初始化结构体
typedef struct
{
	//定时/计数器模式
	uint32_t TIM_Mode;
	//预分频
	uint32_t TIM_Prescaler;
	
}TIM_BaseInitTypeDef;

//定时器匹配模式初始化结构体
typedef struct
{
	//定时器匹配通道
	uint32_t TIM_MC_Channel;
	//匹配计数模式
	uint32_t TIM_ConterMode;
	//重装载值(匹配值)
	uint32_t TIM_Period;
}TIM_MCInitTypeDef;

//定时器输入捕获模式初始化结构体
//typedef struct
//{

//}TIM_CCInitTypeDef;
	
//定时器初始化
void TIM_BaseInit(TIM_TypeDef *TIMx, TIM_BaseInitTypeDef *TIM_BaseInitStruct);
//清除定时器中断标志位
void TIM_ClearITPendingBit(TIM_TypeDef *TIMx, uint32_t TIM_Channel);
//定时器控制函数
void TIM_Cmd(TIM_TypeDef *TIMx, FunctionalState Cmd);
//获取定时器中断状态
ITStatus TIM_GetITStatus(TIM_TypeDef *TIMx, uint32_t TIM_Channel);
//定时器匹配模式初始化
void TIM_MCInit(TIM_TypeDef *TIMx, TIM_MCInitTypeDef *TIM_MCInitStruct);


/****************************************************PWM****************************************************/

typedef struct
{
	volatile uint32_t PWMIR_;		//中断寄存器
	volatile uint32_t PWMTCR_;	//定时器控制寄存器
	volatile uint32_t PWMTC_;		//定时器计数器
	volatile uint32_t PWMPR_;		//预分频寄存器
	volatile uint32_t PWMPC_;		//预分频计数器
	volatile uint32_t PWMMCR_;	//匹配控制寄存器
	volatile uint32_t PWMMR0_;	//匹配寄存器0
	volatile uint32_t PWMMR1_;	//匹配寄存器1
	volatile uint32_t PWMMR2_;	//匹配寄存器2
	volatile uint32_t PWMMR3_;	//匹配寄存器3
	volatile uint32_t other[6];	//
	volatile uint32_t PWMMR4_;	//匹配寄存器4
	volatile uint32_t PWMMR5_;	//匹配寄存器5
	volatile uint32_t PWMMR6_;	//匹配寄存器6
	volatile uint32_t PWMPCR_;	//控制寄存器
	volatile uint32_t PWMLER_;	//锁存使能寄存器
}PWM_TypeDef;

#define PWM1	0x0002
#define PWM2	0x0004
#define PWM3	0x0008
#define PWM4	0x0010
#define PWM5	0x0020
#define PWM6	0x0040

#define PWM_Mode_Single_Edge 0x00
#define PWM_Mode_Double_Edge 0xFF

//PWM基础初始化结构体
typedef struct
{
	//PWM重装载值，用于控制PWM速率（MR0）
	uint32_t PWM_Period;
	//PWM预分频
	uint32_t PWM_Prescaler;
	
}PWM_BaseInitTypeDef;

//PWM基础初始化
void PWM_BaseInit(PWM_BaseInitTypeDef *PWM_BaseInitStruct);
//设置PWM模式
void PWM_SetMode(uint8_t PWMn, uint8_t PWM_Mode);
//设置PWM匹配值（用于设置占空比）
void PWM_SetCompare(uint8_t PWMn, uint32_t PWM_Compare);
//开启PWM输出
void PWM_Start(uint8_t PWMn);
//关闭PWM输出
void PWM_Stop(uint8_t PWMn);
//PWM控制
void PWM_Cmd(FunctionalState Cmd);



/****************************************************UART****************************************************/

typedef struct
{
	volatile uint32_t RBR_THR_DLL;
	volatile uint32_t DLM_IER;
	volatile uint32_t IIR_FCR;
	volatile uint32_t LCR;
	volatile uint32_t MCR_1;
	volatile uint32_t LSR;
	volatile uint32_t MSR_1;
	volatile uint32_t SCR;
	volatile uint32_t ACR;
	volatile uint32_t other0;
	volatile uint32_t FDR;
	volatile uint32_t other1;
	volatile uint32_t TER;
}UART_TypeDef;

#define UART_WordLength_5b 		0x00
#define UART_WordLength_6b 		0x01
#define UART_WordLength_7b 		0x02
#define UART_WordLength_8b 		0x03
	
#define UART_StopBits_1				0x00
#define UART_StopBits_2				0x04
	
#define UART_Parity_None			0x00
#define UART_Parity_Odd				0x08
#define UART_Parity_Even			0x18
#define UART_Parity_Force_1		0x28
#define UART_Parity_Force_0		0x38
	
#define UART_IT_RX_1Byte			0x01
#define UART_IT_RX_4Byte			0x41
#define UART_IT_RX_8Byte			0x81
#define UART_IT_RX_14Byte			0xC1
#define UART_IT_THRE					0x02
#define UART_IT_RX_LINE				0x04

			
#define UART_IT_RX_Available	0x04

#define UART_FIFO							0x01

#define UART0 ((UART_TypeDef *)0xE000C000)
#define UART1 ((UART_TypeDef *)0xE0010000)



typedef struct
{
	//波特率
	uint32_t BaudRate;
	//数据长度
	uint32_t WordLength;
	//停止位
	uint32_t StopBits;
	//校验位
	uint32_t Parity;
	
}UART_InitTypeDef;

//串口初始化
void UART_Init(UART_TypeDef *UARTx, UART_InitTypeDef *UART_InitStruct);
//串口发送数据
void UART_SendData(UART_TypeDef *UARTx, uint8_t data);
//串口接收数据
uint8_t UART_ReceiveData(UART_TypeDef *UARTx);
//串口中断配置
void UART_ITConfig(UART_TypeDef *UARTx, uint8_t UART_IT_Type, FunctionalState Cmd);
//串口控制函数
void UART_Cmd(UART_TypeDef *UARTx, FunctionalState Cmd);
//获取串口中断状态
ITStatus UART_GetITStatus(UART_TypeDef *UARTx, uint8_t UART_IT);
#endif


/****************************************************SPI****************************************************/


typedef struct
{
	volatile uint32_t SPCR;
	volatile uint32_t SPSR;
	volatile uint32_t SPDR;
	volatile uint32_t SPCCR;
	volatile uint32_t other[4];
	volatile uint32_t SPINT;
}SPI_TypeDef;

#define SPI_CPHA_1Edge		0x00
#define SPI_CPHA_2Edge		0x04

#define SPI_CPOL_High			0x00
#define SPI_CPOL_Low			0x10

#define SPI_Mode_Slave  	0x00
#define SPI_Mode_Master		0x20

#define SPI_FirstBit_MSB	0x00
#define SPI_FirstBit_LSB	0x40

#define SPI_STA_ABRT			0x08
#define SPI_STA_MODF			0x10
#define SPI_STA_ROVR			0x20
#define SPI_STA_WCOL			0x40
#define SPI_STA_SPIF			0x80

#define SPI0 ((SPI_TypeDef*)0xE0020000)

typedef struct
{
	//SPI的工作模式
	uint32_t SPI_Mode;
	//选择串行时钟的稳态
	uint32_t SPI_CPOL;
	//设置位捕捉的时钟活动沿
	uint32_t SPI_CPHA;
	//波特率
	uint32_t SPI_BaudRate;
	//指定数据传输从MSB位还是LSB位开始
	uint32_t SPI_FirstBit;
	//使能SPI中断
	FunctionalState SPI_IT_Cmd;
}SPI_InitTypeDef;


//SPI初始化
void SPI_Init(SPI_TypeDef *SPIx, SPI_InitTypeDef *SPI_InitStruct);
//SPI控制函数
//void SPI_Cmd(SPI_TypeDef *SPIx, FunctionalState Cmd);
//获取SPI状态
Status SPI_GetStatus(SPI_TypeDef *SPIx, uint8_t SPI_STA);
//SPI写字节
void SPI_WriteByte(SPI_TypeDef *SPIx, uint8_t data);
//SPI读字节
uint8_t SPI_ReadByte(SPI_TypeDef *SPIx);
//SPI清除中断标志位
void SPI_ClearITPendingBit(SPI_TypeDef *SPIx);


/****************************************************STC****************************************************/

typedef struct
{
	volatile uint32_t ILR_;       //中断位置寄存器
	volatile uint32_t CTC_;       //时钟节拍计数器
	volatile uint32_t CCR_;       //时钟控制寄存器
	volatile uint32_t CIIR_;      //计数器递增中断寄存器
	volatile uint32_t AMR_;       //报警屏蔽寄存器
	volatile uint32_t CTIME0_;    //完整时间寄存器 0
	volatile uint32_t CTIME1_;    //完整时间寄存器 1
	volatile uint32_t CTIME2_;    //完整时间寄存器 2
	volatile uint32_t SEC_;       //秒寄存器
	volatile uint32_t MIN_;       //分寄存器
	volatile uint32_t HOUR_;      //小时寄存器
	volatile uint32_t DOM_;       //日期（月）寄存器
	volatile uint32_t DOW_;       //星期寄存器
	volatile uint32_t DOY_;       //日期（年）寄存器
	volatile uint32_t MONTH_;     //月寄存器
	volatile uint32_t YEAR_;      //年寄存器
	volatile uint32_t other_[8];  //未使用
	volatile uint32_t ALSEC_;     //秒报警值
	volatile uint32_t ALMIN_;     //分报警值
	volatile uint32_t ALHOUR_;    //小时报警值
	volatile uint32_t ALDOM_;     //日期（月）报警值
	volatile uint32_t ALDOW_;     //星期报警值
	volatile uint32_t ALDOY_;     //日期（年）报警值
	volatile uint32_t ALMON_;     //月报警值
	volatile uint32_t ALYEAR_;		//年报警值
	volatile uint32_t PREINT_;		//预分频值，整数部分
	volatile uint32_t PREFRAC_;		//预分频值，小数部分
}RTC_TypeDef;


#define RTC_IT_CIF	0x01				//计数器增量中断
#define RTC_IT_ALF	0x02				//报警寄存器中断


#define RTC_Weekday_Sunday			0x00
#define RTC_Weekday_Monday			0x01
#define RTC_Weekday_Tuesday			0x02
#define RTC_Weekday_Wednesday		0x03
#define RTC_Weekday_Thursday		0x04
#define RTC_Weekday_Friday			0x05
#define RTC_Weekday_Saturday		0x06

#define RTC_Month_January				0x01
#define RTC_Month_February			0x02
#define RTC_Month_March					0x03
#define RTC_Month_April					0x04
#define RTC_Month_May						0x05
#define RTC_Month_June					0x06
#define RTC_Month_July					0x07
#define RTC_Month_August				0x08
#define RTC_Month_September			0x09
#define RTC_Month_October				0x0A
#define RTC_Month_November			0x0B
#define RTC_Month_December			0x0C

#define IM_IT_SEC								0x01
#define IM_IT_MIN								0x02
#define IM_IT_HOUR							0x04
#define IM_IT_DOM								0x08
#define IM_IT_DOW								0x10
#define IM_IT_DOY								0x20
#define IM_IT_MON								0x40
#define IM_IT_YEAR							0x80

#define AL_IT_SEC							0x01
#define AL_IT_MIN							0x02
#define AL_IT_HOUR						0x04
#define AL_IT_DOM							0x08
#define AL_IT_DOW							0x10
#define AL_IT_DOY							0x20
#define AL_IT_MON							0x40
#define AL_IT_YEAR						0x80


//#define RTC ((RTC_TypeDef*)0xE0024000)
extern RTC_TypeDef *RTC;

//初始化结构体(未使用)
//typedef struct
//{
//	uint32_t RTC_Prescaler_Int;	//预分频值，整数部分
//	uint32_t RTC_Prescaler_Frac;//预分频值，小数部分
//	FunctionalState RTC_Cmd;		//RTC使能
//}RTC_InitTypeDef;

//时间设置结构体
typedef struct
{
	uint16_t RTC_Hours;					//时
	uint16_t RTC_Minutes;				//分
	uint16_t RTC_Seconds;				//秒
}RTC_TimeTypeDef;

//日期设置结构体
typedef struct
{
	uint16_t RTC_Year;					//年
	uint16_t RTC_Month;					//月
	uint16_t RTC_Day;						//日
	uint16_t RTC_WeekDay;				//星期
}RTC_DateTypeDef;

//终端设置结构体
typedef struct
{
	uint8_t IM_ITConfig;				//增量中断控制
	uint8_t AL_ITConfig;				//报警中断控制
	RTC_TimeTypeDef Alarm_Time;	//设置报警时间
	RTC_DateTypeDef Alarm_Date;	//设置报警日期
}RTC_ITConfigTypeDef;

//RTC初始化
void RTC_Init(void/*RTC_InitTypeDef *RTC_InitStruct*/);
//RTC中断配置
void RTC_ITConfig(RTC_ITConfigTypeDef *RTC_ITConfigStruct);
//RTC设置时间
void RTC_SetTime(RTC_TimeTypeDef *RTC_TimeStruct);
//RTC设置日期
void RTC_SetDate(RTC_DateTypeDef *RTC_DateStruct);
//RTC清除中断标志位
void RTC_ClearITPendingBit(uint8_t RTC_IT);
//RTC获取中断状态
ITStatus RTC_GetITStatus(uint8_t RTC_IT);
//RTC使能控制
void RTC_Cmd(FunctionalState cmd);
//RTC获取时间
void RTC_GetTime(RTC_TimeTypeDef *RTC_TimeOutput);
//RTC获取日期
void RTC_GetDate(RTC_DateTypeDef *RTC_DateStruct);



/****************************************************ADC/DAC****************************************************/


typedef struct
{
	volatile uint32_t ADCR;
	volatile uint32_t ADDR;
	volatile uint32_t ADGSR_;
}ADC_TypeDef;

#define ADC_Channel0				((uint32_t)0x01)
#define ADC_Channel1				((uint32_t)0x02)
#define ADC_Channel2				((uint32_t)0x04)
#define ADC_Channel3				((uint32_t)0x08)
#define ADC_Channel4				((uint32_t)0x10)
#define ADC_Channel5				((uint32_t)0x20)
#define ADC_Channel6				((uint32_t)0x40)
#define ADC_Channel7				((uint32_t)0x80)

#define ADC_TEST_NONE				((uint32_t)0x00 << 22)
#define ADC_TEST_DIGITAL		((uint32_t)0x01 << 22)
#define ADC_TEST_DAC				((uint32_t)0x02 << 22)
#define ADC_TEST_ONCE				((uint32_t)0x03 << 22)

#define ADC_Accuracy_3Bit		((uint32_t)0x07 << 17)
#define ADC_Accuracy_4Bit		((uint32_t)0x06 << 17)
#define ADC_Accuracy_5Bit		((uint32_t)0x05 << 17)
#define ADC_Accuracy_6Bit		((uint32_t)0x04 << 17)
#define ADC_Accuracy_7Bit		((uint32_t)0x03 << 17)
#define ADC_Accuracy_8Bit		((uint32_t)0x02 << 17)
#define ADC_Accuracy_9Bit		((uint32_t)0x01 << 17)
#define ADC_Accuracy_10Bit	((uint32_t)0x00 << 17)

#define ADC_START_DISABLE		((uint32_t)0x00 << 24)
#define ADC_START_NOW				((uint32_t)0x01 << 24)

#define ADC_EDGE_Falling		((uint32_t)0x00 << 27)
#define ADC_EDGE_Rising			((uint32_t)0x01 << 27)



#define ADC0 ((ADC_TypeDef*)0xE0034000)
#define ADC1 ((ADC_TypeDef*)0xE0060000)

#define ADC_DONE 0x80000000

typedef struct
{
	uint32_t ADC_CLK_Prescaler;
	FunctionalState ADC_BurstMode;
	FunctionalState Power_Down_Mode;
	uint32_t ADC_BurstMode_Accuracy;
	uint32_t ADC_TEST;
	uint32_t ADC_ExternalTrigConv;
	uint32_t ADC_ExternalTrigEdge;
}ADC_InitTypeDef;

//ADC初始化
void ADC_Init(ADC_TypeDef *ADCn, ADC_InitTypeDef *ADC_InitStruct);
//ADC软件转换控制
void ADC_SoftwareStartConvCmd(ADC_TypeDef *ADCn, uint32_t ADC_Channel);
//获取转换状态
uint32_t ADC_GetSoftwareStartConvStatus(ADC_TypeDef *ADCn);
//获取转换值
uint16_t ADC_GetDualModeConversionValue(ADC_TypeDef *ADCn);

/****************************************************看门狗****************************************************/



typedef struct
{
	volatile uint32_t WDMOD_;
	volatile uint32_t WDTC_;
	volatile uint32_t WDFEED_;
	volatile uint32_t WDTV_;
}WDG_TypeDef;

#define WDG_Mode_NONE			0x00
#define WDG_Mode_IT_Only	0x02
#define WDG_MODE_IT_Reset	0x03

extern WDG_TypeDef *WDG;

typedef struct
{
	uint32_t WDG_Mode;
	uint32_t WDG_Timeout;
}WDG_InitTypeDef; 

//初始化看门狗
void WDG_Init(WDG_InitTypeDef *WDG_InitStruct);
//使能看门狗
void WDG_Enable(void);
//喂狗
void WDG_Feed(void);



