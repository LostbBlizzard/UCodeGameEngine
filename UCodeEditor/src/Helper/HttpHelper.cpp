#include "HttpHelper.hpp"
#include <fstream>
EditorStart 

Path newtepfilepath()
{
	return "tep";
}

Optional<String> HttpHelper::DonloadString(const Url& host, const Url& file)
{
#if UCodeGEWindows 
	String Cmd = "curl.exe";
#elif
	String Cmd = "curl";
#endif

	auto tepfilepath = newtepfilepath();

	auto newurl = host + file;

	Cmd += " -Ls";
	Cmd += " " + newurl + " -o " + tepfilepath.generic_string();

	bool isworked = system(Cmd.c_str()) == 0;
	if (isworked)
	{
		std::ifstream t(tepfilepath);
		std::stringstream buffer;
		buffer << t.rdbuf();
		t.close();
		std::filesystem::remove(tepfilepath);

		
		return buffer.str();
	}

	return Optional<String>();
}

EditorEnd
