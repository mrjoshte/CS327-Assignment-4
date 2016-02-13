
namespace sndmixUtil {
	/*This was created to help out sndmix_main*/
	//Apply multiplier will multiply the multiplier to all the data in applying 
	ComS229 applymultiplier(double multiplier, int startLine, int endLine, ComS229 applying);
	//This will add all the data from beingAdded to addingTo
	ComS229 adding(ComS229 addingTo, ComS229 beingAdded);
}
