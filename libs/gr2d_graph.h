#pragma once

#include "gr2d_obj.h"
#include "dynarr.h"
#include <conio.h>
#include "threading.h"

#define MIN_X_SECT_SIZE 60
#define MAX_X_SECT_SIZE 180
#define MIN_Y_SECT_SIZE 60
#define MAX_Y_SECT_SIZE 180

#define MAX_SCALE 1000

#define MAX_X_START_ABS 900000
#define MAX_Y_START_ABS 900000

#define TEXT_PADDING 10

struct Graph2dStyle {
	long margin;
	Rgb  axesColor, secondaryColor, nullAxesColor;
	long arrowsize;
	long dashsize;
	long fontSmallSize, fontBigSize;
	long accuracy;
};
struct Func2d {
	double (*func)(double);
	Rgb color;
	HPEN pen;
	const wchar_t* text;
};

long calcGraphCoord(long margin, double start, long sectSize, double scale, double value) {
	return (double)(value - start) / scale * sectSize + margin;
}
double calcValueByCoord(long margin, double start, long sectSize, double scale, long coord) {
	return (double)(coord - margin) / sectSize * scale + start;
}
bool isInside(int px, int py, int x1, int y1, int x2, int y2) {
	if (px < x1 || px > x2 || py < y1 || py > y2)
		return 0;
	else
		return 1;
}
bool isBetwen(int x, int a, int b) {
	return x > a && x < b;
}

enum class GraphMode { def, info, help };

class Graph2d : public Obj2d {
private:
	DynArr<Func2d> funcs;
	long funcsCount;

	double		xStart, yStart;
	double		xScale, yScale;
	double		xSect, ySect;

	bool		showNullAxes = 1;

	long		margin;
	long		arrowsize;
	long		dashsize;
	
	long		accuracy;

	long		top = 0, right = 0;

	Rgb			axesColor, secondaryColor, nullAxesColor;
	HPEN		axesPen, secondaryPen, nullAxesPen;

	long		fontBigSize, fontSmallSize;
	HFONT		fontBig, fontSmall;

	GraphMode	mode;

	Mutex		mtx;
public:
	void setMode(GraphMode mode_)	{ mode = mode_; }
	void toggleInfoMode()			{ mode = (mode != GraphMode::info ? mode = GraphMode::info : mode = GraphMode::def); }
	void toggleHelpMode()			{ mode = (mode != GraphMode::help ? mode = GraphMode::help : mode = GraphMode::def); }
	void toggleNullAxes()			{ showNullAxes = !showNullAxes; }
	void scaleRelatively(double relScale) {
		mtx.lock();
		if ((xScale <= MAX_SCALE && yScale <= MAX_SCALE) || relScale > 1) {
			xSect *= relScale;
			ySect *= relScale;
			if (xSect > MAX_X_SECT_SIZE || ySect > MAX_Y_SECT_SIZE) {
				divideSections(2);
			}
			if (xSect < MIN_X_SECT_SIZE || ySect < MIN_Y_SECT_SIZE) {
				divideSections(.5);
			}
		}
		mtx.unlock();
	}
	void divideSections(double times) {
		xSect /= times;
		xScale /= times;
		ySect /= times;
		yScale /= times;
	}
	void moveStart(double xOffset, double yOffset) {
		mtx.lock();
		xStart = min(max(xStart + xOffset, -MAX_X_START_ABS), MAX_X_START_ABS);
		yStart = min(max(yStart + yOffset, -MAX_Y_START_ABS), MAX_Y_START_ABS);
		mtx.unlock();
	}

	void setStart(double x, double y) {
		xStart = x;
		yStart = y;
	}
	void setScale(double x, double y) {
		xScale = x;
		yScale = y;
	}

	long getSectX()		{ return xSect; }
	long getSectY()		{ return ySect; }

	double getScaleX()	{ return xScale; }
	double getScaleY()	{ return yScale; }

