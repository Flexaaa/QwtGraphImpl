#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QLabel>
#include <QVector>
#include <QTimer>
#include "widget.h"
#include "ui_widget.h"
//#include "graph/qwtgraphoperation.h"

// Here we set user defined graph configuration
const int LEFT_GRAPG_BORD	=	-125;
const int RIGHT_GRAPG_BORD	=	125;
const int FREQ_CENTER   	=	0;


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    QVBoxLayout *formL = new QVBoxLayout (this);
    qwtPlot = new QwtGraphOperation(this);
    qwtPlot->configurePlot();
    formL->addWidget(qwtPlot);

	QHBoxLayout *hElemLayout = new QHBoxLayout(this);
	QLabel		*labelBords = new QLabel(this);
	leftFreq	= new QLineEdit(this);
	rightFreq	= new QLineEdit(this);
	hElemLayout->addWidget(labelBords);
	hElemLayout->addWidget(leftFreq);
	hElemLayout->addWidget(rightFreq);

	formL->addItem(hElemLayout);

    QFont font;
    font.setPointSize(10);
    qwtPlot->setObjectName(QStringLiteral("qwtPlot"));
    qwtPlot->setFont(font);
    qwtPlot->setMinimumSize(400, 300);
	connect(qwtPlot, SIGNAL(signalScalingResetX(PanXAxisPars)),
		this, SLOT(slotScalingResetX(PanXAxisPars)));

	labelBords->setObjectName(QStringLiteral("labelBords"));
	labelBords->setFont(font);
	labelBords->setText(QString::fromLocal8Bit("Левая и правая границы частот"));

	leftFreq->setObjectName(QStringLiteral("leftFreq"));
	leftFreq->setFont(font);
	leftFreq->setText(QString::number(LEFT_GRAPG_BORD));
	connect(leftFreq, SIGNAL(editingFinished()),
		this, SLOT(on_leftFreq_editingFinished()));

	rightFreq->setObjectName(QStringLiteral("rightFreq"));
	rightFreq->setFont(font);
	rightFreq->setText(QString::number(RIGHT_GRAPG_BORD));
	connect(rightFreq, SIGNAL(editingFinished()),
		this, SLOT(on_rightFreq_editingFinished()));

    BandBorders bb;
    bb.d_minX = FREQ_CENTER + LEFT_GRAPG_BORD;
    bb.d_minY = -10;
    bb.d_maxX = FREQ_CENTER + RIGHT_GRAPG_BORD;
    bb.d_maxY = 10;
    qwtPlot->magnif->setBandBord(bb);

	qwtPlot->setCurveName("Amplitude 10", 0);
	qwtPlot->setCurveName("Amplitude 5", 1);

    timer = new QTimer(this);
    //timer->setInterval(100);
    connect(timer, SIGNAL(timeout()),
            this, SLOT(onTimer()));

    timer->start(500);
}

Widget::~Widget()
{
    delete ui;
}


void Widget::onTimer()
{
    quint16 numP = 501;
    QVector<qreal> xAxisValues, yAxisValues1, yAxisValues2;
    float noiseMax1 = 10;	// Noise amplitude for first curve
	float noiseMax2 = 5;	// Noise amplitude for second curve
    qreal valX = LEFT_GRAPG_BORD;
    qreal adding = (RIGHT_GRAPG_BORD - LEFT_GRAPG_BORD) * 1. / (numP-1);

    xAxisValues.resize(numP);
	yAxisValues1.resize(numP);
    yAxisValues2.resize(numP);
    for (int i=0;i<numP;i++)
    {
        qreal valY = (qreal)rand() / (RAND_MAX + 1) * noiseMax1 - noiseMax1 / 2;
        xAxisValues[i] = valX;
        yAxisValues1[i] = valY;
		valY = (qreal)rand() / (RAND_MAX + 1) * noiseMax2 - noiseMax2 / 2;
		yAxisValues2[i] = valY;
        valX += adding;
    }

    qwtPlot->setData(xAxisValues, yAxisValues1, 1);
	qwtPlot->setData(xAxisValues, yAxisValues2, 2);
    qwtPlot->replot();
}


void Widget::on_leftFreq_editingFinished()
{
	bool isOk;
	qwtPlot->magnif->setLeftXScaleBorder(leftFreq->text().toDouble(), isOk);
}


void Widget::on_rightFreq_editingFinished()
{
	bool isOk;
	qwtPlot->magnif->setRightXScaleBorder(rightFreq->text().toDouble(), isOk);
}


void Widget::slotScalingResetX(PanXAxisPars axisP)
{
	leftFreq->setText(QString::number(axisP.d_xStart, 'g'));
	rightFreq->setText(QString::number(axisP.d_xEnd, 'g'));

	qwtPlot->replot();
}