#include<kernel/types.h>
#include<user/user.h>

int lpipe_first_data(int lp[2],int* first)
{
    if(read(lp[0],first,sizeof(int))==sizeof(int))
    {
        printf("prime %d\n", *first);
        return 0;
    }
    return -1;
}

void transmit_data(int lp[2],int rp[2],int first)
{
    int data;
    while(read(lp[0],&data,sizeof(int))==sizeof(int))
    {
        if(data%first)
            write(rp[1],&data,sizeof(int));
    }
    close(lp[0]);
    close(rp[1]);
}

void primes(int lp[2])
{
    close(lp[1]);
    int first;
    if(lpipe_first_data(lp,&first)==0){
        int p[2];
        pipe(p);
        transmit_data(lp,p,first);
        if(fork()==0)
        {
            primes(p);
            close(p[0]);
        }
        else{
            close(p[0]);
            wait(0);
        }
    }
    exit(0);
}

int main()
{
    int p[2];
    pipe(p);
    for(int i=2;i<=35;i++)
    {
        write(p[1],&i,sizeof(int));
    }
    if(fork()==0)
    {
        primes(p);
    }
    else{
        close(p[0]);
        close(p[1]);
        wait(0);
    }
    exit(0);
}