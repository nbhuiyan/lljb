#include <cstdio>

float foo(){
    float x = 3.1f;
    float y = 0.1f;
    if (x > 0.0f){
        x = x + 1.2f;
        y = y + 1.1f;
    }
    return (x - y);
}

int main(){
    return (int) foo();
}
