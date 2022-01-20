#include "usart.h"	  

//根据传入字符的\r结尾来判断字符长度
int chart_count(u8 *content){
	int i = 0;
	while(content[i]!=zhenwei){
		i++;
	}
	return i;
}

#if EN_USART1  == 1
u8 USART1_RX_BUF[USART_REC_LEN];

//在使用完接收缓存之后要把count和flag置零
//接收计数
u8 USART1_RX_count = 0;
//接收完成标志
u8 USART1_RX_flag = 0;

void USART1_init(u32 bound,u8 Remap_sign){
		USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
		if(Remap_sign == 0 )
		{	
			//使能USART1_clock
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	
			//GPIO端口设置
			//USART1_TX   GPIOA.9//复用推挽输出
			GPIO_Init_One_AF(GPIOA,GPIO_Pin_9, GPIO_Mode_AF_PP,GPIO_Speed_50MHz);
			//USART1_RX	  GPIOA.10初始化
			GPIO_Init_One_IN(GPIOA,GPIO_Pin_10,GPIO_Mode_IN_FLOATING);
		}
		else
		{
			//1.打开重映射时钟以及串口1时钟
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_USART1,ENABLE);
			//2.I/O口重映射开启.可使用goto 查看有哪些端口重映射 
			GPIO_PinRemapConfig(GPIO_Remap_USART1,ENABLE);
			//3.配制重映射引脚, 这里只需配置重映射后的I/O,原来的不需要去配置.
			//USART1_TX PB6
			GPIO_Init_One_AF(GPIOB,GPIO_Pin_6, GPIO_Mode_AF_PP,GPIO_Speed_50MHz);
			//USART1_RX PB7
			GPIO_Init_One_IN(GPIOB,GPIO_Pin_7,GPIO_Mode_IN_FLOATING);
		}
		//Usart1 NVIC 配置
		NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
		NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
		
		 //USART 初始化设置
		USART_InitStructure.USART_BaudRate = bound;//串口波特率
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
		USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
		USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

		USART_Init(USART1, &USART_InitStructure); //初始化串口1
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启串口接受中断
		USART_Cmd(USART1, ENABLE);                    //使能串口1 
}

//发送字符串以第一个\r视为结尾

void USART1_send(u8 *content)
{
	int i=0, j;
	
	j=chart_count(content);
	
	for(i=0;i<j;i++){
		while((USART1->SR&0X40)==0);
		USART1->DR = content[i];
	}
	
	while((USART1->SR&0X40)==0);
	USART1->DR = 0x0D;
	while((USART1->SR&0X40)==0);
	USART1->DR = 0x0A;
}

void USART1_send16(u16 *content,int j)
{
	int i;
	
	for(i=0;i<j;i++){
		while((USART1->SR&0X40)==0);
		USART1->DR = content[i]>>8;
		while((USART1->SR&0X40)==0);
		USART1->DR = content[i]&0x00ff;;
	}
	
	while((USART1->SR&0X40)==0);
	USART1->DR = 0x0D;
	while((USART1->SR&0X40)==0);
	USART1->DR = 0x0A;
}

void USART1_send_len(u8 *content,int j){
	int i=0;
	
	if(j==0)
		j=chart_count((u8*)content);
	
	for(i=0;i<j;i++){
		while((USART1->SR&0X40)==0);
		USART1->DR = content[i];
	}
	
	while((USART1->SR&0X40)==0);
	USART1->DR = 0x0D;
	while((USART1->SR&0X40)==0);
	USART1->DR = 0x0A;
}

void USART1_IRQHandler(){
	u8 res;	
	if(USART1->SR&(1<<5))	//接收到数据
	{	 
		res=USART1->DR; 
		if(USART1_RX_flag == 0){
			USART1_RX_BUF[USART1_RX_count]=res;
			if(res==zhenwei){
				USART1_RX_flag=1;//接收完成
			}else{
				USART1_RX_count++;
			}//save the zhenwei use sign the flag of end
		}else if(res==zhentou){
			USART1_RX_flag=0;
			USART1_RX_count=0;
			USART1_RX_BUF[USART1_RX_count]=res;
			USART1_RX_count++;
		}
	}
}
#endif

