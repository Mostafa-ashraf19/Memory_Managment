#include "Memorygraph.h"
#include <map>
#include<random>
#include <QDebug>
void Memorygraph::RemoveSer()
{
    delete  this->series;
}

Memorygraph::Memorygraph()
{

}

void Memorygraph::BuildMemory(double MemSize)
{
    this->MemSize = MemSize;

    rawMemory = new QBarSet("Raw Memory");
    rawMemory->append(MemSize);
    rawMemory->setColor(QColor(148,170,176));

    /* QStackedBarSeries */
    series = new QStackedBarSeries();
    series->append(rawMemory);

    axisY = new QValueAxis();
    axisY->setTickCount(20);
    axisY->setReverse(true);

    chart = new QChart();
    chart->setTitle("Memory Manipulation\n");

    chart->setAnimationOptions(QChart::SeriesAnimations);

    chart->addSeries(series);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    chartview = new QChartView(chart);
}

void Memorygraph::AddHoles(double StartAddress,double HoleSize)
{
    delete  chartview;

    Holes.push_back(std::make_pair(StartAddress,HoleSize));

    series= new QStackedBarSeries();
    int prev = 0;

    for(auto& hole : Holes)
    {
        QBarSet* SetN = new QBarSet("old Process");
        SetN->setColor(QColor(148,170,176));
        SetN->append((hole.first-prev));
        series->append(SetN);

        QBarSet* SetM = new QBarSet("Hole");
        SetM->setColor(QColor(125, 125, 125));
        SetM->append(hole.second);
        series->append(SetM);
//QColor(148,170,176)   QColor(125, 125, 125) QColor(255, 2, 2)
        prev = hole.first + hole.second;
    }
      QBarSet* SetF = new QBarSet("old Process");
      SetF->setColor(QColor(148,170,176));
      SetF->append(this->MemSize-(Holes.back().first+Holes.back().second));
      series->append(SetF);


    chart = new QChart();
    chart->addSeries(series);

    chart->setTitle("Memory Manipulation\n");

    chart->setAnimationOptions(QChart::SeriesAnimations);

    QValueAxis* AxisY = new QValueAxis();
    AxisY->setTickCount(20);
    AxisY->setReverse(true);
    chart->addAxis(AxisY,Qt::AlignLeft);
    series->attachAxis(AxisY);

    chart->legend()->setVisible(false);

    chart->legend()->setAlignment(Qt::AlignBottom);

    chartview = new QChartView(chart);

}
void Memorygraph::Draw(std::vector<allocated_item_information> AllocationsInfo,vector<string> Processname)
{
//    "free space"
    delete  chartview;
//    delete chart;

    series= new QStackedBarSeries();
    int ProcessSize = Processname.size();
    std::map<std::string,QColor> proc;
    for(int i=0;i<ProcessSize;i++)
    {
        std::srand(i);
        proc[Processname[i]] = QColor(std::rand()%255,std::rand()%255,std::rand()%255); //color;
    }
    for(auto& VecInFo : AllocationsInfo)
    {

        if(VecInFo.process_name == "free space")
        {
            QBarSet* SetM = new QBarSet("Hole");
            SetM->setColor(QColor(125, 125, 125));
            SetM->append(VecInFo.limit);
            series->append(SetM);

        }
        else if (VecInFo.process_name == "allocated space")
        {
            QBarSet* SetM = new QBarSet("Old Process");
            SetM->setColor(QColor(148,170,176));
            SetM->append(VecInFo.limit);
            series->append(SetM);
        }
        else
        {
            QBarSet* SetM = new QBarSet("Process");
//            SetM->setColor(QColor(255, 2, 2));
            SetM->setColor(proc[VecInFo.process_name]);
            SetM->append(VecInFo.limit);
            series->append(SetM);
        }

        qDebug() << QString::fromStdString(VecInFo.process_name) << " " << VecInFo.limit;

    }

    chart = new QChart();
    chart->addSeries(series);

    chart->setTitle("Memory Manipulation\n");

    chart->setAnimationOptions(QChart::SeriesAnimations);

    QValueAxis* AxisY = new QValueAxis();
    AxisY->setTickCount(20);
    AxisY->setReverse(true);
    chart->addAxis(AxisY,Qt::AlignLeft);
    series->attachAxis(AxisY);

    chart->legend()->setVisible(false);

    chart->legend()->setAlignment(Qt::AlignBottom);

    chartview = new QChartView(chart);



}

QStackedBarSeries* Memorygraph::SeriesRef() const
{
    return (this->series);
}


void Memorygraph::UpDateGraph()
{

}

QChartView *Memorygraph::ViewChart()
{
    return  chartview;
}

void Memorygraph::CLeaRData()
{
    delete  chartview;
    this->Holes.clear();
}

