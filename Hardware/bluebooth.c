#include "bluebooth.h"

void bluebooth_Init(int bound){
	USART1_init(bound,0);//=====初始化串口1
	USART1_send("test\r\n");
	
	//蓝牙测试是否成功
//	if(test()==1){
//		//蓝牙搜索
//		int i =search();
//		//是否搜索到蓝牙
//		if(i>0){
//			//蓝牙设备1连接是否成功
//			if(contact(1)==1){
//				send("successful\r\n");
//			}else{
//				send("failed contact(1)\r\n");
//			}
//		}else{
//			send("failed search()\r\n");
//		}
//	}else{
//		send("failed test()\r\n");
//	}
}

//作者：郭浩
//时间：20190625
//给本地蓝牙发送测试指令，判断蓝工作正常
//正常返回1，否则为0
int test(){
	int i;
	while((USART1->SR&0X40)==0);
	USART1->DR = 0x41;
	while((USART1->SR&0X40)==0);
	USART1->DR = 0x54;
	while((USART1->SR&0X40)==0);
	USART1->DR = 0x0D;
	while((USART1->SR&0X40)==0);
	USART1->DR = 0x0A;
	//等待接收，可能导致死循环
	while(USART1_RX_flag!=1);
	if(USART1_RX_BUF[0]=='O'&&USART1_RX_BUF[1]=='K'){
		i = 1;
	}else{
		i = 0;
	}
	USART1_RX_flag = 0 ;
	USART1_RX_count = 0 ;
	return i;
}

//作者：郭浩
//时间：20190625
//蓝发搜索，返回搜索设备个数
//以下是某个成功搜索案例的返回语句：
//OK
//+INQS
//+INQ:1 0x00158700768D
//+INQE
//Devices Found 1
//+Connecting  0x00158700768D
//+Connected  0x00158700768D
int search(){
	int i = 0;
	USART1_send("AT+INQ\r\n");
	while(USART1_RX_flag!=1);
	if(USART1_RX_BUF[0]=='O'&&USART1_RX_BUF[1]=='K'){
		USART1_RX_flag = 0 ;
		USART1_RX_count = 0 ;
		//通过语句格式可知Devices Found 1 代表搜索结束
		while(USART1_RX_BUF[0]!='D'){
			//通过统计+来计数
			if(USART1_RX_flag==1&&USART1_RX_BUF[0]=='+'){
				i++;
				//将计数过的+修改，防止重复计数
				USART1_RX_BUF[0]='-';
				USART1_RX_flag = 0 ;
				USART1_RX_count = 0 ;
			}
		}
	}
	return i-2;
}

//作者：郭浩
//时间：20190625
//蓝发连，参数是要连第几个设备，返回为空
//以下是某个成功搜索案例的返回语句：
//以下是某个成功连案例的返回语句：
//+Connecting  0x00158700768D
//+Connected  0x00158700768D
int contact(int i){
	switch (i){
		case 1: USART1_send("AT+CONN1\r\n"); break;
		case 2: USART1_send("AT+CONN2\r\n"); break;
		case 3: USART1_send("AT+CONN3\r\n"); break;
		case 4: USART1_send("AT+CONN4\r\n"); break;
		case 5: USART1_send("AT+CONN5\r\n"); break;
		case 6: USART1_send("AT+CONN6\r\n"); break;
		case 7: USART1_send("AT+CONN7\r\n"); break;
		default: USART1_send("AT+CONN1\r\n"); break;
	}
	while(USART1_RX_flag!=1);
	//计数两+可知连完成
	if(USART1_RX_BUF[0]=='+'){
		USART1_RX_flag = 0 ;
		USART1_RX_count = 0 ;
		while(USART1_RX_flag!=1);
		if(USART1_RX_BUF[0]=='+'){
			i=1;
		}else{
			i=0;
		}
		USART1_RX_flag = 0 ;
		USART1_RX_count = 0 ;
	}
	return i;
}

//作者：郭浩
//时间：20190630  1853完成
//上位机控制下位机的协议
void protocol(void){
	//判断是否接收完成
	if(USART1_RX_flag==1){
		//判断是不是控制命令C
		if(USART1_RX_BUF[0]=='C'){
			//判断是不是开关命令
			if(USART1_RX_BUF[1]=='0'){
				if(USART1_RX_BUF[2]=='0'){
					//倒立摆启动
					//Flag_Stop=0;
				}else if(USART1_RX_BUF[2]=='1'){
					//倒立摆停止
					//Flag_Stop=1;
				}else{
					//null;
				}
			}
			//判断是不是角度旋转命令
			else if(USART1_RX_BUF[1]=='1'){
				//判断正反转
				if(USART1_RX_BUF[2]=='0'){
					//赋值正速度给角度速度变量
					//speed = (float)USART1_RX_BUF[3];
				}else if(USART1_RX_BUF[2]=='1'){
					//负值反速度给角度速度变量
					//speed = 0 - (float)USART1_RX_BUF[3];
				}else{
					//null;
				}
			}
			//判断是不是位置旋转命令
			else if(USART1_RX_BUF[1]=='2'){
				if(USART1_RX_BUF[2]=='0'){
					//velocity = (float)USART1_RX_BUF[3];
				}else if(USART1_RX_BUF[2]=='1'){
					//velocity = 0 - (float)USART1_RX_BUF[3];
				}else{
					//null;
				}
			}
			//判断是不是角度PID参数控制
			else if(USART1_RX_BUF[1]=='3'){
				//判断是哪个参数：KP,KI,KD
				if(USART1_RX_BUF[2]=='P'){
					//Balance_KP0= getNumber(3);
				}else if(USART1_RX_BUF[2]=='I'){
					//Balance_KI0= getNumber(3);					
				}else if(USART1_RX_BUF[2]=='D'){
					//Balance_KD0= getNumber(3);
				}else{
					//null;
				}				
			}			
			//判断是不是位置PID参数控制
			else if(USART1_RX_BUF[1]=='4'){			
				if(USART1_RX_BUF[2]=='P'){					
					//Position_KP0= getNumber(3);
				}else if(USART1_RX_BUF[2]=='I'){
					//Position_KI0= getNumber(3);					
				}else if(USART1_RX_BUF[2]=='D'){
					//Position_KD0= getNumber(3);
				}else{
					//null;
				}				
			}else{			
			}		
		}else{		
		}
		//串口接收标志清零
		USART1_RX_flag =0;
		USART1_RX_count=0;
	}
}

//			switch ((int)USART1_RX_BUF[1])
//				case 1: break;
//				case 2: break;
//				case 3: break;
//				case 4: break;

//作者：郭浩
//时间：20190630  18xx完成
//获取协议中数字部分的值，由$标志为结束位
//一般数字部分起始位为3，返回值为float
float getNumber(u8 i){
	float result=0;
	while(USART1_RX_BUF[i]!='$'){
		result = (float)USART1_RX_BUF[i]+result*10;
		i++;
	}
	return result;
}

//作者：郭浩
//时间：20190630  18xx完成
//用于调试，查看接收的语句
void returnProtocol(void){
	//判断是否接收完成
	if(USART1_RX_flag==1){
		int i=0;
		int j;
		j=USART1_RX_count;
		while((USART1->SR&0X40)==0);
		USART1->DR = 'R';
		for(;i<j;i++){
			while((USART1->SR&0X40)==0);
			USART1->DR = USART1_RX_BUF[i];
		}
		while((USART1->SR&0X40)==0);
		USART1->DR = 0x0D;
		while((USART1->SR&0X40)==0);
		USART1->DR = 0x0A;
	}
}
