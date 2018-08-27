#include <QTextCodec>
#include <QDomDocument>
// QWT
#include <qevent.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_layout.h>
#include <qwt_plot_magnifier.h>
#include <qwt_legend.h>

#include "qwtgraphoperation.h"

// Define top|bottom and left|right axis here
const int	xAxis = QwtPlot::xBottom;
const int	yAxis = QwtPlot::yLeft;

QwtGraphOperation::QwtGraphOperation(QWidget *parent)
	: QwtPlot(parent)
{
	qRegisterMetaType<PanXAxisPars>("PanXAxisPars");
}

QwtGraphOperation::~QwtGraphOperation()
{

}



QwtGraphOperation::sGraphConf QwtGraphOperation::setDefaultConf()
{
	sGraphConf graphConf;
	graphConf.canvasPalette = Qt::lightGray;
	graphConf.codecName = "Windows-1251";
    // Axes
	graphConf.xAxisText = "Частота [МГц]";
	graphConf.yAxisText = "Амплитуда [дБ]";
	//graphConf.xAxisText = mCodec->toUnicode("Частота [МГц]");
	//graphConf.yAxisText = mCodec->toUnicode("Амплитуда [дБ]");
	graphConf.bb.d_minX = 950;
	graphConf.bb.d_maxX = 1125;
	graphConf.bb.d_minY = 50;
	graphConf.bb.d_maxY = -150;
	// Curves
	graphConf.numberOfCurves = 2;
	graphConf.curvesBrushColor.append(Qt::red);
	graphConf.curvesBrushColor.append(Qt::blue);
	graphConf.curvesStyle.append(Qt::SolidLine);
	graphConf.curvesStyle.append(Qt::SolidLine);
	graphConf.curvesWidth.append(2);
	graphConf.curvesWidth.append(2);
	// Grid
	graphConf.gridExists					= true;
	//graphConf.majorGridColor			= Qt::gray;
	graphConf.majorGridColor			= Qt::darkGray;
	graphConf.majorGridLineStyle	= Qt::SolidLine;
	graphConf.minorGridColor			=	Qt::darkGray;
	graphConf.minorGridLineStyle	=	Qt::DotLine;
	// Markers
	graphConf.markerWidth.append(2);
	graphConf.markerWidth.append(2);
	graphConf.markerLineStyle.append(Qt::SolidLine);
	graphConf.markerLineStyle.append(Qt::DashLine);
	graphConf.markerColor.append(Qt::black);
	graphConf.markerColor.append(Qt::darkGray);
	graphConf.markerValue.append(0);
	graphConf.markerValue.append(1000);

	return graphConf;
}


