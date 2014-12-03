//============================================================================
// Name        : KLogo.cpp
// Author      : Piero Toffanin
// Version     :
// Copyright   : 2011
// Description : Project1 for Compiler Design Spring 2011
//============================================================================

#include <iostream>
#include <string>
#include <stdlib.h>
using namespace std;

#include "Driver.hh"
#include "RuntimeException.hh"

void usage(){
	cout << "Usage: ./klogo <inputfile>" << endl;
	cout << endl;
	cout << "inputfile: Path to the file to be read by klogo" << endl;

	exit(EXIT_SUCCESS);
}

int main(int argc, char **argv) {
	if (argc < 2){
		usage();
	}

	cout << "Begin parsing of " << argv[1] << "... " << endl;

	Driver driver;
	if (!driver.Parse (argv[1])){
	      if (driver.result != NULL){
	    	  if (!driver.ParseErrorFound()){
				  if (driver.result->checkForSemanticErrors() == SemanticCheckSuccess){
					  //driver.result->print(cerr);

					  cout << "Done!" << endl;
					  cout << "-----------" << endl;

					  try{
						  driver.result->execute();

						  cout << endl << "-----------" << endl << "Goodbye!" << endl;
					  }catch(RuntimeException& e){
						  cout << endl << "Program terminated with runtime exception: " << e.what() << endl;
					  }

				  }
	    	  }
	      }
	}

	return EXIT_SUCCESS;
}
