int main(){
    int x = 0;
    int y = 0;
    int i = 0;

    while (i < 10){
        x += 1;
        i++;
    }

    for (i = 0; i < 7; i++){
        y += 1;
    }

    return x - y;
}
