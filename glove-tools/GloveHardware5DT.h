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
// - none

namespace glove {

	class GloveHardware5DT : public IGloveHardware {
		public:

			virtual ~GloveHardware5DT();

			/// Fetch the latest bend information for all fingers and normalize
			void updateData();

			/// Factory function for use with create-by-name
			static GloveHardwarePtr create(std::string const & option);

		protected:
			GloveHardware5DT(std::string const & option);
			fdGlove * _fd;
	};
}

#endif // INCLUDED_GloveHardware5DT_h_GUID_5edd9ba4_a60c_454f_9e0a_f0e135f6d6c4

