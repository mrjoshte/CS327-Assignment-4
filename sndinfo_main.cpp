/*
* File:   sndinfo_main.cpp
* Author: Josh Engelhardt
*
* Created on November 5, 2015, 3:35 PM
*/
#include "ComS229Util.h"
#include <unistd.h>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
using namespace std;
/*
* This program reads all sound files passed as arguments, and for each one, displays the
following.
- The file name
- The file type (.cs229 or optionally for bonus points .wav)
- The sample rate
- The bit depth
- The number of channels
- The number of samples
- The length of the sound (in seconds)
If no files are passed as arguments, then the program should read from standard input. The
only required switch for this program is -h.
*/
int main(int argc, char** argv) {
	if (argc == 1) { //User hasn't entered a file name
		string userin;
		ofstream inputFile;
		if (isatty(fileno(stdin))) //stdin is a terminal
		{
			cerr << "No file was specified" << endl;
			return 0;
		}
		else
		//stdin is a file or a pipe
		inputFile.open("inputFileFromPipe327.cs229");
		//Here we are creating a temporary file which contains the input from the pipe.
		//The next few lines are writing that to a file.
		while (getline(cin, userin)){
			inputFile << userin << "\n";
		}
		inputFile.close();
		ComS229 file;
		//This then actually reads the file that we just wrote to.
		bool result = file.readFile("inputFileFromPipe327.cs229");
		//deletes the temp file
		remove("inputFileFromPipe327.cs229");
		//Then we output.
		if (result)
		{
			cout << "File name wasn't specified.";
			cout << "The SampleRate is: ";
			cout << file.getSampleRate() << endl;
			cout << "The BitRes is: ";
			cout << file.getBitRes() << endl;
			cout << "The number of channels are: ";
			cout <<  file.getChannels() << endl;
			cout << "The number of samples are: ";
			cout << file.getSamples() << endl;
			cout << "The length of the file in seconds is is: ";
			cout << file.getLength() << endl;
		}
	}
	else if (argc == 2) {
		//There is only one element written to the command line. It's either a file, or -h.
		char *identify = (char*)malloc(sizeof(char) * 50);
		identify = argv[1];
		string consoleIn(identify);
		if (identify[0] == '-' && identify[1] == 'h') {
			//Print help screen here.
			cout << "Pipe in a file name. e.g. './sndinfo < testing.cs229'" << endl;
			cout << "\nOtherwise you can write a filename to the command line." << endl;
		}
		else {
			ComS229 file;
			bool result = file.readFile(consoleIn);
			if (result)
			{
				cout << "The file name is: " << endl;
				cout << file.getFileName() << endl;
				cout << "The SampleRate is: ";
				cout << file.getSampleRate() << endl;
				cout << "The BitRes is: ";
				cout << file.getBitRes() << endl;
				cout << "The number of channels are: ";
				cout << file.getChannels() << endl;
				cout << "The number of samples are: ";
				cout << file.getSamples() << endl;
				cout << "The length of the file in seconds is is: ";
				cout << file.getLength() << endl;
			}
		}
	}
	else {
		cerr << "I/O error. You entered too many arguments";
		return 0;
	}
	return 0;
}