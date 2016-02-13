using namespace std;
	/*This class contains a generation object. The generation object has everything
	needed in order to create a sound file. It is used for generation of 
	the waves, generation of the adsr, and also holding the user input to
	the sndgen program.*/
class generation {
	/*These are all saved from the user*/
	bool hasSampleRate;
	int frequency;
	int time;
	double peak;
	double pulse;
	double attack;
	double decay;
	double sustain;
	double release;
	string waveType;
	/*The modified attack samples is used when the attack needs to be shortened. This way the program
	will keep the same slope as the specified attack, but stop where the release starts.*/
	int modifiedAttackSamples;
public:
	generation(); //Constructor. This initializes everything.
	ComS229 creation;  //This is the ComS229 object that the sound file will save to.

	/*Getters and setters*/
	bool ifHasSampleRate();
	void setSampleRateBool(bool);

	int getFrequency();
	bool setFrequency(int);

	int getTime();
	bool setTime(int);


	double getPeak();
	bool setPeak(double);

	double getPulse();
	bool setPulse(double);

	double getAttack();
	bool setAttack(double);

	double getDecay();
	bool setDecay(double);

	double getSustain();
	bool setSustain(double);

	double getRelease();
	bool setRelease(double);

	string getWaveType();
	bool setWaveType(string);

	//This will generate the wave specified by the user. It will also check to make sure the 
	//attack, decay, ect are correct in order to print. This will return true if the adsr is applied successfully.
	bool generateWave();
	//Applies the adsr to the wave. It is only called within generateWave and will work for all wave types.
	bool adsrEnv();
	//This will fill the ComS229 file with zeros to make sure it has enough space.
	void generateSamples(int);
};