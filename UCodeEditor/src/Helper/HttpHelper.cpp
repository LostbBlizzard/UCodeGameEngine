#include "HttpHelper.hpp"
#include <fstream>
#include "FileHelper.hpp"
#include <sstream>
EditorStart 

Path newtepfilepath()
{
	auto r =  FileHelper::GetNewFileName(std::filesystem::temp_directory_path() / "tep");

	for (size_t i = 0; i < r.native().size(); i++)
	{
		if (r.native()[i] == ' ') 
		{
			(*(const_cast<PathString*>(&r.native())))[i] = '_';
		}
	}

	return r;
}

Optional<String> HttpHelper::DonloadString(const Url& host, const Url& file)
{
#if UCodeGEWindows 
	String Cmd = "curl.exe";
#else
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

Optional<Path> HttpHelper::DonloadFile(const Url& host, const Url& file)
{
#if UCodeGEWindows 
	String Cmd = "curl.exe";
#else
	String Cmd = "curl";
#endif

	auto tepfilepath = newtepfilepath();

	auto newurl = host + file;

	Cmd += " -Ls";
	Cmd += " " + newurl + " -o " + tepfilepath.generic_string();

	bool isworked = system(Cmd.c_str()) == 0;

	if (isworked)
	{
		return tepfilepath;
	}
	return Optional<String>();
}

EditorEnd
