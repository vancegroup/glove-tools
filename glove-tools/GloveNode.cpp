/**
	@file GloveNode.cpp
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
#include "GloveNode.h"

// Library/third-party includes
#include <osgDB/ReadFile>
#include <osg/ref_ptr>

// Standard includes
#include <cassert>

namespace glove {
	GloveNode::GloveNode(Glove const & g) :
			_g(g),
			_updater(new GloveUpdater(g)) {
		/// @todo load the model and set the updater here

		osg::ref_ptr<osg::Node> hand = osgDB::readNodeFile("hand-structured.osg");
		assert(hand.valid());
		hand->setUpdateCallback(_updater.get());

		this->addChild(hand);
	}

}
