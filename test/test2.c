int fact(int a){
    if(a>1){
        int b = fact(a-1);
        return a*b;
    }else{
        return 1;
    }
}
int main(){
    int a = fact(3);
}