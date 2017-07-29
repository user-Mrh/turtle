#include<STC15F2K60S2.h>
#include<math.h>
#include<stdio.h>

#define uchar unsigned char
#define uint unsigned int
#define r1 100.0
#define r2 165.0
#define PI 3.1415926




#define     PWM_ON          1   // 定义占空比的电平, 1 或 0

#define     PWM_OFF         (!PWM_ON)





typedef unsigned char BYTE;
typedef unsigned int WORD;

#define FOSC 24000000L          //系统频率
#define BAUD 115200             //串口波特率


#define S1_S0 0x40              //P_SW1.6
#define S1_S1 0x80              //P_SW1.7


#define NONE_PARITY     0       //无校验
#define ODD_PARITY      1       //奇校验
#define EVEN_PARITY     2       //偶校验
#define MARK_PARITY     3       //标记校验
#define SPACE_PARITY    4       //空白校验

#define PARITYBIT NONE_PARITY   //定义校验位 


#define S2RI  0x01              //S2CON.0
#define S2TI  0x02              //S2CON.1
#define S2RB8 0x04              //S2CON.2
#define S2TB8 0x08              //S2CON.3

#define S2_S0 0x01              //P_SW2.0



uint xdata tempo1,tempo2,tempo3,tempoa1,oa1,oa2,oa3,ob1,ob2,ob3,oc1,oc2,oc3,od1,od2,od3,timetemp0,timetemp1; //timetamp计时器初值缓存
int xdata ada1,ada2,ada3,adc1,adc2,adc3,adb1,adb2,adb3,add1,add2,add3;
uchar xdata pwmsig0=0,pwmsig1=0,pwm0=0,pwm1=0;
uchar  ritemp,busy=0;  //pwmsig 电平输出标识	  pwm 各路区分标识
char  loopi=0,printtemp[20];
uint  ch0,ch1,ch2,ch3,s1,s2,sbus_rx_buffer[5];

double xdata testacos,testa,testb,testc;





/*----------------------------
发送串口数据
----------------------------*/
void SendData(BYTE dat)
{
    while (busy);               //等待前面的数据发送完成
    ACC = dat;                  //获取校验位P (PSW.0)
    busy = 1;
    S2BUF = ACC;                //写数据到UART2数据寄存器
}

/*----------------------------
发送字符串
----------------------------*/
void SendString(char *s)
{
    while (*s)                  //检测字符串结束标志
    {
        SendData(*s++);         //发送当前字符
    }
}



void delay(uint xms)		//@24.000MHz
{
uint ys,is;
for(is=xms;is>0;is--)
for(ys=2040;ys>0;ys--);

}

void move(uchar ac,double x,double y,double z)             //ac=1->a   ac=2->b 
{
    if(ac==1)
    {
       tempo1=(((0.75*PI)-atan(y/x))/PI)*2000.0+500.0;
       tempo2=2000.0*(PI-atan(pow((pow(x,2.0)+pow(y,2.0)),0.5)/z)-acos(((pow(r1,2.0)+pow(x,2.0)+pow(y,2.0)+pow(z,2.0)-pow(r2,2.0))/(2.0*r1*pow((pow(x,2.0)+pow(y,2.0)+pow(z,2.0)),0.5)))))/PI+500.0;
       tempo3=500.0+acos((pow(r1,2.0)+pow(r2,2.0)-pow(x,2.0)-pow(y,2.0)-pow(z,2.0))/(2.0*r1*r2))*2000.0/PI;
       oa1=tempo1+ada1;
       oa2=tempo2+ada2;
       oa3=tempo3+ada3;
    }
    if(ac==3)
    {
       tempo1=(((0.75*PI)-atan(y/x))/PI)*2000.0+500.0;
       tempo2=2000.0*(PI-atan(pow((pow(x,2.0)+pow(y,2.0)),0.5)/z)-acos(((pow(r1,2.0)+pow(x,2.0)+pow(y,2.0)+pow(z,2.0)-pow(r2,2.0))/(2.0*r1*pow((pow(x,2.0)+pow(y,2.0)+pow(z,2.0)),0.5)))))/PI+500.0;
       tempo3=500.0+acos((pow(r1,2.0)+pow(r2,2.0)-pow(x,2.0)-pow(y,2.0)-pow(z,2.0))/(2.0*r1*r2))*2000.0/PI;
       oc1=tempo1+adc1;
       oc2=tempo2+adc2;
       oc3=tempo3+adc3;
    }
    if(ac==2)
    {
       tempo1=(((0.25*PI)+atan(y/x))/PI)*2000.0+500.0;
       tempo2=2000.0*(atan(pow((pow(x,2.0)+pow(y,2.0)),0.5)/z)+acos(((pow(r1,2.0)+pow(x,2.0)+pow(y,2.0)+pow(z,2.0)-pow(r2,2.0))/(2.0*r1*pow((pow(x,2.0)+pow(y,2.0)+pow(z,2.0)),0.5)))))/PI+500.0;
       tempo3=500.0+(PI-acos((pow(r1,2.0)+pow(r2,2.0)-pow(x,2.0)-pow(y,2.0)-pow(z,2.0))/(2.0*r1*r2)))*2000.0/PI;
       ob1=tempo1+adb1;
       ob2=tempo2+adb2;
       ob3=tempo3+adb3;
    }
    if(ac==4)
    {
       tempo1=(((0.25*PI)+atan(y/x))/PI)*2000.0+500.0;
       tempo2=2000.0*(atan(pow((pow(x,2.0)+pow(y,2.0)),0.5)/z)+acos(((pow(r1,2.0)+pow(x,2.0)+pow(y,2.0)+pow(z,2.0)-pow(r2,2.0))/(2.0*r1*pow((pow(x,2.0)+pow(y,2.0)+pow(z,2.0)),0.5)))))/PI+500.0;
       tempo3=500.0+(PI-acos((pow(r1,2.0)+pow(r2,2.0)-pow(x,2.0)-pow(y,2.0)-pow(z,2.0))/(2.0*r1*r2)))*2000.0/PI;
       od1=tempo1+add1;
       od2=tempo2+add2;
       od3=tempo3+add3;
    }
}

