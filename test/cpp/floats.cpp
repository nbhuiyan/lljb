float foo(){
    float x = 3.1;
    float y = 0.1;
    if (x > 0){
        x = x + 1.1f;
        y = y + 1.1f;
    }
    return (x - y);
}

int main(){
    return (int) foo();
}
