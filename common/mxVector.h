#pragma once

#define _USE_MATH_DEFINES

#include <math.h>
#include <iostream>

class Vector2 {

public:
	double x;
	double y;

	Vector2();

	Vector2(double x_, double y_);

	virtual ~Vector2();

	Vector2 operator+(const Vector2 &oth);

	void operator+=(const Vector2 &oth);

	Vector2 operator-(const Vector2 &oth);

	void operator-=(const Vector2 &oth);

	Vector2 operator*(const Vector2 &oth);
	Vector2 operator*(const float &oth);
	Vector2 operator*=(const float &oth);
	Vector2 operator*(const double &oth);

	bool operator==(const Vector2 &oth);
	bool operator!=(const Vector2 &oth);

	void operator*=(const Vector2 &oth);

	Vector2 operator/(const Vector2 &oth);

	void operator/=(const Vector2 &oth);

	void operator/=(const float & oth);

	void operator/(const float & oth);

	Vector2 operator-(const float & oth);
	Vector2 operator+(const float & oth);

	void normalize();

	int mag();

	float angleRelTo(Vector2 pos);


private:

};

class Vector3 {

public:
	double x;
	double y;
	double z;

	Vector3();

	Vector3(double x_, double y_, double z_);

	virtual ~Vector3();

	Vector3 operator+(const Vector3 &oth);

	void operator+=(const Vector3 &oth);

	Vector3 operator-(const Vector3 &oth);

	void operator-=(const Vector3 &oth);

	Vector3 operator*(const Vector3 &oth);
	Vector3 operator*(const float &oth);
	Vector3 operator*=(const float &oth);
	Vector3 operator*(const double &oth);

	bool operator==(const Vector3 &oth);
	bool operator!=(const Vector3 &oth);

	void operator*=(const Vector3 &oth);

	Vector3 operator/(const Vector3 &oth);

	void operator/=(const Vector3 &oth);

	void operator/=(const float & oth);

	void operator/(const float & oth);

	Vector3 operator-(const float & oth);
	Vector3 operator+(const float & oth);

	void normalize();

	int mag();

	float angleRelToHorizontal(Vector3 pos);
	float angleRelToVertical(Vector3 pos);

private:

};
