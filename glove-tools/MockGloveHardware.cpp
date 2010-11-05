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
	/// Register this class for create-by-name
	DriverRegistration MockGloveHardwareRegistration("MockGloveHardware", &MockGloveHardware::create);

	GloveHardwarePtr MockGloveHardware::create(std::string const & option) {
		GloveHardwarePtr temp(new MockGloveHardware(option));
		return temp;
	}

}