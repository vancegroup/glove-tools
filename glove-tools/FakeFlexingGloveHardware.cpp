/**
	@file FakeFlexingGloveHardware.cpp
	@brief

	@date 2010

	@author
	Ryan Pavlik
	<rpavlik@iastate.edu> and <abiryan@ryand.net>
	http://academic.cleardefinition.com/
	Iowa State University Virtual Reality Applications Center
	Human-Computer Interaction Graduate Program
*/

// Internal Includes
#include "FakeFlexingGloveHardware.h"

// Library/third-party includes
// - none

// Standard includes
#include <cmath>

namespace glove {
	GloveHardwarePtr FakeFlexingGloveHardware::create(std::string const & option) {
		GloveHardwarePtr temp(new FakeFlexingGloveHardware(option));
		return temp;
	}

	FakeFlexingGloveHardware::FakeFlexingGloveHardware(std::string const & option) :
		_counter(0) {}


	void FakeFlexingGloveHardware::updateData() {
		double bend = (- std::cos(static_cast<double>(_counter) / 50.0) + 1.0) / 2.0;
		_setBend(THUMB, bend);
		_setBend(INDEX_FINGER, bend, bend);
		_setBend(MIDDLE_FINGER, bend, bend);
		_setBend(RING_FINGER, bend, bend);
		_setBend(PINKY_FINGER, bend, bend);
		_counter++;
	}

} // end of namespace glove
