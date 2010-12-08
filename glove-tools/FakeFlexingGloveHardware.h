/** @file FakeFlexingGloveHardware.h
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
#ifndef INCLUDED_FakeFlexingGloveHardware_h_GUID_e4064cee_ec76_40b1_9660_6eef14208c35
#define INCLUDED_FakeFlexingGloveHardware_h_GUID_e4064cee_ec76_40b1_9660_6eef14208c35

// Internal Includes
#include "IGloveHardware.h"

// Library/third-party includes
// - none

// Standard includes
// - none


namespace glove {

	class FakeFlexingGloveHardware : public IGloveHardware {
		public:

			virtual ~FakeFlexingGloveHardware() {}

			/// Fetch the latest bend information for all fingers and normalize
			void updateData();

			/// Factory function for use with create-by-name
			static GloveHardwarePtr create(std::string const & option);

		protected:
			FakeFlexingGloveHardware(std::string const & option);
			unsigned int _counter;
	};
}

#endif // INCLUDED_FakeFlexingGloveHardware_h_GUID_e4064cee_ec76_40b1_9660_6eef14208c35
