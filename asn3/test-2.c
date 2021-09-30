#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
pthread_mutex_t mutex;

int count;

typedef struct {
    int **account;
} Accounts;

void acc_print(int (*account)[3]){
    for(int r=0;r<4;r++){
        for(int q=0;q<3;q++){
            if(q==2){
                printf("%d\n",account[r][q]);
            }else{
                printf("%d ",account[r][q]);
            }
        }   
    }
}

int main(){
    int m=9;
    int n=4;//row
    Accounts temp;
    int **a;
    a = (int **)malloc(n*sizeof(int *));
   for(int i=0;i<n;i++){
     a[i] = (int *)malloc(m*sizeof(int));
    }
    for(int p=0;p<n;p++){
        for(int q=0;q<m;q++){
            a[p][q]=q;
        }
    }
    //acc_print(a);
    
   temp.account = (int **)malloc(n*sizeof(int *));
   for(int i=0;i<n;i++){
     temp.account[i] = (int *)malloc(m*sizeof(int));
    }
    temp.account=a;
    //printf("value is %d",temp.account[2][4]);
    

    char line[100]="practice makes perfect";
    int cout=0;
    char *p ;
    p = strtok(line," ");
    while(p!=NULL){
        p = strtok(NULL," ");
        cout++;
    }

    //printf("cout is %d",cout);

    int *od_count;
    od_count=(int *)malloc(6*sizeof(int));
    for(int j=0;j<5;j++){
        od_count[j]=0;
    }
    int w=10;
    int u=1040;
    int v=500;
    int ac[2][2];
    ac[2][0]=1;
    if(u%-500==0){
        printf("correct\n");
    }
    printf("u/v: %d",u/v);
    return 0;
    
   
}