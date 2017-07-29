#include<STC15F2K60S2.h>
#include<math.h>
#include<stdio.h>

#define uchar unsigned char
#define uint unsigned int
#define r1 100.0
#define r2 162.0
#define PI 3.1415926


   //28Mhz

#define     PWM_ON          1   // 定义占空比的电平, 1 或 0

#define     PWM_OFF         (!PWM_ON)





//typedef unsigned char xdata BYTE;
//typedef unsigned int xdata WORD;

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



uint xdata tempo1,tempo2,tempo3,oa1,oa2,oa3,ob1,ob2,ob3,oc1,oc2,oc3,od1,od2,od3,timetemp0,timetemp1; //timetamp计时器初值缓存
int xdata ada1,ada2,ada3,adc1,adc2,adc3,adb1,adb2,adb3,add1,add2,add3;
uchar xdata walktemp=1,holderchange,pwmsig0=0,pwmsig1=0,pwm0=0,pwm1=0;  //walktemp  步态标识  holderchange 云台信号更新 pwmsig 电平输出标识	  pwm 各路区分标识

uchar idata loopi=0,ritemp,busy=0;
char idata printtemp[40];
uint idata holder1,holder2,ch0,ch1,ch2,ch3,s1,s2,sbus_rx_buffer[5];

double xdata testacos,testa,testb,testc;





/*----------------------------
发送串口数据
----------------------------*/
void SendData(uchar dat)
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
uint is;
uchar i, j;