#if EN_USART2  == 1
u8 USART2_RX_BUF[USART_REC_LEN];
u8 USART2_RX_count = 0;
u8 USART2_RX_flag = 0;

void USART2_init(u32 bound,u8 Remap_sign){
		USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
		if(Remap_sign == 0)
		{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);			
			GPIO_Init_One_AF(GPIOA,GPIO_Pin_2, GPIO_Mode_AF_PP,GPIO_Speed_50MHz);
			GPIO_Init_One_IN(GPIOA,GPIO_Pin_3,GPIO_Mode_IN_FLOATING);

		}
		else
		{
					RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
					RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
					GPIO_PinRemapConfig(GPIO_Remap_USART2,ENABLE);
					GPIO_Init_One_AF(GPIOD,GPIO_Pin_5, GPIO_Mode_AF_PP,GPIO_Speed_50MHz);
					GPIO_Init_One_IN(GPIOD,GPIO_Pin_6,GPIO_Mode_IN_FLOATING);
		}
		NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
		
		USART_InitStructure.USART_BaudRate = bound;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	

		USART_Init(USART2, &USART_InitStructure);
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
		USART_Cmd(USART2, ENABLE);
}


void USART2_send(u8 *content)
{
	int i=0, j;
	
	j=chart_count(content);
	
	for(i=0;i<j;i++){
		while((USART2->SR&0X40)==0);
		USART2->DR = content[i];
	}
	
	while((USART2->SR&0X40)==0);
	USART2->DR = 0x0D;
	while((USART2->SR&0X40)==0);
	USART2->DR = 0x0A;
}

void USART2_send16(u16 *content,int j)
{
	int i;
	
	for(i=0;i<j;i++){
		while((USART2->SR&0X40)==0);
		USART2->DR = content[i]>>8;
		while((USART2->SR&0X40)==0);
		USART2->DR = content[i]&0x00ff;;
	}
	
	while((USART2->SR&0X40)==0);
	USART2->DR = 0x0D;
	while((USART2->SR&0X40)==0);
	USART2->DR = 0x0A;
}

void USART2_send_len(u8 *content,int j){
	int i=0;
	
	if(j==0)
		j=chart_count((u8*)content);
	
	for(i=0;i<j;i++){
		while((USART2->SR&0X40)==0);
		USART2->DR = content[i];
	}
	
	while((USART2->SR&0X40)==0);
	USART2->DR = 0x0D;
	while((USART2->SR&0X40)==0);
	USART2->DR = 0x0A;
}

void USART2_IRQHandler(){
	u8 res;	
	if(USART2->SR&(1<<5))
	{
		res=USART2->DR; 
		if(USART2_RX_flag == 0){
			USART2_RX_BUF[USART2_RX_count]=res;
			if(res==zhenwei){
				USART2_RX_flag=1;
			}else{
				USART2_RX_count++;
			}
		}else if(res==zhentou){
			USART2_RX_flag=0;
			USART2_RX_count=0;
			USART2_RX_BUF[USART2_RX_count]=res;
			USART2_RX_count++;
		}
	}
}
#endif

