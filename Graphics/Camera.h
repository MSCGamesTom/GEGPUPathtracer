#pragma once

#include "Math.h"
#include <cmath>

#define MOVE_SPEED 5.1f
#define ROTATE_SPEED 2.1f

class Camera
{
public:
	Matrix projection;
	Matrix inverseProjection;
	Matrix view;
	Matrix inverseView;
	Vec3 position;
	Vec3 forward;
	Vec3 up;
	float moveSpeed;
	int width;
	int height;
	void init(Matrix P, int _width, int _height)
	{
		projection = P;
		inverseProjection = P.invert();
		width = _width;
		height = _height;
		inverseProjection = inverseProjection.transpose();
	}
	void initView(Matrix _v)
	{
		view = _v;
		inverseView = view.invert();
		inverseView = inverseView.transpose();
		position = inverseView.extractPosition();
		forward = -Vec3(view.a[2][0], view.a[2][1], view.a[2][2]).normalize();
		up = Vec3(view.a[1][0], view.a[1][1], view.a[1][2]).normalize();
		updateViewMatrix();
	}
	void moveForward()
	{
		position += forward * moveSpeed;
		updateViewMatrix();
	}
	void moveBackward()
	{
		position -= forward * moveSpeed;
		updateViewMatrix();
	}
	void moveRight()
	{
		Vec3 right = Cross(forward, up).normalize();
		position += right * moveSpeed;
		updateViewMatrix();
	}
	void moveLeft()
	{
		Vec3 right = Cross(forward, up).normalize();
		position -= right * moveSpeed;
		updateViewMatrix();
	}
	void look(float dx, float dy, float sensitivity)
	{
		Vec3 right = Cross(forward, up).normalize();
		float pitchAngle = dy * sensitivity;
		Matrix pitchMatrix = Matrix::rotateAxis(right, pitchAngle);
		forward = pitchMatrix.mulVec(forward).normalize();
		float yawAngle = dx * sensitivity;
		Matrix yawMatrix = Matrix::rotateAxis(up, yawAngle);
		forward = yawMatrix.mulVec(forward).normalize();
		updateViewMatrix();
	}
	Vec3 dir()
	{
		return forward;
	}
	void updateViewMatrix()
	{
		view = Matrix::lookAt(position, position + forward, up);
		inverseView = view.invert();
		inverseView = inverseView.transpose();
	}
};