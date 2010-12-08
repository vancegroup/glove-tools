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
#include "GlovePointerTypes.h"

// Library/third-party includes
#include <boost/function.hpp>

// Standard includes
#include <vector>
#include <stdexcept>

namespace glove {

	struct GloveConnectionError : public std::runtime_error {
		GloveConnectionError(std::string const& what = "Could not connect to glove!") : std::runtime_error(what) {}
	};

	class IGloveHardware {
		public:
			IGloveHardware();
			virtual ~IGloveHardware() {}

			/// Fetch the latest bend information for all fingers and normalize into [0, 1]
			virtual void updateData() = 0;

			/// Access the bend data for the given finger
			double getBend(Finger finger) const {
				return _bends[finger];
			}

			std::vector<double> getBends() const {
				return _bends;
			}

		protected:
			std::vector<double> _bends;
	};
}

#endif // INCLUDED_IGloveHardware_h_GUID_70ba2c9c_5e00_44d8_b9c9_6a16cb9a2fb7

