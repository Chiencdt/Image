#ifndef READBMPIMAGE_H
#define READBMPIMAGE_H

#include <iostream>
#include <string>
#include <vector>
#include <Windows.h>
#include <fstream>

class ReadBmpImage
{

public:
	void GetImage(const std::string&, 
				const std::string&);

	void ReadImage(const std::string&);

	std::vector<std::string> GetImages(const std::string&);

	std::wstring ConvStringToWstring(const std::string&) const;

	std::vector<std::string> ConvWstringToString(const std::wstring&) const;

	void SaveImageToTxt(const std::string&, const std::shared_ptr<uint8_t[]>&, uint32_t, uint32_t, uint32_t) const;

	std::uint32_t GetInfoHeader(std::ifstream&, const uint32_t) const;

	std::uint16_t GetShortInfoHeader(std::ifstream&, const uint16_t) const;

private:
	std::string _path;
	std::string _nameFile;
	std::string _fullPath;
	std::vector <std::string> _vecFullPath;

};
#endif // !READBMPIMAGE_H
