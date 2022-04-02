#pragma once

#include "gr_rgba.h"
#include <math.h>

//transform
class Angle {
	double ang;
public:
	void operator+= (Angle value) {
		ang += value;
		if (ang > 360)		ang -= 360;
		if (ang < 0)		ang += 360;
	}
	void operator+= (double value) {
		ang += value;
		ang = ang - floor(ang / 360) * 360;
	}
	void operator= (double value) {
		ang = value - floor(value / 360) * 360;
	}

	operator double() { return ang; }

	Angle() {
		ang = 0;
	}
	Angle(double angle) {
		(*this) = angle;
	}
};
class Vec2d {
public:
	double x, y;
	Vec2d operator- () {
		return { -x, -y };
	}
	void operator+= (Vec2d vector) {
		x += vector.x;
		y += vector.y;
	}
	Vec2d operator+ (Vec2d vector) {
		return { x + vector.x, y + vector.y };
	}
	void operator-= (Vec2d vector) {
		(*this) += -vector;
	}
	Vec2d operator- (Vec2d vector) {
		return { x - vector.x, y - vector.y };
	}
	void operator*= (double value) {
		x *= value;
		y *= value;
	}
	Vec2d operator* (double value) {
		return { x * value, y * value };
	}
	Vec2d operator/ (double value) {
		return { x / value, y / value };
	}

	Vec2d rotate(Angle angle) {
		return { x * cos(angle) - y * sin(angle), x * sin(angle) + y * cos(angle) };
	}
	double calcDistance(Vec2d point) {
		return abs(pow((pow(point.y - y, 2) + pow(point.x - x, 2)), 1 / 2.));
	}

	Vec2d() {
		x = y = 0;
	}
	Vec2d(double _x, double _y) {
		x = _x;
		y = _y;
	}
};
class Transform2d {
private:
	Vec2d	position_;
	Angle	rotation_;
	double	scale_;
public:
	void setPos(Vec2d position) { position_ = position; }
	void setPos(double x, double y) { position_ = { x, y }; }
	void setRot(Angle rotation) { rotation_ = rotation; }
	void setScale(double scale) { scale_ = scale; }

	Vec2d getPos() { return position_; }
	Angle getRot() { return rotation_; }
	double getScale() { return scale_; }

	void operator+= (Vec2d offset) { position_ += offset; }
	void operator+= (Angle angle) { rotation_ += angle; }
	void operator*= (double relScale) { scale_ *= relScale; }

	Transform2d operator+ (Vec2d vector) { return { position_ + vector, rotation_, scale_ }; }
	Transform2d operator+ (Angle angle) { return { position_, rotation_ + angle, scale_ }; }
	Transform2d operator* (double relScale) { return { position_, rotation_, scale_ * relScale }; }

	Transform2d() {
		position_ = { 0, 0 };
		rotation_ = 0;
		scale_ = 1;
	}
	Transform2d(double x, double y) {
		position_ = { x, y };
		rotation_ = 0;
		scale_ = 1;
	}
	Transform2d(double x, double y, double rotation) {
		position_ = { x, y };
		rotation_ = rotation;
		scale_ = 1;
	}
	Transform2d(double x, double y, double rotation, double scale) {
		position_ = { x, y };
		rotation_ = rotation;
		scale_ = scale;
	}
	Transform2d(Vec2d position, double rotation, double scale) {
		position_ = position;
		rotation_ = rotation;
		scale_ = scale;
	}
};

class Int2d {
public:
	int x, y;
	Int2d() {
		x = 0;
		y = 0;
	}
	Int2d(int x_, int y_) {
		x = x_;
		y = y_;
	}
};