#include<iostream>
#include"f.hh"
int main()
{
    Circle circle1(1,2,1);
    TangentCircle tangentcircle1(1,5,1);
    ConformalCircle conformalcircle1(1,1,1);
    ConformalCircle conformalcircle2(1,1,1);
    tangentcircle1.set_TangentCircleInf(1,5, circle1);
    conformalcircle1.get_ConformalCircleInf(circle1);
    conformalcircle2.get_ConformalTangentCircleInf(tangentcircle1);
    std::cout<<"圆方程为："<<circle1.get_CircleFunction()<<std::endl;
    std::cout<<"切圆方程为："<<tangentcircle1.get_TangentCircleFunction(circle1)<<std::endl;
    std::cout<<"圆保角方程为："<<conformalcircle1.get_ConformalCircleFunction(circle1)<<std::endl;
    std::cout<<"切圆保角方程为："<<conformalcircle2.get_ConformalTangentCircleFunction()<<std::endl;
    return 0;
}
