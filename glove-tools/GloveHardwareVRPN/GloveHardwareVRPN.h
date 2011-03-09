/** @file GloveHardwareVRPN.h
	@brief

	@date 2011

	@author
	Ryan Pavlik
	<rpavlik@iastate.edu> and <abiryan@ryand.net>
	http://academic.cleardefinition.com/
	Iowa State University Virtual Reality Applications Center
	Human-Computer Interaction Graduate Program
*/

#pragma once
#ifndef INCLUDED_GloveHardwareVRPN_h_GUID_c40e78d5_1b2a_4de9_a9fc_ff9bf93b30dd
#define INCLUDED_GloveHardwareVRPN_h_GUID_c40e78d5_1b2a_4de9_a9fc_ff9bf93b30dd

// Internal Includes
#include "../IGloveHardware.h"

// Library/third-party includes
// - none

// Standard includes
#include <vector>


namespace glove {

	namespace detail {
		struct GloveHardwareVRPNDevice;	
	}

	class GloveHardwareVRPN : public IGloveHardware {
		public:

			virtual ~GloveHardwareVRPN();

			/// Fetch the latest bend information for all fingers with calibration settings
			void updateData();

			/// Factory function for use with create-by-name
			static GloveHardwarePtr create(std::string const & option);
			
			/// Internal use - signal an updated report.
			void signalUpdated(std::vector<double> const& values);
		protected:
			GloveHardwareVRPN(std::string const & option);
			detail::GloveHardwareVRPNDevice * _d;
			std::vector<int> _channelMap;
			std::vector<double> _channels;
			int _minResponseSize;
			double _max;
			double _min;
	};
}

#endif // INCLUDED_GloveHardwareVRPN_h_GUID_c40e78d5_1b2a_4de9_a9fc_ff9bf93b30dd
