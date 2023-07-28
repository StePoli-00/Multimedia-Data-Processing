#pragma once
#include <string>
#include "mat.h"
#include  "types.h"
#include "utils.h"
bool load_pcx(const std::string& filename, mat<uint8_t>& img);