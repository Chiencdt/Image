#ifndef READBMPIMAGE_H
#define READBMPIMAGE_H

#include <iostream>
#include <string>
#include <vector>
#include <Windows.h>

class ReadBmpImage
{

public:
	void GetImage(const std::string&, 
				const std::string&);

	void ReadImage(const std::string&);

	std::vector<std::string> GetImages(const std::string&);

	std::wstring ConvStringToWstring(const std::string&) const;

	std::vector<std::string> ConvWstringToString(const std::wstring&) const;

	void SaveImageToTxt(const std::string&, uint8_t*, int) const;

private:
	std::string _path;
	std::string _nameFile;
	std::string _fullPath;
	std::vector <std::string> _vecFullPath;

};
#endif // !READBMPIMAGE_H