	void draw(Win2d& window) {
		mtx.lock();
		switch (mode) {
		case GraphMode::def:
		{
			long wid, hgt;	//window size
			window.getSize(wid, hgt);

			top = hgt - margin;
			right = wid - margin;

			//grhgt	= top - margin;
			//grwid	= right - margin;

			SelectObject(window.getBitmapHdc(), fontBig);
			SetTextColor(window.getBitmapHdc(), axesColor);
			SetBkMode(window.getBitmapHdc(), TRANSPARENT);

			SelectObject(window.getBitmapHdc(), secondaryPen);
			//secondary lines
			for (long i = margin; i < right; i += xSect) {
				window.drawLine(i, margin, i, top);
			}
			for (long i = margin; i < top; i += ySect) {
				window.drawLine(margin, i, right, i);
			}

			SelectObject(window.getBitmapHdc(), axesPen);
			//Y and X letters
			window.drawText(L"Y", margin - arrowsize - fontBigSize / 2.5 - 3, top + fontBigSize / 2);
			window.drawText(L"X", right, margin - arrowsize);
			//axes
			window.drawLine(margin, margin, margin, top);	//Oy
			window.drawLine(margin, margin, right, margin);	//Ox
			//null axes
			if (showNullAxes) {
				SelectObject(window.getBitmapHdc(), nullAxesPen);
				int nullAxisCoord = calcGraphCoord(margin, xStart, xSect, xScale, 0);
				if (nullAxisCoord <= right && nullAxisCoord >= margin) {
					window.drawLine(nullAxisCoord, margin, nullAxisCoord, top);
				}
				nullAxisCoord = calcGraphCoord(margin, yStart, ySect, yScale, 0);
				if (nullAxisCoord <= right && nullAxisCoord >= margin) {
					window.drawLine(margin, nullAxisCoord, right, nullAxisCoord);
				}
				SelectObject(window.getBitmapHdc(), axesPen);
			}
			//arrows
			//oy arrow
			window.drawLine(margin, top, margin - arrowsize, top - arrowsize);
			window.drawLine(margin, top, margin + arrowsize, top - arrowsize);
			//ox arrow
			window.drawLine(right, margin, right - arrowsize, margin + arrowsize);
			window.drawLine(right, margin, right - arrowsize, margin - arrowsize);
			//dashes and degrees
			SelectObject(window.getBitmapHdc(), fontSmall);

			double xcur, ycur;
			xcur = xStart;
			for (long i = margin; i < right; i += xSect) {
				window.drawLine(i, margin - dashsize, i, margin + dashsize);

				size_t sz = 10;
				wchar_t tmp[10];
				char tmpch[10];
				sprintf_s(tmpch, "%0.2f", xcur);
				mbstowcs_s(&sz, &tmp[0], 10, tmpch, sz);
				window.drawText((LPCWSTR)tmp, i, margin - dashsize);

				xcur += xScale;
			}
			ycur = yStart;
			for (long i = margin; i < top; i += ySect) {
				window.drawLine(margin - dashsize, i, margin + dashsize, i);

				size_t sz = 30;
				wchar_t tmp[30];
				char tmpch[30];
				sprintf_s(tmpch, "%0.2f", ycur);
				mbstowcs_s(&sz, &tmp[0], 30, tmpch, sz);
				window.drawText((LPCWSTR)tmp, margin - (fontSmallSize)*lstrlen(tmp) / 2.5 - dashsize, i);

				ycur += yScale;
			}

			//graph
			for (long i = 0; i < funcsCount; i++) {
				double curx = xStart, cury = funcs[i].func(xStart);	//current values
				//double lastx, lasty;						//last values
				long curxp = margin, curyp = calcGraphCoord(margin, yStart, ySect, yScale, cury);	//current polong
				long lastxp = curxp, lastyp = curyp;							//last polong

				SelectObject(window.getBitmapHdc(), funcs[i].pen);
				while (curxp < right) {
					curxp += accuracy;
					curx = calcValueByCoord(margin, xStart, xSect, xScale, curxp);
					cury = funcs[i].func(curx);
					curyp = calcGraphCoord(margin, yStart, ySect, yScale, cury);

					//if (lastxp >= margin && curxp >= margin && lastxp <= right && curxp <= right &&
					//	lastyp >= margin && curyp >= margin && lastyp <= top && curyp <= top)
					if (isBetwen(curyp, margin, top) || isBetwen(lastyp, margin, top) || (curyp >= top && lastyp <= margin) || (curyp <= margin && lastyp >= top)) {
						lastyp = min(top, max(margin, lastyp));
						curyp = min(top, max(margin, curyp));
						window.drawLine(lastxp, lastyp, curxp, curyp);
					}
					lastxp = curxp;
					lastyp = curyp;
				}
			}
		}
			break;
		case GraphMode::info:
		{
			SetBkMode(window.getBitmapHdc(), TRANSPARENT);
			SelectObject(window.getBitmapHdc(), fontBig);
			for (long i = 0, curypos = TEXT_PADDING; i < funcsCount; i++) {
				SetTextColor(window.getBitmapHdc(), funcs[i].color);
				curypos += TEXT_PADDING + fontBigSize;
				window.drawText(funcs[i].text, TEXT_PADDING, curypos);
			}
		}
			break;
		case GraphMode::help:
		{
			SetBkMode(window.getBitmapHdc(), TRANSPARENT);
			SelectObject(window.getBitmapHdc(), fontBig);
			SetTextColor(window.getBitmapHdc(), axesColor);
			const wchar_t help[] = L"H - help\nI - info\n+/- - scale\nWASD - scrolling\nE - move to (0; 0) point\nQ - set unit segment to 1.00\nF - show/hide null axes\nEsc - exit program\n";
			wchar_t tmp[100];
			for (int i = 0, helplen = lstrlen(help), tmplen = 0, linesCount = 0; i < helplen; i++) {
				if (help[i] != '\n') {
					tmp[tmplen] = help[i];
					tmplen++;
				}
				else {
					tmp[tmplen] = '\0';
					linesCount++;
					window.drawText(tmp, TEXT_PADDING, linesCount * (TEXT_PADDING + fontBigSize));
					tmplen = 0;
				}
			}
		}
		}
		mtx.unlock();
	}

