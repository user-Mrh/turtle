#include<STC15F2K60S2.h>
#include<math.h>
#define uchar unsigned char
#define uint unsigned int

#define     PWM_ON          1   // ����ռ�ձȵĵ�ƽ, 1 �� 0

#define     PWM_OFF         (!PWM_ON)

uint oa1,oa2,oa3,ob1,ob2,ob3,oc1,oc2,oc3,od1,od2,od3,timetemp0,timetemp1; //timetamp��ʱ����ֵ����

uchar pwmsig0=0,pwmsig1=0,pwm0=1,pwm1=1;  //pwmsig ��ƽ�����ʶ	  pwm ��·���ֱ�ʶ



sbit pwma1=P2^6; //����������
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
    pwmc1=PWM_OFF;    
    pwmc2=PWM_OFF;    
    pwmc3=PWM_OFF; 


    
    oa1=1500;
    oa2=1500;
    oa3=1500;
    ob1=1500;
    ob2=1500;
    ob3=1500;
    oc1=1500;
    oc2=1500;
    oc3=1500;
    od1=1500;
    od2=1500;
    od3=1500;




    AUXR &=  (0<<7);    // Timer0 set as 1T/12 mode
    TMOD &= ~(1<<2);    // Timer0 set as Timer
    TMOD |=   0x11;      // Timer0 set as 16 bits 
    TH0 = (65536-oa1) / 256;  //Timer0 Load
    TL0 = (65536-oa1) % 256;
    ET0 = 1;        //Timer0 Interrupt Enable
    PT0 = 1;        //�����ȼ�
    TR0 = 1;        //Timer0 Run
    EA = 1;         //�����ж�
    
    
    while(1);


}


void T0_time() interrupt   1	  //��ʱ��0 ����
{
    if(pwm0==1)                   //pwmƵ��
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
    
    
    else if(pwm0==2)                   //pwmƵ��
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

    
    else if(pwm0==3)                   //pwmƵ��
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

    else if(pwm0==4)                   //pwmƵ��
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



    else if(pwm0==5)                   //pwmƵ��
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


    else if(pwm0==6)                   //pwmƵ��
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
            pwm0++;
        }
        
        TH0 = (65536-timetemp0) / 256;  //Timer0 Load
        TL0 = (65536-timetemp0) % 256;

      


    }



    else if(pwm0==7)                   //pwmƵ��
    {
        if(pwmc1==PWM_OFF)
        {
            pwmc1=PWM_ON;
            timetemp0=2*oc1;
        }
        else
        {
            pwmc1=PWM_OFF;
            timetemp0=5000-2*oc1;
            pwm0++;
        }
        
        TH0 = (65536-timetemp0) / 256;  //Timer0 Load
        TL0 = (65536-timetemp0) % 256;

        


    }


    else  if(pwm0==8)                 //pwmƵ��
    {
        if(pwmc2==PWM_OFF)
        {
            pwmc2=PWM_ON;
            timetemp0=2*oc2;
        }
        else
        {
            pwmc2=PWM_OFF;
            timetemp0=5000-2*oc2;
            pwm0++;
        }
        
        TH0 = (65536-timetemp0) / 256;  //Timer0 Load
        TL0 = (65536-timetemp0) % 256;

        


   }
       else  if(pwm0==9)                 //pwmƵ��
    {
        if(pwmc3==PWM_OFF)
        {
            pwmc3=PWM_ON;
            timetemp0=2*oc3;
        }
        else
        {
            pwmc3=PWM_OFF;
            timetemp0=5000-2*oc3;
            pwm0++;
        }
        
        TH0 = (65536-timetemp0) / 256;  //Timer0 Load
        TL0 = (65536-timetemp0) % 256;

        


   }
     
     
     else  if(pwm0==10)                 //pwmƵ��
    {
        if(pwmd1==PWM_OFF)
        {
            pwmd1=PWM_ON;
            timetemp0=2*od1;
        }
        else
        {
            pwmd1=PWM_OFF;
            timetemp0=5000-2*od1;
            pwm0++;
        }
        
        TH0 = (65536-timetemp0) / 256;  //Timer0 Load
        TL0 = (65536-timetemp0) % 256;

        


   }

          else  if(pwm0==11)                 //pwmƵ��
    {
        if(pwmd2==PWM_OFF)
        {
            pwmd2=PWM_ON;
            timetemp0=2*od2;
        }
        else
        {
            pwmd2=PWM_OFF;
            timetemp0=5000-2*od2;
            pwm0++;
        }
        
        TH0 = (65536-timetemp0) / 256;  //Timer0 Load
        TL0 = (65536-timetemp0) % 256;

        


   }

          else  if(pwm0==12)                 //pwmƵ��
    {
        if(pwmd3==PWM_OFF)
        {
            pwmd3=PWM_ON;
            timetemp0=2*od3;
        }
        else
        {
            pwmd3=PWM_OFF;
            timetemp0=5000-2*od3;
            pwm0=1;
        }
        
        TH0 = (65536-timetemp0) / 256;  //Timer0 Load
        TL0 = (65536-timetemp0) % 256;

        


   }
}
