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

#include <iostream>
#include <cstdlib>
#include <string>
#include <climits>
#include "cmdline.h"
#include "FileList.h"
#include "status.h"

using namespace std;

int main(int argc, char *argv[])
{
	cmdline::parser p;
	p.set_program_name("renamer");
	p.add("help", 'h', "print this message");
	p.add<string>("dir", 'd', "directory containing the files to be renamed", true);
	p.add("hidden", '\0', "Don't ignore hidden files");
	p.add<string>("namestr", 's', "a string describe filename, *,$ represent the original names and the number added", false, "");
	p.add<int>("begin", '\0', "the begin of number added", false, 1, cmdline::range(0, INT_MAX/2));
	p.add<int>("step", '\0', "the step of number added", false, 1, cmdline::range(1, 1000));
	p.add<int>("digit", '\0', "digit of number added(supplement 0 when necessary)", false, 1, cmdline::range(1, 10));
	p.add<string>("extension", 'e', "change file extension", false, "");
	p.add<int>("lower", 'l', "change the [basename(1), extension(2), both(3)] to lower case", false, 3, cmdline::oneof<int>(1, 2, 3));
	p.add<int>("upper", 'u', "change the [basename(1), extension(2), both(3)] to upper case", false, 3, cmdline::oneof<int>(1, 2, 3));
	p.add<string>("replace", 'r', "string need to be replaced", false);
	p.add<string>("to", '\0', "the string will be replaced to this(empty string - delete)", false, "");
	p.add<string>("insert", 'i', "string to insert", false);
	p.add<int>("into", '\0', "insert position", false, 0, cmdline::range(0, 255));
	p.add<string>("append", 'a', "append string to the original names", false);
	p.add<int>("delete", 'D', "which position start to delete", false, 0, cmdline::range(0, 255));
	p.add<int>("dnum", '\0', "number of character to be deleted", false, 1, cmdline::range(0, 255));
	p.add("preview", 'p', "preview the results(Don't execute now)");
	p.add<int>("maxitem", '\0', "max number of showed items, 0 - unlimit", false, 20, cmdline::range(0,100));

	p.parse_check(argc, argv);

	filelist::FileList fList;

	if(
		fList.read(p.get<string>("dir"), !p.exist("hidden"))
	) return 0;
	
	if(p.exist("namestr")){
		if(
			fList.modify(p.get<string>("namestr"), p.get<int>("begin"), p.get<int>("step"), p.get<int>("digit"))
		) return 0;
	}
	if(p.exist("extension")){
		if(
			fList.modifyExtension(p.get<string>("extension"))
		) return 0;
	}
	if(p.exist("lower")){
		if(
			fList.modifyCase(p.get<int>("lower"), filelist::LOWER_CASE)
		) return 0;
	}
	if(p.exist("upper")){
		if(
			fList.modifyCase(p.get<int>("upper"), filelist::UPPER_CASE)
		) return 0;
	}
	if(p.exist("replace")){
		if(
			fList.replace(p.get<string>("replace"), p.get<string>("to"))
		) return 0;
	}
	if(p.exist("insert")){
		if(
			fList.insert(p.get<string>("insert"), p.get<int>("into"))
		) return 0;
	}
	if(p.exist("append")){
		if(
			fList.insert(p.get<string>("append"), 256)
		) return 0;
	}
	if(p.exist("delete")){
		if(
			fList.del(p.get<int>("delete"), p.get<int>("dnum"))
		) return 0;
	}
	if(p.exist("preview")){
		fList.preview(p.get<int>("maxitem"));
	}else{
		string sure;
		cout << "Execute now, are you sure?[y/n]" << endl;
		cin >> sure;
		if(sure == "y" || sure == "Y"){
			if(
				fList.execute()
			) return 0;
		}
	}
	return 0;
}
