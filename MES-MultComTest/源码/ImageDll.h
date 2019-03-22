#pragma once
#include "stdafx.h"
#include<string>
#include"export.h"

EXPORT_API  float ImageDefinition(const std::string ImageSrc);
EXPORT_API  float ImageDarkCorner(const std::string  ImageSrc, const int RGBRange);
