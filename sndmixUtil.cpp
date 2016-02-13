#include "ComS229Util.h"

namespace sndmixUtil {

	ComS229 applymultiplier(double multiplier, int startLine, int endLine, ComS229 applying) {
		//Go through from startLine to endLine and multiply them by multiplier.
		for (int i = startLine; i < endLine; i++) {
			//make a for loop that goes through each row
			vector < long > vec = applying.getData(i);
			for (int j = 0; j < applying.getChannels(); j++) {
				//Then make another for that goes through each channel and times it by the multiplier.
				double temp = vec[j] * multiplier; //This multiplies the data, and saves it to a temp
				if (abs(temp) >= pow(2,applying.getBitRes()-1)) { //There's an error with the bit depth
					cerr << "The value: ";
					cerr << temp;
					cerr << " exceeds the specified bit depth." << endl;
					ComS229 null;
					return null;
				}
				else{ vec[j] = (int)round(temp); } //Sets the data back to the same variable
			}
			applying.setData(vec, i); //Sets the sample created back to the line where it was.
		}
		return applying; //Returns the newly created ComS229 object
	}

	ComS229 adding(ComS229 addingTo, ComS229 beingAdded)
	{
		if (addingTo.getSamples() >= beingAdded.getSamples()) {
			if (beingAdded.getBitRes() != addingTo.getBitRes()) { //If the samples match or addingTo is larger. We will do the adding here 
				cerr << "The bit depth of your files are not equal. The program will now exit." << endl;
				ComS229 null;
				return null;
			}
			for (int i = 0; i < addingTo.getSamples(); i++) {
				vector <long> addTo = addingTo.getData(i);
				if (i < beingAdded.getDataLength()) {
					vector <long> beingAdd = beingAdded.getData(i);
					for (int j = 0; j < addingTo.getChannels(); j++) {
						addTo[j] = beingAdd[j] + addTo[j];
						if (abs(addTo[j]) >= pow(2, addingTo.getBitRes() - 1)) {
							cerr << "The value: ";
							cerr << addTo[j];
							cerr << " exceeds the specified bit depth." << endl;
							ComS229 null;
							return null;
						}
					}
					addingTo.setData(addTo, i);
				}
				else {
					return addingTo;
				}
			}
			return addingTo;
		}
		else{ //This is if the samples do not match where beingAdded is larger.
			if (addingTo.getBitRes() != beingAdded.getBitRes()) {
				cerr << "The bit depth of your files are not equal. The program will now exit.";
				ComS229 null;
				return null;
			}
			for (int i = 0; i < beingAdded.getSamples(); i++) { 
				vector <long> beingAdd = beingAdded.getData(i); //Gets the line to be added to addingTo.
				if (i < addingTo.getDataLength()) {
					vector <long> addTo = addingTo.getData(i); 
					for (int j = 0; j < beingAdded.getChannels(); j++) {
						beingAdd[j] = beingAdd[j] + addTo[j]; //This adds them and sets it to beingAdded
						if (abs(beingAdd[j]) >= pow(2, (beingAdded.getBitRes() - 1))) {
							cerr << "The value: ";
							cerr << beingAdd[j];
							cerr << " exceeds the specified bit depth." << endl;
							ComS229 null;
							return null;
						}
					}
					beingAdded.setData(beingAdd, i);
				}
				else {
					return beingAdded; //We saved all the addition to beingAdded since it was the longer file.
				}
			}
			return beingAdded;
		}
	}

}
