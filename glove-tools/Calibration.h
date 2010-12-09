/** @file Calibration.h
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
#ifndef INCLUDED_Calibration_h_GUID_7a455249_4847_42c1_ada5_8856a142caf0
#define INCLUDED_Calibration_h_GUID_7a455249_4847_42c1_ada5_8856a142caf0

// Internal Includes
#include "Finger.h"

// Library/third-party includes
// - none

// Standard includes
#include <iostream>
#include <iomanip>
#include <vector>

namespace glove {
	class Calibration {
		public:
			Calibration();
			~Calibration();
			
			void startCalibrating();
			void stopCalibrating();
			
			std::vector<double> processBends(std::vector<double> const& bends);
			
			std::vector<double> const& getRanges() const;
			std::vector<double> const& getMins() const;
			std::vector<double> const& getMaxes() const;	
			
		private:
			bool _autoCalibrating;
			bool _init;
			std::vector<double> _ranges;
			std::vector<double> _mins;
			std::vector<double> _maxes;
	};
	
	template<class Stream>
	Stream & operator<<(Stream & s, Calibration const& c) {
		std::vector<double> mins = c.getMins();
		std::vector<double> maxes = c.getMaxes();
		std::vector<double> ranges = c.getRanges();
		s << std::setw(10) << "Min" << std::setw(10) << "Max" << std::setw(10) << "Range" << std::endl;
		for (unsigned int i = 0; i < ranges.size(); ++i) {
			s << std::setw(10) << mins[i] << std::setw(10) << maxes[i] << std::setw(10) << ranges[i]  << std::endl;
		}
		return s;	
	}
}

#endif // INCLUDED_Calibration_h_GUID_7a455249_4847_42c1_ada5_8856a142caf0

