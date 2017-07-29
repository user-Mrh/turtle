#include<STC15F2K60S2.h>
#include<math.h>
#define uchar unsigned char
#define uint unsigned int

#define     PWM_ON          1   // 定义占空比的电平, 1 或 0

#define     PWM_OFF         (!PWM_ON)

uint oa1,oa2,oa3,ob1,ob2,ob3,oc1,oc2,oc3,od1,od2,od3,timetemp0,timetemp1; //timetamp计时器初值缓存
int ada1,ada2,ada3,adc1,adc2,adc3,adb1,adb2,adb3,add1,add2,add3;
uchar pwmsig0=0,pwmsig1=0,pwm0=0,pwm1=0;  //pwmsig 电平输出标识	  pwm 各路区分标识



sbit pwma1=P2^6; //舵机输出引脚
sbit pwma2=P2^7;
sbit pwma3=P1^0;
sbit pwmb1=P1^1;
sbit pwmb2=P1^2;
sbit pwmb3=P1^3;
sbit pwmc1=P1^4;
sbit pwmc2=P1^5;
sbit pwmc3=P1^6;
sbit pwmd1=P1^7;
sbit pwmd2=P5^4;
sbit pwmd3=P5^5;


void delay(uint xms)		//@24.000MHz
{
uint ys,is;
for(is=xms;is>0;is--)
for(ys=2040;ys>0;ys--);

}




void main()
{


    pwma1=PWM_OFF;    
    pwma2=PWM_OFF;    
    pwma3=PWM_OFF;
    pwmb1=PWM_OFF;    
    pwmb2=PWM_OFF;    
    pwmb3=PWM_OFF;
    pwmc1=PWM_OFF;    
    pwmc2=PWM_OFF;    
    
    pwmc3=PWM_OFF; 
    pwmd1=PWM_OFF;    
    pwmd2=PWM_OFF;    
    pwmd3=PWM_OFF; 



    ada1=150;
    ada2=-50;
    ada3=110;
    adb1=0;
    adb2=-25;
    adb3=90;   
    adc1=10;
    adc2=170;
    adc3=25;
    add1=60;
    add2=0;
    add3=70;


    
    oa1=1500+ada1;
    oa2=1000+ada2;
    oa3=1200+ada3;
    ob1=1500+adb1;
    ob2=2000+adb2;
    ob3=1700+adb3;
    oc1=1500+adc1;
    oc2=1000+adc2;
    oc3=1200+adc3;
    od1=1500+add1;
    od2=2000+add2;
    od3=1700+add3;




    AUXR &=  (0<<7);    // Timer0 set as 1T/12 mode
    TMOD &= ~(1<<2);    // Timer0 set as Timer
    TMOD |=   0x11;      // Timer0 set as 16 bits 
    TH0 = (65536-1500) / 256;  //Timer0 Load
    TL0 = (65536-1500) % 256;
    TH1 = (65536-3000) / 256;  //Timer0 Load
    TL1 = (65536-3000) % 256;
    ET0 = 1;        //Timer0 Interrupt Enable
    IP=0xa;        //高优先级
    TR0 = 1;        //Timer0 Run
    ET1 = 1;        //Timer1 Interrupt Enable
    TR1 = 1; 
    EA = 1;         //打开总中断



    delay(500);
    pwm0=1;
    pwm1=7;    
    
    while(1);


}


