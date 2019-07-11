struct Foo {
    int x;
    int y;
};

int fooMemberAdder(struct Foo * foo){
    return foo->x + foo->y;
}

int main(){
    int x = 1;
    int y = 2;
    Foo foo = {x,y};
    return fooMemberAdder(&foo);
}
