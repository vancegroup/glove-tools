/**
	@file MockGloveHardware.cpp
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
#include "MockGloveHardware.h"

// Library/third-party includes
// - none

// Standard includes
// - none

namespace glove {

	GloveHardwarePtr MockGloveHardware::create(std::string const & option) {
		GloveHardwarePtr temp(new MockGloveHardware(option));
		return temp;
	}

	MockGloveHardware::MockGloveHardware(std::string const & option) :
		_bend(0.0) {}

	void MockGloveHardware::setBendValue(double bend) {
		if (bend > 1.0) {
			_bend = 1.0;
		} else if (bend < 0.0) {
			_bend = 0.0;
		} else {
			_bend = bend;
		}
	}

	void MockGloveHardware::updateData() {
		_bends[0] = _bend;
		_bends[1] = _bend;
		_bends[2] = _bend;
		_bends[3] = _bend;
		_bends[4] = _bend;
	}
}
