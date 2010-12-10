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
#include <stdexcept>

namespace glove {

Calibration::Calibration() :
	_autoCalibrating(true) {
}
	
Calibration::Calibration(std::vector<double> const& mins, std::vector<double> const& maxes) :
		_autoCalibrating(false),
		_mins(mins),
		_maxes(maxes) {
	if (_mins.size() != 5 || _maxes.size() != 5) {
		throw new std::invalid_argument("Must pass in 5 mins and 5 maxes!");
	}
	for (unsigned int i = 0; i < 5; ++i) {
		_ranges.push_back(_maxes[i] - _mins[i]);
	}
}
	
Calibration::~Calibration() {}

void Calibration::startCalibrating() {
	_autoCalibrating = true;
}

void Calibration::stopCalibrating() {
	_autoCalibrating = false;
}

std::vector<double> Calibration::processBends(std::vector<double> const& bends) {
	
	if (_autoCalibrating) {
	
		if (_ranges.size() == 0) {
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
		double val = 0;
		if (_ranges[i] > 0) {
			// don't divide by zero the first time
			 val = (bends[i] - _mins[i]) / _ranges[i];
			if (val < 0) {
				/// @todo warn here?
				val = 0;
			} else if (val > 1) {
				/// @todo warn here?
				val = 1;							
			}
		}
		ret.push_back(val);
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
