#pragma once

#include "gr2d_transform.h"
#include "gr2d_lowlevel.h"
#include <Windows.h>

class Obj2d {
public:
	//drawing
	virtual void draw(Win2d& window)			{ }
};

//class Obj2d : public Obj2dStatic {
//private:
//	Transform2d transform;
//public:
//	//set
//	void setTransform(Transform2d& transform_)	{ transform = transform_; }
//	void setPosition(Vec2d& pos)				{ transform.setPos(pos); }
//	void setRotation(Angle& rot)				{ transform.setRot(rot); }
//	void setScale(double& scl)					{ transform.setScale(scl); }
//	//get
//	Transform2d getTransform()					{ return transform; }
//	Transform2d& getTransformLink()				{ return transform; }
//
//	//relative
//	void move(Vec2d offset)						{ transform += offset; }
//	void rotate(double angle)					{ transform += angle; }
//	void scale(double scale)					{ transform *= scale; }
//};