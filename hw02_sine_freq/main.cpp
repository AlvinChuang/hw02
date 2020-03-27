#include "mbed.h"

Serial pc( USBTX, USBRX);
AnalogIn Ain(A0);

const int sample = 512; // sampling freq (sampling rate) (>4(2)*freq) (for freq<100Hz)
float ADCdata[512]; // index
int i;

float freq; // signal frequency
int zeroCross; // numbers of zero crossing
const int offset = 1; // Vdd/2 (zero)
int fistI = -1; // index of first zero crossing
int lastI = -1; // index of last zero crossing
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
        for (i=0; i<sample; i++){
            if ((ADCdata[i]>0 && ADCdata[i+1]<=0)
                || (ADCdata[i]<0 && ADCdata[i+1]>=0))
            {
                firstI = (firstTag==-1) ?  i : -1;
                lastI = i;
                zeroCross++;
            }
        }
        halfCycle = zeroCross - 1; // =(number of cycle)*2
        // freq = numOfCycle / (timeInterval*(lastI-fistI))
        freq = (halfCycle/2.f) / ((1.f/sample)*(lastI-firstI));
        pc.printf("%f\r\n", freq); // print freq on computer
        wait(0.5); // wait 0.5 sec for next freq calculation
    }
}