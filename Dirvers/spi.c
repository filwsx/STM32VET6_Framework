#include "spi.h"

#define temp_SPI_max 200
#define zhentou 0xaa		//帧头
#define zhenwei 0x0d		//帧尾

#if EN_SPI1  == 1

u8 temp_SPI1[temp_SPI_max];

void SPI1_IRQHandler(){
	static int point_Count_SPI1 = 0;
	if(SPI_I2S_GetITStatus(SPI1, SPI_I2S_IT_RXNE) == SET)
	{ 
			while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET); //检查指定的SPI标志位设置与否:发送缓存空标志位
			SPI_I2S_SendData(SPI1,temp_SPI1[point_Count_SPI1]); //通过外设SPIx发送一个数据
		
			while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET); //检查指定的SPI标志位设置与否:接受缓存非空标志位
			temp_SPI1[point_Count_SPI1]=SPI_I2S_ReceiveData(SPI1); //返回通过SPIx最近接收的数据
		
			SPI_I2S_ClearITPendingBit(SPI1,SPI_I2S_IT_RXNE);//clear SPI_rx_FLAG
			point_Count_SPI1++;
   }
}

//SPI1 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
void SPI1_ReadWriteString(u8 *temp)
{	
	SPI1_CS=0;	
	static int point_Count_SPI1;
		for(point_Count_SPI1=0;temp[point_Count_SPI1]!=zhenwei;point_Count_SPI1++){			 	
			while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET); //检查指定的SPI标志位设置与否:发送缓存空标志位	  
			SPI_I2S_SendData(SPI1,temp[point_Count_SPI1]); //通过外设SPIx发送一个数据

			while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET); //检查指定的SPI标志位设置与否:接受缓存非空标志位	
			temp[point_Count_SPI1]=SPI_I2S_ReceiveData(SPI1); //返回通过SPIx最近接收的数据				
		}
	SPI1_CS=1;
}
#endif

#if EN_SPI2  == 1

u8 temp_SPI2[temp_SPI_max];

void SPI2_IRQHandler(){
	static int point_Count_SPI2 = 0;
	if(SPI_I2S_GetITStatus(SPI2, SPI_I2S_IT_RXNE) == SET)
	{ 
			while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET); //检查指定的SPI标志位设置与否:发送缓存空标志位
			SPI_I2S_SendData(SPI2,temp_SPI2[point_Count_SPI2]); //通过外设SPIx发送一个数据
		
			while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET); //检查指定的SPI标志位设置与否:接受缓存非空标志位
			temp_SPI2[point_Count_SPI2]=SPI_I2S_ReceiveData(SPI2); //返回通过SPIx最近接收的数据
		
			SPI_I2S_ClearITPendingBit(SPI2,SPI_I2S_IT_RXNE);//clear SPI_rx_FLAG
			point_Count_SPI2++;
   }
}

//SPI2 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
void SPI2_ReadWriteString(u8 *temp)
{	
	SPI2_CS=0;
	
	static int point_Count_SPI2;
		for(point_Count_SPI2=0;temp[point_Count_SPI2]!=zhenwei;point_Count_SPI2++){			 	
			while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET); //检查指定的SPI标志位设置与否:发送缓存空标志位	  
			SPI_I2S_SendData(SPI2,temp[point_Count_SPI2]); //通过外设SPIx发送一个数据

			while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET); //检查指定的SPI标志位设置与否:接受缓存非空标志位	
			temp[point_Count_SPI2]=SPI_I2S_ReceiveData(SPI2); //返回通过SPIx最近接收的数据				
		}	
		
	SPI2_CS=1;	
}

#endif

#if EN_SPI3  == 1

u8 temp_SPI3[temp_SPI_max];

void SPI3_IRQHandler(){
	static int point_Count_SPI3 = 0;
	if(SPI_I2S_GetITStatus(SPI3, SPI_I2S_IT_RXNE) == SET)
	{ 
			while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET); //检查指定的SPI标志位设置与否:发送缓存空标志位
			SPI_I2S_SendData(SPI3,temp_SPI3[point_Count_SPI3]); //通过外设SPIx发送一个数据
		
			while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE) == RESET); //检查指定的SPI标志位设置与否:接受缓存非空标志位
			temp_SPI3[point_Count_SPI3]=SPI_I2S_ReceiveData(SPI3); //返回通过SPIx最近接收的数据
		
			SPI_I2S_ClearITPendingBit(SPI3,SPI_I2S_IT_RXNE);//clear SPI_rx_FLAG
			point_Count_SPI3++;
   }
}

