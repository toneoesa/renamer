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
	bool iHidden;
public:
	FileList();
	~FileList(){}
	bool read(const std::string &dir, bool ignoreHidden = true);
	void preview(int maxitem = 0);
	bool modify(const std::string &namestr, int begin = 1, int step = 1, int digit = 1);
	bool modifyExtension(const std::string &e);
	bool modifyCase(int part, bool u_l);
	bool replace(const std::string &source, const std::string &str);
	bool insert(const std::string &source, int pos);
	bool del(int pos, int num);
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
	iHidden = true;
}

bool FileList::read(const std::string &dir, bool ignoreHidden)
{
	iHidden = ignoreHidden;
	directory = dir;
	if(directory[directory.length()-1] != '/'){
		directory += '/';
	}
	DIR *d = opendir(directory.c_str());
	if(d == NULL){
		status::printInfo(status::OPEN_DIR_ERROR);
		return 1;
	}

	dirent *d_ent = NULL;
	while( (d_ent = readdir(d)) != NULL ){
		if(strcmp(d_ent->d_name, ".") != 0
		&& strcmp(d_ent->d_name, "..") != 0
		&& d_ent->d_type != DT_DIR){
			if(iHidden && d_ent->d_name[0] == '.'){
				continue;
			}
			if(strchr(d_ent->d_name, '*') != NULL || strchr(d_ent->d_name, '$') != NULL){
				status::printInfo(status::RESERVED_CHAR_IN_OLD_NAME);
				closedir(d);
				return 1;
			}
			files.push_back(File(d_ent->d_name));
		}
	}

	closedir(d);
	if(files.size() == 0){
		status::printInfo(status::NO_FILE);
		return 1;
	}
	return 0;
}

void FileList::preview(int maxitem)
{
	std::cout << "-----PREVIEW-----" << std::endl;
	int lineNum = 1;
	for(std::vector<File>::iterator i = files.begin(); i != files.end() && (lineNum <= maxitem || maxitem == 0); ++i, ++lineNum){
		std::cout << lineNum << "." << std::endl;
		std::cout << "\t" << (*i).oldFullName() << std::endl;
		std::cout << "\t\t\t----->\t\t" << (*i).newFullName() << std::endl;
	}
	std::cout << "-------END-------" << std::endl;
	if(maxitem && int(files.size()-maxitem) > 0){
		std::cout << int(files.size() - maxitem) << " file(s) are not showed." << std::endl;
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
		while( (placeHolderPos = (*i).newName.find('$')) != std::string::npos){
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

bool FileList::replace(const std::string &source, const std::string &str)
{
	for(std::vector<File>::iterator i = files.begin(); i != files.end(); ++i){
		std::size_t pos = 0;
		while( ( pos = (*i).newName.find(source, pos) ) != std::string::npos ){
			(*i).newName.erase(pos, source.length());
			(*i).newName.insert(pos, str);
			pos += str.length();
			if(pos > (*i).newName.length()){
				break;
			}
		}
		if( (*i).newName.length() == 0 ){
			status::printInfo(status::FILENAME_EMPTY);
			return 1;
		}
	}
	return 0;
}

bool FileList::insert(const std::string &source, int pos)
{
	for(std::vector<File>::iterator i = files.begin(); i != files.end(); ++i){
		if(pos >= int((*i).newName.length())){
			(*i).newName.append(source);
		}else{
			(*i).newName.insert(pos, source);
		}
	}
	return 0;
}

bool FileList::del(int pos, int num)
{
	for(std::vector<File>::iterator i = files.begin(); i != files.end(); ++i){
		if(pos >= int((*i).newName.length())){
			continue;
		}else{
			(*i).newName.erase(pos, num);
			if( (*i).newName.length() == 0 ){
				status::printInfo(status::FILENAME_EMPTY);
				return 1;
			}
		}
	}
	return 0;
}

bool FileList::execute()
{
	for(std::vector<File>::iterator i = files.begin(); i != files.end(); ++i){
		if( (*i).newFullName().find('/') != std::string::npos ){
			status::printInfo(status::RESERVED_CHAR_IN_NEW_NAME);
			return 1;
		}
		if(rename( (directory + (*i).oldFullName()).c_str(), (directory + (*i).newFullName()).c_str() )){
			status::printInfo(status::RENAME_ERROR);
			return 1;
		}
	}
	return 0;
}

}
#endif
