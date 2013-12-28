#include <stdio.h>

int main(){
    printf("{");
    for(int i = 0; i < 9999; i++){
        printf("%d, ", i);
    }
    printf("10000}");
}
