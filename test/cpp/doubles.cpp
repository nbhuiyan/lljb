double foo(){
    double x = 3.1;
    double y = 0.1;
    if (x > 0){
        x = x + 1.1;
        y = y + 1.1;
    }
    return (x - y);
}

int main(){
    return (int) foo();
}