void T0_time() interrupt   1	  //定时器0 左舵机
{
    if(pwm0==1)                   //pwm频道
    {
        if(pwma1==PWM_OFF)
        {
            pwma1=PWM_ON;
            timetemp0=2*oa1;
        }
        else
        {
            pwma1=PWM_OFF;
            timetemp0=5000-2*oa1;
            pwm0++;
        }
        
        TH0 = (65536-timetemp0) / 256;  //Timer0 Load
        TL0 = (65536-timetemp0) % 256;
        
        


    }
    
    
    else if(pwm0==2)                   //pwm频道
    {
        if(pwma2==PWM_OFF)
        {
            pwma2=PWM_ON;
            timetemp0=2*oa2;
        }
        else
        {
            pwma2=PWM_OFF;
            timetemp0=5000-2*oa2;
            pwm0++;
        }

        
        TH0 = (65536-timetemp0) / 256;  //Timer0 Load
        TL0 = (65536-timetemp0) % 256;

        


    }

    
    else if(pwm0==3)                   //pwm频道
    {
        if(pwma3==PWM_OFF)
        {
            pwma3=PWM_ON;
            timetemp0=2*oa3;
        }
        else
        {
            pwma3=PWM_OFF;
            timetemp0=5000-2*oa3;
            pwm0++;
        }
        
        TH0 = (65536-timetemp0) / 256;  //Timer0 Load
        TL0 = (65536-timetemp0) % 256;

      


    }

    else if(pwm0==4)                   //pwm频道
    {
        if(pwmb1==PWM_OFF)
        {
            pwmb1=PWM_ON;
            timetemp0=2*ob1;
        }
        else
        {
            pwmb1=PWM_OFF;
            timetemp0=5000-2*ob1;
            pwm0++;
        }
        
        TH0 = (65536-timetemp0) / 256;  //Timer0 Load
        TL0 = (65536-timetemp0) % 256;

        


    }



    else if(pwm0==5)                   //pwm频道
    {
        if(pwmb2==PWM_OFF)
        {
            pwmb2=PWM_ON;
            timetemp0=2*ob2;
        }
        else
        {
            pwmb2=PWM_OFF;
            timetemp0=5000-2*ob2;
            pwm0++;
        }
        
        TH0 = (65536-timetemp0) / 256;  //Timer0 Load
        TL0 = (65536-timetemp0) % 256;

       


    }


    else if(pwm0==6)                   //pwm频道
    {
        if(pwmb3==PWM_OFF)
        {
            pwmb3=PWM_ON;
            timetemp0=2*ob3;
        }
        else
        {
            pwmb3=PWM_OFF;
            timetemp0=5000-2*ob3;
            pwm0=1;
        }
        
        TH0 = (65536-timetemp0) / 256;  //Timer0 Load
        TL0 = (65536-timetemp0) % 256;

      


    }


}







void T1_time() interrupt   3	  //定时器1	
 {


    if(pwm1==7)                   //pwm频道
    {
        if(pwmc1==PWM_OFF)
        {
            pwmc1=PWM_ON;
            timetemp1=2*oc1;
        }
        else
        {
            pwmc1=PWM_OFF;
            timetemp1=5000-2*oc1;
            pwm1++;
        }
        
        TH1 = (65536-timetemp1) / 256;  //Timer0 Load
        TL1 = (65536-timetemp1) % 256;

        


    }


    else  if(pwm1==8)                 //pwm频道
    {
        if(pwmc2==PWM_OFF)
        {
            pwmc2=PWM_ON;
            timetemp1=2*oc2;
        }
        else
        {
            pwmc2=PWM_OFF;
            timetemp1=5000-2*oc2;
            pwm1++;
        }
        
        TH1 = (65536-timetemp1) / 256;  //Timer0 Load
        TL1 = (65536-timetemp1) % 256;

        


   }
       else  if(pwm1==9)                 //pwm频道
    {
        if(pwmc3==PWM_OFF)
        {
            pwmc3=PWM_ON;
            timetemp1=2*oc3;
        }
        else
        {
            pwmc3=PWM_OFF;
            timetemp1=5000-2*oc3;
            pwm1++;
        }
        
        TH1 = (65536-timetemp1) / 256;  //Timer0 Load
        TL1 = (65536-timetemp1) % 256;

        


   }
     
     
     else  if(pwm1==10)                 //pwm频道
    {
        if(pwmd1==PWM_OFF)
        {
            pwmd1=PWM_ON;
            timetemp1=2*od1;
        }
        else
        {
            pwmd1=PWM_OFF;
            timetemp1=5000-2*od1;
            pwm1++;
        }
        
        TH1 = (65536-timetemp1) / 256;  //Timer0 Load
        TL1 = (65536-timetemp1) % 256;

        


   }

          else  if(pwm1==11)                 //pwm频道
    {
        if(pwmd2==PWM_OFF)
        {
            pwmd2=PWM_ON;
            timetemp1=2*od2;
        }
        else
        {
            pwmd2=PWM_OFF;
            timetemp1=5000-2*od2;
            pwm1++;
        }
        
        TH1 = (65536-timetemp1) / 256;  //Timer0 Load
        TL1 = (65536-timetemp1) % 256;

        


   }

          else  if(pwm1==12)                 //pwm频道
    {
        if(pwmd3==PWM_OFF)
        {
            pwmd3=PWM_ON;
            timetemp1=2*od3;
        }
        else
        {
            pwmd3=PWM_OFF;
            timetemp1=5000-2*od3;
            pwm1=7;
        }
        
        TH1 = (65536-timetemp1) / 256;  //Timer0 Load
        TL1 = (65536-timetemp1) % 256;

        


   }
}
