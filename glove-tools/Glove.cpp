/**
	@file Glove.cpp
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
#include "Glove.h"
#include "GloveNode.h"
#include "GloveUpdater.h"
#include "IGloveHardware.h"
#include <GloveToolsConfig.h>

// Library/third-party includes
#ifdef BUILD_WITH_EIGENKF
#include <eigenkf/KalmanFilter.h>
using namespace eigenkf;
#endif

// Standard includes
#include <iostream>

namespace glove {
	namespace detail {
		struct GloveNodeContainer {
			osg::ref_ptr<GloveNode> n;
		};

#ifdef BUILD_WITH_EIGENKF
		typedef SimpleState<2> state_t;
		typedef ConstantProcess<2, state_t> process_t;
		struct GloveFilterContainer {
			EIGEN_MAKE_ALIGNED_OPERATOR_NEW
			KalmanFilter<state_t, process_t> kf;
		};
#endif
	}

	Glove::Glove(GloveHardwarePtr hardware) :
			_node(new detail::GloveNodeContainer),
			_filter(NULL),
			_r(REPORT_HWSCALED),
			_hand(LEFT_HAND),
			_hardware(hardware) {
		if (!_node) {
			throw new std::bad_alloc();
		}
		_node->n = new GloveNode(*this);
		_node->n->setUpdateCallback(new GloveUpdater);
		_bends.push_back(0.0);
		_bends.push_back(0.0);
		_bends.push_back(0.0);
		_bends.push_back(0.0);
		_bends.push_back(0.0);

#ifdef BUILD_WITH_EIGENKF
		_filter = new detail::GloveFilterContainer;
#endif
	}

	Glove::~Glove() {
		delete _node;
		_node = NULL;
#ifdef BUILD_WITH_EIGENKF
		delete _filter;
		_node = NULL;
#endif
	}

	void Glove::updateData() {
		_hardware->updateData();

		std::vector<double> bends = _hardware->getBends();
		std::vector<double> raw = _hardware->getRaw();

		for (unsigned int i = 0; i < 5; ++i) {
			if (bends[i] > 1.0) {
				std::cerr << "WARNING: your GloveHardware driver is returning too high of a bend value (" << bends[i] << ") ! Please report this bug to glovetools." << std::endl;
				bends[i] = 1.0;
			} else if (bends[i] < 0.0) {
				std::cerr << "WARNING: your GloveHardware driver is returning too low of a bend value (" << bends[i] << ") ! Please report this bug to glovetools." << std::endl;
				bends[i] = 0.0;
			}
			if (raw.size() > 0) {
				if (raw[i] > 1.0) {
					std::cerr << "WARNING: your GloveHardware driver is returning too high of a raw value (" << raw[i] << ") ! Please report this bug to glovetools." << std::endl;
					raw[i] = 1.0;
				} else if (raw[i] < 0.0) {
					std::cerr << "WARNING: your GloveHardware driver is returning too low of a raw value (" << raw[i] << ") ! Please report this bug to glovetools." << std::endl;
					raw[i] = 0.0;
				}
			}
		}

		std::vector<double> calib;
		if (raw.size() > 0) {
			assert(raw.size() == 5);
			calib = _calib.processBends(raw);
		}


		/// @todo Eventually will want kalman filter here rather than just copying the latest update
		if (_r == REPORT_RAW && raw.size() > 0) {
			_bends = raw;
			return;
		}

		if (_r == REPORT_CALIBRATED && calib.size() > 0) {
			assert(calib.size() == 5);
			_bends = calib;
			return;
		}
		/*
		if (_r == REPORT_FILTERED) {

		}
		*/

		/// Fallback
		_bends = bends;
	}

	bool Glove::setReportType(ReportType r) {
#ifndef BUILD_WITH_EIGENKF
		/// Disallow filtered mode if built without filtering.
		if (r == REPORT_FILTERED) {
			return false;
		}
#endif
		_r = r;
		return true;
	}

	osg::ref_ptr<osg::Node> Glove::getNode() const {
		return _node->n;
	}

	double Glove::getBend(Finger finger) const {
		return _bends[finger];
	}

	Handedness Glove::getHand() const {
		if (_hardware->getHand() == UNKNOWN_HAND) {
			return _hand;
		} else {
			return _hardware->getHand();
		}
	}

	void Glove::printCalibration(std::ostream & s) const {
		s << _calib;
	}

	Glove::ReportType Glove::getReportType() const {
		return _r;
	}

	void Glove::setHand(Handedness hand) {
	  _hand = hand;
	}

	void Glove::startCalibrating() {
		_calib.startCalibrating();
	}

	void Glove::stopCalibrating() {
		_calib.stopCalibrating();
	}


} // end of namespace glove

