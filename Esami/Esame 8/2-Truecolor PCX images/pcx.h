#pragma once
#include "mat.h"
#include "pgm.h"
#include "types.h"
#include "utils.h"
#include <fstream>
bool load_pcx(const std::string& filename, mat<vec3b>& img);
