/*
* Kim Hagyeong (hakyung.kim)
* Class Project
* cs200
* FALL 2019
*/
#include "StringConverter.h"

std::wstring StringConverter::StringToWide(std::string str)
{
	std::wstring wide_string(str.begin(), str.end());
	return wide_string;
}