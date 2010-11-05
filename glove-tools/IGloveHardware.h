/** @file IGloveHardware.h
	@brief

	@date 2010

	@author
	Ryan Pavlik
	<rpavlik@iastate.edu> and <abiryan@ryand.net>
	http://academic.cleardefinition.com/
	Iowa State University Virtual Reality Applications Center
	Human-Computer Interaction Graduate Program
*/
#pragma once
#ifndef INCLUDED_IGloveHardware_h_GUID_70ba2c9c_5e00_44d8_b9c9_6a16cb9a2fb7
#define INCLUDED_IGloveHardware_h_GUID_70ba2c9c_5e00_44d8_b9c9_6a16cb9a2fb7

// Internal Includes
#include "Finger.h"

// Library/third-party includes
// - none

// Standard includes
// - none

namespace glove {
	class IGloveHardware {
		public:
			IGloveHardware() {}
			virtual ~IGloveHardware() {}

			/// Fetch the latest bend information for all fingers and normalize
			virtual void updateData() = 0;

			/// Access the bend data for the given finger, normalized into [0, 1]
			virtual double getBend(Finger finger) const = 0;
	};
}

#endif // INCLUDED_IGloveHardware_h_GUID_70ba2c9c_5e00_44d8_b9c9_6a16cb9a2fb7

