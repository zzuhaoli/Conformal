#include<iostream>
class Circle
{
  public:
    double m_CircleXPosition;
    double m_CircleYPosition;
    double m_CircleRadius;
    Circle(double CircleXPosition,double CircleYPosition, double CircleRadius);
    std::string get_CircleFunction();
    
};

class TangentCircle
{
  public:
    double m_TangentCircleXPosition;
    double m_TangentCircleYPosition;
    double m_TangentCircleRadius;
    TangentCircle(double TangentCircleXPosition,double TangentCircleYPosition, double TangentCircleRadius);
    void set_TangentCircleInf(double TangentCircleXPosition,double TangentCircleYPosition,const Circle &circle);
    std::string get_TangentCircleFunction(const Circle &circle);
};

class ConformalCircle
{
  public:
    double m_ConformalCircleXPosition;
    double m_ConformalCircleYPosition;
    double m_ConformalCircleRadius;
    ConformalCircle(double ConformalCircleXPosition,double  ConformalCircleYPosition, double  ConformalCircleRadius);
    void get_ConformalCircleInf(const Circle &circle);
    void get_ConformalTangentCircleInf(const TangentCircle &tangentcircle);
    std::string get_ConformalCircleFunction(const Circle &circle);
    std::string get_ConformalTangentCircleFunction();
};

