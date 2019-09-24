#include <cstdio>

void print_condition(int condition){
    printf("condition: %d\n", condition);
}

int main(){
    int condition = 0;

    print_condition(condition);

    switch (condition){
        case 0:
            printf("case 0 reached\n");
            break;
        case 1:
            printf("case 1 reached\n");
            break;
        case 2:
            printf("case 2 reached\n");
            break;
        default:
            printf("default case reached\n");
            break;
    }
    return 0;
}