#if EN_USART3  == 1
u8 USART3_RX_BUF[USART_REC_LEN];
u8 USART3_RX_count = 0;
u8 USART3_RX_flag = 0;
void USART3_init(u32 bound,u8 Remap_sign){
		USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
		if(Remap_sign == 0)
		{		
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);			
			GPIO_Init_One_AF(GPIOB,GPIO_Pin_10, GPIO_Mode_AF_PP,GPIO_Speed_50MHz);
			GPIO_Init_One_IN(GPIOB,GPIO_Pin_11,GPIO_Mode_IN_FLOATING);
		}
		else if(Remap_sign == 1)
		{
					RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
					RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
					GPIO_PinRemapConfig(GPIO_PartialRemap_USART3,ENABLE);
					GPIO_Init_One_AF(GPIOC,GPIO_Pin_10, GPIO_Mode_AF_PP,GPIO_Speed_50MHz);
					GPIO_Init_One_IN(GPIOC,GPIO_Pin_11,GPIO_Mode_IN_FLOATING);
			
		}else if (Remap_sign == 2){
					RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
					RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
					GPIO_PinRemapConfig(GPIO_FullRemap_USART3,ENABLE);
					GPIO_Init_One_AF(GPIOD,GPIO_Pin_8, GPIO_Mode_AF_PP,GPIO_Speed_50MHz);
					GPIO_Init_One_IN(GPIOD,GPIO_Pin_9,GPIO_Mode_IN_FLOATING);
		}
		NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
		
		USART_InitStructure.USART_BaudRate = bound;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	

		USART_Init(USART3, &USART_InitStructure);
		USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
		USART_Cmd(USART3, ENABLE);
}
void USART3_send(u8 *content)
{
	int i=0, j;
	
	j=chart_count(content);
	
	for(i=0;i<j;i++){
		while((USART3->SR&0X40)==0);
		USART3->DR = content[i];
	}
	
	while((USART3->SR&0X40)==0);
	USART3->DR = 0x0D;
	while((USART3->SR&0X40)==0);
	USART3->DR = 0x0A;
}

void USART3_send16(u16 *content,int j)
{
	int i;
	
	for(i=0;i<j;i++){
		while((USART3->SR&0X40)==0);
		USART3->DR = content[i]>>8;
		while((USART3->SR&0X40)==0);
		USART3->DR = content[i]&0x00ff;;
	}
	
	while((USART3->SR&0X40)==0);
	USART3->DR = 0x0D;
	while((USART3->SR&0X40)==0);
	USART3->DR = 0x0A;
}

void USART3_send_len(u8 *content,int j){
	int i=0;
	
	if(j==0)
		j=chart_count((u8*)content);
	
	for(i=0;i<j;i++){
		while((USART3->SR&0X40)==0);
		USART3->DR = content[i];
	}
	
	while((USART3->SR&0X40)==0);
	USART3->DR = 0x0D;
	while((USART3->SR&0X40)==0);
	USART3->DR = 0x0A;
}

void USART3_IRQHandler(){
	u8 res;	
	if(USART3->SR&(1<<5))
	{
		res=USART3->DR; 
		if(USART3_RX_flag == 0){
			USART3_RX_BUF[USART3_RX_count]=res;
			if(res==zhenwei){
				USART3_RX_flag=1;
			}else{
				USART3_RX_count++;
			}
		}else if(res==zhentou){
			USART3_RX_flag=0;
			USART3_RX_count=0;
			USART3_RX_BUF[USART3_RX_count]=res;
			USART3_RX_count++;
		}
	}
}
#endif

#if EN_UART4  == 1
u8 UART4_RX_BUF[USART_REC_LEN];
u8 UART4_RX_count = 0;
u8 UART4_RX_flag = 0;

void UART4_init(u32 bound){
		USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;		
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);			
		GPIO_Init_One_AF(GPIOC,GPIO_Pin_10, GPIO_Mode_AF_PP,GPIO_Speed_50MHz);
		GPIO_Init_One_IN(GPIOC,GPIO_Pin_11,GPIO_Mode_IN_FLOATING);

		NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
		
		USART_InitStructure.USART_BaudRate = bound;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	

		USART_Init(UART4, &USART_InitStructure);
		USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
		USART_Cmd(UART4, ENABLE);
}

void UART4_send(u8 *content)
{
	int i=0, j;
	
	j=chart_count(content);
	
	for(i=0;i<j;i++){
		while((UART4->SR&0X40)==0);
		UART4->DR = content[i];
	}
	
	while((UART4->SR&0X40)==0);
	UART4->DR = 0x0D;
	while((UART4->SR&0X40)==0);
	UART4->DR = 0x0A;
}

