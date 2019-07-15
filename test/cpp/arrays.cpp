void foo(int * x){
    x[3] = 2;
}

int main(){
    int table1[2];
    int table2[2][2];
    table1[0] = 1;
    foo((int *)table2);
    int z = table1[0] + table2[1][1];
    return z;
}
