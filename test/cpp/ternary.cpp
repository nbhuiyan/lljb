int main(){
    int x = 1;
    int y = x ? 2 : 3;
    int z = x + y;

    float a = 0.0f;
    float b = a ? 1.0f : 0.0f;
    float c = a + b;

    return z + c;
}
