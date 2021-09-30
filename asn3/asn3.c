/**
 * @author Yu Wu ywu928
 * 
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>

/* pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; */
pthread_mutex_t mutex;
pthread_mutex_t mutex_dep;

int count=0;

int **account;
int *od_count;//check for overdraft, whether the fee for each $500 has already been applied.


void acc_print(int **account,int account_number){
    for(int r=0;r<account_number;r++){
        for(int q=0;q<9;q++){
            if(q==8){
                printf("%d\n",account[r][q]);
            }else{
                printf("%d ",account[r][q]);
            }
        }   
    }
}

void * depo_run(void *arg){

    //enter critical section
    pthread_mutex_lock(&mutex_dep);

    char *p;
    p = strtok(arg," ");//dep1 or c1

    int a;
    int amount;
    int d_fee;
    int tran_lim;
    int add_fee;
    char a_one[2];
    a_one[1]='\0';

    p = strtok(NULL, " ");//operation
        
    while(p!=NULL){
        p = strtok(NULL," ");//a_index
        a_one[0]=p[1];      
        a=atoi(a_one);//acount index

        d_fee = account[a-1][2];
        tran_lim= account[a-1][5];   
        add_fee= account[a-1][6];
        
        p = strtok(NULL," ");//amount deposit
        amount = atoi(p);
        
        if(tran_lim>0){
            account[a-1][8]+=amount;
            account[a-1][8]-=d_fee;
        }else{
            account[a-1][8]+=amount;
            account[a-1][8]-=d_fee;
            account[a-1][8]-=add_fee;
        }
        
        account[a-1][5]--;
        p = strtok(NULL," ");//d
    }
    
    //leave critical section
    pthread_mutex_unlock(&mutex_dep);

}

