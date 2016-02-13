#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "ComS229Util.h"
#include "sndcatUtil.h"

using namespace sndCatUtil;
/*NOTE on how I am handling specific cases.
1. If the Channels do not match, this is an error.
2. If the Samplerate is different, this is an error
3. If there is a conflict in BitRes, this is an error.
4. Sample size will be changed accordingly.*/
int main(int argc, char** argv) {
	ComS229 addingTo; //This is the first element. It will be the master
	ComS229 beingAdded; //This is the second element. It will be deleted after each read.
	bool output = false;
	bool hasOFile = false;
	string OFileName = "";
	string input;
	if (argc > 1) {
		if (argc == 2) {
			//This means there's only one argument. 
			//The only possibility is they want the help screen.
			char *CInput = new char[50];
			CInput = argv[1];
			if (CInput[0] == '-') {
				if (CInput[1] == 'h') {
					cout << "Enter the names of the files you would like to concatenate separated by spaces." << endl;
					cout << "./sndcat [-o yourfilenamehere.cs229] testing1.cs229 testing2.cs229" << endl;
					cout << "Or enter -h to see the help screen" << endl ; 
				}
				else {
					cout << "Sorry, this switch was not recognized. Please try again." << endl;
				}
			}
			else {
				cout << "Sorry, the input was not recognized. Please try again." << endl;
			}
		}
		if (argc > 2) {
			//This means there's atleast two files.
			try {
				vector<string> vec;
				for (int i = 1; i < argc; i++) {
					string input(argv[i]); //reading in user input.
					vec.push_back(input);
				}
				if ((signed int)vec.size() == 1) {
					//This means there's only one argument. 
					//The only possibility is they're specifying 
					//the output file or wanting the help screen.
					string input = vec[0];
					if (input[0] == '-') {
						if (input[1] == 'h') {
							cout << "Enter the names of the files you would like to concatenate separated by spaces." << endl;
							cout << "Or enter '-o yourfilenamehere.cs229' to specify an output file." << endl;
						}
						else {
							cout << "Sorry, this switch was not recognized. Please try again." << endl;
						}
					}
				}
				if ((signed int)vec.size() > 1) {
					//This means there's atleast two files.
					//Call the method to cat 2 strings and loop it.
					for (int j = 0; j < (signed int) vec.size(); j++) {
						if (vec[j].find("-o") != string::npos) //Saving the outputfile name.
						{
							j++;
							OFileName = vec[j];
							hasOFile = true;
						}
						else if (addingTo.getDataLength() == 0) { //This is reading the first file specified
							addingTo.readFile(vec[j]);
						}
						else if (addingTo.getDataLength() != 0) { //This is reading every other file specified
							if (beingAdded.readFile(vec[j])) {
								addingTo = sndCatUtil::concatenate(addingTo, beingAdded); //This will concatenate the two files
								if (addingTo.getBitRes() == 0) {
									return 0;
								}
								beingAdded.clean(); //removing all data from beingAdded since it will be rewritten if there's more than two files
								output = true;
							}
						}
						else {
							//cerr
						}
					}
					if (hasOFile) {
						addingTo.setFileName(OFileName); //This sets the output name
						sndCatUtil::output(addingTo);    //this outputs to that file
					}
					else if (output) {
						sndCatUtil::stdOutput(addingTo); //This will print to stdout
					}
					else {
						//error, you aren't supposed to output yet.
						cerr << "There was not output file specified and there was no file read";
					}

				}

			}
			catch (std::ios_base::failure e)
			{
				//Handle io exception here
				cerr << "IO EXCEPTION";
			}
		}
	}
	//delete created elements
	else {
		cerr << "No files specified.";
	}
	return 0;
}