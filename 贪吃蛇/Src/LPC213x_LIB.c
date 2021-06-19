/**********************LPC213x_LIB.c************************/
/*	LPC213x系列自定库函数实现															 */
/*	版本：v5.0.0																					 */
/*	最后修改日期：2020.6.2  															 */
/*	作者：SpacePioneer																		 */
/***********************************************************/

#include <LPC213x_LIB.h>


/***************************GPIO****************************/

//毫秒延时函数
void delay(uint16_t ms)
{
	unsigned int i;
	for (; ms > 0; ms--)
		for (i = Fpclk / 1000; i > 0; i--);
}
//微妙延时函数
void delay_us(uint16_t us)
{
	unsigned int i;
	for (; us > 0; us--)
		for (i = Fpclk / 1000000; i > 0; i--);
}

//GPIO初始化
void GPIO_Init(GPIO_TypeDef *Px, GPIO_InitTypeDef *GPIO_InitStruct)
{
	uint32_t position = 0x00u;
	uint32_t ioposition;
	uint32_t iocurrent;
	while (((GPIO_InitStruct->GPIO_Pin) >> position) != 0x00u)
	{
		ioposition = (0x01uL << position);
		iocurrent = (uint32_t)(GPIO_InitStruct->GPIO_Pin) & ioposition;
		if (iocurrent == ioposition)
		{
			if (Px != P1)
			{
				switch (GPIO_InitStruct->GPIO_Mode)
				{
				case GPIO_MODE_INPUT:
				case GPIO_MODE_OUTPUT:
				{
					if (position < 16u)
					{
						PINSEL0 &= ~((uint32_t)0x03 << (position << 1));
					}
					else
					{
						PINSEL1 &= ~((uint32_t)0x03 << ((position - 16u) << 1));
					}
					IO0DIR &= ~((uint32_t)0x01 << position);
					IO0DIR |= (uint32_t)GPIO_InitStruct->GPIO_Mode << position;
				}break;
				case GPIO_MODE_EXTI:
				{
					switch(iocurrent)
					{
					case GPIO_PIN_1:
					case GPIO_PIN_3:
					case GPIO_PIN_7:
					case GPIO_PIN_9:
					{
						PINSEL0 |= (uint32_t)0x03 << (position << 1);
					}break;
					case GPIO_PIN_14:
					case GPIO_PIN_15:
					{
						PINSEL0 &= ~((uint32_t)0x03 << (position << 1));
						PINSEL0 |= (uint32_t)0x02 << (position << 1);
					}break;
					case GPIO_PIN_30:
					{
						PINSEL1 &= ~((uint32_t)0x03 << (position << 1));
						PINSEL1 |= (uint32_t)0x02 << (position << 1);
					}break;
					case GPIO_PIN_16:
					{
						PINSEL1 &= ~((uint32_t)0x03 << (position << 1));
						PINSEL1 |= (uint32_t)0x01 << (position << 1);
					}break;
					default:
					break;
					}
					IO0DIR &= ~((uint32_t)0x01 << position);
					IO0DIR |= (uint32_t)GPIO_MODE_INPUT << position;
				}break;
				default:
					break;
				}
			}
			else
			{
				switch (GPIO_InitStruct->GPIO_Mode)
				{
				case GPIO_MODE_INPUT:
				case GPIO_MODE_OUTPUT:
				{
					PINSEL2 = 0x0000000C;	//统一使用GPIO
					IO0DIR &= ~((uint32_t)0x01 << position);
					IO0DIR |= (uint32_t)GPIO_InitStruct->GPIO_Mode << position;
				}break;
				default:
					break;
				}
			}
		}
		position++;
	}
	
}

GPIO_PinState GPIO_ReadPin(GPIO_TypeDef *Px, uint32_t GPIO_Pin)
{
	GPIO_PinState bitstatus;
	if ((Px->IOPIN & GPIO_Pin) != (uint32_t)GPIO_PIN_RESET)
	{
		bitstatus = GPIO_PIN_SET;
	}
	else
	{
		bitstatus = GPIO_PIN_RESET;
	}

	return bitstatus;
		
}
void GPIO_WritePin(GPIO_TypeDef *Px, uint32_t GPIO_Pin, GPIO_PinState PinState)
{
	if (PinState != GPIO_PIN_RESET)
	{
		Px->IOSET = GPIO_Pin;
	}
	else
	{
		Px->IOCLR = GPIO_Pin;
	}
}

