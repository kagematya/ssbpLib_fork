﻿#include "Util.h"
#include <string>
#include <Windows.h>
#include <stdio.h>
#include "math/vector3.h"
using namespace std;

namespace ss{


double SSRadToDeg(double rad){
	return rad * (180.0 / SS_PI);
}
double SSDegToRad(double deg){
	return deg * (SS_PI / 180.0);
}


void DebugPrintToConsole(const char *filename, int line, const char *format, ...){
	char formatStr[1024];
	sprintf_s(formatStr, "%s(%d): %s\n", filename, line, format);

	va_list argp;
	char buf[1024];
	va_start(argp, format);
	vsprintf_s(buf, formatStr, argp);
	va_end(argp);

	OutputDebugStringA(buf);
}


float lerpAngle(float from, float to, double t){
	float diff = fwrap<float>(to - from, -180.0f, 180.0f);	//角度差を-180...180の範囲に収める
	float angle = from + diff*t;
	return fwrap<float>(angle, 0.0f, 360.0f);
}


} //namespace ss