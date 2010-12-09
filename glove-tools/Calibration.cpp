/**
	@file Calibration.cpp
	@brief

	@date 2010

	@author
	Ryan Pavlik
	<rpavlik@iastate.edu> and <abiryan@ryand.net>
	http://academic.cleardefinition.com/
	Iowa State University Virtual Reality Applications Center
	Human-Computer Interaction Graduate Program
*/

// Internal Includes
#include "Calibration.h"

// Library/third-party includes
// - none

// Standard includes
// - none

namespace glove {

Calibration::Calibration() :
	_autoCalibrating(true) { }
Calibration::~Calibration() {}

void Calibration::startCalibrating() {
	_autoCalibrating = true;
}

void Calibration::stopCalibrating() {
	_autoCalibrating = false;
}

std::vector<double> Calibration::processBends(std::vector<double> const& bends) {
	
	if (_autoCalibrating) {
		if (_mins.size() < 5) {
			// first time!
			for (unsigned int i = 0; i < 5; ++i) {
				_ranges.push_back(0);
				_mins.push_back(bends[i]);
				_maxes.push_back(bends[i]);
			}
		} else {
			// been here before, just update
			for (unsigned int i = 0; i < 5; ++i) {
				if (bends[i] < _mins[i]) {
					_mins[i] = bends[i];
					_ranges[i] = _maxes[i] - _mins[i];
				} else if (bends[i] > _maxes[i]) {
					_maxes[i] = bends[i];
					_ranges[i] = _maxes[i] - _mins[i];
				}
			}		
		}
	} // end if autocalibrating
	
	
	std::vector<double> ret;
	for (unsigned int i = 0; i < 5; ++i) {
		if (_ranges[i] = 0) {
		// don't divide by zero the first time
			ret.push_back(0);
		} else {
			double val = (bends[i] - _mins[i]) / _ranges[i];
			if (val < 0) {
				/// @todo warn here?
				val = 0;
			} else if (val > 1) {
				/// @todo warn here?
				val = 1;							
			}
			ret.push_back(val);
		}
	}
	return ret;			
}

std::vector<double> const& Calibration::getRanges() const {
	return _ranges;
}
std::vector<double> const& Calibration::getMins() const {
	return _mins;
}

std::vector<double> const& Calibration::getMaxes() const {
	return _maxes;
}
			



} // end of namespace glove