void GPIO_TogglePin(GPIO_TypeDef *Px, uint32_t GPIO_Pin)
{
	uint32_t temp = Px->IOSET & GPIO_Pin;
	Px->IOSET ^= GPIO_Pin;
	Px->IOCLR = temp;
}
/***********************************************************************************************************************************************************************/


void EXTI_Init(EXTI_InitTypeDef *EXTI_InitStruct)
{
	EXTMODE = EXTI_InitStruct->EXTI_n & EXTI_InitStruct->EXTI_Trigger;
	EXTPOLAR = EXTI_InitStruct->EXTI_n & (EXTI_InitStruct->EXTI_Trigger >> 4);
	EXTINT = EXTI_InitStruct->EXTI_n;
}

void EXTI_ClearITPendingBit(uint8_t EXTIn)
{
	EXTINT = EXTIn;
}

void EXTI_Cmd(uint32_t EXTIn, FunctionalState Cmd)
{
		if(Cmd == ENABLE)
			VICIntEnable = ((uint32_t)0x01 << EXTIn);
		else
			VICIntEnClr = ((uint32_t)0x01 << EXTIn);
}

/***********************************************************************************************************************************************************************/

void VIC_Init(VIC_InitTypeDef *VIC_InitStruct)
{
	*(VICVectAddr_BASE + VIC_InitStruct->VIC_IRQChannelPriority) = (uint32_t)(VIC_InitStruct->pCallBack);
	*(VICVectCnt_BASE + VIC_InitStruct->VIC_IRQChannelPriority) = 0x00000020 | VIC_InitStruct->VIC_IRQChannel;
	if(VIC_InitStruct->VIC_IRQChannelCmd == ENABLE)
		VICIntEnable = ((uint32_t)0x01 << VIC_InitStruct->VIC_IRQChannel);
	else
		VICIntEnClr = ((uint32_t)0x01 << VIC_InitStruct->VIC_IRQChannel);
}


/***********************************************************************************************************************************************************************/

void TIM_BaseInit(TIM_TypeDef *TIMx, TIM_BaseInitTypeDef *TIM_BaseInitStruct)
{
	TIMx->CTCR = TIM_BaseInitStruct->TIM_Mode;
	TIMx->PR = TIM_BaseInitStruct->TIM_Prescaler;
}

void TIM_ClearITPendingBit(TIM_TypeDef *TIMx, uint32_t TIM_Channel)
{
	TIMx->IR = (uint32_t)0x01 << TIM_Channel;
}

void TIM_Cmd(TIM_TypeDef *TIMx, FunctionalState Cmd)
{
	if(Cmd == ENABLE)
		TIMx->TCR |= 0x00000001; 
	else
		TIMx->TCR &= 0xFFFFFFFE;
}

ITStatus TIM_GetITStatus(TIM_TypeDef *TIMx, uint32_t TIM_Channel)
{
	if(TIMx->IR & ((uint32_t)0x01 << TIM_Channel))
		return SET;
	else
		return RESET;
}


void TIM_MCInit(TIM_TypeDef *TIMx, TIM_MCInitTypeDef *TIM_MCInitStruct)
{
	TIMx->MCR = TIM_MCInitStruct->TIM_ConterMode << (TIM_MCInitStruct->TIM_MC_Channel * 3);
	switch(TIM_MCInitStruct->TIM_MC_Channel)
	{
	case TIM_MC_Channel0:TIMx->MR0 = TIM_MCInitStruct->TIM_Period;break;
	case TIM_MC_Channel1:TIMx->MR1 = TIM_MCInitStruct->TIM_Period;break;
	case TIM_MC_Channel2:TIMx->MR2 = TIM_MCInitStruct->TIM_Period;break;
	case TIM_MC_Channel3:TIMx->MR3 = TIM_MCInitStruct->TIM_Period;break;
	default:
					break;
	}
}


/***********************************************************************************************************************************************************************/

