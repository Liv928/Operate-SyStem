#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>

int main(){
    /*
    for(int j=0;j<num_que;j++){
        for(int r=0;r<num_p;r++){
            if(ready_queues[j][r]!=-1&&ready_queues[j][r+1]!=-1){
                printf("%d ",ready_queues[j][r]);
            }else if(ready_queues[j][r]!=-1&&ready_queues[j][r+1]==-1){
                printf("%d\n",ready_queues[j][r]);
            }
            
        }
    }*/
    int size=15;
    
    int x=size;
    while(x!=0){
        printf("x is %d\n",x);
        x--;
    }
    
    
    return 0;
    /*
     if(ta_time[x3]<=timeq){
                    if(ta_time[x3]!=0){
                        timer+=ta_time[x3];
                        ta_time[x3]=timer-ta_starter[x3];
                        temp_rr_result[ta_index]=x3;
                        ta_index++;
                        indicator--;
                    }
                }
                
                
                else{
                    if(ta_time[x3]==temp_rr[x3]){
                        ta_starter[x3]=timer;
                        timer+=timeq;
                        ta_time[x3]=ta_time[x3]-timeq;
                    }else{
                        timer+=timeq;
                        ta_time[x3]=ta_time[x3]-timeq;
                    }
                }*/
}