#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdint>
#include "RenderStructures.h"



struct LoadBMP
{
	BitmapFileHeaders fileHeader;
	BitmapInfoHeader infoHeader;
	BitmapColorHeader colorHeader;

	std::vector<uint8_t> data;
	std::vector<uint8_t> colorDataRGB;

	

	LoadBMP(const char* fileName) {
		ReadFile(fileName);
	}

	void ReadFile(const char* fileName);

	 
	void CreateBitmapImage(int32_t width, int32_t height, bool hasAlpha = true);
	//void Write(const char *fileName);

	void RGBConversion();


	uint32_t rowStride;
	uint32_t MakeStrideAligned(uint32_t alignStride) {
		uint32_t newStride = rowStride;
		while (newStride % alignStride != 0)
		{
			newStride++;
		}
		return newStride;
	}
	void CheckColorHeader(BitmapColorHeader& colorHeader);

};