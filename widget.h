#ifndef WIDGET_H
#define WIDGET_H

/*
 * Example of QwtGraphOperation implementation
 * Simple plot of fixed number points filled
 * with random values on Y axis.
 * Default configuration for plot is used.
 */

#include <QWidget>
#include "graph/qwtgraphoperation.h"

//class QwtGraphOperation;
class QTimer;
class QLineEdit;

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    Ui::Widget *ui;

    QwtGraphOperation	*qwtPlot;
    QTimer              *timer;

	QLineEdit			*leftFreq;
	QLineEdit			*rightFreq;

public Q_SLOTS:
    void onTimer(); // For making noise genaration output to plot

	void slotScalingResetX(PanXAxisPars axisP);

	void on_leftFreq_editingFinished();
	void on_rightFreq_editingFinished();
};

#endif // WIDGET_H
