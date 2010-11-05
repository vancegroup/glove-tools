/**
	@file GloveHardwareFactory.cpp
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
#include "GloveHardwareFactory.h"

#include <GloveToolsConfig.h>

#include "MockGloveHardware.h"

#ifdef BUILD_WITH_GLOVE5DT
#include "GloveHardware5DT.h"
#endif

// Library/third-party includes
// - none

// Standard includes
#include <iostream>

namespace glove {
	GloveHardwareFactory * GloveHardwareFactory::instance() {
		static GloveHardwareFactory * inst(new GloveHardwareFactory);
		return inst;
	}

	GloveHardwarePtr GloveHardwareFactory::createByName(std::string const & name, std::string const & option) {
		HardwareCreator f = _creators[name];
		if (!f) {
			throw new UnregisteredHardwareTypeException(name);
		}
		return f(option);
	}

	GloveHardwareFactory::Registration::Registration(std::string const & name, HardwareCreator creatorFunc) {
		std::cout << "Registering new GloveHardware creator: " << name << std::endl;
		GloveHardwareFactory::instance()->_creators[name] = creatorFunc;
	}
	
	GloveHardwareFactory::GloveHardwareFactory() {
		/// @todo figure out why the static factory isn't working right
		_creators["MockGloveHardware"] = & MockGloveHardware::create;
		#ifdef BUILD_WITH_GLOVE5DT
		_creators["GloveHardware5DT"] = & GloveHardware5DT::create;
		#endif
	}


}