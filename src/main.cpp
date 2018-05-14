
#include "main.hpp"

int main(int argc, char **argv)
{
	//LIKWID_MARKER_INIT;

//	cout.precision(4);
//	cout << scientific;
	srand(time(NULL));

	int c;
	char* inputFile = NULL;
	char* outputFile = NULL;
	std::ifstream in_f;
	std::ofstream o_f;
	std::streambuf* coutbuf = std::cout.rdbuf(); //save old buf;
	bool a = false;
	bool v = false;
	size_t minI = 0, maxI = 16*1024;
	size_t value = minI - 1;

	while (( c = getopt(argc, argv, "i:o:av:")) != -1){
		switch (c){
			case 'i':
				inputFile = optarg;
				in_f.open(inputFile);
				std::cin.rdbuf(in_f.rdbuf()); //redirect
				break;
			case 'o':
				outputFile = optarg;
				o_f.open(outputFile);
				std::cout.rdbuf(o_f.rdbuf()); //redirect
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
				LogM(gm::LogLvl::Error, argv[0] << ": option '-"<< optopt <<"' requires an argument\n");
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
		gm::logger.logLvlSet(lvl);
		LogM(gm::LogLvl::Fatal, "Fatal msg\n");
		LogM(gm::LogLvl::Critical, "Critical msg\n");
		LogM(gm::LogLvl::Error, "Error msg\n");
		LogM(gm::LogLvl::Warn, "Warn msg\n");
		LogM(gm::LogLvl::Note, "Note msg\n");
		LogM(gm::LogLvl::Info, "Info msg\n");
		LogDEBUG("Debug msg\n");
		std::cout << std::endl;
	}/**/

	std::string err_range_msg
	= "value out of range, enter "+ std::to_string(minI) +" < value < "+ std::to_string(maxI);

	if(v){
		if(!gm::inRange(value, minI, maxI)){
			LogM(gm::LogLvl::Error, "-v "<< value <<" "<< err_range_msg);
			args_err(argv);
		}
	} else {
		if(inputFile == NULL)
			std::cout << "Enter a number: ";
		gm::readInRange(std::cin, value, minI, maxI, std::cout, err_range_msg);
		gm::clearln(std::cin);
	}
	in_f.close();


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

	std::cout <<"Calculate varray of size "<< value << "\n";
	calculateVarray(value);

	gm::Chronometer<10> timer;
	timer.start();
	size_t size = 8*4096*4096;
	size_t iterations = 128;
	for(size_t i = 0; i < iterations; ++i){
		gm::varray<double> a(size);
		a[0] = 1;
		a[size-1] = 1;
	}

	std::cout << "alocating " << size << "; took " << timer.tick()/(double)iterations << std::endl;

	// Cleanup //

	//LIKWID_MARKER_CLOSE;

	std::cout.rdbuf(coutbuf); //redirect

	return 0;
}

void calculateVarray(size_t size){
	gm::varray<double> A(size), B(size), X(size);
/**
	varray<double> A, B, X;

	A.alloc(size);
	B.alloc(size);
	X.alloc(size);
/**/

	gm::logger << "hey " << "\n";


	gm::logger << "hey " << std::endl;
	gm::logger << std::endl;

	LogM(gm::LogLvl::Info,
	   "reg_dn = " << regSize(double) << "\n"
	<< "dn = " << A.vecN() << "\n"
	<< "size = " << A.size() << "\n"
	<< "sizeV = " << A.sizeV() << "\n"
	<< "endVI = " << A.endVI() << "\n"
	<< "first Vec addr = " << &A.atv(0) << "\n"
	<< "second Vec addr = " << &A.atv(1) << std::endl
	);

	LogM(gm::LogLvl::Info, "\n" <<"Initializing varrays "<< "\n");

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

	LogM(gm::LogLvl::Info, "\nExample of 3 ways of looping with vectorization "<< "\n"
	<<"startI = "<< startI <<" endI = "<< endI << "\n");

	LogM(gm::LogLvl::Info, "\n" <<"loop start before vectorization starts"<< "\n"
	<<"from = "<< startI <<" to = "<< A.beginVI(startI) << "\n");

	LogM(gm::LogLvl::Info, "\n" <<"loop with vectorization starts"<< "\n"
	<<"i = "<< A.beginVI(startI) <<"; i < "<< A.endVI(endI)*A.vecN() << "; ++i" << "\n");

	LogM(gm::LogLvl::Info, "\n" <<"loop remainder without vectorization"<< "\n"
	<<"from = "<< A.endVI(endI)*A.vecN() <<" to = "<< endI << "\n"
	<< "if 'from' > 'to' there is no looping" << "\n");

	//for(size_t i = 0; i < A.vecN(); ++i)
	//	X.atv(vi).v[i] += A.atv(vi).v[i] * B.atv(vi).v[i];
/**/
// Way 1
	LogM(gm::LogLvl::Info, "Way 1 \n");

	vectorized_loop(A, 0,size-1,
		i,  {
			X[i] += A[i] * B[i];
		},
		vi,  {
			X.atv(vi).v += A.atv(vi).v * B.atv(vi).v;
		}
	)
/**/
// Way 2
	LogM(gm::LogLvl::Info, "Way 2 \n");

	for (size_t i = startI; i < beginVI; ++i) {
		X[i] += A[i] * B[i];
	}
	for (size_t vi = beginVI/A.vecN(); vi < endVecIndex; ++vi) {
		X.atv(vi).v += A.atv(vi).v * B.atv(vi).v;
	}
	for (size_t i = endVecIndex*A.vecN(); i <= endI; ++i) {
		X[i] += A[i] * B[i];
	}
/**/
// Way 3
	LogM(gm::LogLvl::Info, "Way 3 \n");

	for (size_t i = startI; i < A.beginVI(startI); ++i) {
		X[i] += A[i] * B[i];
	}
	for (size_t vi = A.beginVI(startI)/A.vecN();
	vi < A.endVI(endI); ++vi) {
		X.atv(vi).v += A.atv(vi).v * B.atv(vi).v;
	}
	for (size_t i = A.endVI(endI)*A.vecN(); i <= endI; ++i) {
		X[i] += A[i] * B[i];
	}
/**/
}
}

void args_err(char** argv){
	LogM(gm::LogLvl::Error, "Usage: "<< argv[0] <<" [-i inputFile] [-o outputFile] [-a (sets a = true)] [-v value]\n");
	exit(EXIT_FAILURE);
}
