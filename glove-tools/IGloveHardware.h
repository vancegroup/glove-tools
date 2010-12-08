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
#include "Handedness.h"
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

	struct InvalidGloveOptionError : public std::runtime_error {
		InvalidGloveOptionError(std::string const& what = "Could not understand provided glove option!") : std::runtime_error(what) {}
	};

	class IGloveHardware {
		public:
			IGloveHardware();
			virtual ~IGloveHardware() {}

			/// Fetch the latest bend information for all fingers and normalize into [0, 1]
			virtual void updateData() = 0;

			/// Return the glove's handedness, if our hardware interface tells us this
			Handedness getHand() const {
				return _hand;
			}

			/// Access the bend data for the given finger
			double getBend(Finger finger) const {
				return _bends[finger];
			}

			std::vector<double> getBends() const {
				return _bends;
			}

		protected:
			void _setHand(Handedness const h) {
				_hand = h;
			}

			std::vector<double> _bends;

		private:
			Handedness _hand;
	};
}

#endif // INCLUDED_IGloveHardware_h_GUID_70ba2c9c_5e00_44d8_b9c9_6a16cb9a2fb7

