#include "mxVector.h"

Vector2::Vector2() {

}

Vector2::Vector2(double x_, double y_) {
	x = x_;
	y = y_;
}

Vector2::~Vector2() {

}

Vector2 Vector2::operator+(const Vector2 & oth)
{
	return Vector2(this->x + oth.x, this->y + oth.y);
}

void Vector2::operator+=(const Vector2 & oth)
{
	this->x += oth.x;
	this->y += oth.y;
}

Vector2 Vector2::operator-(const Vector2 & oth)
{
	return Vector2(this->x - oth.x, this->y - oth.y);
}

void Vector2::operator-=(const Vector2 & oth)
{
	this->x -= oth.x;
	this->y -= oth.y;
}

Vector2 Vector2::operator*(const Vector2 & oth)
{
	return Vector2(this->x * oth.x, this->y * oth.y);
}

Vector2 Vector2::operator*(const float & oth)
{
	return Vector2(x * oth, y * oth);
}

Vector2 Vector2::operator*=(const float & oth)
{
	return Vector2(x *= oth, y *= oth);
}

Vector2 Vector2::operator*(const double & oth)
{
	return Vector2(x * oth, y * oth);
}

bool Vector2::operator==(const Vector2 & oth)
{
	if (x == oth.x && y == oth.y) {
		return true;
	}
	return false;
}

bool Vector2::operator!=(const Vector2 & oth)
{
	if (x == oth.x && y == oth.y) {
		return false;
	}
	return true;
}

void Vector2::operator*=(const Vector2 & oth)
{
	this->x *= oth.x;
	this->y *= oth.y;
}

Vector2 Vector2::operator/(const Vector2 & oth)
{
	return Vector2(this->x / oth.x, this->y / oth.y);
}

void Vector2::operator/=(const Vector2 & oth)
{
	this->x /= oth.x;
	this->y /= oth.y;
}

void Vector2::operator/=(const float & oth)
{
	this->x /= oth;
	this->y /= oth;
}

void Vector2::operator/(const float & oth)
{
	this->x / oth;
	this->y / oth;
}

Vector2 Vector2::operator-(const float & oth)
{
	return Vector2(this->x -= oth, this->y -= oth);
}

Vector2 Vector2::operator+(const float & oth)
{
	return Vector2(this->x += oth, this->y += oth);

}

void Vector2::normalize() {
	int i = mag();
	if (i != 0) {
		y = y / i;
		x = x / i;
	}
}

int Vector2::mag() {
	return sqrt(x*x + y*y);
}

float Vector2::angleRelTo(Vector2 pos)
{
	float angle = atan2(pos.x - x, pos.y - y);
	float degrees = angle * 180 / M_PI;
	return degrees;
}


Vector3::Vector3() {

}

Vector3::Vector3(double x_, double y_, double z_) {
	x = x_;
	y = y_;
	z = z_;
}

Vector3::~Vector3() {

}

Vector3 Vector3::operator+(const Vector3 & oth) {
	return Vector3(this->x + oth.x, this->y + oth.y, this->z + oth.z);
}

void Vector3::operator+=(const Vector3 & oth) {
	this->x += oth.x;
	this->y += oth.y;
	this->z += oth.z;
}

Vector3 Vector3::operator-(const Vector3 & oth) {
	return Vector3(this->x - oth.x, this->y - oth.y, this->z - oth.z);
}

void Vector3::operator-=(const Vector3 & oth) {
	this->x -= oth.x;
	this->y -= oth.y;
	this->z -= oth.z;
}

Vector3 Vector3::operator*(const Vector3 & oth) {
	return Vector3(this->x * oth.x, this->y * oth.y, this->y * oth.z);
}

Vector3 Vector3::operator*(const float & oth) {
	return Vector3(x * oth, y * oth, z * oth);
}

Vector3 Vector3::operator*=(const float & oth) {
	return Vector3(x *= oth, y *= oth, z *= oth);
}

Vector3 Vector3::operator*(const double & oth) {
	return Vector3(x * oth, y * oth, z * oth);
}

bool Vector3::operator==(const Vector3 & oth) {
	if (x == oth.x && y == oth.y && z == oth.z) {
		return true;
	}
	return false;
}

bool Vector3::operator!=(const Vector3 & oth) {
	if (x == oth.x && y == oth.y && z == oth.z) {
		return false;
	}
	return true;
}

void Vector3::operator*=(const Vector3 & oth) {
	this->x *= oth.x;
	this->y *= oth.y;
	this->z *= oth.z;
}

Vector3 Vector3::operator/(const Vector3 & oth) {
	return Vector3(this->x / oth.x, this->y / oth.y, this->z / oth.z);
}

void Vector3::operator/=(const Vector3 & oth) {
	this->x /= oth.x;
	this->y /= oth.y;
	this->z /= oth.z;
}

void Vector3::operator/=(const float & oth) {
	this->x /= oth;
	this->y /= oth;
	this->z /= oth;
}

void Vector3::operator/(const float & oth) {
	this->x / oth;
	this->y / oth;
	this->z / oth;
}

Vector3 Vector3::operator-(const float & oth) {
	return Vector3(this->x -= oth, this->y -= oth, this->z -= oth);
}

Vector3 Vector3::operator+(const float & oth) {
	return Vector3(this->x += oth, this->y += oth, this->z += oth);

}

void Vector3::normalize() {
	int i = mag();
	if (i != 0) {
		y = y / i;
		x = x / i;
		z = z / i;
	}
}

int Vector3::mag() {
	return sqrt(x*x + y*y + z*z);
}

float Vector3::angleRelToHorizontal(Vector3 pos) {
	float angle = atan2(pos.x - x, pos.y - y);
	float degrees = angle * 180 / M_PI;
	return degrees;
}

float Vector3::angleRelToVertical(Vector3 pos) {
	float angle = atan2(pos.x - x, pos.z - z);
	float degrees = angle * 180 / M_PI;
	return degrees;
}
