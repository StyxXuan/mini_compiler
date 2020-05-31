struct a{
    int p;
    int t;
};

int fact(int z){
    Print(z);
    if(z > 1){
        return fact(z-1)*z;
    }else{
        return 1;
    }
}

int main()
{
    struct a b;
    int d;
    b.p = 2;
    b.t = 3;
    d = fact(fact(b.p)) + fact(b.t) - 20;
    Print(d);
}

