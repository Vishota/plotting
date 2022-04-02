#pragma once

#include "gr2d_obj.h"

//class Ellipse : Obj2d {
//private:
//	int rx, ry;
//	int x, y;
//	Rgba insideColor, borderColor;
//public:
//	Ellipse(Int2d center, Int2d radius, Rgba color_) {
//		rx = radius.x;
//		ry = radius.y;
//		x = center.x;
//		y = center.y;
//		insideColor = borderColor = color_;
//	}/*
//	void draw(BYTE* bmp, Window) {
//		int i = 0, y = ry, delta = 1 - 2 * rx, error = 0;
//		while (y>=x) {
//			if ((delta < 0) && (error <= 0))	delta += 2 * ++x + 1;
//			if ((delta > 0) && (error > 0))		delta -= 2 * --y + 1;
//			delta += 2 * (++x - --y);
//			}
//		}
//		//int x := 0
//		//int y := R
//		//int delta := 1 - 2 * R
//		//int error := 0
//		//while (y >= x)
//		//    drawpixel(X1 + x, Y1 + y)
//		//    drawpixel(X1 + x, Y1 - y)
//		//    drawpixel(X1 - x, Y1 + y)
//		//    drawpixel(X1 - x, Y1 - y)
//		//    drawpixel(X1 + y, Y1 + x)
//		//    drawpixel(X1 + y, Y1 - x)
//		//    drawpixel(X1 - y, Y1 + x)
//		//    drawpixel(X1 - y, Y1 - x)
//		//    error = 2 * (delta + y) - 1
//		//    if ((delta < 0) && (error <= 0))
//		//        delta += 2 * ++x + 1
//		//        continue
//		//        if ((delta > 0) && (error > 0))
//		//            delta -= 2 * --y + 1
//		//            continue
//		//            delta += 2 * (++x - --y)
//	}*/
//};