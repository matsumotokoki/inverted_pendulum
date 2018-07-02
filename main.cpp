#include "mbed.h"
#include "Motor.h"
#include "QEI.h"
#include "math.h"

#define MODE 0											//FEEDBACK : 0 , PID : 1

BusOut led(LED1,LED2,LED3,LED4);
InterruptIn sw1(SW1);
InterruptIn sw2(SW2);
Ticker control;
Motor motor_left(MOTOR1_IN1,MOTOR1_IN2,MOTOR1_PWM);
Motor motor_right(MOTOR2_IN1,MOTOR2_IN2,MOTOR2_PWM);
BusIn in(GPIO1,GPIO2,GPIO3,GPIO4);
QEI qei_left(GPIO1,GPIO2,NC,48,QEI::X4_ENCODING);
QEI qei_right(GPIO3,GPIO4,NC,48,QEI::X4_ENCODING);
AnalogIn pen(AD5);

const float T = 0.001;
const float MPP = 0.000507;		//MOVE PER PULSE
const float RPA = 0.006141921;		//RADIAN PER ANALOG VALUE

float enc_l=0,enc_r=0;
float x,x0,th,th0;
float xi,thi;
float duty;
float gpen;

char tmp='a';
int type=0;
float* G;

float max=500,min=-500;
float K[6]={0,0,0,0,0,0};
float F[4]={-6.3246  ,-14.7681 , -36.7392  , -7.2317};
//float F[4]={-0.006021,-0.094022,-20.490005,0.0090};

#if(MODE)															//PID

#else																		//FEEDBACK
	void pen_con(){
		enc_l = qei_left.getPulses();
		enc_r = qei_right.getPulses();
		//printf("%5f %5f\n\r",enc_r,enc_l);
		qei_left.reset();
		qei_right.reset();
		x += (enc_l - enc_r) / 2 * MPP;
		th = gpen-(pen.read_u16() >>6 )*RPA;
	
		duty=-(float)(F[0]*x+F[1]*(x-x0)+F[2]*th+F[3]*(th0-th));
 		//printf("%f %f %f %f %f\n\r",x,x-x0,th,th-th0,duty);


		x0=x;
		th0=th;
	
		if(duty<-1)duty=-1;
		if(duty>1)duty=1;
	
		motor_left=duty;
		motor_right=duty;

		if(x>=max)x=max;
		if(x<=min)x=min;

	}
#endif

void sw1_rise(void){
	gpen*=1.002;
	led=led+1;
	//x=0;
}
void sw2_rise(void){
	gpen*=0.998;
	led=led-1;
	//x=0;
}
void initialize(){
	G=(MODE? K:F);
	sw1.rise(sw1_rise);
	sw2.rise(sw2_rise);
	sw1.mode(PullUp);
	sw2.mode(PullUp);
	in.mode(PullUp);
	motor_left.setMaxRatio(1);
	motor_right.setMaxRatio(1);
	qei_left.reset();
	qei_right.reset();
 	x=x0=th=th0=xi=thi=0;
	gpen = (pen.read_u16() >>6 )*RPA;
	led = 0;
}

/*void config(){
		tmp=fgetc(stdin);
		switch(tmp){
			case 'p':gpen=(pen.read_u16() >>6 )*RPA;break;
			case 'l':gpen*=1.0003;led=led+1;break;
			case 'k':gpen*=0.9998;led=led-1;break;
			case '9':G[type]*=10; break;
			case '8':G[type]*=-10; break;
			case 'w':G[type]*=1.001; break;
			case 's':G[type]*=0.999; break;
			case 'W':G[type]+=0.1; break;
			case 'S':G[type]-=0.1; break;
			case 'd':type+=1;
			if(type>=(MODE? 6:4)) {type=0;}break;
			case 'a':type-=1;
				if(type<=-1){type=(MODE? 5:3);}break;
		}
		//x=0;
		for(int i=0;i<(MODE?6:4);i++)printf("[%d]=%f  ",i,G[i]);
		printf("gpen=%f  i=%d\n\r",gpen,type);
}*/

int main() {
	initialize();
	//control.attach(&pen_con,T);
	while(1)pen_con();
}