QwtGraphOperation::sGraphConf QwtGraphOperation::readConfig()
{
	sGraphConf graphConf = setDefaultConf();
	
    QString sElName, sVal;
	QDomElement deTag;
	QDomNode dnTag;
	QDomDocument confDoc("conf_graph");
	QFile file("conf/conf_graph.xml");
	if (!file.open(QIODevice::ReadOnly))
		return graphConf;
	if (!confDoc.setContent(&file)) {
		file.close();
		return graphConf;
	}
	file.close();
	QDomNode dnParent = confDoc.firstChild();				// <xml...>
	// We need extra top tag for correct work. This tag is "head"
	dnParent = dnParent.nextSibling();	// Graph conf
	
	// Canvas tag
	//dnParent = dnParent.firstChild();
	deTag = dnParent.firstChildElement("CanvasPalette");
	sVal = deTag.attribute("value");
	//iVal = sVal.toInt();
	if(sVal.isEmpty())
		return graphConf;
	graphConf.canvasPalette = sVal.toInt();
	
	deTag = dnParent.firstChildElement("CodecName");
	if(!deTag.isNull())
	graphConf.codecName = deTag.attribute("value").toLocal8Bit();

	// axes tag
	//dnParent = dnParent.nextSibling();
	deTag = dnParent.firstChildElement("xAxisText");
	if(!deTag.isNull())
	graphConf.xAxisText = deTag.attribute("value");

	deTag = dnParent.firstChildElement("yAxisText");
	if(!deTag.isNull())
	graphConf.yAxisText = deTag.attribute("value");

	deTag = dnParent.firstChildElement("xAxisMin");
	if(!deTag.isNull())
	{
		sVal = deTag.attribute("value", "0");
		graphConf.bb.d_minX = sVal.toFloat();
	}

	deTag = dnParent.firstChildElement("xAxisMax");
	if(!deTag.isNull())
	{
		sVal = deTag.attribute("value", "1000");
		graphConf.bb.d_maxX = sVal.toFloat();
	}

	deTag = dnParent.firstChildElement("yAxisMin");
	if(!deTag.isNull())
	{
		sVal = deTag.attribute("value", "0");
		graphConf.bb.d_minY = sVal.toFloat();
	}

	deTag = dnParent.firstChildElement("yAxisMax");
	if(!deTag.isNull())
	{
		sVal = deTag.attribute("value", "10");
		graphConf.bb.d_maxY = sVal.toFloat();
	}

	// curves tag
	//dnParent = dnParent.nextSibling();
	dnTag = dnParent.firstChildElement("Curves");
	int num = dnTag.toElement().attribute("number", "0").toInt();
	dnTag = dnTag.firstChild();
	if(!dnTag.isNull())
	{
		graphConf.curvesBrushColor.clear();
		graphConf.curvesStyle.clear();
		graphConf.curvesWidth.clear();
	}
	for (int i=0;i<num;i++)
	{
		if(dnTag.isNull())
		{
			num = i;
			break;
		}
		deTag = dnTag.toElement();
		sVal = deTag.attribute("BrushColor", "2");	// black default
		graphConf.curvesBrushColor.append(sVal.toInt());

		sVal = deTag.attribute("CurveStyle", "1");	// solid line default
		graphConf.curvesStyle.append(sVal.toInt());

		sVal = deTag.attribute("CurveWidth", "2");
		graphConf.curvesWidth.append(sVal.toInt());
		dnTag = dnTag.nextSibling();
	}

	// grid tag
	//dnParent = dnParent.nextSibling();
	dnTag = dnParent.firstChildElement("Curves");
	deTag = deTag.firstChildElement("MajorGridColor");
	sVal = deTag.attribute("value", "5");
	graphConf.majorGridColor = sVal.toFloat();

	//deTag = dnParent.firstChildElement("MajorGridLineStyle");
	dnTag = dnTag.nextSibling();
	sVal = deTag.attribute("value", "1");
	graphConf.majorGridLineStyle = sVal.toFloat();

	//deTag = dnParent.firstChildElement("MinorGridExists");
	dnTag = dnTag.nextSibling();
	sVal = deTag.attribute("value", "0");
	graphConf.minorGridExists = sVal.toFloat();

	//deTag = dnParent.firstChildElement("MinorGridColor");
	dnTag = dnTag.nextSibling();
	sVal = deTag.attribute("value", "4");
	graphConf.minorGridColor = sVal.toFloat();

	//deTag = dnParent.firstChildElement("MinorGridLineStyle");
	dnTag = dnTag.nextSibling();
	sVal = deTag.attribute("value", "3");
	graphConf.minorGridLineStyle = sVal.toFloat();

	return graphConf;		//	TEMP !!!!!
	// markers tag
	dnParent = dnParent.nextSibling();
	dnTag = dnParent.firstChild();
	if(!dnTag.isNull())
	{
		graphConf.markerColor.clear();
		graphConf.markerLineStyle.clear();
		graphConf.markerWidth.clear();
		graphConf.markerValue.clear();
	}
	while(!dnTag.isNull())
	{
		deTag = dnTag.toElement();
		sVal = deTag.attribute("Width", "2");
		graphConf.markerWidth.append(sVal.toInt());

		sVal = deTag.attribute("LineStyle", "1");	// solid line default
		graphConf.markerLineStyle.append(sVal.toInt());

		sVal = deTag.attribute("Color", "2");	// black default
		graphConf.markerColor.append(sVal.toInt());

		sVal = deTag.attribute("value", "0");
		graphConf.markerValue.append(sVal.toInt());
		dnTag = dnTag.nextSibling();
	}

	return graphConf;
}


