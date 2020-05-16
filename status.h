#ifndef _STATUS_H
#define _STATUS_H 1

#include <iostream>

namespace status{

enum status
{
	OPEN_DIR_ERROR,
	NO_FILE,
	NUM_RANGE_LARGE,
	RENAME_ERROR
};

void printInfo(enum status s)
{
	switch(s){
		case OPEN_DIR_ERROR:
			std::cout << "Open directory failed!" << std::endl;
			break;
		case NO_FILE:
			std::cout << "No file to rename!" << std:: endl;
			break;
		case NUM_RANGE_LARGE:
			std::cout << "The range of number is too large!" << std::endl;
			break;
		case RENAME_ERROR:
			std::cout << "Rename failed!" << std::endl;
			break;
			


	}
}



}
#endif