//uchar xdata  a1="#9";
//uchar xdata  a2="#10";
//uchar xdata  a3="#8";
//
//uchar xdata  b1="#0";
//uchar xdata  b2="#1";
//uchar xdata  b3="#2";
//
//uchar xdata  c1="#3";
//uchar xdata  c2="#4";
//uchar xdata  c3="#5";
//
//uchar xdata  d1="#6";
//uchar xdata  d2="#7";
//uchar xdata  d3="#11";

void a1 (uint pwmval)
{
    sprintf(printtemp," #9 P%d \r\n  ",pwmval) ;
    SendString(printtemp);
}











void main()
{





    ada1=-44;
    ada2=-58;
    ada3=17;
    adb1=-30;
    adb2=92;
    adb3=8;   
    adc1=-20;
    adc2=7;
    adc3=61;
    add1=33;
    add2=-40;
    add3=-37;


    
    oa1=1500+ada1;
    oa2=1500+ada2;
    oa3=1500+ada3;
    ob1=1500+adb1;
    ob2=1500+adb2;
    ob3=1500+adb3;
    oc1=1500+adc1;
    oc2=1500+adc2;
    oc3=1500+adc3;
    od1=1500+add1;
    od2=2000+add2;
    od3=1700+add3;


	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x40;		//定时器1时钟为Fosc,即1T
	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		//设定定时器1为16位自动重装方式
	TL1 = 0xBA;		//设定定时初值
	TH1 = 0xFF;		//设定定时初值
	ET1 = 0;		//禁止定时器1中断
	TR1 = 1;		//启动定时器1

	S2CON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x04;		//定时器2时钟为Fosc,即1T
	T2L = 0xC3;		//设定定时初值
	T2H = 0xFF;		//设定定时初值
	AUXR |= 0x10;		//启动定时器2
     
     P_SW2 &= ~S2_S0; 

   
  

 

    PS=1;
    IP2|=0x01; 


    

    EA = 1;         //打开总中断
    ES = 1;         //串口中断
    IE2 |= 0x01;                 //使能串口2中断


    ACC = P_SW1;
    ACC&= ~(S1_S0 | S1_S1);    //S1_S0=0 S1_S1=0
    P_SW1 = ACC;  


    delay(500);
    pwm0=1;
    pwm1=7;
    


    
        
    
    
    while(1)
    {
     a1(1500);
//    sprintf(printtemp,"oa1=%5d  ",oa1) ;
//    SendString(printtemp);
//    sprintf(printtemp,"ch1=%5d  ",ch1) ;
//    SendString(printtemp);
//    sprintf(printtemp,"ch2=%5d  ",ch2) ;
//    SendString(printtemp);
//    sprintf(printtemp,"ch3=%5d  ",ch3) ;
//    SendString(printtemp);
//    sprintf(printtemp,"s1=%5d  ",s1) ;
//    SendString(printtemp);
//    sprintf(printtemp,"s2=%5d  ",s2) ;
//    SendString(printtemp);
//    delay(1000);
//    ob1=(int)((2000.0*(ch0-364.0))/1320.0)+500.0;
//    ob2=(int)((2000.0*(ch1-364.0))/1320.0)+500.0;
//    ob3=(int)((2000.0*(ch2-364.0))/1320.0)+500.0;
//    delay(20);
//    move(1,110,110,90);
//    move(3,110,110,90);
//    move(2,110,110,90);
//    move(4,110,110,90);
    delay(1000);

    }


}





/*----------------------------
UART 中断服务程序
-----------------------------*/
void Uart() interrupt 4 
{
    if (RI)
    {


        RI = 0;
        

        if(loopi>=0&loopi<=5)
        {
        sbus_rx_buffer[loopi]= SBUF;
        ch0 = (sbus_rx_buffer[0]| (sbus_rx_buffer[1] << 8)) & 0x07ff;               //!< Channel 0
        ch1 = ((sbus_rx_buffer[1] >> 3) | (sbus_rx_buffer[2] << 5)) & 0x07ff;       //!< Channel 1
        ch2 = ((sbus_rx_buffer[2] >> 6) | (sbus_rx_buffer[3] << 2) |                 //!< Channel 2
        (sbus_rx_buffer[4] << 10)) & 0x07ff;
        ch3 = ((sbus_rx_buffer[4] >> 1) | (sbus_rx_buffer[5] << 7)) & 0x07ff;       //!< Channel 3
        s1 = ((sbus_rx_buffer[5] >> 4)& 0x000C) >> 2;                                //!< Switch left
        s2 = ((sbus_rx_buffer[5] >> 4)& 0x0003);
        } 
        loopi++; 
        if(loopi==18)loopi=0; 

                                   //!< Switch right

                     //缓存串口数据
                   
    }
    if (TI)
    {
        TI = 0;                 //清除TI位
        busy=0;               //清忙标志
    }
}

/*----------------------------
UART2 中断服务程序
-----------------------------*/
void Uart2() interrupt 8 
{
    if (S2CON & S2RI)
    {
        S2CON &= ~S2RI;         //清除S2RI位
    }
    if (S2CON & S2TI)
    {
        S2CON &= ~S2TI;         //清除S2TI位
        busy = 0;               //清忙标志
    }
}


