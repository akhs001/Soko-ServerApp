#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <SDL.h>

namespace Utils {
	void ShowMessage(std::string msg, std::string title);
	std::vector<std::string> Split(const std::string str, const char delimiter);
}
