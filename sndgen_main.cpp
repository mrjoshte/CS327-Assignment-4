#ifndef include
#include "ComS229Util.h"
#include "sndgen.h"
#endif
using namespace std;

int main(int argc, char** argv) {
	bool go = true;
	string input;
	generation creationFile;
	vector <string> userIn;
	if (argc > 1) {
		for (int i = 1; i < argc; i++) {
			string input(argv[i]);
			userIn.push_back(input);
		}
	}
	else{
		cerr << "No commmand line parameters." << endl;
		return 0;
	}
	while (go) {
		for (int i = 0; i < (signed int)userIn.size(); i++) {
			if (userIn[i].find("-h") != string::npos || userIn[i].find("-H") != string::npos) { //This is the help screen.
				cout << "Enter switches to generate a sound file. See the 'READ ME' file to determine which switches are valid." << endl;
				cout << "Every sound file must contain a bit depth, sample rate, " << endl;
			}
			else if (userIn[i].find("-o") != string::npos || userIn[i].find("-O") != string::npos)//This is specifying the output file.
			{
				i++;
				creationFile.creation.setFileName(userIn[i]);
			}
			else if (userIn[i].find("--bits") != string::npos) //This is specifying bit depth
			{
				i++;
				if (userIn[i].find("8") != string::npos || userIn[i].find("16") != string::npos || userIn[i].find("32") != string::npos) {
					int depth = 0;
					if (!(istringstream(userIn[i]) >> depth)) {
						depth = 0;
					}
					if (depth == 0) {
						cerr << "The specified bit depth was not valid." << endl;
						return 0;
					}

					creationFile.creation.setBitRes(depth);
				}
				else {
					cerr << "The specified bit depth was not valid." << endl;
					return 0;
				}
			}
			else if (userIn[i].find("--sr") != string::npos) //This is specifying sample rate
			{
				i++;
				int sampleR = 0;
				if (!(istringstream(userIn[i]) >> sampleR)) {
					sampleR = 0;
				}
				if (sampleR > 0) {
					creationFile.creation.setSampleRate(sampleR);
					creationFile.setSampleRateBool(true);
				}
				else {
					cerr << "The specified sample rate was not valid." << endl;
					return 0;
				}
			}
			else if (userIn[i].find("-f") != string::npos || userIn[i].find("-F") != string::npos)//This is specifying frequency.
			{
				i++;
				int frequency = 0;
				if (!(istringstream(userIn[i]) >> frequency)) { //give the value to 'Result' using the characters in the stream
					frequency = 0;
				}
				if (frequency > 0) {
					creationFile.setFrequency(frequency);
				}
				else {
					cerr << "The specified frequency was not valid." << endl;
					return 0;
				}
			}
			else if (userIn[i].find("--triangle") != string::npos) //This will create a triangle wave
			{
				creationFile.setWaveType("triangle");
			}
			else if (userIn[i].find("-t") != string::npos || userIn[i].find("-T") != string::npos)//This is specifying length.
			{
				i++;
				if (creationFile.ifHasSampleRate()) {
					double length = 0.0;

					if (!(istringstream(userIn[i]) >> length)) { //give the value to 'Result' using the characters in the stream
						length = 0.0;
					}

					if (length > 0) {
						creationFile.setTime(length);
						creationFile.generateSamples((int)floor(creationFile.creation.getSampleRate()*creationFile.getTime()));
						creationFile.creation.addSamples((int)floor(creationFile.creation.getSampleRate()*creationFile.getTime()));
					}
					else {
						cerr << "The length of time was invalid.";
						return 0;
					}
				}
				else {
					cerr << "The length of time was specified before the sample rate.";
					return 0;
				}
			}
			else if (userIn[i].find("-v") != string::npos || userIn[i].find("-V") != string::npos)//This is specifying peak volume.
			{
				i++;
				double peak = 0.0;
				if (!(istringstream(userIn[i]) >> peak)) { //give the value to 'Result' using the characters in the stream
					peak = 0.0;
				}
				if (!creationFile.setPeak(peak)) {
					cerr << "The given peak is invalid";
					return 0;
				}
			}
			else if (userIn[i].find("-a") != string::npos || userIn[i].find("-A") != string::npos) //This is specifying attack time.
			{
				i++;
				double attack = 0.0;
				if (!(istringstream(userIn[i]) >> attack)) { //give the value to 'Result' using the characters in the stream
					attack = 0.0;
				}
				if (!creationFile.setAttack(attack)) {
					cerr << "The specifed attack time was not valid";
					return 0;
				}
			}
			else if (userIn[i].find("-d") != string::npos || userIn[i].find("-D") != string::npos) //This is specifying decay time. 
			{
				i++;
				double decay = 0.0;
				if (!(istringstream(userIn[i]) >> decay)) { //give the value to 'Result' using the characters in the stream
					decay = 0.0;
				}
				if (!creationFile.setDecay(decay)) {
					cerr << "The given decay time was not valid";
					return 0;
				}
			}

			else if (userIn[i].find("--sine") != string::npos) //This will create a sine wave
			{
				creationFile.setWaveType("sine"); //= sndgen::generateSine(creation, frequency);
			}
			else if (userIn[i].find("--sawtooth") != string::npos) //This will create a sawtooth wave
			{
				creationFile.setWaveType("sawtooth");
			}
			else if (userIn[i].find("-s") != string::npos || userIn[i].find("-S") != string::npos) //This is specifying sustain volume.
			{
				i++;
				double sustain = 0.0;
				if (!(istringstream(userIn[i]) >> sustain)) { //give the value to 'Result' using the characters in the stream
					sustain = 0.0;
				}
				if (!creationFile.setSustain(sustain)) {
					cerr << "The specified sustain time was not valid.";
					return 0;
				}
			}
			else if (userIn[i].find("-r") != string::npos || userIn[i].find("-R") != string::npos) //This is specifying release time in seconds.
			{
				i++;
				double release = 0.0;
				if (!(istringstream(userIn[i]) >> release)) { //give the value to 'Result' using the characters in the stream
					release = 0.0;
				}
				if (!creationFile.setRelease(release)) {
					cerr << "The specified release time was not valid.";
					return 0;
				}
			}
			else if (userIn[i].find("--pulse") != string::npos) //This will create a pulse wave
			{
				creationFile.setWaveType("pulse");
			}
			else if (userIn[i].find("--pf") != string::npos) //This will specify the up time of the pulse wave
			{
				i++;
				double pulse = 0.0;
				if (!(istringstream(userIn[i]) >> pulse)) { //give the value to 'Result' using the characters in the stream
					pulse = 0.0;
				}
				if (!creationFile.setPulse(pulse)) {
					cerr << "The specified pulse was not valid.";
					return 0;
				}
			}
			else {
				cerr << "The switch: ";
				cerr << userIn[i];
				cerr << " was not defined." << endl;
				return 0;
			}
		}
		//THIS IS IMPORTANT. This is where the wave is created.
		//If this returns false, That means the wave was not generated, otherwise it will go to output.
		if (!creationFile.generateWave()) {
			cout << "\nThe wave was not generated." << endl;
		}
		else {
			if (creationFile.creation.getFileName() != "") { //Output to a file
				ofstream outputFile;
				outputFile.open(creationFile.creation.getFileName().c_str());
				outputFile << "CS229" << endl;
				outputFile << "Samples ";
				outputFile << creationFile.creation.getSamples() << endl;
				outputFile << "Channels ";
				outputFile << creationFile.creation.getChannels() << endl;
				outputFile << "BitRes ";
				outputFile << creationFile.creation.getBitRes() << endl;
				outputFile << "Samplerate ";
				outputFile << creationFile.creation.getSampleRate() << endl;
				outputFile << endl;
				outputFile << "Startdata" << endl;
				outputFile << endl;
				for (int i = 0; i < creationFile.creation.getDataLength(); i++) {
					vector <long> temp = creationFile.creation.getData(i);
					for (int i = 0; i < creationFile.creation.getChannels(); i++) {
						outputFile << temp[i];
						outputFile << " ";
					}
					outputFile << endl;
				}
				outputFile.close();
				return 0;
			}
			else { //output to stdout
				cout << "CS229" << endl;
				cout << "Samples ";
				cout << creationFile.creation.getSamples() << endl;
				cout << "Channels ";
				cout << creationFile.creation.getChannels() << endl;
				cout << "BitRes ";
				cout << creationFile.creation.getBitRes() << endl;
				cout << "Samplerate ";
				cout << creationFile.creation.getSampleRate() << endl;
				cout << endl;
				cout << "Startdata" << endl;
				cout << endl;
				for (int i = 0; i < creationFile.creation.getDataLength(); i++) {
					vector <long> temp = creationFile.creation.getData(i);
					for (int i = 0; i < creationFile.creation.getChannels(); i++) {
						cout << temp[i];
						cout << " ";
					}
					cout << endl;
				}
			}
		}

	}
	return 0;
}