#include "mbed.h"

Serial pc( USBTX, USBRX);
AnalogIn Ain(A0);

const int sample = 512; // sampling freq (sampling rate) (>4(2)*freq) (for freq<100Hz)
float ADCdata[sample]; // index
int i;

float freq; // signal frequency
int zeroCross; // numbers of zero crossing
const float offset = 0.2988; // ADC readed input for offset 1V (zero) //doesn't affect freq much
int firstI; // index of first zero crossing
int lastI; // index of last zero crossing
int halfCycle; // number of half cycle (=zeroCross-1)

int main(){
    while(1){
        //sample analog input
        for (i=0; i<sample; i++){ // smaple for 1 sec
            ADCdata[i] = Ain;
            wait(1.f/sample); // wait(timeInterval)
        }
        // zero crossing detection
        zeroCross = 0;
        firstI = -1; // reset firstI
        lastI = -1; // reset lastI
        for (i=0; i<sample; i++){
            if ((ADCdata[i]>offset && ADCdata[i+1]<=offset)
                || (ADCdata[i]<offset && ADCdata[i+1]>=offset))
            {
                if (firstI==-1) firstI=i;
                lastI = i;
                zeroCross++;
            }
        }
        halfCycle = zeroCross - 1; // =(number of cycle)*2
        
        // freq = numOfCycle / (timeInterval*(lastI-fistI))
        //freq = (halfCycle/2.f) / ((1.f/sample)*(lastI-firstI+7)); // without correction factor
        freq = (halfCycle/2.f) / ((1.f/sample)*(lastI-firstI+7)); // +7 is correction factor
        if (freq<0) freq=0; //avoid negative caused by correction factor

        pc.printf("%f\r\n", freq); // print freq on computer
        wait(0.5); // wait 0.5 sec for next freq calculation
    }
}