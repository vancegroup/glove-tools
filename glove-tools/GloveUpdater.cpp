/**
	@file GloveUpdater.cpp
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
#include "GloveUpdater.h"
#include "GloveNode.h"

// Library/third-party includes
// - none

// Standard includes
#include <iostream>
#include <cassert>

namespace glove {

	void GloveUpdater::operator()(osg::Node* node, osg::NodeVisitor* nv) {
		GloveNode * n = dynamic_cast<GloveNode *>(node);
		assert(n);
		n->doUpdate();
		traverse(node, nv);
	}

	GloveDeviceUpdater::GloveDeviceUpdater(Glove & glove) :
		_g(glove) {}

	void GloveDeviceUpdater::operator()(osg::Node* node, osg::NodeVisitor* nv) {
		_g.updateData();
		traverse(node, nv);
	}

}
