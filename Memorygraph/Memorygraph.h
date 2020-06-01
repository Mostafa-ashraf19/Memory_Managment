#ifndef MEMORYGRAPH_H
#define MEMORYGRAPH_H

#include <QtCharts/QChartView>
#include <QtCharts/QStackedBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>

#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QChart>
#include"CodeBase/memory_mangement.h"


using namespace QtCharts;
class Memorygraph
{



 QValueAxis *axisY ;
 QChart * chart;
 QChartView* chartview;

 QBarSet* rawMemory;

 QStackedBarSeries *series;
    double  MemSize,
            HoleStartAdd,
            HoleSize;
std::vector<std::pair<double,double>> Holes;

public:
    void RemoveSer();
    Memorygraph();
    void BuildMemory(double MemSize);
    void AddHoles(double StartAddress,double HoleSize);
    void UpDateGraph();
    QChartView* ViewChart();
    void CLeaRData();
    void Draw(std::vector<allocated_item_information> AllocationsInfo,vector<string>);
public slots:
    QStackedBarSeries* SeriesRef()const;

signals:
   void Clicked(int index);
};

#endif // MEMORYGRAPH_H
