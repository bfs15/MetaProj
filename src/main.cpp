
#include "main.hpp"

int main(int argc, char **argv)
{
	LIKWID_MARKER_INIT;

//	cout.precision(4);
//	cout << scientific;
	srand(time(NULL));

	int c;
	char* inFilename = NULL;
	char* outFilename = NULL;
	gm::redirectStreamToFile<std::istream, std::ifstream> inRedirect(std::cin);
	gm::redirectStreamToFile<std::ostream, std::ofstream> outRedirect(std::cout);

	bool a = false;
	bool v = false;
	size_t minI = 0, maxI = 16*1024;
	size_t value = minI - 1;

	while (( c = getopt(argc, argv, "i:o:av:")) != -1){
		switch (c){
			case 'i':
				inFilename = optarg;
				inRedirect.file(inFilename);
				break;
			case 'o':
				outFilename = optarg;
				outRedirect.file(outFilename);
				break;
			case 'a':	// Generic argument
				a = true;
				break;
			case 'v':
				v = true;
				value = std::stol(optarg);
				break;
			case ':':
			// missing option argument
				std::cerr << argv[0] << ": option '-"<< optopt <<"' requires an argument" << std::endl;
			default:
			args_err(argv);
		}
	}
	std::cout << gm::LogLvlMax <<" levels of logging\n";
	std::vector<gm::LogLvl> Lvls;
	for(int i = 0; i <= gm::LogLvlMax; i++){
		Lvls.push_back((gm::LogLvl)i);
	}

	/**/
	for(auto lvl : Lvls){
		std::cout <<"logger is now "<< lvl << "\n";
		gm::logger.setLvl(lvl);

		gm::logger.msg(gm::LogLvl::Fatal)
		<< "Fatal msg" << std::endl;

		gm::logger.msg(gm::LogLvl::Critical)
		<< "Critical msg" << std::endl;

		gm::logger.msg(gm::LogLvl::Error)
		<< "Error msg" << std::endl;

		gm::logger.msg(gm::LogLvl::Warn)
		<< "Warn msg" << std::endl;

		gm::logger.msg(gm::LogLvl::Note)
		<< "Note msg" << std::endl;

		gm::logger.msg(gm::LogLvl::Info)
		<< "Info msg" << std::endl;

		LogDEBUG(gm::logger, "Debug msg" << std::endl)

		std::cout << std::endl;
	}/**/

	std::string err_range_msg
	= "value out of range, enter "+ std::to_string(minI) +" < value < "+ std::to_string(maxI);

	if(v){
		if(!gm::inRange(value, minI, maxI)){
			std::cout << "-v "<< value <<" "<< err_range_msg;
			args_err(argv);
		}
	} else {
		if(inFilename == NULL)
			std::cout << "Enter a number: ";
		gm::readInRange(std::cin, value, minI, maxI, std::cout, err_range_msg);
		gm::clearln(std::cin);
	}
	inRedirect.close();


	std::string response;
	/**
	cout <<"What is your name?"<< "\n";
	readline(cin, response);
	cout <<"Hi "<< response <<"."<< "\n";
	/**/
	if(a){
		std::cout <<"Do you like pointers?"<< "\n";
		gm::readline(std::cin, response);
	}

	std::cout << "varray needs alignment of " << sizeof(gm::Vec<double>) << std::endl;

	std::cout << "a register has " << regSize(double) << " doubles" << std::endl;

	#ifndef STDVECTOR
		std::cout <<"Calculate varray of size "<< value << "\n";
		calculateVarray(value);
	#endif

	gm::Chronometer<10> timer;
	timer.start();
	size_t size = 8*4096*4096;
	size_t iterations = 128;
	for(size_t i = 0; i < iterations; ++i){
		numVector a(size);
		a[0] = 1;
		a[size-1] = 1;
	}

	std::cout << "alocating " << size << "; took " << timer.tick()/(double)iterations << std::endl;

	// Cleanup //

	LIKWID_MARKER_CLOSE;

	return 0;
}

void calculateVarray(size_t size){
	numVector A(size), B(size), X(size);
/**
	numVector A, B, X;

	A.alloc(size);
	B.alloc(size);
	X.alloc(size);
/**/

	gm::logger.msg(gm::LogLvl::Info)
	<< "reg_dn = " << regSize(double) << "\n"
	<< "dn = " << A.vecN() << "\n"
	<< "size = " << A.size() << "\n"
	<< "sizeV = " << A.sizeV() << "\n"
	<< "endVI = " << A.endVI() << "\n"
	<< "first Vec addr = " << &A.atV(0) << "\n"
	<< "second Vec addr = " << &A.atV(1) << std::endl;

	std::cout << "\n" << "Initializing arrays " << std::endl;

	for (size_t i = 0; i < A.size(); ++i) {
		A[i] = (rand() % 100 + 1);
		B[i] = (rand() % 100 + 1);
		X[i] = 0;
	}
{
	// loop from startI to endI
	size_t startI = 1;
	size_t endI = A.size() -1;
	size_t endVecIndex;
	size_t beginVI = A.loop(startI, endI, endVecIndex);

	std::cout << "\nExample of 3 ways of looping with vectorization "<< "\n"
	<<"startI = "<< startI <<" endI = "<< endI << std::endl;

	std::cout << "\n" <<"loop start before vectorization starts"<< "\n"
	<<"from = "<< startI <<" to = "<< A.beginVI(startI) << std::endl;

	std::cout << "\n" <<"loop with vectorization starts"<< "\n"
	<<"i = "<< A.beginVI(startI) <<"; i < "<< A.endVI(endI)*A.vecN() << "; ++i" << std::endl;

	std::cout << "\n" <<"loop remainder without vectorization"<< "\n"
	<<"from = "<< A.endVI(endI)*A.vecN() <<" to = "<< endI << "\n"
	<< "if 'from' > 'to' there is no looping" << std::endl;

	//for(size_t i = 0; i < A.vecN(); ++i)
	//	X.atV(vi).v[i] += A.atV(vi).v[i] * B.atV(vi).v[i];
/**/
// Way 1
	std::cout << "Way 1" << std::endl;

	gm_vectorized_loop_(A, 0,size-1,
		i,  {
			X[i] += A[i] * B[i];
		},
		vi,  {
			X.atV(vi) += A.atV(vi) * B.atV(vi);
		}
	)
/**/
// Way 2
	std::cout << "Way 2" << std::endl;

	for (size_t i = startI; i < beginVI; ++i) {
		X[i] += A[i] * B[i];
	}
	for (size_t vi = beginVI/A.vecN(); vi < endVecIndex; ++vi) {
		X.atV(vi) += A.atV(vi) * B.atV(vi);
	}
	for (size_t i = endVecIndex*A.vecN(); i <= endI; ++i) {
		X[i] += A[i] * B[i];
	}
/**/
// Way 3
	std::cout << "Way 3" << std::endl;

	for (size_t i = startI; i < A.beginVI(startI); ++i) {
		X[i] += A[i] * B[i];
	}
	for (size_t vi = A.beginVI(startI)/A.vecN();
	vi < A.endVI(endI); ++vi) {
		X.atV(vi) += A.atV(vi) * B.atV(vi);
	}
	for (size_t i = A.endVI(endI)*A.vecN(); i <= endI; ++i) {
		X[i] += A[i] * B[i];
	}
/**/
}
}

void args_err(char** argv){
	std::cerr << "Usage: "<< argv[0] <<" [-i inputFile] [-o outputFile] [-a (sets a = true)] [-v value]" << std::endl;
	exit(EXIT_FAILURE);
}
