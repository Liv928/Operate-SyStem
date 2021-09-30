/*
*@author Yu Wu ywu928
*assignment2_part2
*/

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>


int main(){
    
    
    FILE *fp;
    fp=fopen("cpu_scheduling_input_file.txt","r");
    
    char reading[100];//reading strings from file
    //char *file[1024];
   
    int num_q=0;//number of q 
    int num_tq=0;
    int num_p=0;//number of process

    while(!feof(fp)){
        fscanf(fp,"%s",reading);
        
        if (strstr(reading,"q")){
            num_q++;
        }
        if(strstr(reading,"tq")){
            num_tq++;
        }
        if(strstr(reading,"p")){
	    num_p++;
        }
	    
    }
    int num_que = num_q-num_tq;//number of ready queue
    
    int ready_queues[num_que][num_p];//ready queues that hold all the processes 
    
    //initialize the ready queue
    for(int j=0;j<num_que;j++){
        for(int r=0;r<num_p;r++){
            ready_queues[j][r]=-1;
        }
    }

    rewind(fp);

    //create all the process for each ready queue
    int n;//the index of each queue
    int p=0;//the index of ecah process
    int value;
    
    fscanf(fp,"%s",reading);//first char in the file
    
    while(!feof(fp)){
        
        if (strstr(reading,"q")){
            n++;
            p=0;
            fscanf(fp,"%s",reading);//the index of queue
            fscanf(fp,"%s",reading);//tq
            fscanf(fp,"%s",reading);//the value of tq  
            value = atoi(reading);
            ready_queues[n-1][p]=value;
            p++;
        }
        if(strstr(reading,"p")){
            fscanf(fp,"%s",reading);//cpu burst time
            value = atoi(reading);
            ready_queues[n-1][p]=value;
            p++;
        }
        fscanf(fp,"%s",reading);//q
        
    }
    fclose(fp);
    
    
    
    FILE *outfile;
    outfile = fopen("cpu_scheduling_output_file.txt","w");
    
    
    for(int q=0;q<num_que;q++){
        int p_index=0;
        //number of processes in each ready queue
	    int size=0;
        int timeq=ready_queues[q][0];//time quantum
        
        while(ready_queues[q][p_index]!=-1){
            p_index++;
        }
        size=p_index-1;

        //-------------------------FCFS Scheduling------------------------------
        fprintf(outfile,"The Ready Queue %d Applying FCFS Scheduling\n",q+1);
        fprintf(outfile,"\n");
        fprintf(outfile,"Order of selection by CPU:\n");
        for(int i=0;i<size;i++){
            if(i==p_index-2){
                fprintf(outfile,"p%d\n",i+1);
                fprintf(outfile,"\n");
            }else{
                fprintf(outfile,"p%d ",i+1);
            }
        }
        fprintf(outfile,"Individual waiting times for each process:\n");
        
        //calculate the waiting time for each process     
        int temp[size];//store the waiting time for each process 
        int waitingtime=0;
        int sumtime=0;
        temp[0]=0;
        for(int x1=0;x1<size-1;x1++){
            waitingtime+=ready_queues[q][x1+1];                
            temp[x1+1]=waitingtime;
        }
        for(int y1=0;y1<size;y1++){
            fprintf(outfile,"p%d = %d\n",y1+1,temp[y1]);
        }
        fprintf(outfile,"\n");
        //calculate the average time
        for(int z1=0;z1<size;z1++){
            sumtime+=temp[z1];
        }
        float avgtime =(float)sumtime/size;
        fprintf(outfile,"Average waiting time = %.1f\n",avgtime);
        fprintf(outfile,"\n");
        
        
        //--------------------------SJF Scheduling-------------------------
        fprintf(outfile,"The Ready Queue %d Applying SJF Scheduling\n",q+1);
        fprintf(outfile,"\n");
        fprintf(outfile,"Order of selection by CPU:\n");

        int temp_sjf[size];//store cpu burst times of processes
        int temp_sjf_index[size];//store the index of processes
       
        for(int y2=0;y2<size;y2++){
            temp_sjf[y2]=ready_queues[q][y2+1];
            temp_sjf_index[y2]=y2+1;
        }
	
	//sort processes according to their burst time
        int e,f,temp_value,temp_index_value;
        for(e=0;e<size-1;e++){
            for(f=0;f<size-1-e;f++){
                if(temp_sjf[f]>temp_sjf[f+1]){
                    temp_value=temp_sjf[f];
                    temp_sjf[f]=temp_sjf[f+1];
                    temp_sjf[f+1]=temp_value;

                    temp_index_value=temp_sjf_index[f];
                    temp_sjf_index[f]=temp_sjf_index[f+1];
                    temp_sjf_index[f+1]=temp_index_value;
                }           
            }
        }
        
        for(int x2=0;x2<size;x2++){
            if(x2==size-1){
                fprintf(outfile,"p%d\n",temp_sjf_index[x2]);
            }else{
                fprintf(outfile,"p%d ",temp_sjf_index[x2]);
            }   
        }
        fprintf(outfile,"\n");
        fprintf(outfile,"Individual waiting times for each process:\n");
        int temp_sjf_wt[size];
        int sjf_wt=0;
        temp_sjf_wt[0]=0;
        for(int z2=0;z2<size-1;z2++){
            sjf_wt+=temp_sjf[z2];                
            temp_sjf_wt[z2+1]=sjf_wt;
        }
        for(int u2=0;u2<size;u2++){
            fprintf(outfile,"p%d = %d\n",temp_sjf_index[u2],temp_sjf_wt[u2]);
        }
        fprintf(outfile,"\n");
        sumtime=0;
        for(int v2=0;v2<size;v2++){
            sumtime+=temp_sjf_wt[v2];
        }
        avgtime = (float)sumtime/size;
        fprintf(outfile,"Average waiting time = %.1f\n",avgtime);
        fprintf(outfile,"\n");

        //-------------------------RR Scheduling-----------------------------------
        fprintf(outfile,"The Ready Queue %d Applying Round Robin Scheduling\n",q+1);
        fprintf(outfile,"\n");
       
        int ta_time[size];//turnaround time of each process
        int temp_rr[size];//cpu burst time of each process
        int remain_time[size];//remaining time of each process
        int temp_rr_result[size];
        int ta_starter[size];//start time 
        
        int ta_index=0;
        int timer=0;
        int indicator=size;     

        for(int y3=0;y3<size;y3++){
            temp_rr[y3]=ready_queues[q][y3+1];
            remain_time[y3]=ready_queues[q][y3+1];
        }
        fprintf(outfile,"Order of selection by CPU:\n");
        
        while(indicator!=0){
            for(int x3=0;x3<size;x3++){  
               if(remain_time[x3]<=timeq){
                   if(remain_time[x3]!=0){
                       if(remain_time[x3]==temp_rr[x3]){
                            ta_starter[x3]=timer;
                       }
                       timer+=remain_time[x3];
                       remain_time[x3]=0;
                       ta_time[x3]=timer-ta_starter[x3];
                       temp_rr_result[ta_index]=x3+1;
                       fprintf(outfile,"p%d ",x3+1);
                       //fprintf(outfile,"p%d = %d\n",x3+1,ta_time[x3]);
                       ta_index++;     
                       indicator--;
                   }  
               }else{
                    if(remain_time[x3]==temp_rr[x3]){
                        ta_starter[x3]=timer;
                        timer+=timeq;
                        remain_time[x3]=remain_time[x3]-timeq;
                        fprintf(outfile,"p%d ",x3+1);
                   
                    }else{
                        timer+=timeq;
                        remain_time[x3]=remain_time[x3]-timeq;
                        fprintf(outfile,"p%d ",x3+1);
                    }
                }
            }   
        }
        fprintf(outfile,"\n");
        fprintf(outfile,"\nTurnaround times for each process:\n");
        for(int z3=0;z3<size;z3++){
            fprintf(outfile,"p%d = %d\n",temp_rr_result[z3],ta_time[temp_rr_result[z3]-1]);
        }
        fprintf(outfile,"\n");
    }
    fclose(outfile);
    return 0;

}