PWM_TypeDef *PWM = ((PWM_TypeDef*)0xE0014000);

//PWM基础初始化
void PWM_BaseInit(PWM_BaseInitTypeDef *PWM_BaseInitStruct)
{
	PWM->PWMPR_ = PWM_BaseInitStruct->PWM_Prescaler;
	PWM->PWMMR0_ = PWM_BaseInitStruct->PWM_Period;
	PWM->PWMLER_ = 0x01;
	PWM->PWMMCR_ &= 0x001FFFF8;
	PWM->PWMMCR_ |= 0x00000002;
}

//设置PWM模式
void PWM_SetMode(uint8_t PWMn, uint8_t PWM_Mode)
{
	PWM->PWMPCR_ &= ~PWMn;
	PWM->PWMPCR_ |= PWMn & PWM_Mode;
}

//设置PWM匹配值（用于设置占空比）
void PWM_SetCompare(uint8_t PWMn, uint32_t PWM_Compare)
{
	switch(PWMn)
	{
	case PWM1:PWM->PWMMR1_ = PWM_Compare;break;
	case PWM2:PWM->PWMMR2_ = PWM_Compare;break;
	case PWM3:PWM->PWMMR3_ = PWM_Compare;break;
	case PWM4:PWM->PWMMR4_ = PWM_Compare;break;
	case PWM5:PWM->PWMMR5_ = PWM_Compare;break;
	case PWM6:PWM->PWMMR6_ = PWM_Compare;break;
	default:
		break;
	}
	PWM->PWMLER_ = (uint8_t)PWMn;
}

//开启PWM输出
void PWM_Start(uint8_t PWMn)
{
	PWM->PWMPCR_ |= PWMn << 8;
	switch(PWMn)
	{
	case PWM1:PINSEL0 &= ~(0x00000003 << 0);PINSEL0 |= 0x00000002 << 0;break;
	case PWM2:PINSEL0 &= ~(0x00000003 << 14);PINSEL0 |= 0x00000002 << 14;break;
	case PWM3:PINSEL0 &= ~(0x00000003 << 2);PINSEL0 |= 0x00000002 << 2;break;
	case PWM4:PINSEL0 &= ~(0x00000003 << 16);PINSEL0 |= 0x00000002 << 16;break;
	case PWM5:PINSEL0 &= ~(0x00000003 << 0);PINSEL0 |= 0x00000002 << 0;break;
	case PWM6:PINSEL1 &= ~(0x00000003 << 10);PINSEL1 |= 0x00000001 << 10;break;
	default:
		break;
	}
}

//关闭PWM输出
void PWM_Stop(uint8_t PWMn)
{
	PWM->PWMPCR_ &= ~(PWMn << 8);
}

//PWM控制
void PWM_Cmd(FunctionalState Cmd)
{
	if(Cmd != ENABLE)
	{
		PWM->PWMTCR_ = 0x02;
	}
	else
	{
		PWM->PWMTCR_ = 0x02;
		PWM->PWMTCR_ = 0x09;
	}
}


/***********************************************************************************************************************************************************************/

void UART_Init(UART_TypeDef *UARTx, UART_InitTypeDef *UART_InitStruct)
{
	uint16_t Fdiv;
	if(UARTx == UART0)
	{
		PINSEL0 &= 0xFFFFFFF0;
		PINSEL0 |= 0x00000005;
	}
	else if(UARTx == UART1)
	{
		PINSEL0 &= 0xFFF0FFFF;
		PINSEL0 |= 0x00050000;
	}
	
	UARTx->LCR = 0x80 | UART_InitStruct->WordLength | UART_InitStruct->StopBits | UART_InitStruct->Parity;
	Fdiv = (Fpclk >> 4) / UART_InitStruct->BaudRate;
	UARTx->RBR_THR_DLL = (uint8_t)(Fdiv & 0x00FF);
	UARTx->DLM_IER = (uint8_t)(Fdiv >> 8 & 0x00FF);
	UARTx->LCR &= 0x7F;	//DLAB置0
	UARTx->IIR_FCR |= 0x01;	//使能FIFO
	
	
}

void UART_SendData(UART_TypeDef *UARTx, uint8_t data)
{
	UARTx->RBR_THR_DLL = data;
	while((UARTx->LSR & 0x40) == 0);
}