void UART4_send16(u16 *content,int j)
{
	int i;
	
	for(i=0;i<j;i++){
		while((UART4->SR&0X40)==0);
		UART4->DR = content[i]>>8;
		while((UART4->SR&0X40)==0);
		UART4->DR = content[i]&0x00ff;;
	}
	
	while((UART4->SR&0X40)==0);
	UART4->DR = 0x0D;
	while((UART4->SR&0X40)==0);
	UART4->DR = 0x0A;
}

void UART4_send_len(u8 *content,int j){
	int i=0;
	
	if(j==0)
		j=chart_count((u8*)content);
	
	for(i=0;i<j;i++){
		while((UART4->SR&0X40)==0);
		UART4->DR = content[i];
	}
	
	while((UART4->SR&0X40)==0);
	UART4->DR = 0x0D;
	while((UART4->SR&0X40)==0);
	UART4->DR = 0x0A;
}

void UART4_IRQHandler(){
	u8 res;	
	if(UART4->SR&(1<<5))
	{
		res=UART4->DR; 
		if(UART4_RX_flag == 0){
			UART4_RX_BUF[UART4_RX_count]=res;
			if(res==zhenwei){
				UART4_RX_flag=1;
			}else{
				UART4_RX_count++;
			}
		}else if(res==zhentou){
			UART4_RX_flag=0;
			UART4_RX_count=0;
			UART4_RX_BUF[UART4_RX_count]=res;
			UART4_RX_count++;
		}
	}
}
#endif

#if EN_UART5  == 1
u8 UART5_RX_BUF[USART_REC_LEN];
u8 UART5_RX_count = 0;
u8 UART5_RX_flag = 0;

void UART5_init(u32 bound){
		USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;		
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);			
		GPIO_Init_One_AF(GPIOC,GPIO_Pin_12, GPIO_Mode_AF_PP,GPIO_Speed_50MHz);
		GPIO_Init_One_IN(GPIOD,GPIO_Pin_2,GPIO_Mode_IN_FLOATING);

		NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
		
		USART_InitStructure.USART_BaudRate = bound;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	

		USART_Init(UART5, &USART_InitStructure);
		USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);
		USART_Cmd(UART5, ENABLE);
}


void UART5_send(u8 *content)
{
	int i=0, j;
	
	j=chart_count(content);
	
	for(i=0;i<j;i++){
		while((UART5->SR&0X40)==0);
		UART5->DR = content[i];
	}
	
	while((UART5->SR&0X40)==0);
	UART5->DR = 0x0D;
	while((UART5->SR&0X40)==0);
	UART5->DR = 0x0A;
}

void UART5_send16(u16 *content,int j)
{
	int i;
	
	for(i=0;i<j;i++){
		while((UART5->SR&0X40)==0);
		UART5->DR = content[i]>>8;
		while((UART5->SR&0X40)==0);
		UART5->DR = content[i]&0x00ff;;
	}
	
	while((UART5->SR&0X40)==0);
	UART5->DR = 0x0D;
	while((UART5->SR&0X40)==0);
	UART5->DR = 0x0A;
}

void UART5_send_len(u8 *content,int j){
	int i=0;
	
	if(j==0)
		j=chart_count((u8*)content);
	
	for(i=0;i<j;i++){
		while((UART5->SR&0X40)==0);
		UART5->DR = content[i];
	}
	
	while((UART5->SR&0X40)==0);
	UART5->DR = 0x0D;
	while((UART5->SR&0X40)==0);
	UART5->DR = 0x0A;
}

void UART5_IRQHandler(){
	u8 res;	
	if(UART5->SR&(1<<5))
	{
		res=UART5->DR; 
		if(UART5_RX_flag == 0){
			UART5_RX_BUF[UART5_RX_count]=res;
			if(res==zhenwei){
				UART5_RX_flag=1;
			}else{
				UART5_RX_count++;
			}
		}else if(res==zhentou){
			UART5_RX_flag=0;
			UART5_RX_count=0;
			UART5_RX_BUF[UART5_RX_count]=res;
			UART5_RX_count++;
		}
	}
}
#endif
