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

// Library/third-party includes
// - none

// Standard includes
// - none

namespace glove {
	namespace detail {
		struct GloveNodeContainer {
			osg::ref_ptr<GloveNode> n;
		};
	}

	Glove::Glove(GloveHardwarePtr hardware) :
			_node(new detail::GloveNodeContainer),
			_hand(LEFT_HAND),
			_hardware(hardware) {
		assert(_node);
		_node->n = new GloveNode(*this);
		_bends.push_back(0.0);
		_bends.push_back(0.0);
		_bends.push_back(0.0);
		_bends.push_back(0.0);
		_bends.push_back(0.0);
	}

	Glove::~Glove() {
		delete _node;
		_node = NULL;
	}

	void Glove::updateData() {
		/// @todo implement! should update the hardware, then update our own bend data.
		/// Eventually will want kalman filter here.

	}

	osg::ref_ptr<osg::Node> Glove::getNode() const {
		return _node->n;
	}

	double Glove::getBend(Finger finger) const {
		return _bends[finger];
	}

	Glove::Handedness Glove::getHand() const {
		return _hand;
	}

	void Glove::setHand(Handedness hand) {
	  _hand = hand;
	}


} // end of namespace glove

