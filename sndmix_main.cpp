#ifndef include
#include "ComS229Util.h"
#include "sndcatUtil.h"
#include "sndmixUtil.h"
#endif

using namespace std;

int main(int argc, char** argv) {
	ComS229 addingTo;
	ComS229 beingAdded;
	bool output = false;
	bool hasOFile = false;
	string OFileName = "";
	string input;
	if (argc > 1) {
		char *CInput = new char[50];
		CInput = argv[1];
		if (argc == 2) {
			//This means there's only one argument and the only possibility is they want the help screen.
			CInput = argv[1];
			if (CInput[0] == '-' && CInput[1] == 'h') {
				cout << "Enter the names of the files you would like to mix separated by spaces." << endl;
				cout << "Optionally you may also enter an output file." << endl;
				cout << "The format should be: 'sndmix [-o outputfilename.cs229] mult[multipler] myfile.cs229 ... mult[multipler] mylastfile.cs229'" << endl;
			}
			else {
				cout << "Sorry, the input was not recognized. Please try again." << endl;
			}
		}
		else if (argc > 2) {
			//This means there's atleast two parameters.
			try {
				vector<string> vec;
				for (int i = 1; i < argc; i++) { //Grabs the command line input
					string input(argv[i]);
					vec.push_back(input);
				}
				if ((signed int)vec.size() > 1) {
					//This means there's atleast two files.
					//Call the method to cat 2 strings and loop it.
					int i = 0;
					if (vec[i].find("-o") != string::npos) //Saves the output file
					{
						i++;
						OFileName = vec[i];
						hasOFile = true;
						i++;
					}
					for (int j =i; j < (signed int)vec.size(); j++) {
						int multiplier = 1;
						if (vec[j].find("mult") != string::npos && addingTo.getDataLength() == 0 && vec[j + 1].find(".cs229") != string::npos) { //Saving multipler
							if (!(istringstream(vec[j].substr(4, vec[j].length())) >> multiplier)) { //give the value to 'multiplier' using the characters in the stream
								multiplier = 0;
							}
							if (multiplier == 0 && vec[j].substr(4, vec[j].length()).find("0") == string::npos) { //If the multiplier is zero or wasn't read in correct
								cerr << "You've entered an incorrect multiplier." << endl;
								return 0;
							}
							if (-10 > multiplier || multiplier > 10)//If the multiplier is out of range
							{
								break;
							}
							if (addingTo.getDataLength() == 0) { //this is reading the first file given
								j++;
								if (vec[j].find(".cs229") != string::npos) {
									if (addingTo.readFile(vec[j])) { //Read the file
									//apply the multiplier to it.
										addingTo = sndmixUtil::applymultiplier((double)multiplier, 0, addingTo.getDataLength(), addingTo);
										if (addingTo.getDataLength() == 0) { //This means something wasn't valid within applyingmultiplier
											return 0;
										}
									}
									else {
										break;
									}
								}
								else {
									cerr << "File name was not valid or command line was not valid.";
									return 0;
								}
							}
						}
						else if (vec[j].find("mult") != string::npos && addingTo.getDataLength() != 0 && vec[j + 1].find(".cs229") != string::npos) {
							//Reads in the multiplier
							if (!(istringstream(vec[j].substr(4, vec[j].length())) >> multiplier)) { //give the value to 'multiplier' using the characters in the stream
								multiplier = 0;
							}
							if (multiplier == 0 && vec[j].substr(4, vec[j].length()).find("0") == string::npos) {
								cerr << "You've entered an incorrect multiplier." << endl; //If the multiplier is zero or wasn't read in correct
								return 0;
							}
							if (-10 > multiplier || multiplier > 10)
							{
								cerr << "You've entered an incorrect multiplier." << endl;//If the multiplier is out of range
								return 0;
							}
							j++;
							if (beingAdded.readFile(vec[j])) { //reads the next file
								//Applies the multiplier to the other file
								beingAdded = sndmixUtil::applymultiplier((double)multiplier, 0, beingAdded.getDataLength(), beingAdded);
								if (beingAdded.getBitRes() == 0) { //Something went wrong applying the multiplier
									return 0;
								}
								addingTo = sndmixUtil::adding(addingTo, beingAdded); //Adds the two files together
								beingAdded.clean(); //erases the data from beingAddded
								if (addingTo.getBitRes() == 0) { //Something went wrong during the adding process
									return 0;
								}
								//Then add line where this file is applied to the addingTo file.
							}
							else {
								break;
							}
						}

						else {
							cerr << "File name was not valid or command line was not valid.";
							return 0;
						}
					}

					if (hasOFile) {
						addingTo.setFileName(OFileName);
						sndCatUtil::output(addingTo);
						//output method from sndmix if there is a file given
					}
					else if (output) {
						sndCatUtil::stdOutput(addingTo);
						//output method from sndmix if there isn't a file given
					}
					else {
						//error, you aren't supposed to output yet.
						cerr << "Exception. You've entered in incorrect parameters" << endl;
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
	else {
		cerr << "There was no file or switches read" << endl;
	}
	return 0;
}