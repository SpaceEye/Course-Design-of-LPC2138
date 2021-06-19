/**********************LPC213x_LIB.h************************/
/*	LPC213xϵ����ؼĴ������ṹ��ͺ�������							 	 */
/*	�汾��v5.0.0																					 */
/*	�汾�Ķ�������PWM��ADC�����Ź�����ع��ܣ��޸���֪bug  */
/*	����޸����ڣ�2020.6.2																 */
/*	���ߣ�SpacePioneer																		 */
/***********************************************************/
#include <stdint.h>
#include <LPC213X.H>

#ifndef _LPC213X_LIB_H_
#define _LPC213X_LIB_H_

#define Fpclk 12000000					//��������/Hz		//ʹ��11.0592MHz�������Ϊ11059200

void delay(uint16_t ms);				//������ʱ����
void delay_us(uint16_t us);			//΢����ʱ����

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

//����״̬
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

//GPIO��ʼ���ṹ�嶨��
typedef struct
{
	//GPIO����
	uint32_t GPIO_Pin;
	//GPIOģʽ
	uint32_t GPIO_Mode;
	
}GPIO_InitTypeDef;


//�������Ŷ���
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

//GPIO��ʼ��
void GPIO_Init(GPIO_TypeDef *Px, GPIO_InitTypeDef *GPIO_InitStruct);
//������
GPIO_PinState GPIO_ReadPin(GPIO_TypeDef *Px, uint32_t GPIO_Pin);
//д����
void GPIO_WritePin(GPIO_TypeDef *Px, uint32_t GPIO_Pin, GPIO_PinState PinState);
//��ת����״̬
void GPIO_TogglePin(GPIO_TypeDef *Px, uint32_t GPIO_Pin);


/************************************************�ⲿ�ж�***********************************************************/


//EINT��ʼ���ṹ�嶨��
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

//�ⲿ�жϳ�ʼ��
void EXTI_Init(EXTI_InitTypeDef *EXTI_InitStruct);
//����жϱ�־λ
void EXTI_ClearITPendingBit(uint8_t EXTIn);
//�ж��������
void EXTI_Cmd(uint32_t EXTIn, FunctionalState Cmd);


/****************************************************�����жϿ���****************************************************/

//�����ж�ͨ��
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
	//�ж�����ͨ��
	uint32_t VIC_IRQChannel;
	//�ж����ȼ�
	uint32_t VIC_IRQChannelPriority;
	//����
	FunctionalState VIC_IRQChannelCmd;
	//�жϷ�����
	void (*pCallBack)(void)__irq;
	
}VIC_InitTypeDef;

//VIC��ʼ������
void VIC_Init(VIC_InitTypeDef *VIC_InitStruct);

/****************************************************��ʱ/������****************************************************/