	void setStyle(Graph2dStyle style) {
		funcsCount		= 1;
		margin			= style.margin;
		arrowsize		= style.arrowsize;
		axesColor		= style.axesColor;
		secondaryColor	= style.secondaryColor;
		nullAxesColor	= style.nullAxesColor;
		dashsize		= style.dashsize;

		fontBigSize		= style.fontBigSize;
		fontSmallSize	= style.fontSmallSize;

		axesPen			= CreatePen(PS_SOLID, 1, axesColor);
		secondaryPen	= CreatePen(PS_SOLID, 1, secondaryColor);
		nullAxesPen		= CreatePen(PS_SOLID, 1, nullAxesColor);

		fontBig			= CreateFont(fontBigSize, fontBigSize / 2.5, 0, 90, 2, 0, 0, 0, 0, 0, 0, 0, 0, L"Roboto");
		fontSmall		= CreateFont(fontSmallSize, fontSmallSize / 2.5, 0, 90, 2, 0, 0, 0, 0, 0, 0, 0, 0, L"Roboto");

		accuracy		= style.accuracy;

		mode			= GraphMode::def;

		axesPen			= CreatePen(PS_SOLID, 1, axesColor);
		secondaryPen	= CreatePen(PS_SOLID, 1, secondaryColor);
		nullAxesPen		= CreatePen(PS_SOLID, 1, nullAxesColor);

		fontBig = CreateFont(fontBigSize, fontBigSize / 2.5, 0, 90, 2, 0, 0, 0, 0, 0, 0, 0, 0, L"Roboto");
		fontSmall = CreateFont(fontSmallSize, fontSmallSize / 2.5, 0, 90, 2, 0, 0, 0, 0, 0, 0, 0, 0, L"Roboto");
	}

	Graph2d(double (*func_)(double), Rgb graphColor, Graph2dStyle style, long xSect_, long ySect_, double xScale_, double yScale_, double xStart_, double yStart_, const wchar_t text[]) {
		setStyle(style);

		funcsCount	= 1;
		funcs.add({func_, graphColor, CreatePen(PS_SOLID, 1, graphColor), text});

		xSect			= xSect_;
		ySect			= ySect_;
		xScale			= xScale_;
		yScale			= yScale_;
		xStart			= xStart_;
		yStart			= yStart_;

		mode			= GraphMode::def;
	}
	Graph2d(Func2d func, Graph2dStyle style, long xSect_, long ySect_, double xScale_, double yScale_, double xStart_, double yStart_) {
		setStyle(style);

		funcsCount	= 1;
		func.pen	= CreatePen(PS_SOLID, 1, func.color);
		funcs.add(func);

		xSect			= xSect_;
		ySect			= ySect_;
		xScale			= xScale_;
		yScale			= yScale_;
		xStart			= xStart_;
		yStart			= yStart_;

		mode			= GraphMode::def;
	}
	Graph2d(Func2d* funcs_, long funcsCount_, Graph2dStyle style, long xSect_, long ySect_, double xScale_, double yScale_, double xStart_, double yStart_) {
		setStyle(style);

		funcsCount		= funcsCount_;
		for (long i = 0; i < funcsCount; i++) {
			funcs_[i].pen = CreatePen(PS_SOLID, 1, funcs_[i].color);
			funcs.add(funcs_[i]);
		}

		xSect			= xSect_;
		ySect			= ySect_;
		xScale			= xScale_;
		yScale			= yScale_;
		xStart			= xStart_;
		yStart			= yStart_;

		mode			= GraphMode::def;
	}
};

void graphControl(Graph2d* graph, bool* redrawNeeded) {
	while (1) {
		switch (toupper(_getch())) {
		case 'I': case 'Ø':
			graph->toggleInfoMode();
			break;
		case '+':
			graph->scaleRelatively(1.05);
			break;
		case '-':
			graph->scaleRelatively(1 / 1.05);
			break;
		case 'A': case 'Ô':
			graph->moveStart(-graph->getScaleX(), 0);
			break;
		case 'D': case 'Â':
			graph->moveStart(graph->getScaleX(), 0);
			break;
		case 'S': case 'Û':
			graph->moveStart(0, -graph->getScaleY());
			break;
		case 'W': case 'Ö':
			graph->moveStart(0, graph->getScaleY());
			break;
		case 'E': case 'Ó':
			graph->setStart(0, 0);
			break;
		case 'Q': case 'É':
			graph->setScale(1, 1);
			break;
		case 'H': case 'Ð':
			graph->toggleHelpMode();
			break;
		case 'F':
			graph->toggleNullAxes();
			break;
		case 27:
			exit(0);
		}
		(*redrawNeeded) = 1;
	}
}