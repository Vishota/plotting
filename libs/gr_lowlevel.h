#pragma once
#include <windows.h>
#include <thread>
#include "gr_rgba.h"

#define SYS_SCL 1.25

const int MAXWID = GetSystemMetrics(SM_CXSCREEN);
const int MAXHGT = GetSystemMetrics(SM_CYSCREEN);

class Window {
private:
	HWND hWnd = GetConsoleWindow();                             //window handle
	HDC hdc = GetDC(hWnd);                                      //window hdc
	HDC bdc = CreateCompatibleDC(hdc);                          //buff hdc
	BITMAPINFOHEADER bmphead = { sizeof(bmphead), MAXWID, MAXHGT, 1, 24, BI_RGB, (DWORD)(MAXWID * MAXHGT * 4), 0, 0, 0, 0 };    //bmp info header
	RGBQUAD bmpcolors = { 0, 0, 0, 0 };                         //black color
	BITMAPINFO bmpinfo = { bmphead, bmpcolors };                //constructing bitmapinfo using header and colors
	BYTE* bmpmem = new BYTE[MAXWID * MAXHGT * 4];               //allocating memory for bitmap
	HBITMAP bmph = CreateDIBSection(bdc, &bmpinfo, DIB_RGB_COLORS, (void**)&bmpmem, 0, 0);  //creating dib section
	int byteinline = ((MAXWID * 24 + 31) / 32) * 4;             //counting how many bytes does every line contain
	RECT area;													//client area rectangle
	int wid, hgt;
public:
	HDC getWindowHdc() {
		return hdc;
	}
	HDC getBitmapHdc() {
		return bdc;
	}
	void disableSelection() {
		DWORD fdwMode = ~ENABLE_QUICK_EDIT_MODE;
		SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), fdwMode);
	}
	int calcPosInArr(int x, int y) {
		return y * byteinline + 3 * x;
	}
	void setPixel(int x, int y, Rgb color) {
		int posInArr = calcPosInArr(x, y);
		bmpmem[posInArr] = color.b;
		bmpmem[posInArr + 1] = color.g;
		bmpmem[posInArr + 2] = color.r;
	}
	void draw(int xStart, int yStart, int xEnd, int yEnd, Rgb color) {
		int posInArr = calcPosInArr(xStart, yStart), lineDiff = calcPosInArr(xStart, yStart + 1) - calcPosInArr(xEnd, yStart);
		for (int i = yStart; i < yEnd; i++) {
			for (int j = xStart; j < xEnd; j++) {
				bmpmem[posInArr] = color.b;
				bmpmem[posInArr + 1] = color.g;
				bmpmem[posInArr + 2] = color.r;
				posInArr += 3;
			}
			posInArr += lineDiff;
		}
	}
	void draw(Rgb color) {
		draw(0, 0, wid, hgt, color);
	}
	Rgb getPixel(int x, int y) {
		int posInArr = y * byteinline + 3 * x;
		return {bmpmem[posInArr + 2], bmpmem[posInArr + 1], bmpmem[posInArr]};
	}
	/*
	void drawObj(Obj2d* obj) {
		for (int i = 0; i < area.right; i++)
			for (int j = 0; j < area.bottom; j++) {
				setPixel(i, j, (obj->calcPoint({ (double)i, (double)j })));
			}
	}
	void redraw() {
		SetDIBitsToDevice(hdc, 0, 0, area.right, area.bottom, 0, 0, 0, area.bottom, bmpmem, &bmpinfo, DIB_RGB_COLORS);	//redrawing
	}
	void updateSize() {
		GetClientRect(hWnd, &area);
	}
	*/
	void updateSize() {
		GetClientRect(hWnd, &area);
		wid = area.right - area.left;
		hgt = area.bottom - area.top;
	}
	void getSize(int& width, int& height) {
		width = wid;
		height = hgt;
	}
	void getSize(long& width, long& height) {
		width = wid;
		height = hgt;
	}
	void redraw() {
		updateSize();
		SetDIBitsToDevice(hdc, 0, 0, area.right, area.bottom, 0, 0, 0, area.bottom, bmpmem, &bmpinfo, DIB_RGB_COLORS);	//redrawing
	}
	RECT getArea() {
		return area;
	}
	Window() {
		disableSelection();
		SelectObject(bdc, bmph);								//connecting buff hdc with bitmap
		updateSize();
	}
	~Window() {
		DeleteObject(bdc);
		DeleteObject(bmph);
	}
};