typedef struct
{
	//�жϼĴ���
	volatile uint32_t IR;
	//��ʱ�����ƼĴ���
	volatile uint32_t TCR;
	//��ʱ��������
	volatile uint32_t TC;
	//Ԥ��Ƶ�Ĵ���
	volatile uint32_t PR;
	//Ԥ��Ƶ������
	volatile uint32_t PC;
	//ƥ����ƼĴ���
	volatile uint32_t MCR;
	//ƥ��Ĵ���0-3
	volatile uint32_t MR0;
	volatile uint32_t MR1;
	volatile uint32_t MR2;
	volatile uint32_t MR3;
	//������ƼĴ���
	volatile uint32_t TxCCR;
	//����Ĵ���0-3
	volatile uint32_t CR0;
	volatile uint32_t CR1;
	volatile uint32_t CR2;
	volatile uint32_t CR3;
	//�ⲿƥ��Ĵ���
	volatile uint32_t EMR;
	volatile uint32_t other[12];
	//�������ƼĴ���
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

//��ʱ��������ʼ���ṹ��
typedef struct
{
	//��ʱ/������ģʽ
	uint32_t TIM_Mode;
	//Ԥ��Ƶ
	uint32_t TIM_Prescaler;
	
}TIM_BaseInitTypeDef;

//��ʱ��ƥ��ģʽ��ʼ���ṹ��
typedef struct
{
	//��ʱ��ƥ��ͨ��
	uint32_t TIM_MC_Channel;
	//ƥ�����ģʽ
	uint32_t TIM_ConterMode;
	//��װ��ֵ(ƥ��ֵ)
	uint32_t TIM_Period;
}TIM_MCInitTypeDef;

//��ʱ�����벶��ģʽ��ʼ���ṹ��
//typedef struct
//{

//}TIM_CCInitTypeDef;
	
//��ʱ����ʼ��
void TIM_BaseInit(TIM_TypeDef *TIMx, TIM_BaseInitTypeDef *TIM_BaseInitStruct);
//�����ʱ���жϱ�־λ
void TIM_ClearITPendingBit(TIM_TypeDef *TIMx, uint32_t TIM_Channel);
//��ʱ�����ƺ���
void TIM_Cmd(TIM_TypeDef *TIMx, FunctionalState Cmd);
//��ȡ��ʱ���ж�״̬
ITStatus TIM_GetITStatus(TIM_TypeDef *TIMx, uint32_t TIM_Channel);
//��ʱ��ƥ��ģʽ��ʼ��
void TIM_MCInit(TIM_TypeDef *TIMx, TIM_MCInitTypeDef *TIM_MCInitStruct);


/****************************************************PWM****************************************************/

typedef struct
{
	volatile uint32_t PWMIR_;		//�жϼĴ���
	volatile uint32_t PWMTCR_;	//��ʱ�����ƼĴ���
	volatile uint32_t PWMTC_;		//��ʱ��������
	volatile uint32_t PWMPR_;		//Ԥ��Ƶ�Ĵ���
	volatile uint32_t PWMPC_;		//Ԥ��Ƶ������
	volatile uint32_t PWMMCR_;	//ƥ����ƼĴ���
	volatile uint32_t PWMMR0_;	//ƥ��Ĵ���0
	volatile uint32_t PWMMR1_;	//ƥ��Ĵ���1
	volatile uint32_t PWMMR2_;	//ƥ��Ĵ���2
	volatile uint32_t PWMMR3_;	//ƥ��Ĵ���3
	volatile uint32_t other[6];	//
	volatile uint32_t PWMMR4_;	//ƥ��Ĵ���4
	volatile uint32_t PWMMR5_;	//ƥ��Ĵ���5
	volatile uint32_t PWMMR6_;	//ƥ��Ĵ���6
	volatile uint32_t PWMPCR_;	//���ƼĴ���
	volatile uint32_t PWMLER_;	//����ʹ�ܼĴ���
}PWM_TypeDef;

#define PWM1	0x0002
#define PWM2	0x0004
#define PWM3	0x0008
#define PWM4	0x0010
#define PWM5	0x0020
#define PWM6	0x0040

#define PWM_Mode_Single_Edge 0x00
#define PWM_Mode_Double_Edge 0xFF

//PWM������ʼ���ṹ��
typedef struct
{
	//PWM��װ��ֵ�����ڿ���PWM���ʣ�MR0��
	uint32_t PWM_Period;
	//PWMԤ��Ƶ
	uint32_t PWM_Prescaler;
	
}PWM_BaseInitTypeDef;

//PWM������ʼ��
void PWM_BaseInit(PWM_BaseInitTypeDef *PWM_BaseInitStruct);
//����PWMģʽ
void PWM_SetMode(uint8_t PWMn, uint8_t PWM_Mode);
//����PWMƥ��ֵ����������ռ�ձȣ�
void PWM_SetCompare(uint8_t PWMn, uint32_t PWM_Compare);
//����PWM���
void PWM_Start(uint8_t PWMn);
//�ر�PWM���
void PWM_Stop(uint8_t PWMn);
//PWM����
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
	//������
	uint32_t BaudRate;
	//���ݳ���
	uint32_t WordLength;
	//ֹͣλ
	uint32_t StopBits;
	//У��λ
	uint32_t Parity;
	
}UART_InitTypeDef;

//���ڳ�ʼ��
void UART_Init(UART_TypeDef *UARTx, UART_InitTypeDef *UART_InitStruct);
//���ڷ�������
void UART_SendData(UART_TypeDef *UARTx, uint8_t data);
//���ڽ�������
uint8_t UART_ReceiveData(UART_TypeDef *UARTx);
//�����ж�����
void UART_ITConfig(UART_TypeDef *UARTx, uint8_t UART_IT_Type, FunctionalState Cmd);
//���ڿ��ƺ���
void UART_Cmd(UART_TypeDef *UARTx, FunctionalState Cmd);
//��ȡ�����ж�״̬
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
	//SPI�Ĺ���ģʽ
	uint32_t SPI_Mode;
	//ѡ����ʱ�ӵ���̬
	uint32_t SPI_CPOL;
	//����λ��׽��ʱ�ӻ��
	uint32_t SPI_CPHA;
	//������
	uint32_t SPI_BaudRate;
	//ָ�����ݴ����MSBλ����LSBλ��ʼ
	uint32_t SPI_FirstBit;
	//ʹ��SPI�ж�
	FunctionalState SPI_IT_Cmd;
}SPI_InitTypeDef;


//SPI��ʼ��
void SPI_Init(SPI_TypeDef *SPIx, SPI_InitTypeDef *SPI_InitStruct);
//SPI���ƺ���
//void SPI_Cmd(SPI_TypeDef *SPIx, FunctionalState Cmd);
//��ȡSPI״̬
Status SPI_GetStatus(SPI_TypeDef *SPIx, uint8_t SPI_STA);
//SPIд�ֽ�
void SPI_WriteByte(SPI_TypeDef *SPIx, uint8_t data);
//SPI���ֽ�
uint8_t SPI_ReadByte(SPI_TypeDef *SPIx);
//SPI����жϱ�־λ
void SPI_ClearITPendingBit(SPI_TypeDef *SPIx);


/****************************************************STC****************************************************/

