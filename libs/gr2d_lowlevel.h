#pragma once

#include "gr_lowlevel.h"

class Win2d : public Window {
private:

public:
	void drawLine(int x1, int y1, int x2, int y2) {
		MoveToEx(getBitmapHdc(), x1, MAXHGT - y1, 0);
		LineTo(getBitmapHdc(), x2, MAXHGT - y2);
	}
	void drawText(LPCWSTR text, int x, int y) {
		TextOut(getBitmapHdc(), x, MAXHGT - y, text, lstrlen(text));
	}
};