//void QwtGraphOperation::configurePlot(QTextCodec* mCodec)
void QwtGraphOperation::configurePlot()
{
	sGraphConf gConf = readConfig();

	QwtPlotCanvas *canvas = new QwtPlotCanvas();
	canvas->setFocusIndicator( QwtPlotCanvas::CanvasFocusIndicator );
	canvas->setFocusPolicy( Qt::StrongFocus );
	canvas->setPalette( Qt::lightGray );
	setCanvas( canvas );
	// axes
	this->setAxisTitle(xAxis, gConf.xAxisText);
	this->setAxisTitle(yAxis, gConf.yAxisText);

	plotLayout()->setAlignCanvasToScales( true );

	// Legend
	QwtLegend *legend = new QwtLegend;
	//legend->setDefaultItemMode( QwtLegendData::Checkable );
	insertLegend( legend, QwtPlot::RightLegend);
	// grid
	if(gConf.gridExists)
	{
		QBrush qBr;
		QwtPlotGrid * grid = new QwtPlotGrid();
		grid->enableX(true);
		grid->enableXMin(gConf.minorGridExists);
		grid->enableY(true);
		grid->enableYMin(gConf.minorGridExists);
		qBr.setColor((Qt::GlobalColor)gConf.majorGridColor);
		qBr.setStyle((Qt::BrushStyle)gConf.majorGridLineStyle);
		grid->setMajorPen(QPen(qBr, 0, (Qt::PenStyle)gConf.majorGridLineStyle));
		qBr.setColor((Qt::GlobalColor)gConf.minorGridColor);
		qBr.setStyle((Qt::BrushStyle)gConf.minorGridLineStyle);
		grid->setMinorPen(QPen(qBr, 0, (Qt::PenStyle)gConf.minorGridLineStyle));

		QwtScaleDiv scaleDiv = grid->xScaleDiv();
		scaleDiv.setInterval(0.0, 100);
		grid->setXDiv(scaleDiv);
		scaleDiv = grid->yScaleDiv();
		scaleDiv.setInterval(0.001, 80);
		grid->setYDiv(scaleDiv);

		grid->attach(this);
	}
	
	// format axes
	//QwtScaleDraw *scale;
	//scale = const_cast<QwtScaleDraw *>(this->axisScaleDraw(QwtPlot::xBottom));
	//scale->setLabelFormat('f', 0, 0);


	// Curves for making points output
	for(int i=0;i<gConf.numberOfCurves;i++)
	{
		QwtPlotCurve *curve = new QwtPlotCurve("");
		QPen pen;  // creates a default pen
		pen.setStyle((Qt::PenStyle)gConf.curvesStyle[i]);
		pen.setWidth(2);
		pen.setBrush((Qt::GlobalColor)gConf.curvesBrushColor[i]);
		// Add additional style if needed
		//pen.setCapStyle(Qt::RoundCap);
		//pen.setJoinStyle(Qt::RoundJoin);
		curve->setTitle("Curve ");
		curve->setPen(pen);
		curve->attach(this);
		curves.append(curve);
	}

	magnif = new MyQwtMagnif( this->canvas() );
	connect(magnif, SIGNAL(signalScalingResetX(PanXAxisPars*)),
					this,		SLOT(slotScalingResetX(PanXAxisPars*)));
	//zoom
	magnif->zoomer = new QwtPlotZoomer( this->canvas() );
	magnif->zoomer->setMousePattern( QwtEventPattern::MouseSelect1,
																		Qt::LeftButton, Qt::ControlModifier );
	magnif->zoomer->setTrackerMode(QwtPicker::AlwaysOn);
	connect(magnif->zoomer, SIGNAL(zoomed (const QRectF&)),
					magnif, SLOT(onZoomerZoom(const QRectF&)));

	magnif->panner = new MyQwtPlotPanner( this->canvas(), magnif );
	magnif->panner->setMouseButton(Qt::RightButton);

	magnif->setBandBord(gConf.bb);
	magnif->setZoomOutKey (Qt::Key_Home, Qt::NoModifier);
	
	/*for(int i=0;i<gConf.markerColor.size();i++)
	{
		QColor	mColor(gConf.markerColor[i]);
		QPen		mPen(gConf.markerLineStyle[i]);
		QwtPlotMarker *pmark = new QwtPlotMarker ("");
		pmark->setLineStyle(QwtPlotMarker::VLine);
		pmark->setXValue(2000);
		mPen = marker2->linePen();
		mPen.setStyle(Qt::DashLine);
		pmark->setLinePen(cGray, 2, Qt::DashLine);
	}*/
	QPen mPen(Qt::SolidLine);
	marker1 = new QwtPlotMarker ("");
	marker1->setLineStyle(QwtPlotMarker::VLine);
	marker1->setXValue(950);
	marker1->attach(this);

	QColor cGray(Qt::darkGray);
	mPen.setStyle(Qt::DashLine);
	marker2 = new QwtPlotMarker ("");
	marker2->setLineStyle(QwtPlotMarker::VLine);
	marker2->setXValue(2000);
	marker2->setLinePen(cGray, 2, Qt::DashLine);
	marker2->attach(this);
}


