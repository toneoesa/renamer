/*
	西安电子科技大学
	TS001001工程概论
	第20小组
	刘
	俞
	张
	2020年5月19日

	训练项目，严格测试前，请勿用于正式生产
*/

#ifndef _STATUS_H
#define _STATUS_H 1

#include <iostream>

namespace status{

enum status
{
	SUCCESS = 0,
	OPEN_DIR_ERROR,
	RESERVED_CHAR_IN_OLD_NAME,
	RESERVED_CHAR_IN_NEW_NAME,
	NO_FILE,
	NUM_RANGE_LARGE,
	FILENAME_EMPTY,
	RENAME_ERROR,
};

void printInfo(enum status s)
{
	switch(s){
		case SUCCESS:
			break;
		case OPEN_DIR_ERROR:
			std::cout << "("  << OPEN_DIR_ERROR << ") Open directory failed!" << std::endl;
			break;
		case RESERVED_CHAR_IN_OLD_NAME:
			std::cout << "(" << RESERVED_CHAR_IN_OLD_NAME << ") Inappropriate character in original filename!" << std::endl;
			break;
		case RESERVED_CHAR_IN_NEW_NAME:
			std::cout << "(" << RESERVED_CHAR_IN_OLD_NAME << ") Inappropriate character in new filename!" << std::endl;
			break;
		case NO_FILE:
			std::cout << "("  << NO_FILE << ") No file to rename!" << std:: endl;
			break;
		case NUM_RANGE_LARGE:
			std::cout << "("  << NUM_RANGE_LARGE << ") The range of number is too large!" << std::endl;
			break;
		case FILENAME_EMPTY:
			std::cout << "(" << FILENAME_EMPTY << ") Some filenames are cleared!" << std::endl;
			break;
		case RENAME_ERROR:
			std::cout << "("  << RENAME_ERROR << ") Rename failed!" << std::endl;
			break;
	}
}

}
#endif
