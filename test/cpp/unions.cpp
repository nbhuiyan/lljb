#include <cstdint>

union Foo{
    int32_t m1;
    int64_t m3;
};

int main(){
    Foo foo;
    Foo bar;
    foo.m1 = 1;
    bar.m3 = 2;
    return foo.m1 + bar.m3;
}