void QwtGraphOperation::setData(QVector<double>& xP, QVector<double>&yP, int numCurve)
{
	if( (numCurve < 1) || (numCurve > curves.size()) )
	{
		// Error
	}
	else
	{
		curves[numCurve-1]->setSamples(xP, yP);
	}
}

void QwtGraphOperation::setCurveName(QString name, int index)
{
	if(name != "")
	{
		curves[index]->setTitle(name);
		curves[index]->setStyle(QwtPlotCurve::Lines);
	}
	else
	{
		curves[index]->setStyle(QwtPlotCurve::NoCurve);
		curves[index]->setTitle("");
	}
}

/*void QwtGraphOperation::setBandBord(BandBorders &bBord /*=0*)
{
	if(magnif)
		magnif->setBandBord(bBord);
}*/


void QwtGraphOperation::slotScalingResetX(PanXAxisPars* panGrPar)
{
	// We have to recalculate position of markers according to scale
	PanXAxisPars newPanPar;
	QwtScaleMap scaleMap = canvasMap( xAxis );
	double s1 = scaleMap.s1();
	double s2 = scaleMap.s2();
	double koef = (s2 - s1) / (scaleMap.p2() - scaleMap.p1());

	// -2 for error in setting marker (don't know why)
	newPanPar.d_xStart = panGrPar->d_xStart;
	newPanPar.d_xEnd = panGrPar->d_xEnd;
	if(panGrPar->d_FmLeft < 0)
	{
		if(panGrPar->d_FmRight < 0)
		{
			newPanPar.d_FmLeft = marker1->xValue();
			newPanPar.d_FmRight = marker2->xValue();
		}
		else
		{
			newPanPar.d_FmLeft = marker1->xValue();
			newPanPar.d_FmRight = (panGrPar->d_FmRight-2)*koef + s1;
			marker2->setXValue(newPanPar.d_FmRight);
		}
	}
	else
	{
		newPanPar.d_FmLeft = (panGrPar->d_FmLeft-2)*koef + s1;
		newPanPar.d_FmRight = marker2->xValue();
		marker1->setXValue(newPanPar.d_FmLeft);
	}
	replot();
    Q_EMIT signalScalingResetX(newPanPar);
}


void QwtGraphOperation::setLeftXMarker(double newVal)
{
	marker1->setXValue(newVal);
}


void QwtGraphOperation::setRightXMarker(double newVal)
{
	marker2->setXValue(newVal);
}


bool QwtGraphOperation::resizeXToBorders(double leftB, double rightB)
{
	bool isOk = true;
	//if( (leftB < bBord.d_minX) || (rightB > bb->d_maxX) || (leftB > rightB) )
	//	return false;

	magnif->setLeftXScaleBorder(leftB, isOk);

	if(isOk)
		magnif->setRightXScaleBorder(rightB, isOk);
	this->replot();
	return isOk;
}


