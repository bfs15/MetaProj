/**
@mainpage

Description
*/

#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <ctgmath>
//#include <likwid.h>
#include <unistd.h>
#include <limits>
#include <cstddef>

#include "varray.hpp"
//#include "Matrix.hpp"
#include "Chronometer.hpp"

#include "io.hpp"
#include "Logger.hpp"

using namespace std;
using namespace gm;

int main(int argc, char** argv);

void args_err(char** argv);

void calculateVarray(size_t size);