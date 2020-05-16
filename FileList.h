#ifndef _FILELIST_H
#define _FILELIST_H 1

#include <cstdio>
#include <string>
#include <vector>
#include <dirent.h>
#include <climits>
#include <algorithm>
#include "status.h"

namespace filelist{

const bool LOWER_CASE = false;
const bool UPPER_CASE = true;
const int BASENAME_PART = 1;
const int EXTENSION_PART = 2;

struct File
{
	bool hidden;
	std::string oldName;
	std::string newName;
	std::string oldExtension;
	std::string newExtension;
	File(const std::string &s);
	std::string oldFullName();
	std::string newFullName();
};

class FileList
{
private:
	std::vector<File> files;
	std::string directory;
	bool ignoreHidden;
public:
	FileList();
	~FileList(){}
	bool read(const std::string &d, bool ih = true);
	void preview(int limit = 0);
	bool modify(const std::string &namestr, int begin = 1, int step = 1, int digit = 1);
	bool modifyExtension(const std::string &e);
	bool modifyCase(int part, bool u_l);
	bool execute();
};

File::File(const std::string &s)
{
	hidden = false;
	std::string::size_type dotPos = s.find_last_of('.');
	if(dotPos == 0){
		hidden = true;
		newName = oldName = s.substr(1);
	}else if(dotPos == std::string::npos){
		newName = oldName = s;
	}else{
		newName = oldName = s.substr(0, dotPos);
		newExtension = oldExtension = s.substr(dotPos+1);
	}
}

std::string File::oldFullName()
{
	return std::string(
		( hidden ? "." : "" )
		+ oldName
		+ ( oldExtension.empty() ? "" : ("." + oldExtension) )
	);
}
std::string File::newFullName()
{
	return std::string(
		( hidden ? "." : "" )
		+ newName
		+ ( newExtension.empty() ? "" : ("." + newExtension) )
	);
}

FileList::FileList()
{
	ignoreHidden = true;
}

bool FileList::read(const std::string &d, bool ih)
{
	ignoreHidden = ih;
	directory = d;
	if(directory[directory.length()-1] != '/'){
		directory += '/';
	}
	DIR *dir = opendir(directory.c_str());
	if(dir == NULL){
		status::printInfo(status::OPEN_DIR_ERROR);
		return 1;
	}

	dirent *d_ent = NULL;
	while( (d_ent = readdir(dir)) != NULL ){
		if(strcmp(d_ent->d_name, ".") != 0
		&& strcmp(d_ent->d_name, "..") != 0
		&& d_ent->d_type != DT_DIR){
			if(ignoreHidden && d_ent->d_name[0] == '.'){
				continue;
			}
			files.push_back(File(d_ent->d_name));
		}
	}

	if(files.size() == 0){
		status::printInfo(status::NO_FILE);
		return 1;
	}
	return 0;
}

void FileList::preview(int limit)
{
	std::cout << "-----PREVIEW-----" << std::endl;
	int lineNum = 1;
	for(std::vector<File>::iterator i = files.begin(); i != files.end() && (lineNum <= limit || limit == 0); ++i, ++lineNum){
		std::cout << lineNum << "." << std::endl;
		std::cout << "\t" << (*i).oldFullName() << std::endl;
		std::cout << "\t\t\t----->\t\t" << (*i).newFullName() << std::endl;
	}
	std::cout << "-------END-------" << std::endl;
	if(limit && files.size()-limit > 0){
		std::cout << "There are " << files.size() - limit << "file(s) are not showed." << std::endl;
	}
}

bool FileList::modify(const std::string &namestr, int begin, int step, int digit)
{
	if( begin + step*files.size() >= INT_MAX){
		status::printInfo(status::NUM_RANGE_LARGE);
		return 1;
	}

	int number = begin;
	char numStr[20];

	for(std::vector<File>::iterator i = files.begin(); i != files.end(); ++i){
		(*i).newName = namestr;
		std::string::size_type placeHolderPos;

		while( (placeHolderPos = (*i).newName.find('*')) != std::string::npos){
			(*i).newName.erase(placeHolderPos, 1);
			(*i).newName.insert(placeHolderPos, (*i).oldName);
		}

		sprintf(numStr, "%*d", digit, number);
		for(int i = 0; numStr[i]; ++i){
			if(numStr[i] == ' '){
				numStr[i] = '0';
			}
		}
		while( (placeHolderPos = (*i).newName.find('#')) != std::string::npos){
			(*i).newName.erase(placeHolderPos, 1);
			(*i).newName.insert(placeHolderPos, numStr);
		}
		number += step;
	}
	return 0;
}

bool FileList::modifyExtension(const std::string &e)
{
	for(std::vector<File>::iterator i = files.begin(); i != files.end(); ++i){
		(*i).newExtension = e;
	}
	return 0;
}

bool FileList::modifyCase(int part, bool u_l)
{
	if(part & BASENAME_PART){
		std::cout << "TEST\tBASENAME_PART" << std::endl;
		for(std::vector<File>::iterator i = files.begin(); i != files.end(); ++i){
			transform((*i).newName.begin(), (*i).newName.end(), (*i).newName.begin(), (u_l == UPPER_CASE ? ( (int (*)(int))toupper ) : ( (int (*)(int))tolower ) ));
		}
	}
	if(part & EXTENSION_PART){
		std::cout << "TEST\tEXTENSION_PART" << std::endl;
		for(std::vector<File>::iterator i = files.begin(); i != files.end(); ++i){
			transform((*i).newExtension.begin(), (*i).newExtension.end(), (*i).newExtension.begin(), (u_l == UPPER_CASE ? ( (int (*)(int))toupper ) : ( (int (*)(int))tolower ) ));
		}
	}
	return 0;
}

bool FileList::execute()
{
	for(std::vector<File>::iterator i = files.begin(); i != files.end(); ++i){
		if(rename( (directory + (*i).oldFullName()).c_str(), (directory + (*i).newFullName()).c_str() )){
			status::printInfo(status::RENAME_ERROR);
			return 1;
		}
	}
	return 0;
}


}
#endif