//////////////////////////////////////////////////////////////////////////


class MyQwtMagnif::PrivateData
{
public:
	PrivateData():
				wheelFactor( 0.9 ),
				wheelModifiers( Qt::NoModifier ),
				keyFactor( 0.9 ),
				zoomInKey( Qt::Key_Plus ),
				zoomInKeyModifiers( Qt::NoModifier ),
				zoomOutKey( Qt::Key_Minus ),
				zoomOutKeyModifiers( Qt::NoModifier )
			{
			}
			double wheelFactor;
			Qt::KeyboardModifiers wheelModifiers;

			double keyFactor;

			int zoomInKey;
			Qt::KeyboardModifiers zoomInKeyModifiers;

			int zoomOutKey;
			Qt::KeyboardModifiers  zoomOutKeyModifiers;
};

MyQwtMagnif::MyQwtMagnif(QWidget *parent)
	:QwtPlotMagnifier(parent)
{
	//pBord.m_Fstart_band_MHz = pBord.m_Fend_band_MHz = 0;
	bBord.d_minX = 0;
	bBord.d_minY = -100;
	bBord.d_maxX = 250;
	bBord.d_maxY = 100;
	d_data = new PrivateData();
	pGraphParX.d_xStart = 1000;
	pGraphParX.d_xEnd		= 2000;
	pGraphParX.d_FmLeft = 1000;
	pGraphParX.d_FmRight = 2000;
	pGraphParY.d_yStart = 30;
	pGraphParY.d_yEnd		= -150;
	setAxisEnabled(QwtPlot::yLeft, true);
	setAxisEnabled(QwtPlot::xBottom, true);
	setAxisEnabled(QwtPlot::yRight, false);
	setAxisEnabled(QwtPlot::xTop, false);
};

MyQwtMagnif::~MyQwtMagnif()
{
	delete d_data;
};


void MyQwtMagnif::setBandBord(BandBorders bandBord /*=0*/)
{
	bool isOk;
	bBord = bandBord;
	setLeftXScaleBorder(bBord.d_minX, isOk);
	setRightXScaleBorder(bBord.d_maxX, isOk);
	setBotYScaleBorder(bBord.d_minY, isOk);
	setTopYScaleBorder(bBord.d_maxY, isOk);

	QRectF rect(bBord.d_minX, bBord.d_minY, bBord.d_maxX-bBord.d_minX, bBord.d_maxY-bBord.d_minY);
	zoomer->setZoomBase(rect);
}


void	MyQwtMagnif::onZoomerZoom(const QRectF& rect)
{
	qreal x1, x2, y1, y2;
	qreal x1o, x2o, y1o, y2o;
	QRectF old_rect = zoomer->zoomBase();
	//QRectF new_rect(old_rect.x(), v1, old_rect.width(), v2 - v1);
	//zoomer->setZoomBase(new_rect);
	rect.getCoords(&x1, &y1, &x2, &y2);
	old_rect.getCoords(&x1o, &y1o, &x2o, &y2o);
	if( (x1 == x1o) && (y1 == y1o))
		return;

	pGraphParX.d_xStart = x1;
	pGraphParX.d_xEnd = x2;
	pGraphParX.d_FmLeft = -1;
	pGraphParX.d_FmRight = -1;
	pGraphParY.d_yStart = y1;
	pGraphParY.d_yEnd = y2;
	//pGraphParY.d_FmBot = y1;
	//pGraphParY.d_FmTop = y2;

	//pGraphParX.d_FmLeft = -1;
	//pGraphParX.d_FmRight = -1;
    Q_EMIT signalScalingResetX(&pGraphParX);
}


