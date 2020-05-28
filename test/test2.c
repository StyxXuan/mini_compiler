int fact(int a){
    if(a>1){
        int b = fact(a-1);
        return a*b;
    }else{
        return 1;
    }
}

struct Str{
    int stra;
    int strb;
};

int main(){
    struct Str p;
    p.stra = 1;
    p.strb = 2;

    int a = fact(p.stra);
}