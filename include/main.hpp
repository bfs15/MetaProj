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
#include <unistd.h>
#include <limits>
#include <cstddef>

#ifdef LIKWID_PERFMON
	#include <likwid.h>
#else
	#define LIKWID_MARKER_INIT
	#define LIKWID_MARKER_THREADINIT
	#define LIKWID_MARKER_SWITCH
	#define LIKWID_MARKER_REGISTER(regionTag)
	#define LIKWID_MARKER_START(regionTag)
	#define LIKWID_MARKER_STOP(regionTag)
	#define LIKWID_MARKER_CLOSE
	#define LIKWID_MARKER_GET(regionTag, nevents, events, time, count)
#endif

#include "varray.hpp"
//#include "Matrix.hpp"
#include "Chronometer.hpp"

#include "io.hpp"
#include "Logger.hpp"



int main(int argc, char** argv);

void args_err(char** argv);

void calculateVarray(size_t size);
