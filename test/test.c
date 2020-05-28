struct a{
    int p;
    int t;
};

int fact(int z){
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
    b.p = 3;
    b.t = -2;
    d = fact(fact(b.p)) + fact(b.t) - 20;
}

