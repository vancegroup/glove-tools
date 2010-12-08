/** @file GloveHardware5DT.h
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
#ifndef INCLUDED_GloveHardware5DT_h_GUID_5edd9ba4_a60c_454f_9e0a_f0e135f6d6c4
#define INCLUDED_GloveHardware5DT_h_GUID_5edd9ba4_a60c_454f_9e0a_f0e135f6d6c4

// Internal Includes
#include "IGloveHardware.h"

// Library/third-party includes
#define LPVOID void *
#include <fglove.h>
#undef LPVOID

// Standard includes
#include <string>

namespace glove {

	struct NoUSBGlove5DTFound : public GloveConnectionError {
		NoUSBGlove5DTFound() : GloveConnectionError("USB address specified, but no 5DT USB gloves connected!") {}
	};

	struct USBGlove5DTConnectionFailed : public GloveConnectionError {
		USBGlove5DTConnectionFailed() : GloveConnectionError("Unable to open 5DT data glove on USB port!") {}
	};

	struct SerialGlove5DTConnectionFailed : public GloveConnectionError {
		SerialGlove5DTConnectionFailed() : GloveConnectionError("Unable to open 5DT data glove on serial port!") {}
	};

	class GloveHardware5DT : public IGloveHardware {
		public:

			virtual ~GloveHardware5DT();

			/// Fetch the latest bend information for all fingers with calibration settings
			void updateData();

			/// Factory function for use with create-by-name
			static GloveHardwarePtr create(std::string const & option);

			// Return true if glove is right handed, NULL on error
			bool isRightHanded();

			// Return the type of the glove, NULL on error
			std::string returnGloveType();

			// Return the number of sensors on the glove, NULL on error
			int returnNumSensors();

			// Resets the calibration settings back to default
			void resetGloveCalibration();

		protected:
			GloveHardware5DT(std::string const & option);
			fdGlove * _fd;
	};
}

#endif // INCLUDED_GloveHardware5DT_h_GUID_5edd9ba4_a60c_454f_9e0a_f0e135f6d6c4

