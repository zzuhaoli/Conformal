#include <TCanvas.h>
#include <TGraph.h>
#include <circle.hh>
#include <cmath>
void drawCircle() {
  // 定义圆的参数
  Circle c1(1, 1, sqrt(2));
  TangentCircle tc1(1, 1, 0);
  TangentCircle tc2(1, 1, 0);
  tc1.set_TangentCircleInf(1, 1.9, c1);
  tc2.set_TangentCircleInf(2.6, 0, c1);
  ConformalCircle cc1(1, 1, 1);
  ConformalCircle ctc1(1, 1, 1);
  ConformalCircle ctc2(1, 1, 1);
  cc1.get_ConformalCircleInf(c1);
  ctc1.get_ConformalTangentCircleInf(tc1);
  ctc2.get_ConformalTangentCircleInf(tc2);
  // double centerX[6] =
  // {c1.m_CircleXPosition,tc1.m_TangentCircleXPosition,tc2.m_TangentCircleXPosition,cc1.m_ConformalCircleXPosition,ctc1.m_ConformalCircleXPosition,ctc2.m_ConformalCircleXPosition};
  // double centerY[6] =
  // {c1.m_CircleYPosition,tc1.m_TangentCircleYPosition,tc2.m_TangentCircleYPosition,cc1.m_ConformalCircleYPosition,ctc1.m_ConformalCircleYPosition,ctc2.m_ConformalCircleYPosition};
  // double radius[6] = {c1.m_CircleRadius, tc1.m_TangentCircleRadius,
  // tc2.m_TangentCircleRadius,cc1.m_ConformalCircleRadius,ctc1.m_ConformalCircleRadius,ctc2.m_ConformalCircleRadius};
  double centerX[5] = {c1.m_CircleXPosition, tc1.m_TangentCircleXPosition,
                       tc2.m_TangentCircleXPosition,
                       ctc1.m_ConformalCircleXPosition,
                       ctc2.m_ConformalCircleXPosition};
  double centerY[5] = {c1.m_CircleYPosition, tc1.m_TangentCircleYPosition,
                       tc2.m_TangentCircleYPosition,
                       ctc1.m_ConformalCircleYPosition,
                       ctc2.m_ConformalCircleYPosition};
  double radius[5] = {c1.m_CircleRadius, tc1.m_TangentCircleRadius,
                      tc2.m_TangentCircleRadius, ctc1.m_ConformalCircleRadius,
                      ctc2.m_ConformalCircleRadius};

  // 定义圆上的点数
  int numPoints = 5000;
  auto mg = new TMultiGraph();
  TGraph *g[6];
  // 创建新画布
  TCanvas *canvas = new TCanvas("canvas", "Multiple Circles", 600, 600);

  // 绘制多个圆
  for (int i = 0; i < 5; i++) {
    // 设置坐标点
    g[i] = new TGraph(numPoints);
    for (int j = 0; j < numPoints; ++j) {
      double theta = 2 * TMath::Pi() * j / numPoints;
      double x = centerX[i] + radius[i] * TMath::Cos(theta);
      double y = centerY[i] + radius[i] * TMath::Sin(theta);
      g[i]->SetPoint(j, x, y);
      if (i < 3) {
        g[i]->SetLineColor(1);
      } else {
        g[i]->SetLineColor(2);
      }
      g[i]->SetLineWidth(2);
    }
    // double a=cc1.m_ConformalCircleXPosition;
    // double b=cc1.m_ConformalCircleYPosition;
    // TF1 *f1= new TF1("f","(1-[a]*x)/[b]",-5,5);
    TF1 *f1 = new TF1("f", "1-1*x", -5, 5);
    g[5] = new TGraph(f1);
    g[5]->SetLineColor(2);
    mg->Add(g[i]);
    mg->Add(g[5]);
    mg->Draw("APL"); // 绘制线条
  }

  // 显示画布
  canvas->Update();
  std::cout << "圆方程为：" << c1.get_CircleFunction() << std::endl;
  std::cout << "第一个切圆方程为：" << tc1.get_TangentCircleFunction(c1)
            << std::endl;
  std::cout << "第二个切圆方程为：" << tc2.get_TangentCircleFunction(c1)
            << std::endl;
  std::cout << "保角圆方程为：" << cc1.get_ConformalCircleFunction(c1)
            << std::endl;
  std::cout << "第一个保角切圆方程为："
            << ctc1.get_ConformalTangentCircleFunction() << std::endl;
  std::cout << "第二个保角切圆方程为："
            << ctc2.get_ConformalTangentCircleFunction() << std::endl;
}
