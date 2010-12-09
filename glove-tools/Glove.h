/** @file Glove.h
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
#ifndef INCLUDED_Glove_h_GUID_3988aa8d_bba7_42c5_a014_704428ba4d35
#define INCLUDED_Glove_h_GUID_3988aa8d_bba7_42c5_a014_704428ba4d35

// Internal Includes
#include "GlovePointerTypes.h"
#include "Finger.h"
#include "Handedness.h"

// Library/third-party includes
#include <osg/ref_ptr>
#include <osg/Node>

// Standard includes
// - none

namespace glove {
	namespace detail {
		struct GloveNodeContainer;
		struct GloveFilterContainer;
	}

	class Glove {
		public:
			Glove(GloveHardwarePtr hardware);
			~Glove();

			enum ReportType {
				/// raw values from the device
				REPORT_RAW,

				/// values scaled by the device's hardware/drivers
				REPORT_HWSCALED,

				/// values scaled by glove-tools
				REPORT_CALIBRATED,

				/// values scaled and filtered by glove-tools
				REPORT_FILTERED
			};

			/// Fetch updated data from hardware and process it.
			void updateData();

			/// Set requested report type.  Returns true if successful.
			bool setReportType(ReportType r);


			/// @name Accessors
			/// @{
			osg::ref_ptr<osg::Node> getNode() const;
			double getBend(Finger finger) const;
			Handedness getHand() const;
			/// @}

			/// @name Mutators
			/// @{
			void setHand(Handedness hand);
			/// @}
		private:
			detail::GloveNodeContainer * _node;
			detail::GloveFilterContainer * _filter;

			ReportType _r;

			Handedness _hand;

			GloveHardwarePtr _hardware;
			std::vector<double> _bends;

	};

} // end of namespace glove

#endif // INCLUDED_Glove_h_GUID_3988aa8d_bba7_42c5_a014_704428ba4d35

