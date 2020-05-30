struct person{
    int height;
    int weight;
};

int cal_bmi(int h, int w){
    return w / (h*h);
}

int main(){
    struct person a;
    int result;
    a.height = 10;
    a.weight = 40;

    result = cal_bmi(a.height, a.weight);
}