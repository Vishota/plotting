#pragma once

class Rgb {
public:
	int r, g, b;
	operator COLORREF() {
		return RGB(r, g, b);
	}
	Rgb() {
		r = g = b = 0;
	}
	Rgb(int grayscale) {
		r = g = b = grayscale;
	}
	Rgb(int red, int green, int blue) {
		r = red;
		g = green;
		b = blue;
	}
};

class Rgba {
public:
	Rgb rgb;
	double alpha;
	operator Rgb() {
		return rgb;
	}
	Rgba() {
		rgb.r = rgb.g = rgb.b = 0;
		alpha = 1;
	}
	Rgba(int grayscale) {
		rgb.r = rgb.g = rgb.b = grayscale;
		alpha = 1;
	}
	Rgba(int grayscale, double alpha_) {
		rgb.r = rgb.g = rgb.b = grayscale;
		alpha = alpha_;
	}
	Rgba(int red, int green, int blue) {
		rgb.r = red;
		rgb.g = green;
		rgb.b = blue;
		alpha = 1;
	}
	Rgba(int red, int green, int blue, double _alpha) {
		rgb.r = red;
		rgb.g = green;
		rgb.b = blue;
		alpha = _alpha;
	}
	Rgba(Rgb rgb_) {
		rgb = rgb_;
		alpha = 1;
	}
	Rgba(Rgb rgb_, double alpha_) {
		rgb = rgb_;
		alpha = alpha_;
	}
};