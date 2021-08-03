#include <reg51.h>
#include <stdio.h>
#include <string.h>

#define satir1 0x80
#define satir2 0xC0
#define satir3 0x94
#define satir4 0xD4


void Timer0Delay(void);
void cct_init(void);
void delay(int);
void lcdinit(void);
void writecmd(int);
void writedata(char);
void ReturnHome(void);

void gecikme(unsigned int);
void lcd_yaz(unsigned char d[]);
void T0M1Delay(void);
void ReturnHome(void);
void para_yaz(void);
sbit RS=P1^6;
sbit E=P1^7;

sbit bestl=P1^0;
sbit ontl=P1^1;
sbit yirmitl=P1^2;
sbit ellitl=P1^3;
sbit yuztl=P1^4;
sbit ikiyuztl=P1^5;


unsigned int p0;
unsigned int p1;
unsigned int p2;
unsigned int p3;
unsigned int p4;
unsigned int p5;

unsigned int bes_say;
unsigned int on_say;
unsigned int yirmi_say;
unsigned int elli_say;
unsigned int bir_say;
unsigned int ikiyuz_say;


unsigned int x;
unsigned int y;
unsigned long int total;
unsigned char guncel_tutar[]="";
unsigned char satir2_paralar[]="";
unsigned char satir3_paralar[]="";
unsigned char satir4_paralar[]="";
unsigned char reset[]="";
unsigned int a;
unsigned int b;

void timer0(void) interrupt 1{ //timerinterrupt0
		
		writecmd(0x94);
		lcd_yaz("Sistem Resetlendi");
		writecmd(0xD4);
		lcd_yaz(guncel_tutar);
		ET0=0;
		total=0;
}

void T0M1Delay(void){
        TH0 = 0x3C; //yaklasik 50000 
        TL0 = 0xB0;
        TR0=1; //timer0 baslatildi.
        while(TF0==0); //timer flag overflow'u beklenir.
        TR0=0;
        TF0=0;
}

void delay(int a){
	int i;
	for(i=0;i<a;i++);
}

void gecikme(unsigned int time){
	unsigned int i,j;
	for(i=0;i<time;i++){
		for(j=0;j<100;j++){
		}
	}
}

void INTER(void) interrupt 0{
	
	if(p0==0){
		bes_say+=1;
		total+=5;	
	}

	if(p1==0){
		on_say+=1;
		total+=10;	
	}
	if(p2==0){
		yirmi_say+=1;
		total+=20;	
	}
	if(p3==0){
		elli_say+=1;
		total+=50;	
	}
	if(p4==0){
		bir_say+=1;
		total+=100;
	}
	if(p5==0){
		ikiyuz_say+=1;
		total+=200;
	}
	a=(int)total;
	sprintf(guncel_tutar,"%d TL",a);
	writecmd(0x80);
	lcd_yaz(guncel_tutar);
	para_yaz();

	}					
  
	

void para_yaz(void){
	
	writecmd(0xC0);
	sprintf(satir2_paralar,"5:%d 50:%d",bes_say,elli_say);
	lcd_yaz(satir2_paralar);
	writecmd(0x94);
	sprintf(satir3_paralar,"10:%d 100:%d",on_say,bir_say);
	lcd_yaz(satir3_paralar);
	writecmd(0xD4);
	sprintf(satir4_paralar,"20:%d 200:%d",yirmi_say, ikiyuz_say);
	lcd_yaz(satir4_paralar);



}


void lcd_yaz(char str[]){
	unsigned char j=0;
	while(str[j]!=0){
		writedata(str[j]);
		j++;
	}
	while(j<20){
		writedata(' ');
		j++;
	
	}
	
}
void lcdinit(void)
{
    delay(15000);
   writecmd(0x30);
     delay(4500);
   writecmd(0x30);
     delay(300);
   writecmd(0x30);
     delay(650);

   writecmd(0x38);    //function set
   writecmd(0x0c);    //display on,cursor off,blink off
   writecmd(0x01);    //clear display
   writecmd(0x06);    //entry mode, set increment 
}
void writecmd(int z)
{
   RS = 0;             // => RS = 0
   P2 = z;             //Data transfer
   E  = 1;             // => E = 1
   delay(150);
   E  = 0;             // => E = 0
   delay(150);
}

void writedata(char t){
	RS=1;
	P2=t;
	E=1;
	delay(150);
	E=0;
	delay(150);
}

void ReturnHome(void)     //Return to 0 location
{
  writecmd(0x02);
    delay(1500);
}
void main(void){
	
	P0=0x00;
	P1=0xFF;
	P2=0x00;
	P3=0x14;
	TMOD=0x01;
	EX0=1;
	IT0=1;
	EA=1;
	
	lcdinit();
	writecmd(0x80);
	lcd_yaz("PARA SAYMA MAKINESI");
	
	ReturnHome();
	
	
	for(x=0 ; x<100 ; x++){
        for(y=0 ; y<6 ; y++){ //30sn boyunca bekliyor. interr gelmezse timer interupta girer.
						p0=bestl;
						p1=ontl;
						p2=yirmitl;
						p3=ellitl;
						p4=yuztl;
						p5=ikiyuztl;
						T0M1Delay();
    }
	}
	ET0=1;
	
}
	




