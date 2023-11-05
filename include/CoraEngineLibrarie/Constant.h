#pragma once
#define PI 3.14159265
const int width = 600;
const int height = 600;
const float rayon = 5;
const int vitesse = 1; // Speed player
const int v_angle = 10; // Speed angle
const int angle_vision = 64; // FOV
const int precision_angle = 3; //equals to number of values in 1 degre

const unsigned int sizemaxX = 32, sizemaxY = 32;

const int fps = 144;

const float rate_map = 5;

inline float toRadian(float degree) { return (PI / 180) * degree; }
inline float dCos(float degree) { return cos(toRadian(degree)); }
inline float dSin(float degree) { return sin(toRadian(degree)); }
inline float dTan(float degree) { return tan(toRadian(degree)); }