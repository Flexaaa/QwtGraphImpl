#ifndef GRAPHBASESTRUCTS_H
#define GRAPHBASESTRUCTS_H

// Setting borders if needed
struct BandBorders
{
	double	d_minX;
	double	d_maxX;
	double	d_minY;
	double	d_maxY;
	void operator=(const BandBorders& bbnew)
	{
		d_minX = bbnew.d_minX;
		d_maxX = bbnew.d_maxX;
		d_minY = bbnew.d_minY;
		d_maxY = bbnew.d_maxY;
	};
};

// Structs to control current graph borders and markers
// Used in signal
struct PanXAxisPars
{
	// current visible range
	double d_xStart;
	double d_xEnd;

	double d_FmLeft;		// Left marker
	double d_FmRight;		// Right marker
};


struct PanYAxisPars
{
	// current visible range
	double d_yStart;
	double d_yEnd;

	// Markers
	double d_FmBot;
	double d_FmTop;
};

#endif	// GRAPHBASESTRUCTS_H