#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupqwt();
    readdata();
    curves.at(0)->setSamples(x,ch0);
    curves.at(1)->setSamples(x,ch1);
    curves.at(2)->setSamples(x,ch2);
    curves.at(3)->setSamples(x,ch3);
    curves.at(0)->attach(ui->qwtPlot);
    curves.at(1)->attach(ui->qwtPlot);
    curves.at(2)->attach(ui->qwtPlot);
    curves.at(3)->attach(ui->qwtPlot);
    curves.at(0)->setYAxis(QwtPlot::yLeft);
    curves.at(1)->setYAxis(QwtPlot::yRight);
    curves.at(2)->setYAxis(QwtPlot::yLeft);
    curves.at(3)->setYAxis(QwtPlot::yRight);
    ui->qwtPlot->replot();


}

MainWindow::~MainWindow()
{
    delete ui;
    curves.clear();
    delete zoom_x;
    delete zoom_y;
    delete grid;

}

void MainWindow::setupqwt()
{
    ui->qwtPlot->setCanvasBackground(Qt::black);

    grid = new QwtPlotGrid;
    grid->enableXMin(true);
    grid->setMajorPen(QPen(Qt::white,1,Qt::DotLine));
    grid->setMinorPen(QPen(Qt::gray,1,Qt::DotLine));
    grid->attach(ui->qwtPlot);
    ui->qwtPlot->enableAxis(QwtPlot::yRight);

    QwtPlotMagnifier *zoom_x = new QwtPlotMagnifier( ui->qwtPlot->canvas());
    QwtPlotMagnifier *zoom_y = new QwtPlotMagnifier( ui->qwtPlot->canvas() );
    zoom_x->setMouseButton(Qt::MidButton);
    zoom_x->setWheelModifiers(Qt::ShiftModifier);
    zoom_x->setAxisEnabled(QwtPlot::xBottom, true);
    zoom_x->setAxisEnabled(QwtPlot::yRight ,false);
    zoom_x->setAxisEnabled(QwtPlot::yLeft ,false);
    zoom_y->setMouseButton(Qt::MidButton);
    zoom_y->setWheelModifiers(Qt::ControlModifier);
    zoom_y->setAxisEnabled(QwtPlot::xBottom,false);
    zoom_y->setAxisEnabled(QwtPlot::yRight,true);
    zoom_y->setAxisEnabled(QwtPlot::yLeft,true);

    QwtPlotPanner *d_panner = new QwtPlotPanner(ui->qwtPlot->canvas() );
    d_panner->setMouseButton( Qt::RightButton );

    curves.append(new QwtPlotCurve("ch0"));
    curves.append(new QwtPlotCurve("ch1"));
    curves.append(new QwtPlotCurve("ch2"));
    curves.append(new QwtPlotCurve("ch3"));
    curves.at(0)->setPen(QPen(Qt::darkRed,2,Qt::SolidLine));
    curves.at(1)->setPen(QPen(Qt::green ,2,Qt::SolidLine));
    curves.at(2)->setPen(QPen(Qt::red,2,Qt::SolidLine));
    curves.at(3)->setPen(QPen(Qt::darkGreen,2,Qt::SolidLine));

}

void MainWindow::readdata()
{
    file.setFileName("/home/heavy/Документы/1/1.dat");
    x.clear();
    if ( file.open(QIODevice::ReadWrite) )
    {
        quint8 data;
        file.seek(0);
        for (qint64 i=0; i<file.size(); i++)
        {
           datastream.setDevice(&file);
           datastream>>data;
           x.append(i);
           switch (i%4) {
           case 0:

               ch0.append((quint8)data);
               break;
           case 1:
               ch1.append((quint8)data);
               break;
           case 2:
               ch2.append((quint8)data);
               break;
           case 3:
               ch3.append((quint8)data);
               break;
           }
        }
        file.close();
    }
}

void MainWindow::on_lineEdit_returnPressed()
{
    bool ok;
    for (int i = 0; i<x.size();i++)
    {
        x[i]=i-ui->lineEdit->text().toInt(&ok);
    }

    curves.at(0)->setSamples(x,ch0);
    curves.at(3)->setSamples(x,ch3);
    ui->qwtPlot->replot();
}

