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

		_bends = _hardware->getBends();
		/// @todo Eventually will want kalman filter here rather than just copying the latest update
		for (unsigned int i = 0; i < 5; ++i) {
			if (_bends[i] > 1.0) {
				std::cerr << "WARNING: your GloveHardware driver is returning too high of a bend value (" << _bends[i] << ") ! Please report this bug to glovetools." << std::endl;
				_bends[i] = 1.0;
			} else if (_bends[i] < 0.0) {
				std::cerr << "WARNING: your GloveHardware driver is returning too low of a bend value (" << _bends[i] << ") ! Please report this bug to glovetools." << std::endl;
				_bends[i] = 0.0;
			}
		}
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

	void Glove::setHand(Handedness hand) {
	  _hand = hand;
	}


} // end of namespace glove

