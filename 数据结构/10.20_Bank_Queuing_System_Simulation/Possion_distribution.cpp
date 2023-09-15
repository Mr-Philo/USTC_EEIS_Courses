#include<random>
#include<iostream>

int main() 
{
    std::random_device id;
    std::default_random_engine time(id());//随机数种⼦
    std::poisson_distribution<int>Generator(5);//5是泊松分布均值
    int Poisson_Data[100],Sum=0;
    for (int i = 0; i < 100; i++)
    {
        Poisson_Data[i] = Generator(time);//⽣成服从泊松分布的数据
        printf("%2d ", Poisson_Data[i]);
        if((i+1)%10==0)
        std::cout << std::endl;
        Sum += Poisson_Data[i];
    }
    std::cout << "Avg:" << float(Sum / 100.0 )<< std::endl;
    getchar();
}