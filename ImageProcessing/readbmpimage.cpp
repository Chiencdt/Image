#include "readbmpimage.h"
#include <fstream>
#include <cstdint>
#include "bmpstructure.h"

const std::wstring _emptyWstring(L"");
const std::string _emptyString("");

const uint32_t _width = 18;
const uint32_t _height = 22;
const uint32_t _dataOffset = 10;
const uint32_t _bitPerPixel = 28;
const uint16_t _signature = 0;

void ReadBmpImage::GetImage(const std::string& path,
							const std::string& nameFile) 
{

	_path = path;
	_nameFile = nameFile;
	std::string fullPath = _path + _nameFile + ".bmp";
	ReadImage(fullPath);

}

void ReadBmpImage::ReadImage(const std::string& fullPath)
{

	if (!fullPath.empty()) {

		_fullPath = fullPath;
		std::ifstream file(fullPath, std::ios::binary);

		if (!file.is_open()) {
			std::cerr << "Error: Could not open the file." << std::endl;
			return;
		}

		/*BMPHeader header;
		BMPInfoHeader infoHeader;*/

		// Read BMP header
		/*file.read(reinterpret_cast<char*>(&header), sizeof(BMPHeader));
		file.read(reinterpret_cast<char*>(&infoHeader), sizeof(BMPInfoHeader));*/

		uint16_t signature(GetShortInfoHeader(file, _signature));

		// Check if the file is actually a BMP file by verifying signature
		if (signature != 0x4D42) {
			std::cerr << "Error: Not a BMP file." << std::endl;
			return;
		}

		//file.seekg(18, std::ios::beg); // Move to the position of width in the info header
		//int32_t width;
		//file.read(reinterpret_cast<char*>(&width), sizeof(int32_t));

		//file.seekg(22, std::ios::beg); // Move to the position of height in the info header
		//int32_t height;
		//file.read(reinterpret_cast<char*>(&height), sizeof(int32_t));

		//file.seekg(10, std::ios::beg); // Move to the position of data offset
		//uint32_t dataOffset;
		//file.read(reinterpret_cast<char*>(&dataOffset), sizeof(uint32_t));

		//file.seekg(28, std::ios::beg); // Move to the position of bit per pixel
		//uint32_t bitPerPixel;
		//file.read(reinterpret_cast<char*>(&bitPerPixel), sizeof(uint32_t));

		int32_t width(GetInfoHeader(file, _width));
		int32_t height(GetInfoHeader(file, _height));
		int32_t dataOffset(GetInfoHeader(file, _dataOffset));
		int32_t bitPerPixel(GetInfoHeader(file, _bitPerPixel));

		// Read image data (assuming 24 bits per pixel)
		uint32_t dataSize = width * height;
		if (bitPerPixel == 24) {

			dataSize = width * height * 3;

		}

		uint32_t padding = (4 - (width * 3) % 4) % 4;
		dataSize += padding * height;

		std::shared_ptr<uint8_t[]> imageData(new uint8_t[dataSize]);
		file.seekg(dataOffset, std::ios::beg); // Move to the beginning of image data
		file.read(reinterpret_cast<char*>(imageData.get()), dataSize);

		// Close the file
		file.close();

		SaveImageToTxt(fullPath, imageData, width, height, bitPerPixel);

	}

}

std::vector<std::string> ReadBmpImage::GetImages(const std::string& path)
{

	std::vector< std::string > names;
	_path = path;

	try
	{

		std::wstring searchPath = ConvStringToWstring(_emptyString);
		
		searchPath += L"\\*.*"; // Concatenate folder path with wildcard pattern

		names = ConvWstringToString(searchPath);
		
	}
	catch (const std::exception& ex)
	{

		std::cerr << "Error: " << ex.what() << std::endl;

	}

	return names;
}

std::wstring ReadBmpImage::ConvStringToWstring(const std::string& path) const
{

	std::wstring searchPath = L"";

	int bufferSize = MultiByteToWideChar(CP_UTF8, 0, path.c_str(), -1, nullptr, 0);

	if (bufferSize > 0) {

		std::vector<wchar_t> buffer(bufferSize);
		MultiByteToWideChar(CP_UTF8, 0, path.c_str(), -1, buffer.data(), bufferSize);
		searchPath = buffer.data();
		return searchPath;

	}

	std::cerr << "Failed to convert string to wide string." << std::endl;
	throw std::exception("Failed to convert string to wide string.");

}

std::vector<std::string> ReadBmpImage::ConvWstringToString(const std::wstring& searchPath) const
{

	std::vector<std::string> names;
	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile(searchPath.c_str(), &fd);
	if (hFind != INVALID_HANDLE_VALUE) {

		do {

			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {

				// Convert WCHAR* to std::string
				int bufferSize = WideCharToMultiByte(CP_UTF8, 0, fd.cFileName, -1, nullptr, 0, nullptr, nullptr);
				if (bufferSize > 0) {
					std::vector<char> buffer(bufferSize);
					WideCharToMultiByte(CP_UTF8, 0, fd.cFileName, -1, buffer.data(), bufferSize, nullptr, nullptr);
					names.push_back(buffer.data());

				}

			}

		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);

		return names;

	}

	throw std::runtime_error("Failed to find files in folder.");

}

void ReadBmpImage::SaveImageToTxt(const std::string& fileName, const std::shared_ptr<uint8_t[]> &data, uint32_t width, uint32_t height, uint32_t bitPerPixel) const
{
	std::string f = "D:\\Image\\2021_05_03\\UP0305\\2.txt";
	std::ofstream file(f);
	if (file.is_open())
	{

		uint32_t index = 0;
		for (int h = 0; h < height; h++)
		{

			if (bitPerPixel == 24) {

				index = h * width * 3;
				uint8_t* value = &data.get()[index];

				for (int w = 0; w < width; w++)
				{

					file << static_cast<int>(*value) << " ";
					file << static_cast<int>(*(value++)) << " ";
					file << static_cast<int>(*(value++)) << " ";
					value++;

				}
			}
			else {

				index = h * width;
				uint8_t* value = &data.get()[index];
				for (int w = 0; w < width; w++)
				{

					file << static_cast<int>(*value) << " ";
					value++;

				}
			}

		}
		/*for (int sData = 0; sData < sizeData; sData++)
		{
			
			file << static_cast<int>(*value) << " ";
			if (sData != (sizeData - 1)) {
				value++;
			}

		}*/
		file.close();
		std::cout << "Image data saved to " << fileName << std::endl;
	}
	else {
		std::cerr << "Unable to open file for writing: " << fileName << std::endl;
	}
}

std::uint32_t ReadBmpImage::GetInfoHeader(std::ifstream& file, const uint32_t info) const
{

	file.seekg(info, std::ios::beg); // Move to the position of width in the info header
	int32_t infoHeader;
	file.read(reinterpret_cast<char*>(&infoHeader), sizeof(int32_t));
	return infoHeader;

}

std::uint16_t ReadBmpImage::GetShortInfoHeader(std::ifstream& file, const uint16_t info) const
{

	file.seekg(info, std::ios::beg); // Move to the position of width in the info header
	uint16_t infoHeader;
	file.read(reinterpret_cast<char*>(&infoHeader), sizeof(uint16_t));
	return infoHeader;

}
