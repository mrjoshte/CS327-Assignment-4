#ifndef include
#include "ComS229Util.h"
#include "sndgen.h"
#include <math.h>
#endif

using namespace std;
/*This class contains a generation object. The generation object has everything
	needed in order to create a sound file. It is used for generation of 
	the waves, generation of the adsr, and also holding the user input to
	the sndgen program.*/
generation::generation() {
	this->hasSampleRate = false;
	this->frequency = 0;
	this->time = 0;
	this->peak = 1;
	this->attack = 0;
	this->decay = 0;
	this->sustain = 0;
	this->modifiedAttackSamples = 0;
	this->release = 0;
	this->waveType = "";
	this->creation.setChannels(1);
}

bool generation::ifHasSampleRate() {
	return this->hasSampleRate;
}
void generation::setSampleRateBool(bool hasRate) {
	this->hasSampleRate = hasRate;
}

int generation::getFrequency() {
	return this->frequency;
}
bool generation::setFrequency(int givenFrequency){
		this->frequency = givenFrequency;
		return true;
}

int generation::getTime(){
	return this->time;
}
bool generation::setTime(int time) {
	if (time > 0) {
		this->time = time;
		return true;
	}
	else {
		return false;
	}
}
double generation::getPeak(){
	return this->peak;
}
bool generation::setPeak(double givenPeak){
	if (0 <= givenPeak && givenPeak <= 1) {
		this->peak = givenPeak;
		return true;
	}
	else {
		return false;
	}
}
double generation::getPulse(){
	return this->pulse;
}
bool generation::setPulse(double givenPulse){
	if (0 <= givenPulse && givenPulse <= 1) {
		this->pulse = givenPulse;
		return true;
	}
	else {
		return false;
	}
}

double generation::getAttack(){
	return this->attack;
}
bool generation::setAttack(double givenAttack){
	if (givenAttack < getTime()) {
		this->attack = givenAttack;
		return true;
	}
	else {
		return false;
	}
}

double generation::getDecay(){
	return this->decay;
}
bool generation::setDecay(double givenDecay){
	if (givenDecay < getTime()) {
		this->decay = givenDecay;
		return true;
	}
	else {
		return false;
	}
}

double generation::getSustain(){
	return this->sustain;
}
bool generation::setSustain(double givenSustain){
	if (givenSustain < getTime()) {
		this->sustain = givenSustain;
		return true;
	}
	else {
		return false;
	}
}

double generation::getRelease(){
	return this->release;
}
bool generation::setRelease(double givenRelease){
	if (givenRelease < getTime()) {
		this->release = givenRelease;
		return true;
	}
	else {
		return false;
	}
}

string generation::getWaveType(){
	return this->waveType;
}
bool generation::setWaveType(string waveType){
	if (waveType.compare("sine") == 0 || waveType.compare("triangle") == 0 || waveType.compare("sawtooth") == 0 || waveType.compare("pulse") == 0) {
		this->waveType = waveType;
		return true;
	}
	else {
		return false;
	}
}


/*This method will create the wave that is specified by the user. The ADSR will not be applied during this time. 
The first section of this will actually be used to determine if the adsr will need to be shorted.
At the end of each time a wave is devolped, I will call the adsrEnv() function. This will work for every wave form.*/
bool generation::generateWave() {
	if (this->getFrequency() == 0) { //This means the user didn't enter a frequency and the program will quit. 
		return false;
	}
	if (this->getTime() <= 0) {
		return false;
	}
	if (getAttack() + getDecay() + getRelease() >= getTime()) { //This means that something needs to get shortened.
		if (getRelease() >= getTime()) { //This indicates that the release time is longer than the actual time and that just doesn't fly. This will exit the program.
			return false;
		}
		else if(getAttack() + getRelease() >= getTime()) {
			//Remove the appropriate amount of time from attack
			//This modified attack time will be used to determine how many samples will be used in attack.
			//I need to keep the attack that the user specified so I know the slope of the attack.
			modifiedAttackSamples = (int)round(creation.getSampleRate()*(getTime() - getRelease()));
			setDecay(0);
		}
		else {
			//This will shorten the decay accordingly.
			setDecay(getTime() - (getAttack() + getRelease()));
		}
	}
	if (this->getWaveType().compare("sine") == 0) {
		//Creates the sine wave!
		for (float i = 0; i < creation.getSamples(); i++) {
			vector <long> insert;
			insert.push_back((long)round(sin((i / (double)creation.getSampleRate()) * 2 * 3.14159 * this->getFrequency())*(pow(2, (double)creation.getBitRes() - 1) - 1))*getPeak());
			creation.setData(insert, i);
		}
		
		return adsrEnv();
	}
	else if (this->getWaveType().compare("triangle") == 0) {
		//Creates the Triangle wave!
		double amplitude = pow(2, creation.getBitRes() - 1) - 1;
		double period =creation.getSampleRate() / (double)getFrequency() ;
		for (int i = 0; i < creation.getSamples(); i++) {
			vector <long> insert;
			long y = ((-1)*(4 * amplitude)/period)*(abs(fmod(i, period) - (period/2))-(period/4));
			insert.push_back(y);
			creation.setData(insert, i);
		}
		return adsrEnv();

	}
	else if (this->getWaveType().compare("sawtooth") == 0) {
		//Creates the sawtooth wave!
		double period = creation.getSampleRate() / (double)getFrequency();
		double amplitude = pow(2, creation.getBitRes() - 1) - 1;
		double multiplier = amplitude*2/period;
		double toUse = (-1)*(pow(2, creation.getBitRes() - 1) - 1);
		for (float i = 0; i < creation.getSamples(); i++) {
			vector <long> insert; 
			if (toUse >= (pow(2, creation.getBitRes() - 1) - 1)-1) {
				toUse = (-1)*(pow(2, creation.getBitRes() - 1) - 1);
			}
			long y = (toUse);
			insert.push_back(y);
			creation.setData(insert, i);
			toUse = toUse + multiplier;
		}
		return adsrEnv();
	}
	else if (this->getWaveType().compare("pulse") == 0) {
		//Create the pulse wave!
		double period = creation.getSampleRate() / (double)getFrequency();
		double multiplier = (-1);
		double toUse = (pow(2, creation.getBitRes() - 1) - 1);
		int periodCounter = 0;
		for (float i = 0; i < creation.getSamples(); i++) {
			vector <long> insert;
			if (periodCounter == period*getPulse()) {
				toUse = toUse * multiplier;
			}
			if (periodCounter == period) {
				toUse = toUse * multiplier;
				periodCounter = 0;
			}
			insert.push_back(toUse);

			creation.setData(insert, i);
			periodCounter++;
		}
		return adsrEnv();
	}
	else
		return false;
}

