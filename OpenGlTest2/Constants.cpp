#include "stdafx.h"
#include "Constants.h"

Constants* Constants::getConstants()
{
	static Constants* constants = nullptr;

	if (constants == nullptr)
	{
		constants = new Constants();
	}

	return constants;
}

char* Constants::game_name(){
	return "Around the space";
};