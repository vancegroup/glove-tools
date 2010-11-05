/** @file MockGloveHardware.h
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
#ifndef INCLUDED_MockGloveHardware_h_GUID_99f489e0_8c85_4427_9700_d8a9c79eb90c
#define INCLUDED_MockGloveHardware_h_GUID_99f489e0_8c85_4427_9700_d8a9c79eb90c

// Internal Includes
#include "IGloveHardware.h"

// Library/third-party includes
// - none

// Standard includes
// - none

namespace glove {

	class MockGloveHardware : public IGloveHardware {
		public:
			
			virtual ~MockGloveHardware();

			/// Fetch the latest bend information for all fingers and normalize
			virtual void updateData();

			/// Access the bend data for the given finger, normalized into [0, 1]
			virtual double getBend(Finger finger) const;

			/// Factory function for use with create-by-name
			static GloveHardwarePtr create(std::string const & option);

		protected:
			MockGloveHardware(std::string const & option);
	};
}
#endif // INCLUDED_MockGloveHardware_h_GUID_99f489e0_8c85_4427_9700_d8a9c79eb90c

