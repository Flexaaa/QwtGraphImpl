#ifndef QWTGRAPHOPERATION_H
#define QWTGRAPHOPERATION_H

#include <QWidget>
#include <qwt_plot.h>
#include <qwt_plot_magnifier.h>
#include <qwt_plot_panner.h>
#include "GraphBaseStructs.h"

/*
 *	Implementation of QWT 2d plot. 
 *	Setup from file for axis, scales, grid, curves. Two markers exist.
 *	Reimplemented scaling with mouse wheel, canvas moving and rescale. 
 *	QwtGraphOperation - main class for graph setup and adding data to it.
 *	MyQwtMagnif - used for scaling purposes.
 *	MyQwtPlotPanner - reimplemented moveCanvas function for adding scaling borders.
 *	
 *	Information about scaling changes is in signalScalingResetX(PanXAxisPars).
 */

class QTextCodec;
class QwtPlotCurve;
class QwtPlotMarker;
class QwtPlotZoomer;
class QwtPlotPanner;

class MyQwtPlotPanner;

// TODO add translation file from ini config to make all the names

class MyQwtMagnif : public QwtPlotMagnifier
{
	Q_OBJECT

public:
	MyQwtMagnif(QWidget *parent);
	~MyQwtMagnif();

	QwtPlotZoomer *zoomer;
	MyQwtPlotPanner *panner;

    // Setting borders for maximum scaling
	void	setBandBord(BandBorders);
    void	setBaseMarkers(double, double);	// Saving markers position. NOT IMPLEMENTED

    // Scaling to borders. Checking if greater then values of BandBorders
	double	setLeftXScaleBorder(double, bool&);
	double	setRightXScaleBorder(double, bool&);
	double	setBotYScaleBorder(double, bool&);
	double	setTopYScaleBorder(double, bool&);

    // Emmiting scaling signals
	void	setRescale();
	
    // If you like to move graph outside the class
	void  moveCanvas (int dx, int dy);

public Q_SLOTS:
	void	onZoomerZoom(const QRectF& rect);

protected:
	// Checking scaling borders there
	// Also checking if Shift is pressed - resize only Y axis
	void  widgetWheelEvent (QWheelEvent *);
    void  widgetMousePressEvent (QMouseEvent *pEvent);

	void  rescale (double factor, int mousePos);

Q_SIGNALS:// Resize event. Implement if needed
	void	signalScalingResetX(PanXAxisPars*);
	void	signalScalingResetY(PanYAxisPars*);

private:
	class PrivateData;
	PrivateData		*d_data;
	BandBorders		bBord;
	PanXAxisPars	pGraphParX;
	PanYAxisPars	pGraphParY;
};

class QwtGraphOperation : public QwtPlot
{
	Q_OBJECT

public:
	QwtGraphOperation(QWidget *parent);
	~QwtGraphOperation();

    MyQwtMagnif		*magnif;
    struct  sGraphConf;     // Defined at the end of the class

    // We need to invoke this function to create plot configuration
	void	configurePlot();

    // Setting data for curve with specified number (1..curves number)
    void	setData(QVector<double>&, QVector<double>&, int number);
	// Setting name for curve and legend. Index starts from 0
	void	setCurveName(QString, int index);

    // Setting markers outside the class
	void	setLeftXMarker(double);
	void	setRightXMarker(double);

	// External resizing graph
	bool	resizeXToBorders(double, double);

Q_SIGNALS:
	// Signal returning current rescaling parameters - PanXAxisPars
	void	signalScalingResetX(PanXAxisPars);

public Q_SLOTS:
	void	slotScalingResetX(PanXAxisPars*);


private:
	QwtPlot				*qwtPlot;

	QVector<QwtPlotCurve*> curves;

	QwtPlotMarker	*marker1;
	QwtPlotMarker	*marker2;

	// Configuration of plot parameters
    // Reading from file
	sGraphConf readConfig();
    // Default configuration (if there is no file to read)
	sGraphConf setDefaultConf();

public:

	enum AxisNumbers	// Same as for QWT
	{
		//! Y axis left of the canvas
		yLeft,

		//! Y axis right of the canvas
		yRight,

		//! X axis below the canvas
		xBottom,

		//! X axis above the canvas
		xTop,

		//! Number of axes
		axisCnt
	};

	struct sGraphConf
	{
		unsigned char canvasPalette;		// Defines background color
		QByteArray codecName;						// If exists codec name for retranslating axis text
		QString xAxisText;
		QString yAxisText;

		// Default axis borders
		BandBorders bb;

		// Grid params
		bool	gridExists;
		int		majorGridColor;
		int		majorGridLineStyle;
		bool	minorGridExists;
		int		minorGridColor;
		int		minorGridLineStyle;

		// Curve style parameters
		int numberOfCurves;
		QVector<int>	curvesStyle;
		QVector<int>	curvesWidth;
		QVector<int>	curvesBrushColor;

		// Markers
        QVector<int>	markerLineStyle; // size can be 0 - no marker, values as for QWT
        QVector<int>	markerColor;
        QVector<int>	markerWidth;
		QVector<double>	markerValue;
	};
};


class MyQwtPlotPanner : public QwtPlotPanner
{
	Q_OBJECT

		MyQwtMagnif	*pMagnf;

public:
		MyQwtPlotPanner(QWidget *, MyQwtMagnif*);
		~MyQwtPlotPanner();

public Q_SLOTS:
	void  moveCanvas (int dx, int dy);
};


#endif // QWTGRAPHOPERATION_H
