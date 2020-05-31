int gcd(int n, int m){
    int mode;
    mode = m%n;

    if(mode == 0){
        return n;
    }else{
        return gcd(n, mode);
    }
}

int main()
{
    int res;
    res = gcd(25, 20);
    Print(res);
}