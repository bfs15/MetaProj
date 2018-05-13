
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
	ifstream in_f;
	ofstream o_f;
	streambuf* coutbuf = cout.rdbuf(); //save old buf;
	bool a = false;
	bool v = false;
	size_t minI = 0, maxI = 16*1024;
	size_t value = minI - 1;

	while (( c = getopt(argc, argv, "i:o:av:")) != -1){
		switch (c){
			case 'i':
				inputFile = optarg;
				in_f.open(inputFile);
				cin.rdbuf(in_f.rdbuf()); //redirect
				break;
			case 'o':
				outputFile = optarg;
				o_f.open(outputFile);
				cout.rdbuf(o_f.rdbuf()); //redirect
				break;
			case 'a':	// Generic argument
				a = true;
				break;
			case 'v':
				v = true;
				value = stol(optarg);
				break;
			case ':':
			// missing option argument
				LogM(Level::Error, argv[0] << ": option '-"<< optopt <<"' requires an argument\n");
			default:
			args_err(argv);
		}
	}
	cout << LevelMax <<" levels of logging\n";
	vector<Level> Lvls;
	for(int i = 0; i <= LevelMax; i++){
		Lvls.push_back((Level)i);
	}

	/**/
	for(auto lvl : Lvls){
		cout <<"logger is now "<< lvl << "\n";
		logger.setLevel(lvl);
		LogM(Level::Fatal, "Fatal msg\n");
		LogM(Level::Critical, "Crit msg\n");
		LogM(Level::Error, "Error msg\n");
		LogM(Level::Warning, "Warning msg\n");
		LogM(Level::Notification, "Notification msg\n");
		LogM(Level::Information, "Information msg\n");
		LogDEBUG("Debug msg\n");
		cout << endl;
	}/**/

	string err_range_msg
	= "value out of range, enter "+ to_string(minI) +" < value < "+ to_string(maxI);

	if(v){
		if(!inRange(value, minI, maxI)){
			LogM(Level::Error, "-v "<< value <<" "<< err_range_msg);
			args_err(argv);
		}
	} else {
		if(inputFile == NULL)
			cout << "Enter a number: ";
		readInRange(cin, value, minI, maxI, cout, err_range_msg);
		clearln(cin);
	}
	in_f.close();


	string response;
	/**
	cout <<"What is your name?"<< "\n";
	readline(cin, response);
	cout <<"Hi "<< response <<"."<< "\n";
	/**/
	if(a){
		cout <<"Do you like pointers?"<< "\n";
		readline(cin, response);
	}

	cout << "varray need alignment of " << sizeof(Vec<double>) << endl;

	cout << "register has " << regSize(Vec<double>) << " doubles" << endl;

	cout <<"Calculate varray of size "<< value << "\n";
	calculateVarray(value);


	// Cleanup //

	//LIKWID_MARKER_CLOSE;

	cout.rdbuf(coutbuf); //redirect

	return 0;
}

void calculateVarray(size_t size){
	varray<double> A(size), B(size), X(size);
/**
	varray<double> A, B, X;

	A.alloc(size);
	B.alloc(size);
	X.alloc(size);
/**/
	LogM(Level::Information,
	   "reg_dn = " << regSize(double) << "\n"
	<< "dn = " << A.vecN() << "\n"
	<< "size = " << A.size() << "\n"
	<< "sizeV = " << A.sizeV() << "\n"
	<< "endVI = " << A.endVI() << "\n"
	<< "first elem addr = " << &A[0] << "\n"
	<< "first elem addr = " << &A.atv(0) << "\n"
	<< "second Vec addr = " << &A.atv(1) << "\n"
	);

	LogM(Level::Information, "\n" <<"Initializing varrays "<< "\n");

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

	LogM(Level::Information, "\nExample of 3 ways of looping with vectorization "<< "\n"
	<<"startI = "<< startI <<" endI = "<< endI << "\n");

	LogM(Level::Information, "\n" <<"loop start before vectorization starts"<< "\n"
	<<"from = "<< startI <<" to = "<< A.beginVI(startI) << "\n");

	LogM(Level::Information, "\n" <<"loop with vectorization starts"<< "\n"
	<<"i = "<< A.beginVI(startI) <<"; i < "<< A.endVI(endI)*A.vecN() << "; ++i" << "\n");

	LogM(Level::Information, "\n" <<"loop remainder without vectorization"<< "\n"
	<<"from = "<< A.endVI(endI)*A.vecN() <<" to = "<< endI << "\n"
	<< "if 'from' > 'to' there is no looping" << "\n");

// Way 1
	LogM(Level::Information, "Way 1 \n");

	vectorized_loop(A, i, startI, endI, {
		X[i] += A[i] * B[i];
	})

// Way 2
	/**/
	for (size_t i = startI; i < beginVI; ++i) {
		X[i] += A[i] * B[i];
	}
	/**
	for (size_t vi = beginVI/A.vecN(); vi < endVecIndex; ++vi) {
		X.atv(vi).v += A.atv(vi).v * B.atv(vi).v;
	}
	/**/
	for (size_t i = endVecIndex*A.vecN(); i <= endI; ++i) {
		X[i] += A[i] * B[i];
	}

// Way 3

	for (size_t i = startI; i < A.beginVI(startI); ++i) {
		X[i] += A[i] * B[i];
	}
	/**/
	for (size_t vi = A.beginVI(startI)/A.vecN();
	vi < A.endVI(endI); ++vi) {
		cout << &X.atv(vi).v << endl;
		cout << &X.atv(vi) << endl;
		//for(size_t i = 0; i < A.vecN(); ++i)
		//	X.atv(vi).v[i] += A.atv(vi).v[i] * B.atv(vi).v[i];
		X.atv(vi).v += A.atv(vi).v * B.atv(vi).v;
	}
	/**/
	for (size_t i = A.endVI(endI)*A.vecN(); i <= endI; ++i) {
		X[i] += A[i] * B[i];
	}
	/**/
}
}

void args_err(char** argv){
	LogM(Level::Error, "Usage: "<< argv[0] <<" [-i inputFile] [-o outputFile] [-a (sets a = true)] [-v value]\n");
	exit(EXIT_FAILURE);
}
