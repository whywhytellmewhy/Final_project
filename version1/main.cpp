#include"mbed.h"
#include "bbcar.h"
#include "bbcar_rpc.h"
#include <stdio.h>
#include <ctype.h>
#include <math.h>
using namespace std::chrono;

#define CENTER_BASE 1500

BufferedSerial pc(USBTX,USBRX); //tx,rx
BufferedSerial uart(D10,D9); //tx,rx
//BufferedSerial uart(A1,A0);

BufferedSerial xbee(D1, D0);//(D10, D9)

Ticker servo_ticker;
PwmOut pin5(D5), pin6(D6);

DigitalInOut pin10(D4);//D10
//DigitalOut led1(LED1);

//BufferedSerial pc(USBTX,USBRX); //tx,rx
//BufferedSerial uart(D1,D0); //tx,rx

//--------------(added)--------------

/*Ticker encoder_ticker;
DigitalIn encoder(D11);

Thread thread_encoder;
Thread thread_ping;

volatile int steps;
volatile int last;*/

Thread thread_carmove;

Thread thread_AprilTag;

//Thread thread_Xbee;
//Thread thread_uart;

void uart_detect(Arguments *in, Reply *out);
RPCFunction rpcuartdetect(&uart_detect, "ANSWER");
//void uart_detect(double x,double y,double z);


char c;
int q=0;
int i=0;
int part=1;
int AprlTag_ID=0;
char buffer='a';
char digits[4]={0};
int digit=0;
double answer_1=0.0;
double answer_2=0.0;
double answer_3=0.0;
double calculate_answer=0.0;
double mid_answer=0.0;
//--------------------------------------

BBCar car(pin5, pin6, servo_ticker);


void AprilTag(){
   if(digits[1]=='p'){
      //calculate_answer=(.2f)((((int)digits[0])-48)+(((int)digits[2])-48));
      calculate_answer=(double)((((int)digits[0])-48)+(((int)digits[2])-48));
   }else if(digits[1]=='m'){
      calculate_answer=(double)((((int)digits[0])-48)-(((int)digits[2])-48));
   }else if(digits[1]=='t'){
      calculate_answer=(double)((((int)digits[0])-48)*(((int)digits[2])-48));
   }else if(digits[1]=='d'){
      mid_answer=(double)(((int)digits[0])-48)/(double)(((int)digits[0])-48);
      calculate_answer=(double)((int)(mid_answer*100.0))/100.0;
   }else{
      //printf("Error!!!!!\n");
      printf("Calculate Error!!!!!\n");
   }
   printf("x1 __ x2 = y: %c %c %c = %f\n",digits[0],digits[1],digits[2],calculate_answer);
   printf("calculate_answer: %f\n",calculate_answer);
   while(1){
      //printf("AprilTagAprilTagAprilTagAprilTag\n");
      //printf("part:%d\n",part);
      //printf("AprlTag_ID:%d\n",AprlTag_ID);
      if(AprlTag_ID==1){
         //if(answer_1==1.0){
         if(answer_1==calculate_answer){
            car.stop();
         printf("stopstopstopstopstop111111111111111\n");
         return;
         }
      }else if(AprlTag_ID==2){
         //if(answer_2==1.0){
         if(answer_2==calculate_answer){
            car.stop();
         printf("stopstopstopstopstop22222222222222\n");
         return;
         }
      }else if(AprlTag_ID==3){
         //if(answer_3==1.0){
         if(answer_3==calculate_answer){
            car.stop();
         printf("stopstopstopstopstop333333333\n");
         return;
         }
      }
      ThisThread::sleep_for(100ms);
   }
}

void carmove(){
    parallax_ping  ping1(pin10);
    ThisThread::sleep_for(2s);
    car.goStraight(15);//12
    //car.turn(15,0.95);
    //printf("GOog!!\n");
   while(1) {
       printf("ping: %f\n",(float)ping1);
      if((float)ping1>30) ;//led1 = 1;
      else {
         //led1 = 0;
         printf("stop11111!!!");

         part=2;

         car.stop();
         //printf("stop222222!!!");
         ThisThread::sleep_for(300ms);
      //   car.turn(-120,0.2);
      //   ThisThread::sleep_for(1s);
      //   car.stop();
      //   ThisThread::sleep_for(2s);
///
         car.turn(100,-0.3);
         ThisThread::sleep_for(1s);
         car.stop();
         ThisThread::sleep_for(250ms);
         car.turn(150,0.3);
         ThisThread::sleep_for(1500ms);
         
         //car.goStraight(150);
         //ThisThread::sleep_for(1s);
         car.turn(150,0.32);//0.3
         ThisThread::sleep_for(2000ms);
         car.stop();
         ThisThread::sleep_for(300ms);



         //car.goStraight(150);
         //car.turn(150,-0.9);
         //ThisThread::sleep_for(200ms);//750//250
         car.stop();
         ThisThread::sleep_for(300ms);


         car.turn(150,-0.25);//-0.35
         ThisThread::sleep_for(1s);
         car.stop();
         ThisThread::sleep_for(300ms);
         car.goStraight(50);
        ThisThread::sleep_for(1s);
         car.goStraight(15);
///         
         thread_AprilTag.start(AprilTag);
//         //ThisThread::sleep_for(1500ms);
         ThisThread::sleep_for(150s);//150s
         car.stop();

         break;
      }
      ThisThread::sleep_for(1s);
   }
}

