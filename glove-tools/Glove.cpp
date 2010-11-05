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

	Glove::Glove(GloveHardwarePtr hardware) :
			_hand(LEFT_HAND),
			_hardware(hardware) {
		_node = new GloveNode(*this);
		_bends.push_back(0.0);
		_bends.push_back(0.0);
		_bends.push_back(0.0);
		_bends.push_back(0.0);
		_bends.push_back(0.0);
	}

	void Glove::updateData() {
		/// @todo implement!

	}



	osg::ref_ptr<osg::Node> Glove::getNode() const {
		return _node;
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

