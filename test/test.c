int b;
int fact(int p){
    int c = 0;
    c = p + 1;
    return c;
}

int main(){
    int a = 10 + 24*2;
    int d;
    if(a < 0){
        d = 10;
    }else{
        d = fact(9);
    }

    while(a>0){
        a = a+1;
    }
}

