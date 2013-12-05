/*
	Name: Karl Anthony James
	Class: Operating Systems
	Due: 12/5/2013
	CacheMemory
	A program to simulate a cache memory server.
*/

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <queue>
#include <algorithm>

using namespace std;

struct Exception{
	Exception(string message){
		cout << endl << message << endl;
	}
};

struct CachePage{
	int address;
	int references;
};

bool compare(const CachePage& page1, const CachePage& page2){
	return (page1.references > page2.references);
}

void eval(istream& input, ostream& output){
	CachePage pages[20];
	int requestCount = 0;
	queue<int> pageRequests;
	string inputString;
	while (getline(input, inputString)){
		pageRequests.push(atoi(inputString.c_str()));
	}

	for (int i = 0; i <= 19; i++){
		CachePage page;
		page.address = 0;
		page.references = 0;
		pages[i] = page;
	}

	for (;;){
		requestCount++;
		int request = pageRequests.front();
		bool spotFound = false;
		for (int i = 0; i <= 19; i++){
			if (pages[i].address == request){
				pages[i].references++;
				spotFound = true;
				break;
			}
		}

		if (!spotFound){
			int oldAddress = request;
			for (int i = 0; i <= 19; i++){
				if (pages[i].address > 0){
					int swap = oldAddress;
					oldAddress = pages[i].address;
					pages[i].address = swap;
				}
				else{ //This is a truly empty spot, we can insert and stop
					pages[i].address = oldAddress;
					pages[i].references = 1;
					break;
				}
			}
		}

		pageRequests.pop();

		if (requestCount % 20 == 0){
			for (int i = 0; i <= 19; i++){
				if (pages[i].references > 0){
					pages[i].references--;
				}
			}
		}

		std::stable_sort(pages, pages + 20, compare);
		
		if (requestCount <= 10 || pageRequests.size() < 10 || requestCount % 20 == 0){
			output << endl;
			output << "page: " << request << endl;
			output << std::left << setw(10) << "slot" << setw(10) << "disk" << endl;
			output << std::left << setw(10) << "number" << setw(10) << "address" << setw(10) << "value" << endl;
			
			for (int i = 0; i < sizeof(pages) / sizeof(CachePage); i++){
				output << std::right << setw(6) << i + 1 << setw(10) << (pages[i].address < 0 ? 0 : pages[i].address) << setw(10) << (pages[i].references < 0 ? 0 : pages[i].references) << endl;
			}
			output << endl;
		}

		if (pageRequests.empty())
			break;
	}
}

int main(int argc, char* argv[]){
	try{
		std::string path;
		std::string outputPath;
		if(argc <= 1){
			path = "cache.in";
		}
		else{
			path = argv[1];
		}

		if(argc > 2){
			outputPath = argv[2];
		}
		else{
			outputPath = "cache.out";
		}

		ifstream inputFile(path.c_str());
		if(!inputFile)
			throw Exception("The tm file " + path + " could not be found");

		ofstream outputFile(outputPath.c_str());
		if(!outputFile)
			throw Exception("Could not open output file " + outputPath + ".");

		eval(inputFile, outputFile);

		inputFile.close();
		outputFile.close();
	}
	catch(...){
		cout << endl << "An unexpected error occured. Ending program." << endl;
		exit(EXIT_FAILURE);
	}

	return 0;
}