/*
* File:   ComS229Util.cpp
* Author: Josh Engelhardt
*
* Created on November 11, 2015, 5:00 PM
*/
#include "ComS229Util.h"
#ifndef include 
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#endif

using namespace std;


/*Read header for discription of each method and variable.*/


ComS229::ComS229() { //Constructor for ComS229 Object.
	this->fileName = "";
	this->samples = 0;
	this->channels = 0;
	this->bitRes = 0;
	this->sampleRate = 0;
	this->hasSamples = false;
}
void ComS229::clean() {
	data.clear();
	this->fileName.clear();
	this->samples = 0;
	this->channels = 0;
	this->bitRes = 0;
	this->sampleRate = 0;
	this->hasSamples = false;
}

string ComS229::getFileName() {
	return this->fileName;
}
void ComS229::setFileName(string newName) {
	this->fileName = newName;
}
int ComS229::getSamples() {
	return this->samples;
}
void ComS229::addSamples(int toAdd) {

	this->samples = this->samples + toAdd;
}
int ComS229::getChannels() {
	return this->channels;
}
void ComS229::setChannels(int j) {
	this->channels = j;
}
int ComS229::getBitRes() {
	return this->bitRes;
}
void ComS229::setBitRes(int set) {
	if (set == 8 || set == 16 || set == 32) {
		this->bitRes = set;
	}
}
long int ComS229::getSampleRate() {
	return this->sampleRate;
}
void ComS229::setSampleRate(int set) {
	if (set > 0) {
		this->sampleRate = set;
	}
}
double ComS229::getLength() {
	//Total length in seconds    samples/samplerate
	return (double)this->samples / (double)this->sampleRate;
}
vector <long> ComS229::getData(int lineNum)
{
	return this->data[lineNum];
}
void ComS229::setData(vector < long > line)
{
	this->data.push_back(line);
}
void ComS229::setData(vector < long > line, int lineNum)
{
	this->data[lineNum] = line;
}
int ComS229::getDataLength() {

	return this->data.size();
}

/*This file is called in order to read in a file. I didn't include it as part of the constructor for 
  a very specific reason. This is because I couldn't return to the main function
  that an error occured during the reading.*/