void MyQwtMagnif::widgetMousePressEvent(QMouseEvent *pEvent)
{
	if(pEvent->button() & Qt::MouseButton::LeftButton)
	{
		// Markers only!!!
		if(pEvent->modifiers() & Qt::ShiftModifier)
		{
			pGraphParX.d_FmRight = pEvent->x();
			pGraphParX.d_FmLeft = -1;
		}
		else
		{
			pGraphParX.d_FmRight = -1;
			pGraphParX.d_FmLeft = pEvent->x();
		}
		//QwtPlotMagnifier::widgetMousePressEvent(pEvent);
        //emit signalScalingResetX(&pGraphParX);
        Q_EMIT signalScalingResetX(&pGraphParX);
	}
	else if(pEvent->button() & Qt::MouseButton::MiddleButton)
	{
		setBandBord(bBord);
		Q_EMIT signalScalingResetX(&pGraphParX);
	}
}


void  MyQwtMagnif::widgetWheelEvent (QWheelEvent *pEvent)
{
	// Code from qwt_magnifier::widgetWheelEvent
	// and qwt_plot_magnifier::rescale
	/*
		A positive delta indicates that the wheel was
		rotated forwards away from the user; a negative
		value indicates that the wheel was rotated
		backwards toward the user.
		Most mouse types work in steps of 15 degrees,
		in which case the delta value is a multiple
		of 120 (== 15 * 8).
	*/
	int mousePos = 0;
	double f = qPow( d_data->wheelFactor,
		qAbs( pEvent->delta() / 120.0 ) );

	//if ( pEvent->delta() > 0 )
	if (pEvent->delta() < 0)
		f = 1 / f;

	// Checking if shift is on - scaling only Y axis
	if ( pEvent->modifiers() & Qt::ShiftModifier )
  {
		setAxisEnabled(yAxis, true);
		setAxisEnabled(xAxis, false);
		mousePos = pEvent->y();
	}
	else
	{
		setAxisEnabled(xAxis, true);
		setAxisEnabled(yAxis, false);
		mousePos = pEvent->x();
	}
	rescale( f, mousePos );
}


void MyQwtMagnif::rescale(double factor, int mousePos)
{
	/*
	 *	Same code as for qwt library, with adding scaling to borders.
	 */
	QwtPlot* plt = plot();
	if ( plt == NULL )
		return;

	factor = qAbs( factor );

	bool doReplot = false;

	const bool autoReplot = plt->autoReplot();

	for ( int axisId = 0; axisId < QwtPlot::axisCnt; axisId++ )
	{
		if ( isAxisEnabled( axisId ) )
		{
			QRectF old_rect = zoomer->zoomBase();
			const QwtScaleMap scaleMap = plt->canvasMap( axisId );

			double v1 = scaleMap.s1();
			double v2 = scaleMap.s2();

			if ( scaleMap.transformation() )
			{
				// the coordinate system of the paint device is always linear
				v1 = scaleMap.transform( v1 ); // scaleMap.p1()
				v2 = scaleMap.transform( v2 ); // scaleMap.p2()
			}
			double mouseFactor = mousePos / (scaleMap.p2() - scaleMap.p1());
			if(mouseFactor < 0)
				mouseFactor = 0.5;
			if(factor > 1)
				mouseFactor = (mouseFactor + 0.5*2) / 3;
			else
			mouseFactor = (mouseFactor + 0.5*3) / 4;
			const double center = v1 + mouseFactor * ( v2 - v1 ) ;
			const double width = 0.5 * ( v2 - v1 ) * factor;
			//double prevWidth = v2 - v1;
			double prevWidth = width * 2;		// Saving length for borders checks

			v1 = center - width;
			v2 = center + width;


			if(bBord.d_maxX != bBord.d_minX)
			{
				if( (axisId == QwtPlot::xBottom) || (axisId == QwtPlot::xTop))
				{
					if(v1 < bBord.d_minX)
						v1 = bBord.d_minX;
					if(v2 > bBord.d_maxX)
						v2 = bBord.d_maxX;

					// Additional check for correct scaling if mouse cursor is close to the graph border
					if(prevWidth > (bBord.d_maxX - bBord.d_minX) )
						prevWidth = bBord.d_maxX - bBord.d_minX;
					if( (v2 - v1) < prevWidth )
					{
						if(v1 == bBord.d_minX)
							v2 = v1 + prevWidth;
						else
							v1 = v2 - prevWidth;
					}
					if(v1 > v2)
						v2 = v1 + 10;
					pGraphParX.d_xStart = v1;
					pGraphParX.d_xEnd = v2;
					QRectF old_rect = zoomer->zoomBase();
					QRectF new_rect(v1, old_rect.y(), v2-v1, old_rect.height());
					zoomer->setZoomBase(new_rect);
				}
				if( (axisId == QwtPlot::yLeft) || (axisId == QwtPlot::yRight))
				{
					if(v1 < bBord.d_minY)
						v1 = bBord.d_minY;
					if(v2 > bBord.d_maxY)
						v2 = bBord.d_maxY;
					
					if(prevWidth > (bBord.d_maxY - bBord.d_minY) )
						prevWidth = bBord.d_maxY - bBord.d_minY;
					if( (v2 - v1) < prevWidth )
					{
						if(v1 == bBord.d_minY)
							v2 = v1 + prevWidth;
						else
							v1 = v2 - prevWidth;
					}

					if(v1 > v2)
						v2 = v1 + 10;
					
					pGraphParY.d_yStart = v1;
					pGraphParY.d_yEnd = v2;
					QRectF old_rect = zoomer->zoomBase();
					QRectF new_rect(old_rect.x(), v1, old_rect.width(), v2 - v1);
					zoomer->setZoomBase(new_rect);
				}
			}

			if ( scaleMap.transformation() )
			{
				v1 = scaleMap.invTransform( v1 );
				v2 = scaleMap.invTransform( v2 );
			}

			plt->setAxisScale( axisId, v1, v2 );

			doReplot = true;
		}
	}

	plt->setAutoReplot( autoReplot );

	if ( doReplot )
		plt->replot();

	pGraphParX.d_FmLeft = -1;
	pGraphParX.d_FmRight = -1;
    Q_EMIT signalScalingResetX(&pGraphParX);
}



