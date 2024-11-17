#include "global.hpp"
#include <fstream>
#include <string>
#include <iostream>

using namespace std;

using namespace retdec;
using namespace yaracpp;

void searchFile(const char* curDir, bool useInsideDirectory)
{
	char temp[MAX_PATH] = { 0 };
	lstrcpyA(temp, curDir);
	lstrcatA(temp, "\\");
	lstrcatA(temp, "*.*");

	WIN32_FIND_DATAA ffd = { 0 };
	auto file = FindFirstFileA(temp, &ffd);

	if (file == INVALID_HANDLE_VALUE)
	{
		return;
	}
	do
	{
		if (!lstrcmpA(ffd.cFileName, ".")
			|| !lstrcmpA(ffd.cFileName, ".."))
		{
			continue;
		}

		memset(temp, 0, sizeof(temp));
		lstrcpyA(temp, curDir);
		lstrcatA(temp, "\\");
		lstrcatA(temp, ffd.cFileName);

		if (strstr(temp, ".yar"))
		{

		}

		if (useInsideDirectory &&
			ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			searchFile(temp, useInsideDirectory);
			continue;
		}

	} while (FindNextFileA(file, &ffd) != 0);
	
	FindClose(file);
}

void ParseYarLink(std::string pathYarLink, std::vector<string>& outLink)
{
	ifstream ifstreams(pathYarLink);

	string line;

	bool skipInclude = false;

	auto clearPath = pathYarLink.substr(0, pathYarLink.find_last_of("\\/"));

	int i = 0;
	while (getline(ifstreams, line))
	{
		if (line.empty())
		{
			continue;
		}

		if (line.find("/*") != string::npos)
		{
			//std::cout << "Start Skip" << std::endl;
			skipInclude = true;
			continue;
		}

		if (line.find("*/") != string::npos)
		{
			//std::cout << "End Skip" << std::endl;
			skipInclude = false;
			continue;
		}

		if (skipInclude)
		{
			continue;
		}

		auto newPath = clearPath + "\\" + std::string(line.begin() + 11, line.end() - 1);
		i++;

	/*	if (i > 58)
		{
			break;
		}
		*/
		/*if (newPath.find("REvil_Cert.yar") != string::npos)
		{
			std::cout << "End Skip" << std::endl;
			continue;
		}*/

		outLink.push_back(newPath);
		std::cout << newPath << std::endl;
	}

	ifstreams.close();
}

int main()
{

	std::vector<string> linkYara;
	ParseYarLink("C:\\Users\\admin\\Desktop\\Temka\\YaraRules\\all.yar", linkYara);

	YaraDetector yara;
	if (!yara.isInValidState())
	{
		return 0;
	}
	//"C:\\Users\\admin\\Desktop\\Temka\\YaraRules\\InResearch/UASlogans.yar"
	//if (!yara.addRuleFile("C:\\Users\\admin\\Desktop\\Temka\\YaraRules\\InResearch\\UASlogans.yar"))
	//{
	//	std::cout << "Error yara: " << "C:\\Users\\admin\\Desktop\\Temka\\YaraRules\\InResearch\\UASlogans.yar" << std::endl;
	//}

	for (auto path : linkYara)
	{
		if (!yara.addRuleFile(path))
		{
			std::cout << "Error yara: " << path << std::endl;
		}
		//break;

	}

	if (!yara.analyze("C:\\Users\\admin\\Desktop\\1.txt", 0))
	{
		MessageBox(0, 0, L"No detect", 0);
	}


	for (auto& detect : yara.getDetectedRules())
	{
		MessageBoxA(0, 0, detect.getName().c_str(), 0);
		
	}

	while (true)
	{

	}
	return 0;
}