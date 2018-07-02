#include "mbed.h"
#include "Motor.h"
#include "QEI.h"
#include "math.h"

#define MODE 0											//FEEDBACK : 0 , PID & FEEDBACK: 1

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
float gx=0;
float p[4]={0};			//パラメータ*gain

int i,j;
float* G;

float max=500,min=-500;
float K[6]={0,0,0,0,0,0};
float F[4]={-1.6330 , -11.6684 , -19.5464 ,  -2.9072};

#if(MODE)															//PID & FEEDBACK

#else																		//FEEDBACK
	void pen_con(){
		enc_l = qei_left.getPulses();
		enc_r = qei_right.getPulses();
		//printf("%5f %5f\n\r",enc_r,enc_l);
		qei_left.reset();
		qei_right.reset();
		x += (enc_l - enc_r) / 2 * MPP;
		th = gpen-(pen.read_u16() >>6 )*RPA;
		p[0]=F[0]*x;
		p[1]=F[1]*(x-x0);
		p[2]=F[2]*th;
		p[3]=F[3]*(th-th0);
		for(i=0;i<4;i++)j=(p[i]>p[j]?i:j);
		led=1<<j;
		duty=-(float)(p[0]+p[1]+p[2]+p[3]);
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
	gpen+=RPA;
	//led=led+1;
	x=gx;
}
void sw2_rise(void){
	gpen-=RPA;
	//led=led-1;
	x=gx;
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
	x=gx;
	gpen = (pen.read_u16() >>6 )*RPA;
	led = 0;
}


int main() {
	initialize();
	//control.attach(&pen_con,T);
	while(1)pen_con();
}