double MyQwtMagnif::setLeftXScaleBorder(double val, bool& isOk)
{
	QwtPlot *plt = plot();
	QwtScaleMap scaleMap = plt->canvasMap( xAxis );

	double v1 = scaleMap.s1();
	double v2 = scaleMap.s2();

	if(val > v2)
	{
		isOk = false;
		return 0;
	}
	
	if(val < bBord.d_minX)
		val = bBord.d_minX;

	v1 = val;
	pGraphParX.d_xStart = val;

	plt->setAxisScale( xAxis, v1, v2 );
	plt->replot();

	isOk = true;
	return val;
}


double MyQwtMagnif::setRightXScaleBorder(double val, bool& isOk)
{
	QwtPlot *plt = plot();
	QwtScaleMap scaleMap = plt->canvasMap( xAxis );

	double v1 = scaleMap.s1();
	double v2 = scaleMap.s2();

	if (val < v1)
	{
		isOk = false;
		return 0;
	}

	if(val > bBord.d_maxX)
		val = bBord.d_maxX;
	
	v2 = val;
	pGraphParX.d_xEnd = val;

	plt->setAxisScale( xAxis, v1, v2 );
	plt->replot();

	isOk = true;
	return val;
}



double MyQwtMagnif::setBotYScaleBorder(double val, bool& isOk)
{
	QwtPlot *plt = plot();
	QwtScaleMap scaleMap = plt->canvasMap( yAxis );

	double v1 = scaleMap.s1();
	double v2 = scaleMap.s2();

	if (val > v2)
	{
		isOk = false;
		return 0;
	}

	v1 = val;
	pGraphParY.d_yStart = val;

	plt->setAxisScale( yAxis, v1, v2 );
	plt->replot();

	isOk = true;
	return val;
}