QVector<qreal> MainWindow::filter(QVector<qreal> *in, qreal coeff )              //in - вход фильтра, coeff - коэф.фильтра от 0 до 1
{
    QVector<qreal> out(in->size());                                         //Выход фильтра
    for (int i=1; i<in->size(); i++)
    {
        out[i]=coeff*in->at(i) + (1.0-coeff)*out.at(i-1);                   //сам фильтр
    }
    return out;

}
void MainWindow::resetAndPrepareData()
{
    ch0.clear();
    ch1.clear();
    ch2.clear();
    ch3.clear();
    readdata();
}

void MainWindow::on_lineEdit_2_returnPressed()
{
    bool ok;
    resetAndPrepareData();
    ch0=filter(&ch0,ui->lineEdit_2->text().toFloat(&ok));
    ch1=filter(&ch1,ui->lineEdit_2->text().toFloat(&ok));
    ch2=filter(&ch2,ui->lineEdit_2->text().toFloat(&ok));
    ch3=filter(&ch3,ui->lineEdit_2->text().toFloat(&ok));
    curves.at(0)->setSamples(x,ch0);
    curves.at(1)->setSamples(x,ch1);
    curves.at(2)->setSamples(x,ch2);
    curves.at(3)->setSamples(x,ch3);
    ui->qwtPlot->replot();
}
QVector<qreal> MainWindow::integrate(QVector<qreal> *in, qreal zeroLevel)
{
    QVector<qreal> out(in->size());
    for (int i=1; i<in->size(); i++)
    {
        if (in->at(i)>zeroLevel)
        {
            out[i]=in->at(i) + out.at(i-1);
        }
        else
        {
            out[i]=0;
        }
    }

    return out;
}
void MainWindow::reverseSignal()
{
  for (int i=0; i<ch1.size();i++)
  {
      if (ch1.at(i)>max[1]) max[1]=ch1.at(i);
  }
  for (int i=0; i<ch3.size();i++)
  {
      if (ch3.at(i)>max[1]) max[1]=ch1.at(i);
  }
  for (int i=0; i<ch1.size();i++)
  {
      ch1[i]=ch1.at(i)*(-1)+max[1];
  }
  for (int i=0; i<ch3.size();i++)
  {
      ch3[i]=ch3.at(i)*(-1)+max[1];
  }
  curves.at(1)->setSamples(x,ch1);
  curves.at(3)->setSamples(x,ch3);
}

void MainWindow::on_checkBox_toggled(bool checked)
{

    resetAndPrepareData();
    if (checked)
    {
        integrateAllPiezo();
    }
    else
    {
        resetAndPrepareData();
    }
    curves.at(1)->setSamples(x,ch1);
    curves.at(3)->setSamples(x,ch3);
    ui->qwtPlot->replot();
}

void MainWindow::integrateAllPiezo()
{
    bool ok;
    ch1=integrate(&ch1,ui->zeroLevel->text().toInt(&ok));
    ch3=integrate(&ch3,ui->zeroLevel->text().toInt(&ok));
    curves.at(1)->setSamples(x,ch1);

    curves.at(3)->setSamples(x,ch3);

}

void MainWindow::divide()
{
    bool ok;
    resetAndPrepareData();
    if (ui->checkBox->isChecked())
    {
        integrateAllPiezo();
    }

    for (int i=0;i<ch0.size();i++)
    {
       ch0[i]=ch0.at(i)/ui->div0->text().toFloat(&ok);

    }
    curves.at(0)->setSamples(x,ch0);

    for (int i=0;i<ch1.size();i++)
    {
       ch1[i]=ch1.at(i)/ui->div1->text().toFloat(&ok);

    }
    curves.at(1)->setSamples(x,ch1);

    for (int i=0;i<ch2.size();i++)
    {
       ch2[i]=ch2.at(i)/ui->div2->text().toFloat(&ok);

    }
    curves.at(2)->setSamples(x,ch2);

    for (int i=0;i<ch3.size();i++)
    {
       ch3[i]=ch3.at(i)/ui->div3->text().toFloat(&ok);
    }
    curves.at(3)->setSamples(x,ch3);

    ui->qwtPlot->replot();

}

void MainWindow::on_div1_returnPressed()
{
    divide();
}

void MainWindow::on_div3_returnPressed()
{
    divide();
}

void MainWindow::on_checkBox_2_toggled(bool checked)
{
    if (checked)
    {
        reverseSignal();
        ui->qwtPlot->replot();
    }
}