void SPI3_ReadWriteString(u8 *temp)
{	
	SPI3_CS=0;
	
	static int point_Count_SPI3;
		for(point_Count_SPI3=0;temp[point_Count_SPI3]!=zhenwei;point_Count_SPI3++){			 	
			while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET); //检查指定的SPI标志位设置与否:发送缓存空标志位	  
			SPI_I2S_SendData(SPI3,temp[point_Count_SPI3]); //通过外设SPIx发送一个数据

			while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE) == RESET); //检查指定的SPI标志位设置与否:接受缓存非空标志位	
			temp[point_Count_SPI3]=SPI_I2S_ReceiveData(SPI3); //返回通过SPIx最近接收的数据				
		}
		
	SPI3_CS=1;	
}

#endif


//SPI口初始化
void SPIX_Init(SPI_TypeDef* SPIX,uint16_t SPI_Mode,u8 SPI_BaudRatePrescaler)
{	 
 	GPIO_InitTypeDef GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //PB13/14/15复用推挽输出 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	if(SPIX==((SPI_TypeDef *) SPI1_BASE)){

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE );//PORTB时钟使能
		RCC_APB2PeriphClockCmd(	RCC_APB2Periph_SPI1,  ENABLE );//SPI1时钟使能
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
		GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA
		GPIO_SetBits(GPIOA,GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);  //PB13/14/15上拉
		
	}else if(SPIX==((SPI_TypeDef *) SPI2_BASE)){
	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE );//PORTB时钟使能
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE );//SPI2时钟使能
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
		GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB
		GPIO_SetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);  //PB13/14/15上拉
		
	}else if(SPIX==((SPI_TypeDef *) SPI3_BASE)){
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE );//PORTB时钟使能
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3,ENABLE );//SPI2时钟使能
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
		GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB
		GPIO_SetBits(GPIOB,GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5);  //PB13/14/15上拉
	}

	if(SPI_Mode==SPI_Mode_Slave){
		
		NVIC_InitTypeDef NVIC_InitStructure;
		
		if(SPIX==((SPI_TypeDef *) SPI1_BASE))
		{
			NVIC_InitStructure.NVIC_IRQChannel=SPI1_IRQn;
		}
		else if(SPIX==((SPI_TypeDef *) SPI2_BASE))
		{
			NVIC_InitStructure.NVIC_IRQChannel=SPI2_IRQn;
		}
		else if(SPIX==((SPI_TypeDef *) SPI3_BASE))
		{
			NVIC_InitStructure.NVIC_IRQChannel=SPI3_IRQn;
		}
		
		SPI_I2S_ITConfig(SPIX, SPI_I2S_IT_RXNE, ENABLE);
		SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;		//设置SPI工作模式:设置为主SPI
		SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
		
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority=2;
		NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
		NVIC_Init(&NVIC_InitStructure);
	}
	
	if(SPI_Mode==SPI_Mode_Master){
		GPIO_InitTypeDef GPIO_InitStructure2;
		
		GPIO_InitStructure2.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出
		GPIO_InitStructure2.GPIO_Speed = GPIO_Speed_50MHz;
		
		if(SPIX==((SPI_TypeDef *) SPI1_BASE))
		{
			GPIO_InitStructure2.GPIO_Pin = GPIO_Pin_4;  // PB12 推挽
			GPIO_Init(GPIOA, &GPIO_InitStructure2);
			GPIO_SetBits(GPIOA,GPIO_Pin_4);
		}
		else if(SPIX==((SPI_TypeDef *) SPI2_BASE))
		{
			GPIO_InitStructure2.GPIO_Pin = GPIO_Pin_12;
			GPIO_Init(GPIOB, &GPIO_InitStructure2);
			GPIO_SetBits(GPIOB,GPIO_Pin_12);
		}
		else if(SPIX==((SPI_TypeDef *) SPI3_BASE))
		{
			GPIO_InitStructure2.GPIO_Pin = GPIO_Pin_15;
			GPIO_Init(GPIOA, &GPIO_InitStructure2);
			GPIO_SetBits(GPIOA,GPIO_Pin_15);
		}

		SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
		SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	}
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//串行同步时钟的空闲状态为高电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//串行同步时钟的第二个跳变沿（上升或下降）数据被采样
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//定义波特率预分频的值:波特率预分频值为256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式

	assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
	SPIX->CR1&=0XFFC7;
	SPIX->CR1|=SPI_BaudRatePrescaler;	//设置SPI2速度
	
	SPI_Init(SPIX, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
	SPI_Cmd(SPIX,ENABLE); 
}
