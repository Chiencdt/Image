#include "readbmpimage.h"
#include <fstream>
#include <cstdint>
#include "bmpstructure.h"

const std::wstring _emptyWstring(L"");
const std::string _emptyString("");

void ReadBmpImage::GetImage(const std::string& path,
							const std::string& nameFile) 
{

	_path = path;
	_nameFile = nameFile;
	std::string fullPath = _path + "//" + _nameFile + ".bmp";
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

		BMPHeader header;
		BMPInfoHeader infoHeader;

		// Read BMP header
		file.read(reinterpret_cast<char*>(&header), sizeof(BMPHeader));
		file.read(reinterpret_cast<char*>(&infoHeader), sizeof(BMPInfoHeader));

		// Check if the file is actually a BMP file by verifying signature
		if (header.signature != 0x4D42) {
			std::cerr << "Error: Not a BMP file." << std::endl;
			return;
		}

		// Print image dimensions
		std::cout << "Width: " << infoHeader.width << std::endl;
		std::cout << "Height: " << infoHeader.height << std::endl;

		// Read image data (assuming 24 bits per pixel)
		int dataSize = infoHeader.width * infoHeader.height * 3; // 3 bytes per pixel (RGB)
		std::shared_ptr<uint8_t> imageData(new uint8_t[dataSize]);
		file.seekg(header.dataOffset, std::ios::beg); // Move to the beginning of image data
		file.read(reinterpret_cast<char*>(imageData.get()), dataSize);

		// Close the file
		file.close();

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

void ReadBmpImage::SaveImageToTxt(const std::string& fileName, uint8_t *data, int sizeData) const
{

	std::ofstream file(fileName);
	if (file.is_open())
	{
		for (int sData = 0; sData < sizeData; sData++)
		{
			file << static_cast<int>(data[sData]) << " ";
		}
		file.close();
		std::cout << "Image data saved to " << fileName << std::endl;
	}
	else {
		std::cerr << "Unable to open file for writing: " << fileName << std::endl;
	}
}

