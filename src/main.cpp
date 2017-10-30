
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
	size_t min = 0, max = 16*1024;
	size_t value = min - 1;

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
		cout <<"logger is now "<< lvl << endl;
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
	= "value out of range, enter "+ to_string(min) +" < value < "+ to_string(max);
	
	if(v){
		if(!inRange(value, min, max)){
			LogM(Level::Error, "-v "<< value <<" "<< err_range_msg);
			args_err(argv);
		}
	} else {
		if(inputFile == NULL)
			cout << "Enter a number: ";
		readInRange(cin, value, min, max, cout, err_range_msg);
		clearln(cin);
	}
	in_f.close();
	
	
	string response;
	/**
	cout <<"What is your name?"<< endl;
	readline(cin, response);
	cout <<"Hi "<< response <<"."<< endl;
	/**/
	if(a){
		cout <<"Do you like pointers?"<< endl;
		readline(cin, response);
	}
	
	cout <<"Calculate varray of size "<< value << endl;
	calculateVarray(value);
	
	// Cleanup //
	
	//LIKWID_MARKER_CLOSE;
	
	cout.rdbuf(coutbuf); //redirect
	
	return 0;
}

void calculateVarray(size_t size){
	varray<double> A, B, X;
	A.alloc_size(size);
	B.alloc_size(size);
	X.alloc_size(size);
	
	LogM(Level::Information,
	   "\nA.regEN() = "<< A.regEN() << "\n"
	<< "A.size() = "<< A.size() << "\n"
	<< "A.mSizeVecMem = "<< A.mSizeVecMem << "\n"
	<< "A.endVec() = "<< A.endVec() << "\n"
	<< "\n");
	
	for (size_t i = 0; i < A.size(); i++) {
		A[i] = (rand() % 100 + 1);
		B[i] = (rand() % 100 + 1);
		X[i] = -(rand() % 100 + 1);
	}
	
	// loop with vectorization
	for (size_t iv = 0; iv < A.sizeVec(); iv++) {
		X.atv(iv).v += A.atv(iv).v * B.atv(iv).v;
	}
	// loop what's left
	// from A.end_v() to (size-1)
	for (size_t i = A.vecEnd(); i < A.size(); i++) {
		X[i] += A[i] * B[i];
	}
	
	printv(X);
}

void args_err(char** argv){
	LogM(Level::Error, "Usage: "<< argv[0] <<" [-i inputFile] [-o outputFile] [-a (sets a = true)] [-v value]\n");
	exit(EXIT_FAILURE);
}