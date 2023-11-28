#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//#include <sys/wait.h>

int main(){
       
       system ("echo $name; whoami; uname -a; basename \"$(pwd)\"");
       system("date; whoami; ps; ls -l");
}      