double MyQwtMagnif::setTopYScaleBorder(double val, bool& isOk)
{
	QwtPlot *plt = plot();
	QwtScaleMap scaleMap = plt->canvasMap( yAxis );
	double v1 = scaleMap.s1();
	double v2 = scaleMap.s2();

	if (val < v1)
	{
		isOk = false;
		return 0;
	}

	v2 = val;
	pGraphParY.d_yEnd = val;

	plt->setAxisScale( yAxis, v1, v2 );
	plt->replot();

	isOk = true;
	return val;
}


void MyQwtMagnif::setRescale()
{
    Q_EMIT signalScalingResetX(&pGraphParX);
    Q_EMIT signalScalingResetY(&pGraphParY);
}


void  MyQwtMagnif::moveCanvas (int dx, int dy)
{
	double sm = 0;
	QwtPlot *plt = plot();
	QwtScaleMap scaleMap = plt->canvasMap( xAxis );
	double v1 = pGraphParX.d_xStart;
	double v2 = pGraphParX.d_xEnd;
	double p1 = scaleMap.p1();
	double p2 = scaleMap.p2();
	double shiftV = v2 - v1;
	QRectF old_rect = zoomer->zoomBase();

	double dxS = (v2 - v1) * dx / (p2 - p1);
	if(dxS < 0)
	{
		sm = v1 + dxS;
		if(sm < bBord.d_minX)
		{
			pGraphParX.d_xStart = bBord.d_minX;
			pGraphParX.d_xEnd		= bBord.d_minX + shiftV;
		}
		else
		{
			pGraphParX.d_xStart = sm;
			pGraphParX.d_xEnd = v2 + dxS;
		}
	}
	else
	{
		sm = v2 + dxS;
		if(sm > bBord.d_maxX)
		{
			pGraphParX.d_xStart	= bBord.d_maxX - shiftV;
			pGraphParX.d_xEnd		= bBord.d_maxX;
		}
		else
		{
			pGraphParX.d_xStart = v1 + dxS;
			pGraphParX.d_xEnd		= sm;
		}
	}

	pGraphParX.d_FmLeft = -1;
	pGraphParX.d_FmRight = -1;
    Q_EMIT signalScalingResetX(&pGraphParX);

	scaleMap = plt->canvasMap( yAxis );
	v1 = pGraphParY.d_yStart;
	v2 = pGraphParY.d_yEnd;
	p1 = scaleMap.p1();
	p2 = scaleMap.p2();
	shiftV = v2 - v1;
	double dyS = (v2 - v1) * dy / (p2 - p1);
	if(dyS < 0)
	{
		sm = v1 + dyS;
		if(sm < bBord.d_minY)
		{
			pGraphParY.d_yStart = bBord.d_minY;
			pGraphParY.d_yEnd		= bBord.d_minY + shiftV;
		}
		else
		{
			pGraphParY.d_yStart = sm;
			pGraphParY.d_yEnd		= v2 + dyS;
		}
	}
	else
	{
		sm = v2 + dyS;
		if(sm > bBord.d_maxY)
		{
			pGraphParY.d_yStart	= bBord.d_maxY - shiftV;
			pGraphParY.d_yEnd		= bBord.d_maxY;
		}
		else
		{
			pGraphParY.d_yStart = v1 + dyS;
			pGraphParY.d_yEnd		= sm;
		}
	}

	//
	QRectF new_rect(pGraphParX.d_xStart, pGraphParY.d_yStart, old_rect.width(), old_rect.height());
	zoomer->setZoomBase(new_rect);

	plt->setAxisScale( xAxis, pGraphParX.d_xStart, pGraphParX.d_xEnd );
	plt->setAxisScale( yAxis, pGraphParY.d_yStart, pGraphParY.d_yEnd );
	plt->replot();
}



MyQwtPlotPanner::MyQwtPlotPanner(QWidget *pW, MyQwtMagnif* pM)
	: QwtPlotPanner(pW)
{
	pMagnf = pM;
}

MyQwtPlotPanner::~MyQwtPlotPanner()
{
	;
}

void  MyQwtPlotPanner::moveCanvas (int dx, int dy)
{
	pMagnf->moveCanvas(-dx, -dy);
}
