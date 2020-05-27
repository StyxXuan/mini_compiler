int a;

int fact(int p){
    int c = 0;
    c = p + 1;
    return c;
}

int main(){
    int a = 10 + 24*2;
    int b;
    if(a > 0){
        b = 10;
    }else{
        b = fact(9);
    }

    while(a>0){
        a = a+1;
    }
}