void * thread_run(void *arg)
{   
    //enter critical section
    pthread_mutex_lock(&mutex);
    
    char *p;
    p = strtok(arg," ");//dep1 or c1
    
    //printf("[%#lx]value of count: %c\n", pthread_self(),type);
    
    char a_one[2];
    char a_two[2];
    a_one[1]='\0';
    a_two[1]='\0';
    int a,b;
    int value;
    int d_fee;
    int w_fee;
    int t_fee;
    int t_fee_b;
    int tran_lim;
    int tran_lim_b;
    int add_fee;
    int add_fee_b;
    int odp_fee;
    int current_bal;
    int bal_check;
    int o_check;
    int multiple;
    
    p = strtok(NULL, " ");//operation
        
    while(p!=NULL){    
        //deposit 
        if(strstr(p,"d")){
            p=strtok(NULL, " ");
              
            a_one[0]=p[1];      
            a=atoi(a_one);//acount index

            d_fee = account[a-1][2];
            tran_lim= account[a-1][5];
            add_fee= account[a-1][6];
            odp_fee= account[a-1][7];
            current_bal= account[a-1][8];
            
            p=strtok(NULL, " ");//amount deposit
            value = atoi(p);
              
            if(tran_lim>0){
                bal_check = current_bal +value -d_fee;
                if(bal_check>0){
                    account[a-1][8]+=value;  
                    account[a-1][8]-=d_fee;
                }else{
                    if(account[a-1][7]==0){
                        printf("No Overdraft Protection for Account%d. Transaction denied.",a);
                    }else{
                        if(bal_check<-5000){
                            printf("Account%d has reached Maxinum Overdraft Amount. Transaction denied.",a);
                        }else{
                            o_check= account[a-1][8]/-500;
                            multiple=o_check+1-od_count[a-1];
                            if(bal_check-multiple*odp_fee<-5000){
                                printf("Account%d has reached Maxinum Overdraft Amount. Transaction denied.",a);
                            }else{
                                account[a-1][8]+=value;  
                                account[a-1][8]-=d_fee;    
                                account[a-1][8]-=multiple*odp_fee;
                                od_count[a-1]+=multiple;
                            }     
                        }
                        if(account[a-1][8]/-500>o_check){
                            if(account[a-1][8]-odp_fee<-5000){
                                od_count-=multiple;
                                account[a-1][8]+=multiple*odp_fee;
                                account[a-1][8]-=value;  
                                account[a-1][8]+=d_fee;
                                printf("Account%d has reached Maxinum Overdraft Amount. Transaction denied.",a);
                            }else{
                                o_check= account[a-1][8]/-500;
                                multiple=o_check+1-od_count[a-1];
                                account[a-1][8]-=multiple*odp_fee;
                                od_count[a-1]+=multiple;
                            }
                        }
                    }
                }     
            }else{
                bal_check = current_bal +value -d_fee -add_fee;
                if(bal_check>0){
                    account[a-1][8]+=value;  
                    account[a-1][8]-=d_fee;
                    account[a-1][8]-=add_fee;
                }else{
                    if(account[a-1][7]==0){
                        printf("No Overdraft Protection for Account%d. Transaction denied.",a);
                    }else{
                        if(bal_check<-5000){
                            printf("Account%d has reached Maxinum Overdraft Amount. Transaction denied.",a);
                        }else{
                            o_check= account[a-1][8]/-500;
                            multiple=o_check+1-od_count[a-1];
                            if(bal_check-multiple*odp_fee<-5000){
                                printf("Account%d has reached Maxinum Overdraft Amount. Transaction denied.",a);
                            }else{
                                account[a-1][8]+=value;  
                                account[a-1][8]-=d_fee;
                                account[a-1][8]-=add_fee;    
                                account[a-1][8]-=multiple*odp_fee;
                                od_count[a-1]+=multiple;
                            }     
                        }
                        if(account[a-1][8]/-500>o_check){
                            if(account[a-1][8]-odp_fee<-5000){
                                od_count-=multiple;
                                account[a-1][8]+=multiple*odp_fee;
                                account[a-1][8]-=value;  
                                account[a-1][8]+=d_fee;
                                account[a-1][8]+=add_fee;
                                printf("Account%d has reached Maxinum Overdraft Amount. Transaction denied.",a);
                            }else{
                                o_check= account[a-1][8]/-500;
                                multiple=o_check+1-od_count[a-1];
                                account[a-1][8]-=multiple*odp_fee;
                                od_count[a-1]+=multiple;
                            }
                        }
                    }
                }    
            }
            account[a-1][5]--;
            p = strtok(NULL," ");//operation 
            continue;
        }
        //withdraw
        if(strstr(p,"w")){  
            p=strtok(NULL, " ");
            a_one[0]=p[1];      
            a=atoi(a_one);//acount index

            w_fee = account[a-1][3];
            tran_lim= account[a-1][5];
            add_fee= account[a-1][6];
            odp_fee= account[a-1][7];
            current_bal= account[a-1][8];

            p=strtok(NULL, " ");
            value = atoi(p);
            
            if(tran_lim>0){
                bal_check = current_bal -value -w_fee;
                if(bal_check>0){
                    account[a-1][8]-=value;  
                    account[a-1][8]-=w_fee;
                }else{
                    if(account[a-1][7]==0){
                        printf("No Overdraft Protection for Account%d. Transaction denied.",a);
                    }else{
                        if(bal_check<-5000){
                            printf("Account%d has reached Maxinum Overdraft Amount. Transaction denied.",a);
                        }else{
                            o_check= account[a-1][8]/-500;
                            multiple=o_check+1-od_count[a-1];
                            if(bal_check-multiple*odp_fee<-5000){
                                printf("Account%d has reached Maxinum Overdraft Amount. Transaction denied.",a);
                            }else{
                                account[a-1][8]-=value;  
                                account[a-1][8]-=w_fee;   
                                account[a-1][8]-=multiple*odp_fee;
                                od_count[a-1]+=multiple;
                            }     
                        }
                        if(account[a-1][8]/-500>o_check){
                            if(account[a-1][8]-odp_fee<-5000){
                                od_count-=multiple;
                                account[a-1][8]+=multiple*odp_fee;
                                account[a-1][8]+=value;  
                                account[a-1][8]+=w_fee;
                                printf("Account%d has reached Maxinum Overdraft Amount. Transaction denied.",a);
                            }else{
                                o_check= account[a-1][8]/-500;
                                multiple=o_check+1-od_count[a-1];
                                account[a-1][8]-=multiple*odp_fee;
                                od_count[a-1]+=multiple;
                            }
                        }
                    }
                }     
            }else{
                bal_check = current_bal -value -w_fee -add_fee;
                if(bal_check>0){
                    account[a-1][8]-=value;  
                    account[a-1][8]-=w_fee;
                    account[a-1][8]-=add_fee;
                }else{
                    if(account[a-1][7]==0){
                        printf("No Overdraft Protection for Account%d. Transaction denied.",a);
                    }else{
                        if(bal_check<-5000){
                            printf("Account%d has reached Maxinum Overdraft Amount. Transaction denied.",a);
                        }else{
                            o_check= account[a-1][8]/-500;
                            multiple=o_check+1-od_count[a-1];
                            if(bal_check-multiple*odp_fee<-5000){
                                printf("Account%d has reached Maxinum Overdraft Amount. Transaction denied.",a);
                            }else{
                                account[a-1][8]-=value;  
                                account[a-1][8]-=w_fee;
                                account[a-1][8]-=add_fee;    
                                account[a-1][8]-=multiple*odp_fee;
                                od_count[a-1]+=multiple;
                            }     
                        }
                        if(account[a-1][8]/-500>o_check){
                            if(account[a-1][8]-odp_fee<-5000){
                                od_count-=multiple;
                                account[a-1][8]+=multiple*odp_fee;
                                account[a-1][8]+=value;  
                                account[a-1][8]+=w_fee;
                                account[a-1][8]+=add_fee;
                                printf("Account%d has reached Maxinum Overdraft Amount. Transaction denied.",a);
                            }else{
                                o_check= account[a-1][8]/-500;
                                multiple=o_check+1-od_count[a-1];
                                account[a-1][8]-=multiple*odp_fee;
                                od_count[a-1]+=multiple;
                            }
                        }
                    }
                }    
            }
            account[a-1][5]--;
            p=strtok(NULL, " ");
            continue;
        }
        //transfer
        if(strstr(p,"t")){
            
            p=strtok(NULL, " ");//account a
            a_one[0]=p[1];
            a=atoi(a_one);

            t_fee = account[a-1][2];
            tran_lim= account[a-1][5];
            add_fee= account[a-1][6];
            odp_fee= account[a-1][7];
            current_bal= account[a-1][8];
              
            p=strtok(NULL, " ");//account b
            a_two[0]=p[1];
            b=atoi(a_two);

            t_fee_b = account[b-1][2];
            tran_lim_b= account[b-1][5];
            add_fee_b= account[b-1][6];
              
            p=strtok(NULL, " ");//amount
            value = atoi(p);
            
            if(tran_lim>0){
                bal_check = current_bal -value -t_fee;
                if(bal_check>=0){
                    account[a-1][8]-=value;  
                    account[a-1][8]-=t_fee;
                    account[b-1][8]+=value;
                    if(tran_lim_b>0){
                        account[b-1][8]-=t_fee_b;  
                    }else{
                        account[b-1][8]-=t_fee_b;
                        account[b-1][8]-=add_fee_b;
                    }
                }else{
                    if(account[a-1][7]==0){
                        printf("No Overdraft Protection for Account%d. Transaction denied.",a);
                    }else{
                        if(bal_check<-5000){
                            printf("Account%d has reached Maxinum Overdraft Amount. Transaction denied.",a);
                        }else{
                            o_check= account[a-1][8]/-500;
                            multiple=o_check+1-od_count[a-1];
                            if(bal_check-multiple*odp_fee<-5000){
                                printf("Account%d has reached Maxinum Overdraft Amount. Transaction denied.",a);
                            }else{
                                account[a-1][8]-=value;  
                                account[a-1][8]-=t_fee;   
                                account[a-1][8]-=multiple*odp_fee;
                                account[b-1][8]+=value;
                                if(tran_lim_b>0){
                                    account[b-1][8]-=t_fee_b;  
                                }else{
                                    account[b-1][8]-=t_fee_b;
                                    account[b-1][8]-=add_fee_b;
                                }
                                od_count[a-1]+=multiple;
                            }     
                        }
                        if(account[a-1][8]/-500>o_check){
                            if(account[a-1][8]-odp_fee<-5000){
                                od_count-=multiple;
                                account[a-1][8]+=multiple*odp_fee;
                                account[a-1][8]+=value;  
                                account[a-1][8]+=t_fee;
                                printf("Account%d has reached Maxinum Overdraft Amount. Transaction denied.",a);
                            }else{
                                o_check= account[a-1][8]/-500;
                                multiple=o_check+1-od_count[a-1];
                                account[a-1][8]-=multiple*odp_fee;
                                od_count[a-1]+=multiple;
                            }
                        }
                    }
                }     
            }else{
                bal_check = current_bal -value -w_fee -add_fee;
                if(bal_check>=0){
                    account[a-1][8]-=value;  
                    account[a-1][8]-=w_fee;
                    account[a-1][8]-=add_fee;
                    account[b-1][8]+=value;
                    if(tran_lim_b>0){
                        account[b-1][8]-=t_fee_b;  
                    }else{
                        account[b-1][8]-=t_fee_b;
                        account[b-1][8]-=add_fee_b;
                    }
                }else{
                    if(account[a-1][7]==0){
                        printf("No Overdraft Protection for Account%d. Transaction denied.",a);
                    }else{
                        if(bal_check<-5000){
                            printf("Account%d has reached Maxinum Overdraft Amount. Transaction denied.",a);
                        }else{
                            o_check= account[a-1][8]/-500;
                            multiple=o_check+1-od_count[a-1];
                            if(bal_check-multiple*odp_fee<-5000){
                                printf("Account%d has reached Maxinum Overdraft Amount. Transaction denied.",a);
                            }else{
                                account[a-1][8]-=value;  
                                account[a-1][8]-=w_fee;
                                account[a-1][8]-=add_fee;    
                                account[a-1][8]-=multiple*odp_fee;
                                account[b-1][8]+=value;
                                if(tran_lim_b>0){
                                    account[b-1][8]-=t_fee_b;  
                                }else{
                                    account[b-1][8]-=t_fee_b;
                                    account[b-1][8]-=add_fee_b;
                                }
                                od_count[a-1]+=multiple;
                            }     
                        }
                        if(account[a-1][8]/-500>o_check){
                            if(account[a-1][8]-odp_fee<-5000){
                                od_count-=multiple;
                                account[a-1][8]+=multiple*odp_fee;
                                account[a-1][8]+=value;  
                                account[a-1][8]+=w_fee;
                                account[a-1][8]+=add_fee;
                                printf("Account%d has reached Maxinum Overdraft Amount. Transaction denied.",a);
                            }else{
                                o_check= account[a-1][8]/-500;
                                multiple=o_check+1-od_count[a-1];
                                account[a-1][8]-=multiple*odp_fee;
                                od_count[a-1]+=multiple;
                            }
                        }
                    }
                }    
            }
                 
            account[a-1][5]--;
            account[b-1][5]--;
            p=strtok(NULL, " ");//operation
            continue;
        }
            
        
          
    }

    //leave critical section
    pthread_mutex_unlock(&mutex);
}

