struct Foo {
    int x;
    int y[2];
};

int fooMemberAdder(struct Foo * foo){
    return foo->x + foo->y[1];
}

int main(){
    int x = 1;
    Foo foo = {x,{1,2}};
    return fooMemberAdder(&foo);
}
