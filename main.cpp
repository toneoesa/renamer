#include <iostream>
#include <cstdlib>
#include <string>
#include <climits>
#include "cmdline.h"
#include "FileList.h"

using namespace std;

int main(int argc, char *argv[])
{
	cmdline::parser p;
	p.set_program_name("renamer");
	p.add("help", 'h', "print this message");
	p.add<string>("dir", 'd', "directory containing the file to be renamed", true);
	p.add<string>("namestr", 's', "a string describe filename, *,# represent the original names and the number added", false, "");
	p.add<int>("begin", '\0', "the begin of number added", false, 1, cmdline::range(0, INT_MAX/2));
	p.add<int>("step", '\0', "the step of number added", false, 1, cmdline::range(1, 1000));
	p.add<int>("digit", '\0', "digit of number added(supplement 0 when necessary)", false, 1, cmdline::range(1, 10));
	p.add<string>("extension", 'e', "change file extension", false, "");
	p.add("hidden", '\0', "Don't ignore hidden files");
	p.add<int>("lower", '\0', "change the [basename(1), extension(2), all(3)] to lower case", false, 4, cmdline::oneof<int>(1, 2, 3));
	p.add<int>("upper", '\0', "change the [basename(1), extension(2), all(3)] to upper case", false, 4, cmdline::oneof<int>(1, 2, 3));
	p.add("preview", 'p', "preview the results");
	p.add("maxitem", '\0', "max number of showed items, 0 - unlimit", false, 20, cmdline::range(0,100));
	p.parse_check(argc, argv);

	filelist::FileList fList;
	fList.read(p.get<string>("dir"), !p.exist("hidden"));
	if(p.exist("namestr")){
		fList.modify(p.get<string>("namestr"), p.get<int>("begin"), p.get<int>("step"), p.get<int>("digit"));
	}
	if(p.exist("extension")){
		fList.modifyExtension(p.get<string>("extension"));
	}
	if(p.exist("lower")){
		fList.modifyCase(p.get<int>("lower"), filelist::LOWER_CASE);
	}
	if(p.exist("upper")){
		fList.modifyCase(p.get<int>("upper"), filelist::UPPER_CASE);
	}
	if(p.exist("preview")){
		fList.preview(p.get<int>("maxitem"));
	}else{
		fList.execute();
	}
	return 0;
}
