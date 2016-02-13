/*
* File:   ComS229Util.h
* Author: Josh Engelhardt
*
* Created on November 11, 2015, 5:50 PM
*/

#ifndef include
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#endif

using namespace std;

/*This class is what my program considers to be a .cs229 file. It contains variables
  for everything contained within a cs229 file. It also contains methods to get and set
  those variables. The most imporant method from this is the readFile method. This method
  willl read in the cs229 file and save the data.*/
class ComS229 {
	string fileName;
	int samples;
	int channels;
	int bitRes;
	long int sampleRate;
	/*This is the MOST imporant variable of the whole program. 
	It contains all the data for a cs229 file.*/
	vector< vector<long> > data; 
	bool hasSamples;
public:
	ComS229(); //Constructor. This will initialize the variables to 0.
	void clean(); //This will reset all the variables to 0 and remove the data.
	
		/*This file is called in order to read in a file. I didn't include it as part of the constructor for 
		a very specific reason. This is because I couldn't return to the main function
		that an error occured during the reading.*/
		
	bool readFile(string); //Reads the cs229 file and calls other functions to save variables.
	/*These here are the getters.*/
	string getFileName();
	int getSamples();
	int getChannels();
	int getBitRes();
	long int getSampleRate();
	double getLength(); //Get's the length of the cs229 file in seconds
	vector < long, std::allocator<long> > getData(int); //Gets a specified sample of data
	int getDataLength(); //returns the size of the data.
	

	/*These here are all the setters*/
	void addSamples(int); //This will add the specified number to the samples variable
	void setData(vector < long, std::allocator<long> >); //This will push a sample to the end of the data variable.
	void setData(vector < long, std::allocator<long> >, int); //This will push a sample to the specified line of the data variable.
	void setFileName(string);
	void setSampleRate(int);
	void setBitRes(int);
	void setChannels(int);
};