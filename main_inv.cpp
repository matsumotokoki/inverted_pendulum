#include "mbed.h"
#include "Motor.h"
#include "QEI.h"
#include "math.h"
#include "ctype.h"
Ticker controler;
BusOut led(LED1,LED2,LED3,LED4);
InterruptIn sw1(SW1);
InterruptIn sw2(SW2);
Motor motor_left(MOTOR1_IN1,MOTOR1_IN2,MOTOR1_PWM);
Motor motor_right(MOTOR2_IN1,MOTOR2_IN2,MOTOR2_PWM);
BusIn in(GPIO1,GPIO2,GPIO3,GPIO4);
QEI qei_left(GPIO1,GPIO2,NC,48,QEI::X4_ENCODING);
QEI qei_right(GPIO3,GPIO4,NC,48,QEI::X4_ENCODING);
AnalogIn pen(AD1);
const float T = 0.01;
const float MPP = 0.000507;
const float RPA = 0.006142*3.3/5;

float enc_l,enc_r;
float x,x0,th,th0;
float duty;

float F[4]={-0.006021,-0.094022,-20.490005,0.0090};
char pname[4][128]={"F1","F2","F3","F4"};
float gpen;

void pen_con(){
	th = gpen-(pen.read_u16() >>6 )*RPA*3.3/5.0;
	enc_l = qei_left.getPulses();
	enc_r = qei_right.getPulses();			//ƒZƒ“ƒT’lŽæ“¾
  qei_left.reset();
  qei_right.reset();
	x += (float)(enc_l - enc_r) / 2 * MPP;
	if(x >500)x = 500;
	if(x<-500)x =-500;
	
	duty=(float)(F[0]*x+F[1]*(x-x0)/T+F[2]*th+F[3]*(th-th0)/T);

	//printf("%f %f %f %f %f\n\r",x,x-x0,th,th-th0,duty);

	x0=x;
	th0=th;
	
/*	if(duty<-1)duty=-1;
	if(duty>1)duty=1;
*/	
	motor_left=-duty;
	motor_right=-duty;
}
void sw1_rise(void){
	gpen*=1.002;
	led=led+1;
	x=0;
	printf("%6lf:::::%6lf\r\n",gpen,th);
}
void sw2_rise(void){
	gpen*=0.998;
	led=led-1;
	x=0;
	printf("%6lf:::::%6lf\r\n",gpen,th);
}
int main() {

	motor_left.setMaxRatio(1);
	motor_right.setMaxRatio(1);

	led = 0;
	char tmp='a';
	int type=0;
	sw1.mode(PullUp);
	sw2.mode(PullUp);
	in.mode(PullUp);
	
	x=x0=th=th0=0;
	gpen=(pen.read_u16() >>6 )*RPA*3.3/5.0;
	//while(sw1);
	controler.attach(&pen_con,0.01);
	while(1){
		tmp=fgetc(stdin);
		switch(tmp){
			case 'p':gpen=(pen.read_u16() >>6 )*RPA*3.3/5.0;break;
			case 'L':gpen*=1.003;break;
			case 'K':gpen*=0.997;break;
			case '9':F[type]*=10; break;
			case '8':F[type]*=-10; break;
			case 'w':F[type]*=1.001; break;
			case 's':F[type]*=0.999; break;
			case 'W':F[type]+=0.1; break;
			case 'S':F[type]-=0.1; break;
			case 'd':type+=1;
				if(type>=4) {type=0;}break;
			case 'a':type-=1;
				if(type<=-1){type=3;}break;
		}
		x=0;
		printf(/*"\033[2K\r*/" F%i  F1=%6f  F2=%6f  F3=%6f  F4=%6f \r\n",type+1,F[0],F[1],F[2],F[3]);
	}
}




/*
#include "mbed.h"
#include "Motor.h"
#include "QEI.h"
BusOut led(LED1,LED2,LED3,LED4);
BusIn sw1(SW1);
BusIn sw2(SW2);
Ticker control;
Motor motor_left(MOTOR1_IN1,MOTOR1_IN2,MOTOR1_PWM);
Motor motor_right(MOTOR2_IN1,MOTOR2_IN2,MOTOR2_PWM);
BusIn in(GPIO1,GPIO2,GPIO3,GPIO4);
void myinit();
void pen_con();
const float sampling_time = 0.01;//0.02;
float KP =1,KI=1,KD=1;
float adv,thpen,ppen,p0pen,ipen,i0pen,dpen=0;
float mp=0;
AnalogIn pen(AD7);
float gpen = 0;
float c=1.0;

void pen_con(){
	adv=pen.read_u16()*0.01;//>>6;
	thpen=1*adv;
	ppen+=(gpen-thpen);
	ipen=ppen-p0pen;
	dpen=ipen-i0pen;
	p0pen=ppen;
	i0pen=ipen;
	mp=ppen*KI+ipen*KP+dpen*KD;
	motor_left=mp;
	motor_right=mp;
}
int main() {
while(1){
	adv=800;
	myinit();
	gpen=pen.read_u16()*0.01;
	gpen=gpen*1.22433+1;//sampling_time;
	printf("%f\r\n",gpen);
while(1){
	ppen=0;
	wait(0.1);
	c=0.91;//0.91;
	led=1;
	KP=0.045*c;
	KI=0.0015*c;
	KD=0.00005*c;
	ppen=0;
	control.attach(&pen_con, sampling_time);
	while(adv<=gpen+400&&adv>=gpen-450sw1==1){//sw1==1){
		//printf("halo\r\n");
	}
	led=3;
	control.detach();
	motor_left=0;
	motor_right=0;
	
	if(sw2==0){break;}
	while(sw1==0){}
	while(sw1==1){
	led=1;
	//printf("waitin\r\n");
	ppen=0;
	}
	while(sw1==0){}
}
}
}
void myinit(void){
	motor_left.setMaxRatio(1.0);
	motor_right.setMaxRatio(1.0); 
	sw1.mode(PullUp);
	sw2.mode(PullUp);
	in.mode(PullUp);
}

*/