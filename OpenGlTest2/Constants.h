#include "stdafx.h"
class Constants
{
public:
	static Constants* Constants::getConstants();

	const bool oculus = false;

	const int window_width = 1280;
	const int window_heigh = 720;

	static char* game_name();

};

