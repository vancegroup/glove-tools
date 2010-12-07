/**
	@file GloveHardware5DT.cpp
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
#include "GloveHardware5DT.h"

// Library/third-party includes
// - none

// Standard includes
// - none

namespace glove {
	GloveHardwarePtr GloveHardware5DT::create(std::string const & option) {
		GloveHardwarePtr temp(new GloveHardware5DT(option));
		return temp;
	}

	GloveHardware5DT::GloveHardware5DT(std::string const & option) :
			_fd(NULL) {
		/// @todo do stuff
		/// @todo open glove device ehre
	}

	GloveHardware5DT::~GloveHardware5DT() {
		if (_fd) {
			/// @todo close glove here
		}
	}

	void GloveHardware5DT::updateData() {
		/// @todo do stuff
	}
}