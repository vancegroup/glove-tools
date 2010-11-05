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
// - none

namespace glove {

	GloveUpdater::GloveUpdater(Glove const & g) :
		_g(g) {}

	void GloveUpdater::operator()(osg::Node* node, osg::NodeVisitor* nv) {
		GloveNode * n = static_cast<GloveNode *>(node);
		/// @todo using the data from _g, update the transforms for the joints
		double angle = _g.getBend(THUMB) * something;
		setAttitude...

		/// @todo also update the switch node's active child, based on the handedness of the glove
		/* multiplying by:
			1	0	0	0
			0	-1	0	0
			0	0	1	0
			0	0	0	1
		  should flip across the y axis */
	}

	GloveDeviceUpdater::GloveDeviceUpdater(Glove & glove) :
		_g(glove) {}

	void GloveDeviceUpdater::operator()(osg::Node* /*node*/, osg::NodeVisitor* /*nv*/) {
		_g.updateData();
	}

}