#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QDataStream>
#include <qwt_plot_canvas.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_curve.h>
#include <qwt_symbol.h>
#include <qwt_plot_magnifier.h>
#include <qwt_plot_panner.h>
#include <qwt_scale_div.h>
#include <qwt_plot_marker.h>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    QFile file;
    QDataStream datastream;
    QVector<double> ch0,ch1,ch2,ch3,x;
    QwtPlotMagnifier *magnifier;        //зумер
    QwtPlotMagnifier *zoom_x, *zoom_x1,*zoom_x2;
    QwtPlotMagnifier *zoom_y, *zoom_y1, *zoom_y2;
    QwtPlotPanner *d_panner,*d_panner1,*d_panner2;            //перемещатель
    QwtPlotGrid *grid,*grid1,*grid2;                  //сетка
   // QwtPlotCurve *curv1,*curv2;         //две кривые
    QwtPlotMarker *m1;                  //маркер- горизонтальная линия в 0.
    QVector<QwtPlotCurve*> curves;
    QVector<qreal> filter(QVector<qreal> *in, qreal coeff );
    QVector<qreal> integrate(QVector<qreal> *in, qreal zeroLevel);
    qreal max[4];
    void resetAndPrepareData();
    void readdata();
    void setupqwt();
    void divide();
    void integrateAllPiezo();
    void reverseSignal();
    ~MainWindow();


private slots:
    void on_lineEdit_returnPressed();

    void on_lineEdit_2_returnPressed();

    void on_checkBox_toggled(bool checked);

    void on_div1_returnPressed();

    void on_div3_returnPressed();

    void on_checkBox_2_toggled(bool checked);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
