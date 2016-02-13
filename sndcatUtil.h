#pragma once
//This is created to help sndcat and sndmix.
namespace sndCatUtil {
	/*This will concatenate the second ComS229 to the first (beingAdded to addingTo)*/
	ComS229 concatenate(ComS229 addingTo, ComS229 beingAdded);
	/*These output to standard out and to a file*/
	void stdOutput(ComS229 output);
	void output(ComS229 output);
}