typedef struct
{
	volatile uint32_t ILR_;       //�ж�λ�üĴ���
	volatile uint32_t CTC_;       //ʱ�ӽ��ļ�����
	volatile uint32_t CCR_;       //ʱ�ӿ��ƼĴ���
	volatile uint32_t CIIR_;      //�����������жϼĴ���
	volatile uint32_t AMR_;       //�������μĴ���
	volatile uint32_t CTIME0_;    //����ʱ��Ĵ��� 0
	volatile uint32_t CTIME1_;    //����ʱ��Ĵ��� 1
	volatile uint32_t CTIME2_;    //����ʱ��Ĵ��� 2
	volatile uint32_t SEC_;       //��Ĵ���
	volatile uint32_t MIN_;       //�ּĴ���
	volatile uint32_t HOUR_;      //Сʱ�Ĵ���
	volatile uint32_t DOM_;       //���ڣ��£��Ĵ���
	volatile uint32_t DOW_;       //���ڼĴ���
	volatile uint32_t DOY_;       //���ڣ��꣩�Ĵ���
	volatile uint32_t MONTH_;     //�¼Ĵ���
	volatile uint32_t YEAR_;      //��Ĵ���
	volatile uint32_t other_[8];  //δʹ��
	volatile uint32_t ALSEC_;     //�뱨��ֵ
	volatile uint32_t ALMIN_;     //�ֱ���ֵ
	volatile uint32_t ALHOUR_;    //Сʱ����ֵ
	volatile uint32_t ALDOM_;     //���ڣ��£�����ֵ
	volatile uint32_t ALDOW_;     //���ڱ���ֵ
	volatile uint32_t ALDOY_;     //���ڣ��꣩����ֵ
	volatile uint32_t ALMON_;     //�±���ֵ
	volatile uint32_t ALYEAR_;		//�걨��ֵ
	volatile uint32_t PREINT_;		//Ԥ��Ƶֵ����������
	volatile uint32_t PREFRAC_;		//Ԥ��Ƶֵ��С������
}RTC_TypeDef;


#define RTC_IT_CIF	0x01				//�����������ж�
#define RTC_IT_ALF	0x02				//�����Ĵ����ж�


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

//��ʼ���ṹ��(δʹ��)
//typedef struct
//{
//	uint32_t RTC_Prescaler_Int;	//Ԥ��Ƶֵ����������
//	uint32_t RTC_Prescaler_Frac;//Ԥ��Ƶֵ��С������
//	FunctionalState RTC_Cmd;		//RTCʹ��
//}RTC_InitTypeDef;

//ʱ�����ýṹ��
typedef struct
{
	uint16_t RTC_Hours;					//ʱ
	uint16_t RTC_Minutes;				//��
	uint16_t RTC_Seconds;				//��
}RTC_TimeTypeDef;

//�������ýṹ��
typedef struct
{
	uint16_t RTC_Year;					//��
	uint16_t RTC_Month;					//��
	uint16_t RTC_Day;						//��
	uint16_t RTC_WeekDay;				//����
}RTC_DateTypeDef;

//�ն����ýṹ��
typedef struct
{
	uint8_t IM_ITConfig;				//�����жϿ���
	uint8_t AL_ITConfig;				//�����жϿ���
	RTC_TimeTypeDef Alarm_Time;	//���ñ���ʱ��
	RTC_DateTypeDef Alarm_Date;	//���ñ�������
}RTC_ITConfigTypeDef;

//RTC��ʼ��
void RTC_Init(void/*RTC_InitTypeDef *RTC_InitStruct*/);
//RTC�ж�����
void RTC_ITConfig(RTC_ITConfigTypeDef *RTC_ITConfigStruct);
//RTC����ʱ��
void RTC_SetTime(RTC_TimeTypeDef *RTC_TimeStruct);
//RTC��������
void RTC_SetDate(RTC_DateTypeDef *RTC_DateStruct);
//RTC����жϱ�־λ
void RTC_ClearITPendingBit(uint8_t RTC_IT);
//RTC��ȡ�ж�״̬
ITStatus RTC_GetITStatus(uint8_t RTC_IT);
//RTCʹ�ܿ���
void RTC_Cmd(FunctionalState cmd);
//RTC��ȡʱ��
void RTC_GetTime(RTC_TimeTypeDef *RTC_TimeOutput);
//RTC��ȡ����
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

//ADC��ʼ��
void ADC_Init(ADC_TypeDef *ADCn, ADC_InitTypeDef *ADC_InitStruct);
//ADC���ת������
void ADC_SoftwareStartConvCmd(ADC_TypeDef *ADCn, uint32_t ADC_Channel);
//��ȡת��״̬
uint32_t ADC_GetSoftwareStartConvStatus(ADC_TypeDef *ADCn);
//��ȡת��ֵ
uint16_t ADC_GetDualModeConversionValue(ADC_TypeDef *ADCn);

/****************************************************���Ź�****************************************************/



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

//��ʼ�����Ź�
void WDG_Init(WDG_InitTypeDef *WDG_InitStruct);
//ʹ�ܿ��Ź�
void WDG_Enable(void);
//ι��
void WDG_Feed(void);



