#include "mbed.h"
#include "Motor.h"
#include "QEI.h"

BusOut led(LED1,LED2,LED3,LED4);
BusIn sw1(SW1);
BusIn sw2(SW2);

Motor motor_left(MOTOR1_IN1,MOTOR1_IN2,MOTOR1_PWM);
Motor motor_right(MOTOR2_IN1,MOTOR2_IN2,MOTOR2_PWM);
BusIn in(GPIO1,GPIO2,GPIO3,GPIO4);
QEI qei_left(GPIO1,GPIO2,NC,48,QEI::X4_ENCODING);
QEI qei_right(GPIO3,GPIO4,NC,48,QEI::X4_ENCODING);
void myinit();
double enc_l,enc_r=0;
double spl_now,spr_now=0;
double spl_must,spr_must=0;
double spl_kp, spr_kp,   spl_ki, spr_ki,   spl_kd, spr_kd=0;
double spl_kp_pre, spr_kp_pre,   spl_ki_pre, spr_ki_pre,   spl_kd_pre, spr_kd_pre=0;
const double sampling_time = 0.1;
const double move_per_pulse = 0.046;
const double ki = 0.8, kp = 0.5, kd = 0.31;
double ml,mr;




int main(void){
	myinit();
	printf("L: %lf  R:  %lf \r\n",enc_l,enc_r);
	while(1){
		motor_left=1;
		motor_right=1;
	enc_l = qei_left.getPulses();  
	enc_r = qei_right.getPulses();  
	//enc_l = -1 * enc_l;
	printf("L: %lf  R:  %lf \r\n",enc_l,enc_r);
	wait(1);
	}
}


void myinit(void){
	sw1.mode(PullUp);
	sw2.mode(PullUp);
	in.mode(PullUp);
}
