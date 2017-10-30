#ifndef CHRONOMETER_HPP
#define CHRONOMETER_HPP

#include <sys/time.h>
#include <iostream>
#include <cstddef>

namespace gm
{


/**
 * @return returns current clock time in seconds 
 */
double tv_sec(timeval* tp){
	return ((tp->tv_sec + tp->tv_usec*1.0e-6));
}


template<size_t size>
class Chronometer
{
public:
	timeval clock[size];
	//clock_t n_clock[size];
	size_t c;
	
	Chronometer(){
		init();
	}
	
	void init(){
		gettimeofday(&clock[0], NULL);
		//n_clock[0] = std::clock();
		c = 0;
	}
	
	double tick(){
		c = (c + 1) % (ptrdiff_t)size;
		gettimeofday(&clock[c], NULL);
		//n_clock[c] = std::clock();
		
		timeval elapsed_tv;
		elapsed_tv.tv_sec = clock[c].tv_sec - clock[c-1].tv_sec;
		elapsed_tv.tv_usec = clock[c].tv_usec - clock[c-1].tv_usec;
		
		//*cl = ((double)(n_clock[c] - n_clock[c-1]) / CLOCKS_PER_SEC);
		return tv_sec(&elapsed_tv);
	}
	
	
};


}
#endif // CHRONOMETER_H
