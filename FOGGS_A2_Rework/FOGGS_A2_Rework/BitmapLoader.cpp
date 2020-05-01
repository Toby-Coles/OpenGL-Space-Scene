#include "BitmapLoader.h"

void LoadBMP::ReadFile(const char* fileName)
{
	std::ifstream image{ fileName, std::ios_base::binary };
	if (image)
	{
		// Check file type is bitmap
		image.read((char*)&fileHeader, sizeof(fileHeader));
		if (fileHeader.fileType != 0x4D42) {
			throw std::runtime_error("Error: Incorrect file format");
		}
		
		//Read the image file
		image.read((char*)&infoHeader, sizeof(infoHeader));

		//Check the bit mask information is suitable for image processing
		if (infoHeader.bitCount == 32)
		{
			if (infoHeader.size >= (sizeof(BitmapInfoHeader) + sizeof(BitmapColorHeader)))
			{
				image.read((char*)&colorHeader, sizeof(colorHeader));
				CheckColorHeader(colorHeader);
			}
			else
			{
				std::cerr << "Warning! The file \"" << fileName << "\" does not contain bit mask information\n";
				throw std::runtime_error("Error: Unrecognized file format.");
			}
		}

		//Jump to pixel data location
		image.seekg(fileHeader.offsetData, image.beg);

		if (infoHeader.bitCount == 32) {
			infoHeader.size = sizeof(BitmapInfoHeader) + sizeof(BitmapColorHeader);
			fileHeader.offsetData = sizeof(BitmapFileHeaders) + sizeof(BitmapInfoHeader) + sizeof(BitmapColorHeader);
		}
		else {
			infoHeader.size = sizeof(BitmapInfoHeader);
			fileHeader.offsetData = sizeof(BitmapFileHeaders) + sizeof(BitmapInfoHeader);
			
		}
			fileHeader.fileSize = fileHeader.offsetData;
		
			if (infoHeader.height < 0) {
			 throw std::runtime_error("The program can treat only BMP images with the origin in the bottom left corner!");
			}
		
			data.resize(infoHeader.width * infoHeader.height * infoHeader.bitCount / 8);
		//Checking the necesity of row padding

			if (infoHeader.width % 4 == 0) {
				image.read((char*)data.data(), data.size());
				fileHeader.fileSize += data.size();
				
			}
			else {
				rowStride = infoHeader.width * infoHeader.bitCount / 8;
				uint32_t newStride = MakeStrideAligned(4);
				std::vector<uint8_t> paddingRow(newStride - rowStride);
				
					for (int y = 0; y < infoHeader.height; ++y) {
						image.read((char*)(data.data() + rowStride * y), rowStride);
						image.read((char*)paddingRow.data(), paddingRow.size());
					
					}
				fileHeader.fileSize += data.size() + infoHeader.height * paddingRow.size();
			
			}
			
	}
	else 
	{
		 throw std::runtime_error("Unable to open the input image file.");
	}
}
//Constructs an image from the read in data 
void LoadBMP::CreateBitmapImage(int32_t width, int32_t height, bool hasAlpha)
{
	if (width <= 0 || height <=0)
	{
	throw std::runtime_error("Error: Incorrect height and width values");
	}

	infoHeader.width = width;
	infoHeader.height = height;
	if (hasAlpha)
	{
		infoHeader.size = sizeof(BitmapInfoHeader) + sizeof(BitmapColorHeader);
		fileHeader.offsetData = sizeof(BitmapFileHeaders) + sizeof(BitmapInfoHeader) + sizeof(BitmapColorHeader);

		infoHeader.bitCount = 32;
		infoHeader.compression = 3;
		rowStride = width * 4;
		data.resize(rowStride * height);
		fileHeader.fileSize = fileHeader.offsetData + data.size();

	}

	else
	{
		infoHeader.bitCount = 24;
		infoHeader.compression = 0;
		rowStride = width * 3;
		data.resize(rowStride * height);
		uint32_t newSride = MakeStrideAligned(4);
		fileHeader.fileSize = fileHeader.offsetData + data.size() + infoHeader.height * (newSride - rowStride);
	}

}


//Convert the data to the correct usable RGB format
void LoadBMP::RGBConversion() {
	colorDataRGB.reserve(data.size());
	uint32_t channels = infoHeader.bitCount / 8;
	
	for (uint32_t t = 0; t < infoHeader.height; t++)
	{
		for (uint32_t z = 0; z < infoHeader.width; ++z)
		{
			int index = channels * (t * infoHeader.width + z);
			colorDataRGB.emplace_back(data[index + 0]); //B
			colorDataRGB.emplace_back(data[index + 1]); //G
			colorDataRGB.emplace_back(data[index + 2]); //R
		}
	}
}


void LoadBMP::CheckColorHeader(BitmapColorHeader& colorHeader)
{
	//Check the format of the stored BMP data (wether its BGRA and if the color space type is sRGB)
	BitmapColorHeader expectedColorHeader;
	if (expectedColorHeader.redMask != colorHeader.redMask ||
		expectedColorHeader.blueMask != colorHeader.blueMask ||
		expectedColorHeader.greenMask != colorHeader.greenMask ||
		expectedColorHeader.alphaMask != colorHeader.alphaMask)
	{
		throw std::runtime_error("Incorrect color mask format: Ecpected BGRA");
	}

	if (expectedColorHeader.colorSpaceType != colorHeader.colorSpaceType)
	{
		throw std::runtime_error("Incorrect color space type: Expected sRGB");
	}
}
	
