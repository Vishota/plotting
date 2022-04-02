#include <iostream>
#include <thread>
#include <cmath>
#include "base.h"

#include "gr2d.h"
#include "gr2d_graph.h"

double parabola(double x) {
	return x * x;
}
double calcY(double x) {
	return 2 * (cos(x) * cos(x) - 1);
}
double calcS(double x) {
	double s = 0, r = 1;
	for (int k = 1; k <= 30; k++)
	{
		r *= -1 * (2 * x) * (2 * x) / ((2 * k - 1) * 2 * k);
		s += r;
	}
	return s;
}
double calcYSDifference(double x) {
	return abs(calcY(x) - calcS(x));
}

int main() {
	setlocale(LC_ALL, "ru");

	Win2d wn;
	bool redrawNeeded = 1;

	Func2d funcs[3] = {
		{calcY,				{255, 0, 0},	0, L"Y(x)"},
		{calcS,				{0, 200, 255},	0, L"S(x)"},
		{calcYSDifference,	{255, 200, 0},	0, L"|Y(x) - S(x)|"},
	};

	Graph2dStyle grstyle{
		75,							//margin
		{0, 255, 0}, {0, 50, 0}, {255},	//colors(axes and secondary lines)
		10,							//arrow size
		3,							//dashes size
		16, 30,						//little and big font sizes
		1							//accuracy
	};
	Graph2d graph{
		funcs, 3,		//functions and functions count
		grstyle,		//style of graph
		100, 100,		//sect size(in pixels)
		4, 4,			//scale size
		-16., -4.		//start values
	};

	//FramesCounter framesCounter;
	//std::thread([framesCounter]() {framesCounter.countThread(); });

	//graph.setMode(GraphMode::info);
	std::thread ctrl(graphControl, &graph, &redrawNeeded);
	ctrl.detach();

	int wid = 0, hgt = 0;
	int lastWid = 0, lastHgt = 0;

	while (1) {
		wn.getSize(wid, hgt);
		if ((wid - lastWid) || (hgt - lastHgt) || redrawNeeded) {
			wn.draw({ 0 });
			graph.draw(wn);
			redrawNeeded = 0;
		}
		wn.redraw();
		lastWid = wid;
		lastHgt = hgt;
		//framesCounter.countFrame();
	}
}