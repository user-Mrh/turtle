#include<STC15F2K60S2.h>
#include<math.h>
#include<stdio.h>

#define uchar unsigned char
#define uint unsigned int
#define r1 100.0
#define r2 180.0
#define PI 3.1415926 



#define     PWM_ON          1   // ����ռ�ձȵĵ�ƽ, 1 �� 0

#define     PWM_OFF         (!PWM_ON)





typedef unsigned char BYTE;
typedef unsigned int WORD;

#define FOSC 24000000L          //ϵͳƵ��
#define BAUD 115200             //���ڲ�����


#define S1_S0 0x40              //P_SW1.6
#define S1_S1 0x80              //P_SW1.7


#define NONE_PARITY     0       //��У��
#define ODD_PARITY      1       //��У��
#define EVEN_PARITY     2       //żУ��
#define MARK_PARITY     3       //���У��
#define SPACE_PARITY    4       //�հ�У��

#define PARITYBIT NONE_PARITY   //����У��λ 


uint xdata tempo1,tempo2,tempo3,tempoa1,oa1,oa2,oa3,ob1,ob2,ob3,oc1,oc2,oc3,od1,od2,od3,timetemp0,timetemp1; //timetamp��ʱ����ֵ����
int xdata ada1,ada2,ada3,adc1,adc2,adc3,adb1,adb2,adb3,add1,add2,add3;
uchar xdata pwmsig0=0,pwmsig1=0,pwm0=0,pwm1=0;
uchar  ritemp,busy;  //pwmsig ��ƽ�����ʶ	  pwm ��·���ֱ�ʶ
char  loopi=0,printtemp[20];
uint  ch0,ch1,ch2,ch3,s1,s2,sbus_rx_buffer[5];

double xdata testacos,testa,testb,testc;



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



/*----------------------------
�����ַ���
----------------------------*/
void SendData(BYTE dat)
{
    while (busy);               //�ȴ�ǰ������ݷ������
    ACC = dat;                  //��ȡУ��λP (PSW.0)
    if (P)                      //����P������У��λ
    {
#if (PARITYBIT == ODD_PARITY)
        TB8 = 0;                //����У��λΪ0
#elif (PARITYBIT == EVEN_PARITY)
        TB8 = 1;                //����У��λΪ1
#endif
    }
    else
    {
#if (PARITYBIT == ODD_PARITY)
        TB8 = 1;                //����У��λΪ1
#elif (PARITYBIT == EVEN_PARITY)
        TB8 = 0;                //����У��λΪ0
#endif
    }
    busy = 1;
    SBUF = ACC;                 //д���ݵ�UART���ݼĴ���
}


void SendString(char *s)
{
    while (*s)                  //����ַ���������־
    {
        SendData(*s++);         //���͵�ǰ�ַ�
    }
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
    ada2=-10;
    ada3=40;
    adb1=0;
    adb2=-25;
    adb3=90;   
    adc1=10;
    adc2=160;
    adc3=-10;
    add1=60;
    add2=-15;
    add3=50;


    
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

	SCON = 0x50;		//8λ����,�ɱ䲨����
	AUXR |= 0x40;		//��ʱ��1ʱ��ΪFosc,��1T
	AUXR &= 0xFE;		//����1ѡ��ʱ��1Ϊ�����ʷ�����
	TMOD &= 0x0F;		//�趨��ʱ��1Ϊ16λ�Զ���װ��ʽ
	TL1 = 0xC4;		//�趨��ʱ��ֵ
	TH1 = 0xFF;		//�趨��ʱ��ֵ
	ET1 = 0;		//��ֹ��ʱ��1�ж�
	TR1 = 1;		//������ʱ��1

    AUXR &= ~(1<<7);    // Timer0 set as 1T/12 mode
    TMOD &= ~(1<<2);    // Timer0 set as Timer
    TMOD |=   0x01;      // Timer0 set as 16 bits 
    
    TH0 = (65536-1500) / 256;  //Timer0 Load
    TL0 = (65536-1500) % 256;
    


    ET0 = 1;        //Timer0 Interrupt Enable
    TR0 = 1;        //Timer0 Run
    

    IP=0xa;         //�����ȼ�
//    PS=1;
 
    

    EA = 1;         //�����ж�
    ES = 1;         //�����ж�

    ACC = P_SW1;
    ACC &= ~(S1_S0 | S1_S1);    //S1_S0=0 S1_S1=0
    P_SW1 = ACC;  


    delay(500);
    pwm0=1;
    pwm1=7;
    


    
        
    
    
    while(1)
    {
//    sprintf(printtemp,"ch0=%5d  ",ch0) ;
//    SendString(printtemp);
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
    move(1,100,100,50);
    move(3,100,100,50);
    move(2,100,100,50);
    move(4,100,100,50);
    delay(20);

    }


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
            timetemp0=5000-timetemp0;
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
            timetemp0=5000-timetemp0;
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
            timetemp0=5000-timetemp0;
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
            timetemp0=5000-timetemp0;
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
            timetemp0=5000-timetemp0;
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
            timetemp0=5000-timetemp0;
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
            timetemp0=5000-timetemp0;
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
            timetemp0=5000-timetemp0;
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
            timetemp0=5000-timetemp0;
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
            timetemp0=5000-timetemp0;
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
            timetemp0=5000-timetemp0;
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
            timetemp0=5000-timetemp0;
            pwm0=1;
        }
        
        TH0 = (65536-timetemp0) / 256;  //Timer0 Load
        TL0 = (65536-timetemp0) % 256;

        


   }
}

/*----------------------------
UART �жϷ������
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

                     //���洮������
                   
    }
    if (TI)
    {
        TI = 0;                 //���TIλ
        busy=0;               //��æ��־
    }
}