int main(){

    FILE *fp;
    fp=fopen("assignment_3_input_file.txt","r");    

    char line[256];

    int line_num=0;
    int acc_num=0;
    int cli_num=0;
    int dep_num=0;

    while(!feof(fp)){
		fgets(line,sizeof(line),fp);

        if(line[0]=='a'){
            acc_num++;
        }
        if(line[0]=='d'){
            dep_num++;
        }
        if(line[0]=='c'){
            cli_num++;
        }
        line_num++;	
    }
    
    int j;

    account = (int **)malloc(acc_num*sizeof(int *));
    for(j=0;j<acc_num;j++){
        account[j] = (int *)malloc(9*sizeof(int));
    }

    od_count=(int *)malloc(acc_num*sizeof(int));
    for(j=0;j<acc_num;j++){
        od_count[j]=0;
    }

    //allocate memory for a string array
    char **fLine;
    fLine=malloc(line_num*sizeof(char*));  
    for(int i=0;i<line_num;i++){  
        fLine[i]=malloc(4096*sizeof(char));  
    } 
    int fL=0;//number of lines read from the file
      
    rewind(fp);

    while(!feof(fp)){
		fgets(line,sizeof(line),fp);
        strcpy(fLine[fL],line);
        fL++;
    }
    rewind(fp);

    char reading[256];
    int value;
    int acc_index=0;

    //store account information into account array
    while(!feof(fp))
	{
        fscanf(fp,"%s",reading);//a1
        char type[50];
        strcpy(type,reading);

        fscanf(fp,"%s",reading);//type 
        
        if (strstr(reading,"type")){
            account[acc_index][0]=acc_index+1;//index of account
            
            fscanf(fp,"%s",reading);
            if(strstr(reading,"bus")){
                account[acc_index][1]=0;//0 indecate business account
            }else{
                account[acc_index][1]=1;//1 indicate personal account
            }
            fscanf(fp,"%s",reading);//deposit
            fscanf(fp,"%s",reading);//deposit fee
            value = atoi(reading);
            account[acc_index][2]=value;
            fscanf(fp,"%s",reading);//withdraw
            fscanf(fp,"%s",reading);//withdraw fee
            value = atoi(reading);
            account[acc_index][3]=value;
            fscanf(fp,"%s",reading);//transfer
            fscanf(fp,"%s",reading);//transfer fee
            value = atoi(reading);
            account[acc_index][4]=value;
            fscanf(fp,"%s",reading);//transactions
            fscanf(fp,"%s",reading);//limit number
            value = atoi(reading);
            account[acc_index][5]=value;
            fscanf(fp,"%s",reading);//additional fee
            value = atoi(reading);
            account[acc_index][6]=value;
            fscanf(fp,"%s",reading);//overdraft
            fscanf(fp,"%s",reading);//Y or N
            if(strstr(reading,"Y")){
                fscanf(fp,"%s",reading);
                value = atoi(reading);
                account[acc_index][7]=value;
            }else{
                account[acc_index][7]=0;
            }
            account[acc_index][8]=0;//balance
            acc_index++;   
        }
    }
    fclose(fp);
    
    //acc_print(account,acc_num);

    //Account information initialized.
    //acc_print(account,acc_num);
    printf("\n");
    //----------------------------------------------------------------------------------------------------------
    
    int x;
    int thr_num =line_num-acc_num;
    
    pthread_t threads_dep[dep_num];
    pthread_t threads[cli_num];
    
    if (pthread_mutex_init(&mutex_dep, NULL) != 0){
        printf("\n mutex init failed\n");
        return 1;
    }  

    printf("From main: Going to create Depositor Threads...\n");
    for (x = acc_num; x<(acc_num+dep_num); x++){
        //printf("depositor line:%s\n",fLine[x]);
        pthread_create(&threads_dep[x], NULL, depo_run, (void *)fLine[x]);
    }
	
    for (x = 0; x<dep_num; x++){
        pthread_join(threads_dep[x], NULL);	
    }
	
    pthread_mutex_destroy(&mutex_dep); 

    if (pthread_mutex_init(&mutex, NULL) != 0){
        printf("\n mutex init failed\n");
        return 1;
    }  
    
	printf("From main: Going to create Client Threads...\n");
    for (x = (acc_num+dep_num); x<line_num; x++){
        //printf("client line:%s\n",fLine[x]);
        pthread_create(&threads[x], NULL, thread_run, (void *)fLine[x]);
    }
	
    for (x = 0; x<cli_num; x++){
        pthread_join(threads[x], NULL);	
    }
	
    pthread_mutex_destroy(&mutex); 
    
    acc_print(account,acc_num);

    for (j = 0; j < acc_num; j++){
        free(account[j]);
    }
    free(account);
    free(od_count);
    return 0;
    return 0;
}