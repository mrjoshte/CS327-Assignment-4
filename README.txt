Josh Engelhardt
mrjoshte@iastate.edu
ComS 327
12/11/2015

Please email me if there are problems.
No extra credit programs are contained here.
Included should be all files and their discriptions.
-------------------------------------------------------------------------
There are four main functions for this project. I tried to put the least
amount of code in there as possible. However, there were some things that
didn't seem worth making a whole method for, or that I needed to parse the
user input.
-----------------------------------------------------------------------
-ComS229Util.cpp
-ComS229Util.h

First is the ComS229Util class. This class is the hinge of all the other 
files and classes. This object is what I consider the sound file.
The purpse of it is to hold all the information from a .cs229 file.
#It contains variables for everything in a cs229 file.
	The name of the file, the number of samples, the number of channels, 
	the bit depth, the sample rate, weather or not the file has samples given,
	and lastly it has all the data contained in a vector < vector <long> >.

#Methods for this class include 
All the getters and setters
A clean function which will completely erase the contents of the file.
	This is used within sndcat when you may be contatinating  more than one file.
A setData function which will push a sample to the back of the list
	Also a setData override which will push a sample to a specific line.
Lastly and most importantly is the readFile function. This file is called 
	in order to read in a file. I didn't include it as part of the constructor for 
	a very specific reason. This is because I couldn't return to the main function
	that an error occured during the reading.
-----------------------------------------------------------------
-sndinfo_main.cpp
	:::Uses ComS229Util:::
All this class really does is parse the user input and then outputs.
The main thing for this class is to use a ComS229 object, then call the read
	function which will parse the file specified.

-----------------------------------------------------------------
-sndcat_main.cpp
	:::Uses sndcatUtil:::

This class just parses the user input, makes checks, then decides how to apply the functions contained in
the sndcatUtil.cpp file.	

-sndcatUtil.ccp
-sndcatUtil.h
So these aren't actually classes. It's a namespace. When starting the project my impression of namespaces
	was that they were similar to classes.

#Methods include
	concatenate: This is the main function of this whole executible.
		It will make checks and concatenate the sound files.
	stdOutput
		This will output to the standard output when the user didn't
		specify a file name. 
	output
		This will output to a specific file.
		
	NOTE ON HOW I AM HANDLING SPECIFIC CASES.
	1. If the Channels do not match, this is an error.
	2. If the Samplerate is different, this is an error
	3. If there is a conflict in BitRes, this is an error.
	4. Sample size will be changed accordingly.

-----------------------------------------------------------------
-sndmix_main.cpp
:::Uses ComS229Util, sndcatUtil, sndMixUtil:::

sndcatUtil is only used for the output functions

#################################FIRST THE INPUT##########################################
	I think I interpreted this a little bit incorrect, but never the less..
	when inputting to the program it should be like this example:

	
	./sndmix -o output.cs229 mult5 firstFile.cs229 mult2 secondFile.cs229
	or
	./sndmix mult5 firstFile.cs229 mult2 secondFile.cs229

	
	When specifing the multiplier you MUST use mult[number here]
#########################################################################################
	
The sndmix function is pretty basic. It just parses the user input, contains variables for what its going to print, makes sure the input is valid ect.
output is handled by the sndcat fucntion, and then otherwise it calls functions from the sndmixUtil.
	
-sndmixUtil.cpp
-sndmixUtil.h

sndmixUtil is also a namespace. It contains functions for sndmix_main.

#Methods include
	applymultiplier: This will just apply the multiplier  specified	by 
		the user to one ComS229 object.
	adding: This will add two ComS229 obejcts together, then return as a new ComS229
		object. 

----------------
-sndgen_main.cpp
	
	This class is a little hard to read, but what it does is parse the user input,
	use functions from sndgen to save the user intput, call functions from sndgen
	to create the wave specified, then output. It also makes checks to make sure it's
	okay to save the input from the user.
	
-----------------------------------------------------------------
-sndgen.cpp
-sndgen.h

This class is the second most imporant class of this project.
This class contains a generation object. The generation object has everything
	needed in order to create a sound file. It is used for generation of 
	the waves, generation of the adsr, and also holding the user input to
	the sndgen program.
#This class contains variables for everything the user inputs.
	It contains: if there's a samplerate, frequency, time, peak, pulse
	attack, decay, sustain, release, the wave type, and a special modifiedAttackSamples.
		The modified attack samples is used when the attack needs to be shortened. This way the program
		will keep the same slope as the specified attack, but stop where the release starts.
	MOST IMPORANTLY: It contains a ComS229 object. This is what the generation object is
	writing to.

	
#Methods for the generation class include 
	getters and setters as needed for the variables.
	The generation constructor. This will initialize the generation object.
	adsr: This will apply the adsr to a completed wave file.
	generateWave: This will create the wave that the user specifies in sndgen_main.
		It will then call the adsr method to apply the adsr.
	generateSamples: This will fill a ComS229 object with zerosI
		I did this to make sure there was space in the file for how long the user wanted it.