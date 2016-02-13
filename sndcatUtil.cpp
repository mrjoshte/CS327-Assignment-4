#include "ComS229Util.h"
#include "sndcatUtil.h"

namespace sndCatUtil {

	/*NOTE on how I am handling specific cases.
	1. If the Channels do not match, this is an error.
	2. If the Samplerate is different, this is an error
	3. If there is a conflict in BitRes, the largest number will be new the BitRes.
	4. Sample size will be changed accordingly.*/
	ComS229 concatenate(ComS229 addingTo, ComS229 beingAdded)
	{
		if (addingTo.getChannels() != beingAdded.getChannels()) { //If the Channels do not match, this is an error.
			cerr << "The files you're trying to concatenate doesn't have the same number of channels" << endl;
			ComS229 null;
			return null;
		}
		else if (addingTo.getSampleRate() != beingAdded.getSampleRate()) { //If the Samplerate is different, this is an error
			cerr << "The files you're trying to concatenate doesn't have the same SampleRate" << endl;
			ComS229 null;
			return null;
		}
		if (addingTo.getBitRes() != beingAdded.getBitRes()) { //If there is a conflict in BitRes, the largest number will be new the BitRes.
			cerr << "The files you're trying to concatenate doesn't have the same BitRes" << endl;
			ComS229 null;
			return null;
		}
		for (int i = 0; i < beingAdded.getDataLength(); i++) { //Adding the second file to the first.
			addingTo.setData(beingAdded.getData(i));
		}
		addingTo.addSamples(beingAdded.getSamples());
		return addingTo;
	}
	/*This will output to a specific filename*/
	void output(ComS229 output)
	{
		ofstream outputFile;
		outputFile.open(output.getFileName().c_str());
		outputFile << "CS229" << endl;
		outputFile << "Samples ";
		outputFile << output.getSamples() << endl;
		outputFile << "Channels ";
		outputFile << output.getChannels() << endl;
		outputFile << "BitRes ";
		outputFile << output.getBitRes() << endl;
		outputFile << "Samplerate ";
		outputFile << output.getSampleRate() << endl;
		outputFile << endl;
		outputFile << "Startdata" << endl;
		outputFile << endl;
		for (int i = 0; i < output.getDataLength(); i++) { //Loops through the data to output
			vector <long> temp = output.getData(i);
			for (int i = 0; i < output.getChannels(); i++) {
				outputFile << temp[i];
				outputFile << " "; //Spaces between channels
			}
			outputFile << endl; //new line for samples
		}
		outputFile.close();
	}
	/*This will output to standard output*/
	void stdOutput(ComS229 output)
	{
		cout << "CS229" << endl;
		cout << "Samples ";
		cout << output.getSamples() << endl;
		cout << "Channels ";
		cout << output.getChannels() << endl;
		cout << "BitRes ";
		cout << output.getBitRes() << endl;
		cout << "Samplerate ";
		cout << output.getSampleRate() << endl;
		cout << endl;
		cout << "Startdata" << endl;
		cout << endl;
		for (int i = 0; i < output.getDataLength(); i++) { //loops to output to the standard output
			vector <long> temp = output.getData(i);
			for (int i = 0; i < output.getChannels(); i++) {
				cout << temp[i];
				cout << " "; //spaces between channels
			}
			cout << endl; //new line between samples
		}
	}

}