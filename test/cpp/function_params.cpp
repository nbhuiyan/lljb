void increment(int * valPtr, int addend){
    *valPtr += addend;
}

int addTwoNumbers(int param1, int param2){
    return param1+param2;
}

int main(){
    int x = 0;
    int y = 0;
    increment(&x,1);
    increment(&y,2);
    return addTwoNumbers(x,y);
}
