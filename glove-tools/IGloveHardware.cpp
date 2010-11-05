/**
	@file IGloveHardware.cpp
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
#include "IGloveHardware.h"

// Library/third-party includes
// - none

// Standard includes
// - none

namespace glove {
	std::map<std::string, HardwareCreator> IGloveHardware::s_creators;

	GloveHardwarePtr IGloveHardware::createByName(std::string const & name, std::string const & option) {
		HardwareCreator f = s_creators[name];
		if (!f) {
			throw new UnregisteredHardwareTypeException(name);
		}
		return f(option);
	}

	void IGloveHardware::registerDriver(std::string const & name, HardwareCreator creatorFunc) {
		s_creators[name] = creatorFunc;
	}

	DriverRegistration::DriverRegistration(std::string const & name, HardwareCreator creatorFunc) {
		IGloveHardware::registerDriver(name, creatorFunc);
	}
}