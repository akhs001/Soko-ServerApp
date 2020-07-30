#include "Utils.h"
#include <Windows.h>


void Utils::ShowMessage(std::string msg, std::string title)
{
	MessageBoxA(0, msg.c_str(), title.c_str(), MB_OK);
}


//Because C++ doesnt have a string.Split function I want to Create my own.
//My idea is to have a string like "thanos:Athanasiadis" and i want to seperate the string 
//using the delimiter ":" so my result is a vector Array of strings with 
//0-> Thanos
//1-> Athanasiadis
//I want to use this method to pass everything via a message, like Color,names,message.
std::vector<std::string> Utils::Split( const std::string str , const char delimiter)
{
	//store the string for spliting in a temp string
	std::string temp = str;
	std::vector<std::string> result;

	int ind=0;
	do
	{
		ind= temp.find(delimiter);
		//Get the left part and store it in the vector array
		result.push_back(temp.substr(0, ind));
		// remove this part from the string so we can go to the next delimiter +1 is for removing the delimiter as well
		temp.erase(0, ind+1);

	} while (ind > 0);
	
	//If its the lastOne addIt to the list as welll
	//result.push_back(temp);


	return result;



}