for(is=xms;is>0;is--)
{

	i = 28;
	j = 57;
	do
	{
		while (--j);
	} while (--i);
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
void d(uint pwmval1,uint pwmval2,uint pwmval3,uint movetime)
{
    sprintf(printtemp," #6 P%d #7 P%d #11 P%d T%d \r\n  ",pwmval1,pwmval2,pwmval3,movetime) ;
    SendString(printtemp);

}
void b(uint pwmval1,uint pwmval2,uint pwmval3,uint movetime)
{
    sprintf(printtemp," #0 P%d  #1 P%d  #2 P%d T%d \r\n  ",pwmval1,pwmval2,pwmval3,movetime) ;
    SendString(printtemp);

}

void c(uint pwmval1,uint pwmval2,uint pwmval3,uint movetime)
{
    sprintf(printtemp," #3 P%d  #4 P%d  #5 P%d  T%d \r\n  ",pwmval1,pwmval2,pwmval3,movetime) ;
    SendString(printtemp);

}

void a(uint pwmval1,uint pwmval2,uint pwmval3,uint movetime)
{
    sprintf(printtemp," #9 P%d #10 P%d  #8 P%d T%d \r\n  ",pwmval1,pwmval2,pwmval3,movetime) ;
    SendString(printtemp);

}


void move(uchar ac,double x,double y,double z,uint timetemp)             //ac=1->a   ac=2->b 
{
    if(ac==1)
    {  
       x-=27.0*sin(atan(y/x));
       y-=27.0*cos(atan(y/x));
       tempo1=(((0.75*PI)-atan(y/x))/PI)*1600.0+700.0;
       tempo2=1600.0*(PI-atan(pow((pow(x,2.0)+pow(y,2.0)),0.5)/z)-acos((pow(r1,2.0)+pow(x,2.0)+pow(y,2.0)+pow(z,2.0)-pow(r2,2.0))/(2.0*r1*pow((pow(x,2.0)+pow(y,2.0)+pow(z,2.0)),0.5))))/PI+700.0;
       tempo3=700.0+(acos((pow(r1,2.0)+pow(r2,2.0)-pow(x,2.0)-pow(y,2.0)-pow(z,2.0))/(2.0*r1*r2))-0.15)*1600.0/PI;
       a(tempo1+ada1,tempo2+ada2,tempo3+ada3,timetemp);
       

    }
    if(ac==3)
    {
       x-=27.0*sin(atan(y/x));
       y-=27.0*cos(atan(y/x));
       tempo1=(((0.75*PI)-atan(y/x))/PI)*1600.0+700.0;
       tempo2=1600.0*(PI-atan(pow((pow(x,2.0)+pow(y,2.0)),0.5)/z)-acos((pow(r1,2.0)+pow(x,2.0)+pow(y,2.0)+pow(z,2.0)-pow(r2,2.0))/(2.0*r1*pow((pow(x,2.0)+pow(y,2.0)+pow(z,2.0)),0.5))))/PI+700.0;
       tempo3=700.0+(acos((pow(r1,2.0)+pow(r2,2.0)-pow(x,2.0)-pow(y,2.0)-pow(z,2.0))/(2.0*r1*r2))-0.15)*1600.0/PI;
       c(tempo1+adc1,tempo2+adc2,tempo3+adc3,timetemp);
    }
    if(ac==2)
    {
       x-=27.0*sin(atan(y/x));
       y-=27.0*cos(atan(y/x));
       tempo1=(((0.25*PI)+atan(y/x))/PI)*1600.0+700.0;
       tempo2=1600.0*(atan(pow((pow(x,2.0)+pow(y,2.0)),0.5)/z)+acos(((pow(r1,2.0)+pow(x,2.0)+pow(y,2.0)+pow(z,2.0)-pow(r2,2.0))/(2.0*r1*pow((pow(x,2.0)+pow(y,2.0)+pow(z,2.0)),0.5)))))/PI+700.0;
       tempo3=700.0+(PI+0.15-acos((pow(r1,2.0)+pow(r2,2.0)-pow(x,2.0)-pow(y,2.0)-pow(z,2.0))/(2.0*r1*r2)))*1600.0/PI;
       b(tempo1+adb1,tempo2+adb2,tempo3+adb3,timetemp);

    }
    if(ac==4)
    {
       x-=27.0*sin(atan(y/x));
       y-=27.0*cos(atan(y/x));
       tempo1=(((0.25*PI)+atan(y/x))/PI)*1600.0+700.0;
       tempo2=1600.0*(atan(pow((pow(x,2.0)+pow(y,2.0)),0.5)/z)+acos(((pow(r1,2.0)+pow(x,2.0)+pow(y,2.0)+pow(z,2.0)-pow(r2,2.0))/(2.0*r1*pow((pow(x,2.0)+pow(y,2.0)+pow(z,2.0)),0.5)))))/PI+700.0;
       tempo3=700.0+(PI+0.15-acos((pow(r1,2.0)+pow(r2,2.0)-pow(x,2.0)-pow(y,2.0)-pow(z,2.0))/(2.0*r1*r2)))*1600.0/PI;
       d(tempo1+add1,tempo2+add2,tempo3+add3,timetemp);

    }
}


void holder()
{


    if(s1==1)                                       //归位
    {
        holder1=1350;
        holder2=1350;
        sprintf(printtemp,"#13 P1350 #12 P1350\r\n") ;
        SendString(printtemp); 

    }


    if(s1==3)                         //水平模式
    {
        
        if(ch0<1024)      //云台左转
        {
            holderchange=1;
            if((1024-ch0)<=220)holder1+=3;
            else    if((1024-ch0)<=440&(1024-ch0)>220)holder1+=18;
            else holder1+=50;
            if(holder1>2250)holder1=2250;
    
    
        }
    
         else   if(ch0>1024)
        {
            holderchange=1;
            if((ch0-1024)<=220)holder1-=3;
            else    if((ch0-1024)<=440&(ch0-1024)>220)holder1-=18;
            else holder1-=50;
            if(holder1<500)holder1=500;
    
    
        }
        

        if(holderchange==1)
        {
            holderchange=0;
            sprintf(printtemp,"#13 P%d #12 P%d\r\n",holder1,holder2) ;
            SendString(printtemp); 
        }
    }



    if(s1==2)                           //3d模式
    {
        
        if(ch0<1024)      //云台左转
        {
            holderchange=1;
            if((1024-ch0)<=220)holder1+=3;
            else    if((1024-ch0)<=440&(1024-ch0)>220)holder1+=18;
            else holder1+=50;
            if(holder1>2250)holder1=2250;
    
    
        }
    
         else   if(ch0>1024)
        {
            holderchange=1;
            if((ch0-1024)<=220)holder1-=3;
            else    if((ch0-1024)<=440&(ch0-1024)>220)holder1-=18;
            else holder1-=50;
            if(holder1<500)holder1=500;
    
    
        }
        
        if(ch1>1024)
        {
            holderchange=1;
            if((ch1-1024)<=220)holder2-=3;
            else    if((ch1-1024)<=440&(ch1-1024)>220)holder2-=10;
            else holder2-=18;
            if(holder2<=1000)holder2=1000;
    
        }
    
        else   if(ch1<1024)
        {
            holderchange=1;
            if((1024-ch1)<=220)holder2+=3;
            else    if((1024-ch1)<=440&(1024-ch1)>220)holder2+=10;
            else holder2+=18;
            if(holder2>2100)holder2=2100;
    
     
    
        }
        
        if(holderchange==1)
        {
            holderchange=0;
            sprintf(printtemp,"#13 P%d #12 P%d\r\n",holder1,holder2) ;
            SendString(printtemp); 
        }
    }
    

       
    
 
}
void walk()
{
    if (walktemp==1)
    {
            move(1,70,121,100,1000);
            move(3,70,121,70,1);
            move(2,70,53,100,1000);
            move(4,70,53,100,1000);
            move(3,70,121,100,1);
            walktemp++;   
      
    }
     else   if (walktemp==2)
    {
            move(1,70,19,70,1);
            move(3,70,87,100,1000);
            move(2,70,87,100,1000);
            move(4,70,19,100,1000);
            move(1,70,19,100,1);
            walktemp++;   
      
    }
    else if (walktemp==3)
    {
            move(4,70,121,70,1);
            move(1,70,53,100,1000);
            move(3,70,53,100,1000);
            move(2,70,121,100,1000);
            move(4,70,121,100,1);
            walktemp++;   
      
    }
    else  
    {
            move(2,70,19,70,1);
            move(1,70,87,100,1000);
            move(3,70,19,100,1000);
            move(4,70,87,100,1000);
            move(2,70,19,100,1);
            walktemp=1;   
      
    }    
}











void main()
{





    ada1=-30;
    ada2=-31;
    ada3=15;
    adb1=-9;
    adb2=-78;
    adb3=-11;   
    adc1=-30;
    adc2=8;
    adc3=-33;
    add1=33;
    add2=-100;
    add3=-69;


    


    holder1=1350;
    holder2=1350;


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
    

    move(1,70,86.67,100,500);
    move(2,70,20.00,100,500);
    move(3,70,20.00,100,500);
    move(4,70,86.67,100,500);
    
        
//    s2=3;
    
    while(1)
    {       
    

       
        while(s2==1)
        {
            holder();
            delay(20);
        }
        while(s2==3)
        {
         sprintf(printtemp,"PL 0 SQ 0 SM %f  \r\n",(((double)ch3-1024.0)/660.0)*200.0) ;

         SendString(printtemp);   

        }
        while(s2==2)
        {
            move(1,50,115,70,500);
            move(2,50,115,70,500);
            move(3,50,115,70,500);
            move(4,50,115,70,500);
        }

//     a1(1500);
//    sprintf(printtemp,"oa1=%5d  ",oa1) ;
//    SendString(printtemp);
//        sprintf(printtemp,"ch1=%5d  ",ch1) ;
//        SendString(printtemp);
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
//    delay(1000);

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


