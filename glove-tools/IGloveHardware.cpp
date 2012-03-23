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
	IGloveHardware::IGloveHardware() :
		_hand(UNKNOWN_HAND) {
		_bends.push_back(0.0);
		_bends.push_back(0.0);
		_bends.push_back(0.0);
		_bends.push_back(0.0);
		_bends.push_back(0.0);
	}

	void IGloveHardware::_setBend(Finger const f, double bend, double raw) {
		_bends[f] = bend;
		if (raw > 0) {
			while (_raw.size() < 5) {
				_raw.push_back(0.0);
			}
			_raw[f] = raw;
		}

	}
}