/*This will be used to apply the adsr to all the waves above. There are lots of extra conditional statements because of the special case exceptions for adsr.*/
bool generation::adsrEnv() {
	int attackSamples = (int)round(creation.getSampleRate()*getAttack()); //Number of attack samples given by the user.
	if (modifiedAttackSamples == 0) {
		modifiedAttackSamples = (int)round(creation.getSampleRate()*getAttack()); //this is the modified attack samples if they need to be shortened.
	}
	int decaySamples = (int)round(creation.getSampleRate()*getDecay()); //Self explanitory
	int releaseSamples = (int)round(creation.getSampleRate()*getRelease());
	int sustainSamples = creation.getSamples() - modifiedAttackSamples - decaySamples - releaseSamples;

	if (modifiedAttackSamples + decaySamples + sustainSamples + releaseSamples != creation.getDataLength()) {
		//This is just an extra check in case all the samples don't add up.
		return false;
	}
	//So the multipliers used here are not really multipliers. 
	//I start with an inital number to start the attack, then it will add the multiplier after every time.
	//So essentially what happens is that at the end of the attack, you will be at the peak.
	double const attackMultiplier = getPeak() / attackSamples;
	double toUseAttack = attackMultiplier;
	int attackCount = 0;
	//the modifiedAttackSamples is used here in case the attack was shortened.
	//This way you still use the rate of increase for the attack the user defined, but it will be cut short.
	for (attackCount = 0; attackCount < modifiedAttackSamples; attackCount++) {
		vector <long> toChange;
		toChange.push_back(creation.getData(attackCount)[0] * toUseAttack);
		toUseAttack += attackMultiplier;
		creation.setData(toChange, attackCount);
	}
	//See the comments above about the attack, the same principles apply, but in reverse. 
	double const decayMultiplier = (getPeak() - getSustain()) / decaySamples;
	double toUseDecay = getPeak() - decayMultiplier;
	int decayCount = attackCount;
	for (decayCount = attackCount; decayCount < modifiedAttackSamples + decaySamples; decayCount++) {
		vector <long> toChange;
		toChange.push_back(creation.getData(decayCount)[0] * toUseDecay);
		toUseDecay -= decayMultiplier;
		creation.setData(toChange, decayCount);
	}
	//Sustain is straightforward, if there is no sustain, it will simply skip over this loop.
	int sustainCount = decayCount;
	for (sustainCount = decayCount; sustainCount < modifiedAttackSamples + decaySamples + sustainSamples; sustainCount++) {
		vector <long> toChange;
		toChange.push_back(creation.getData(sustainCount)[0] * getSustain());
		creation.setData(toChange, sustainCount);
	}
	double releaseMultiplier = 0;
	double toUseRelease = 0;
	//So this group of if/elses is to determine where the release starts.
	if (sustainSamples == 0) {
		if (decaySamples == 0) {
			//This is if release starts at the end of attack
			releaseMultiplier = toUseAttack / releaseSamples;
			toUseRelease = toUseAttack - releaseMultiplier;
		}
		else {
			//This is if release starts at the end of decay
			releaseMultiplier = toUseDecay / releaseSamples;
			toUseRelease = toUseDecay - releaseMultiplier;
		}
	}
	else {
		//And this is if release starts at the end of sustain.
		releaseMultiplier = getSustain() / releaseSamples;
		toUseRelease = getSustain() - releaseMultiplier;
	}
	//Then it's the same as decay.
	int releaseCount = sustainCount;
	for (releaseCount = sustainCount; releaseCount < creation.getDataLength(); releaseCount++) {
		vector <long> toChange;
		toChange.push_back(creation.getData(releaseCount)[0] * toUseRelease);
		toUseRelease -= releaseMultiplier;
		creation.setData(toChange, releaseCount);
	}
	//We know that the adsr was applied successfully, so we return true to tell main that we completed the program.
	return true;
}

void generation::generateSamples(int samples) {
	//This will go through and fill up ComS229 data with 0's.
	for (int i = 0; i < samples; i++) {
		vector <long> vect;
		vect.push_back(0);
		creation.setData(vect);
	}
}