void uart_detect(Arguments *in, Reply *out){
//void uart_detect(double x,double y,double z){
   answer_1 = in->getArg<double>();
   answer_2 = in->getArg<double>();
   answer_3 = in->getArg<double>();
   //answer_1 = x;
   //answer_2 = y;
   //answer_3 = z;
   printf("(answer_1,answer_2,answer_3): (%.2f,%.2f,%.2f)\n",answer_1,answer_2,answer_3);


   uart.set_baud(9600);
   thread_carmove.start(carmove);
   while(1){
      //printf("nooooooooooolooooo\n");
      if(uart.readable()){
            char recv[1];
            uart.read(recv, sizeof(recv));
            pc.write(recv, sizeof(recv));

            c=recv[0];
            if(part==1){
               if(isdigit(c) == 1 || c=='p' || c=='m' || c=='t' || c=='d'){
                  if(c==buffer && c!=digits[0] && c!=digits[1] && c!=digits[2] && digit<3){
                     digits[digit]=c;
                     digit=digit+1;
                     buffer='a';
                     //printf("buffer[4]= %c,%c,%c\n",digits[0],digits[1],digits[2]);
                  }else{
                     buffer=c;
                  }
               }
            }else{
               if(c=='I'){
                  i=i+1;
                  q=q+1;
               }else if(c=='D'){
                  q=q+1;
                  //printf("IDDDDDDDDDDDDDDDDDDDDDDDDDDD\n");
               }else if(q==2 && c=='1'){
                  AprlTag_ID=1;
                  //q=q+1;
                  q=0;
               }else if(q==2 && c=='2'){
                  AprlTag_ID=2;
                  //q=q+1;
                  q=0;
               }else if(q==2 && c=='3'){
                  AprlTag_ID=3;
                  //q=q+1;
               }
            }
            
      }
   }
}


int main(){
   /*double x_1=2.1234567;
   int xi=0;
   printf("int: %d",(int)x_1);//2
   x_1=x_1*10000.0;
   printf("*double: %f",x_1);//21234.567
   printf("*int: %d",(int)x_1);//21234
   //x_1=x_1/10000.0;
   //printf("*and/double: %f",x_1);//2.1234567
   //printf("*and/int: %d",(int)x_1);//2
   xi=(int)x_1;
   printf("*int: %d",xi);//21234
   x_1=(double)xi/10000.0;
   printf("*and/double: %f",x_1);//2.1234*/

   //uart.set_baud(9600);
   //thread_carmove.start(carmove);

//   uart_detect(0.0,1.0,3.5);

   //while(1){
       /*while(1){
         car.goStraight(200);
            ThisThread::sleep_for(3s);
            car.stop();
            ThisThread::sleep_for(1s);

            pc.set_baud(9600);
            encoder_ticker.attach(&encoder_control, 10ms);
            
            
            steps = 0;
            last = 0;
            car.goStraight(50);
            while(steps*6.5*3.14/32 < 40) {
                ThisThread::sleep_for(10ms);
            }
            car.stop();
            //ThisThread::sleep_for(10s);
            ThisThread::sleep_for(5s);
       }*/

       //car.goStraight(20);

       //thread_uart.start(uart_detect);
       //car.goStraight(15);
       printf("Start!!!!!\n");
       char buf[256], outbuf[256];
   FILE *devin = fdopen(&xbee, "r");
   FILE *devout = fdopen(&xbee, "w");
   while (1) {
      memset(buf, 0, 256);
      //printf("111111111111111!!!\n");
      for( int i = 0; ; i++ ) {
         char recv = fgetc(devin);
         //printf("%c\n",recv);
         printf("%c\n",recv);
         if(recv == '\n') {
            //printf("323333333333333333333!!!\n");
            printf("\r\n");
            break;
         }
         buf[i] = fputc(recv, devout);
         //printf("4444444444444444444444444!!!\n");
      }
   //printf("555555555555555555555555!!!\n");
   RPC::call(buf, outbuf);
   }

    
   //}
}