uint8_t UART_ReceiveData(UART_TypeDef *UARTx)
{
	return UARTx->RBR_THR_DLL;
}

void UART_ITConfig(UART_TypeDef *UARTx, uint8_t UART_IT_Type, FunctionalState Cmd)
{
	UARTx->DLM_IER &= 0xF8;
	UARTx->IIR_FCR &= 0x3F;
	if(Cmd == ENABLE)
	{
		UARTx->DLM_IER |= UART_IT_Type & 0x07;
		UARTx->IIR_FCR |= UART_IT_Type & 0xC0;
	}
	
}

void UART_Cmd(UART_TypeDef *UARTx, FunctionalState Cmd)
{
	if(Cmd != ENABLE)
		UARTx->TER = 0x00;
	else
		UARTx->TER = 0x80;
}

ITStatus UART_GetITStatus(UART_TypeDef *UARTx, uint8_t UART_IT)
{
	if((UARTx->IIR_FCR & 0x0F) == UART_IT)
		return SET;
	else
		return RESET;
}

/***********************************************************************************************************************************************************************/

void SPI_Init(SPI_TypeDef *SPIx, SPI_InitTypeDef *SPI_InitStruct)
{
	if(SPIx == SPI0)
	{
		PINSEL0 &= 0xFFFF00FF;
    PINSEL0 |= 0x00005500;
	}
	SPIx->SPCR = SPI_InitStruct->SPI_CPHA | SPI_InitStruct->SPI_CPOL | SPI_InitStruct ->SPI_Mode | SPI_InitStruct->SPI_FirstBit | (SPI_InitStruct->SPI_IT_Cmd << 7);
	SPIx->SPCCR = Fpclk / SPI_InitStruct->SPI_BaudRate;
}

//void SPI_Cmd(SPI_TypeDef *SPIx, FunctionalState Cmd);

Status SPI_GetStatus(SPI_TypeDef *SPIx, uint8_t SPI_STA)
{
	if((SPIx->SPSR & SPI_STA) != 0x00)
		return SET;
	else
		return RESET;
}
void SPI_WriteByte(SPI_TypeDef *SPIx, uint8_t data)
{
	SPIx->SPDR = data;
	while((SPIx->SPSR & SPI_STA_SPIF) == RESET);
}

uint8_t SPI_ReadByte(SPI_TypeDef *SPIx)
{
	while((SPIx->SPSR & SPI_STA_SPIF) == SET);
	return SPIx->SPDR;
}

void SPI_ClearITPendingBit(SPI_TypeDef *SPIx)
{
	SPIx->SPINT = 0x01;
}


/***********************************************************************************************************************************************************************/
RTC_TypeDef *RTC = ((RTC_TypeDef*)0xE0024000);


//RTC初始化
void RTC_Init(void/*RTC_InitTypeDef *RTC_InitStruct*/)
{
	//预分频器设置
	RTC->PREINT_ = (int)(Fpclk / 32768) - 1;
	RTC->PREFRAC_ = Fpclk - ((RTC->PREINT_ + 1) * 32768);
	//关闭CLK 配置完成后手动开启
	RTC->CCR_ = 0x00;
	//关闭增量中断
	RTC->CIIR_ = 0x00;
	//关闭报警中断
	RTC->AMR_ = 0xFF;
}

//RTC中断配置
void RTC_ITConfig(RTC_ITConfigTypeDef *RTC_ITConfigStruct)
{
	RTC->CIIR_ 		= RTC_ITConfigStruct->IM_ITConfig & 0xFF;
	RTC->AMR_ 		= ~RTC_ITConfigStruct->AL_ITConfig & 0xFF;
	
	RTC->ALSEC_ 	= RTC_ITConfigStruct->Alarm_Time.RTC_Seconds;
	RTC->ALMIN_ 	= RTC_ITConfigStruct->Alarm_Time.RTC_Minutes;
	RTC->ALHOUR_ 	= RTC_ITConfigStruct->Alarm_Time.RTC_Hours;
	RTC->ALDOM_ 	= RTC_ITConfigStruct->Alarm_Date.RTC_Day;
	RTC->ALDOW_ 	= RTC_ITConfigStruct->Alarm_Date.RTC_WeekDay;
	RTC->ALMON_ 	= RTC_ITConfigStruct->Alarm_Date.RTC_Month;
	RTC->ALYEAR_ 	= RTC_ITConfigStruct->Alarm_Date.RTC_Year;
}

