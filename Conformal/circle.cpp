#include<iostream>
#include"f.hh"
#include<cmath>
//初始化圆
Circle::Circle(double CircleXPosition,double CircleYPosition, double CircleRadius):m_CircleXPosition(CircleXPosition),m_CircleYPosition(CircleYPosition),m_CircleRadius(CircleRadius){}
//返回圆方程
std::string Circle::get_CircleFunction()
{
    std::string function = "(x-" + std::to_string(m_CircleXPosition) + ")^2+(y-" + std::to_string(m_CircleYPosition) + ")^2=" + std::to_string(pow(m_CircleRadius, 2));
    return function;
}
//初始化切圆方程
TangentCircle::TangentCircle(double TangentCircleXPosition,double TangentCircleYPosition, double TangentCircleRadius):m_TangentCircleXPosition(TangentCircleXPosition),m_TangentCircleYPosition(TangentCircleYPosition),m_TangentCircleRadius(TangentCircleRadius){}
//计算切圆半径
 void TangentCircle::set_TangentCircleInf(double TangentCircleXPosition,double TangentCircleYPosition, const Circle &circle)
{
     m_TangentCircleXPosition=TangentCircleXPosition;
     m_TangentCircleYPosition=TangentCircleYPosition;
     double result=sqrt(pow(m_TangentCircleXPosition-circle.m_CircleXPosition,2)+pow(m_TangentCircleYPosition-circle.m_CircleYPosition,2));
        if(result<circle.m_CircleRadius)
        {

            m_TangentCircleRadius=circle.m_CircleRadius-result;
        }
        else
        {
            m_TangentCircleRadius=result-circle.m_CircleRadius;
        }
}
//返回切圆方程
std::string TangentCircle::get_TangentCircleFunction(const Circle &circle)
{
    double result=sqrt(pow(m_TangentCircleXPosition-circle.m_CircleXPosition,2)+pow(m_TangentCircleYPosition-circle.m_CircleYPosition,2));
	if(result<circle.m_CircleRadius)
	{   
	    
	    m_TangentCircleRadius=circle.m_CircleRadius-result;
	}
	else
	{
	    m_TangentCircleRadius=result-circle.m_CircleRadius;
	}
    std::string function = "(x-" + std::to_string(m_TangentCircleXPosition) + ")^2+(y-" + std::to_string(m_TangentCircleYPosition) + ")^2=" + std::to_string(pow(m_TangentCircleRadius, 2));
    return function;
}
//初始化保角属性
ConformalCircle::ConformalCircle(double ConformalCircleXPosition,double  ConformalCircleYPosition, double  ConformalCircleRadius):m_ConformalCircleXPosition(ConformalCircleXPosition),m_ConformalCircleYPosition(ConformalCircleYPosition),m_ConformalCircleRadius(ConformalCircleRadius){}
//获取圆保角属性
void ConformalCircle::get_ConformalCircleInf(const Circle &circle)
{
   if(pow(circle.m_CircleXPosition,2)+pow(circle.m_CircleYPosition,2)==pow(circle.m_CircleRadius,2))
     {
        m_ConformalCircleXPosition=circle.m_CircleXPosition;
	m_ConformalCircleYPosition=circle.m_CircleYPosition;
     }
   else
     {
        m_ConformalCircleXPosition=2*circle.m_CircleXPosition/ (pow(circle.m_CircleXPosition,2)+ pow(circle.m_CircleYPosition,2)-pow(circle.m_CircleRadius,2));
        m_ConformalCircleYPosition=2*circle.m_CircleYPosition/ (pow(circle.m_CircleXPosition,2)+ pow(circle.m_CircleYPosition,2)-pow(circle.m_CircleRadius,2));
        m_ConformalCircleRadius=2*circle.m_CircleRadius/ (pow(circle.m_CircleXPosition,2)+ pow(circle.m_CircleYPosition,2)-pow(circle.m_CircleRadius,2));
     }
}
//获取切圆保角属性
void ConformalCircle::get_ConformalTangentCircleInf(const TangentCircle &tangentcircle)
{
    m_ConformalCircleXPosition=2*tangentcircle.m_TangentCircleXPosition/(pow(tangentcircle.m_TangentCircleXPosition,2)+ pow(tangentcircle.m_TangentCircleYPosition,2)-pow(tangentcircle.m_TangentCircleRadius,2));
    m_ConformalCircleYPosition=2*tangentcircle.m_TangentCircleYPosition/(pow(tangentcircle.m_TangentCircleXPosition,2)+ pow(tangentcircle.m_TangentCircleYPosition,2)-pow(tangentcircle.m_TangentCircleRadius,2));
    m_ConformalCircleRadius=2*tangentcircle.m_TangentCircleRadius/(pow(tangentcircle.m_TangentCircleXPosition,2)+ pow(tangentcircle.m_TangentCircleYPosition,2)-pow(tangentcircle.m_TangentCircleRadius,2));
}
//返回圆保角方程
std::string ConformalCircle::get_ConformalCircleFunction(const Circle &circle)
{
     if(pow(circle.m_CircleXPosition,2)+pow(circle.m_CircleYPosition,2)==pow(circle.m_CircleRadius,2))
     {
	  std::string function = "x*" + std::to_string(m_ConformalCircleXPosition) + "+y*" + std::to_string(m_ConformalCircleYPosition)+"=1";
    return function;
     }	     
     else
     {
	  std::string function = "(x-" + std::to_string(m_ConformalCircleXPosition) + ")^2+(y-" + std::to_string(m_ConformalCircleYPosition) + ")^2=" + std::to_string(pow(m_ConformalCircleRadius, 2));
    return function;
     }
}
//返回切圆保角方程
std::string ConformalCircle::get_ConformalTangentCircleFunction()
{
     std::string function = "(x-" + std::to_string(m_ConformalCircleXPosition) + ")^2+(y-" + std::to_string(m_ConformalCircleYPosition) + ")^2=" + std::to_string(pow(m_ConformalCircleRadius, 2));
    return function;
}