bool ComS229::readFile(string file)
{
	string fileLine;
	string fileLine2;
	bool process = true;
	bool startData = false;
	this->fileName = file;
	ifstream cs229File;
	ifstream cs229Filecopy;
	cs229File.exceptions(cs229File.failbit | cs229File.badbit);
	try
	{
		cs229File.open(file.c_str());
		getline(cs229File, fileLine); //Collects the first line to make sure it starts with CS229 
	}
	catch (ifstream::failure e)
	{
		cerr << "Exception opening/reading file" << endl; //Makes sure there's no error reading first line
		return false;
	}
	if (cs229File.fail()) {
		cerr << "Exception opening/reading file" << endl;//Makes sure there's no error reading first line
		return false;
	}

	for (int i = 0; i < (signed int)fileLine.size(); i++) { //sets first line to lowercase
		fileLine[i] = tolower(fileLine[i]);
	}
	if (fileLine.find("cs229") != string::npos) {
		while (!startData && cs229File.eof() == false) { //Starts reading line by line until it hits the line startdata.
			try {
				getline(cs229File, fileLine);    //reads line
			}
			catch (std::ios_base::failure e) {
				cerr << "StartData was never found. Perhaps you had a comment behind it?";
				return false;
			}
			if (!fileLine.empty() && fileLine.find('#') == string::npos) { //If we have an empty line or a line starting with '#' we will skip that line.
				for (int i = 0; i < (signed int)fileLine.size(); i++) {
					fileLine[i] = tolower(fileLine[i]);
				}
			}
			else {
				if (fileLine.empty()) {   //If there's an empty line, skip
					process = false;
				}
				else if ((fileLine.at(0) != '#' || fileLine.at(1) != '#')) {//If a line has a comment, skip
					for (int i = 0; i < (signed int)fileLine.size(); i++) {  
						fileLine[i] = tolower(fileLine[i]);            //set line to lowercase
					}
				}
			}
			string::size_type j = fileLine.find("samples");
			if (fileLine.find("startdata") != string::npos && process) { //Checks if the line says, startdata
				startData = true;
				break;
			}
			else if (fileLine.find("samplerate") != string::npos && process)//Checks if the line says, startdata
			{
				int Result;
				if (!(istringstream(fileLine.substr(11)) >> Result)) { //give the value to 'Result' using the characters in the stream
					Result = 0;
				}
				else {
					this->sampleRate = Result;
				}
			}
			else if (fileLine.find("samples") != string::npos && process)//Checks if the line says, samples
			{
				int Result;
				if (!(istringstream(fileLine.substr(8)) >> Result)) { //give the value to 'Result' using the characters in the stream
					Result = 0;
				}
				else {
					this->samples = Result; //if the files read, then set samples and say it has samples
					this->hasSamples = true;
				}
			}
			else if (fileLine.find("channels") != string::npos && process)//Checks if the line says, channels
			{
				int Result;
				if (!(istringstream(fileLine.substr(9)) >> Result)) { //give the value to 'Result' using the characters in the stream
					Result = 0;
				}
				else {
					this->channels = Result;//if the line reads successfully, it saves the channels
				}
			}
			else if (fileLine.find("bitres") != string::npos && process) //Checks if the line says, bitres
			{
				int Result;
				if (!(istringstream(fileLine.substr(7)) >> Result)) { //give the value to 'Result' using the characters in the stream
					Result = 0;
				}
				else {
					this->bitRes = Result;//If the read is successful, save the value
				}
			}
			process = true;
		}
		while (startData) { //Here is where we start reading data.
			if (this->channels == 0) {
				//EXCEPTION channel size not given.
				cerr << "EXCEPTION Channels, BitRes, or SampleRate was not specified." << endl;
				return false;
			}
			if (this->bitRes == 0) {
				//EXCEPTION bitres not given.
				cerr << "EXCEPTION Channels, BitRes, or SampleRate was not specified." << endl;
				return false;
			}
			if (this->sampleRate == 0) {
				//EXCEPTION samplerate not given.
				cerr << "EXCEPTION Channels, BitRes, or SampleRate was not specified." << endl;
				return false;
			}
			int rows = 0;
			if (hasSamples) { //This will read the data if there was a specified number of samples
				for (int rows = 0; rows < samples; rows++) {
					vector<long> v;
					int columns = 0;
					for (columns = 0; columns < channels; columns++) {
						if (columns > this->channels)
						{
							//EXCEPTION channel size out of bounds.
							cerr << "EXCEPTION channel size out of bounds." << endl;
							return false;
						}
						try {
							cs229File >> fileLine;
						}
						catch (std::ios_base::failure e)
						{
							cerr << "Exception when reading file." << endl;
							return 0;
						}
						if (fileLine.find("#") == string::npos && process) {
							int Result;
							if (!(istringstream(fileLine) >> Result)) { //give the value to 'Result' using the characters in the stream
								Result = 0;
							}
							int j = Result;
							if (abs(j) < pow(2, bitRes - 1)) {
								v.push_back(j);
							}
							else {
								//EXCEPTION number being read in isn't within the bitres given.
								cerr << "EXCEPTION number being read in isn't within the bitres given." << endl;
								return false;
							}
						}
						else {
							getline(cs229File, fileLine2);
							columns--;
						}
					}
					if (columns == channels) {
						this->data.push_back(v);
					}
					else {
						cerr << "EXCEPTION: There were more channels than was specified by the header" << endl;
						return false;
					}
				}
				try
				{
					cs229File >> fileLine;

					if (fileLine.find('#') == string::npos) {
						cerr << "EXCEPTION: There was more data than was specified by the samples header" << endl;
						return false;
					}
				}
				catch (std::ios_base::failure e) {

				}
				startData = false;
			}
			else {//This means we weren't given a sample size
				while (cs229File.eof() == false) { //Loops through lines
					vector<long> v;
					for (int columns = 0; columns < channels; columns++) { //Loops through columns, reads a single row
						if (columns > this->channels)
						{
							//EXCEPTION channel size out of bounds.
							cerr << "EXCEPTION channel size out of bounds." << endl;
							return false;
						}
						try {
							cs229File >> fileLine; //Read a line
						}
						catch (std::ios_base::failure e)
						{
							//End of samples reached
							startData = false;
							break;
						}
						if (fileLine.find("#") == string::npos && process) { //If there's not a comment
							int Result;
							if (!(istringstream(fileLine) >> Result)) { //give the value to 'Result' using the characters in the stream
								Result = 0;
							}
							int j = Result;
							if (abs(j) < pow(2, bitRes - 1)) { //Makes sure the value is within the bit depth
								v.push_back(j);
							}
							else {
								//EXCEPTION number being read in isn't within the bitres given.
								cerr << "EXCEPTION number being read in isn't within the bitres given." << endl;
								return false;
							}
						}
						else {
							getline(cs229File, fileLine2); //Gets the next line
							columns--;
						}
					}
					if (startData == true) {
						this->data.push_back(v); //This means we're still in the data section. It will save the row we just got
						rows++;
					}
				}
				startData = false; //This means we've reached the end of the samples
			}
		}
		if (hasSamples) {
			if (samples != (signed int)this->data.size()) {
				//EXCEPTION we were given samples and it was shorter or longer than that.
				cerr << "EXCEPTION Samples given does not match samples read." << endl;
				return false;
			}
		}
		else {
			this->samples = this->data.size(); //This saves the number of samples if we we're given that.
		}
	}
	else {
		cerr << "The file you entered is not valid." << endl;
		//EXCEPTION Construct failed because the file didn't start with cs229
		return false;
	}
	cs229File.close();
	return true;  //The file was read successfully
}