//RTC设置时间
void RTC_SetTime(RTC_TimeTypeDef *RTC_TimeStruct)
{
	RTC->SEC_  = RTC_TimeStruct->RTC_Seconds;
	RTC->MIN_  = RTC_TimeStruct->RTC_Minutes;
	RTC->HOUR_ = RTC_TimeStruct->RTC_Hours;
}

//RTC设置日期
void RTC_SetDate(RTC_DateTypeDef *RTC_DateStruct)
{
	
	int days[13] = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 };
	int temp = days[RTC_DateStruct->RTC_Month - 1];
	RTC->DOM_ 	= RTC_DateStruct->RTC_Day;
	RTC->DOW_		= RTC_DateStruct->RTC_WeekDay;
	RTC->MONTH_	= RTC_DateStruct->RTC_Month;
	RTC->YEAR_	= RTC_DateStruct->RTC_Year;
	//判断是否是闰年
	if ((RTC_DateStruct->RTC_Year % 4 == 0 && RTC_DateStruct->RTC_Year % 100 != 0) || RTC_DateStruct->RTC_Year % 400 == 0)
	{
		if (RTC_DateStruct->RTC_Month > 2)
		{
			temp++;
		}
	}
	RTC->DOY_ = temp + RTC_DateStruct->RTC_Day;
}

//RTC清除中断标志位
void RTC_ClearITPendingBit(uint8_t RTC_IT)
{
	RTC->ILR_ = RTC_IT;
}

//RTC获取中断状态
ITStatus RTC_GetITStatus(uint8_t RTC_IT)
{
	if((RTC->ILR_ & RTC_IT) != RESET)
		return SET;
	else
		return RESET;
}

//RTC使能控制
void RTC_Cmd(FunctionalState cmd)
{
	if(cmd != DISABLE)
		RTC->CCR_ = 0x01;
	else
		RTC->CCR_ = 0x00;
}

//RTC获取时间
void RTC_GetTime(RTC_TimeTypeDef *RTC_TimeOutput)
{
	RTC_TimeOutput->RTC_Hours		= (RTC->CTIME0_ & 0x001F0000) >> 16;
	RTC_TimeOutput->RTC_Minutes	= (RTC->CTIME0_ & 0x00003F00) >> 8;
	RTC_TimeOutput->RTC_Seconds	= (RTC->CTIME0_ & 0x0000003F) >> 0;
}

//RTC获取日期
void RTC_GetDate(RTC_DateTypeDef *RTC_DateStruct)
{
	RTC_DateStruct->RTC_Year		= (RTC->CTIME1_ & 0x0FFF0000) >> 16;
	RTC_DateStruct->RTC_Month		= (RTC->CTIME1_ & 0x00000F00) >> 8;
	RTC_DateStruct->RTC_Day			= (RTC->CTIME1_ & 0x0000001F) >> 0;
	RTC_DateStruct->RTC_WeekDay	= (RTC->CTIME0_ & 0x07000000) >> 24;
}


/***********************************************************************************************************************************************************************/
//ADC初始化
void ADC_Init(ADC_TypeDef *ADCn, ADC_InitTypeDef *ADC_InitStruct)
{
	uint32_t temp = 0x00000001;
	
	temp |= (ADC_InitStruct->ADC_CLK_Prescaler & 0xFF) << 8 | ADC_InitStruct->ADC_TEST;
	
	if(ADC_InitStruct->Power_Down_Mode != ENABLE)
		temp |= (uint32_t)0x01 << 21;
	
	
	
	if(ADC_InitStruct->ADC_BurstMode != ENABLE)
	{
		temp |= (uint32_t)0x01 << 16 | ADC_InitStruct->ADC_BurstMode_Accuracy;
	}
	else
	{
		if(ADC_InitStruct->ADC_ExternalTrigConv > 1)
			temp |= ADC_InitStruct->ADC_ExternalTrigConv | ADC_InitStruct->ADC_ExternalTrigEdge;
		else
			temp |= ADC_InitStruct->ADC_ExternalTrigConv;
	}
	
	ADCn->ADCR = temp;
}
//ADC软件转换控制
void ADC_SoftwareStartConvCmd(ADC_TypeDef *ADCn, uint32_t ADC_Channel)
{
	if(ADCn == ADC0)
	{
		switch(ADC_Channel)
		{
		case ADC_Channel0:PINSEL1 &= ~(0x00000003 << 22);PINSEL1 |= 0x00000001 << 22;break;
		case ADC_Channel1:PINSEL1 &= ~(0x00000003 << 24);PINSEL1 |= 0x00000001 << 24;break;
		case ADC_Channel2:PINSEL1 &= ~(0x00000003 << 26);PINSEL1 |= 0x00000001 << 26;break;
		case ADC_Channel3:PINSEL1 &= ~(0x00000003 << 28);PINSEL1 |= 0x00000001 << 28;break;
		case ADC_Channel4:PINSEL1 &= ~(0x00000003 << 18);PINSEL1 |= 0x00000001 << 18;break;
		//case ADC_Channel5:PINSEL0 &= ~(0x00000003 << 0);PINSEL0 |= 0x00000002 << 0;break;
		case ADC_Channel6:PINSEL0 &= ~(0x00000003 << 8);PINSEL0 |= 0x00000003 << 8;break;
		case ADC_Channel7:PINSEL0 &= ~(0x00000003 << 10);PINSEL0 |= 0x00000003 << 10;break;
		default:
			break;
		}
	}
	else if(ADCn == ADC1)
	{
		switch(ADC_Channel)
		{
		case ADC_Channel0:PINSEL0 &= ~(0x00000003 << 12);PINSEL0 |= 0x00000003 << 12;break;
		case ADC_Channel1:PINSEL0 &= ~(0x00000003 << 16);PINSEL0 |= 0x00000003 << 16;break;
		case ADC_Channel2:PINSEL0 &= ~(0x00000003 << 20);PINSEL0 |= 0x00000003 << 20;break;
		case ADC_Channel3:PINSEL0 &= ~(0x00000003 << 24);PINSEL0 |= 0x00000003 << 24;break;
		case ADC_Channel4:PINSEL0 &= ~(0x00000003 << 26);PINSEL0 |= 0x00000003 << 26;break;
		case ADC_Channel5:PINSEL0 &= ~(0x00000003 << 30);PINSEL0 |= 0x00000003 << 30;break;
		case ADC_Channel6:PINSEL1 &= ~(0x00000003 << 10);PINSEL1 |= 0x00000002 << 10;break;
		case ADC_Channel7:PINSEL1 &= ~(0x00000003 << 12);PINSEL1 |= 0x00000001 << 12;break;
		default:
			break;
		}
	}
	ADCn->ADCR &= 0xF8FEFFF0;
	ADCn->ADCR |= ADC_Channel;
	ADCn->ADCR |= ADC_START_NOW;
}
//获取转换状态
uint32_t ADC_GetSoftwareStartConvStatus(ADC_TypeDef *ADCn)
{
	return ADCn->ADDR & ADC_DONE;
}
//获取转换值
uint16_t ADC_GetDualModeConversionValue(ADC_TypeDef *ADCn)
{
	return (ADCn->ADDR >> 5) & 0x3FF;
}

/***********************************************************************************************************************************************************************/

WDG_TypeDef *WDG = ((WDG_TypeDef*)0xE0000000);

//初始化看门狗
void WDG_Init(WDG_InitTypeDef *WDG_InitStruct)
{
	WDG->WDTC_ = WDG_InitStruct->WDG_Timeout;
	WDG->WDMOD_ = WDG_InitStruct->WDG_Mode;
}

//使能看门狗
void WDG_Enable(void)
{
	WDG->WDFEED_ = 0x55;
	WDG->WDFEED_ = 0xAA;
}
//喂狗

void WDG_Feed(void)
{
	WDG->WDFEED_ = 0x55;
	WDG->WDFEED_ = 0xAA;
}






