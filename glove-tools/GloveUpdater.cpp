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
		// using the data from _g, update the transforms for the joints
		double fingerAngle;
		double fingerBendScale = 0.5;

		/*for (unsigned int j = 0; j < 5; j++)
		{
			fingerAngle = _g.getBend(j) * fingerBendScale;
			for (unsigned int i = 0; i < _joints[j].size(); i++) //should be 2x
			{
				_joints[j][i]->matrix.makeRotate(fingerAngle, osg::Vec3f(1.0f, 0.0f, 0.0f)); //rotate around X axis
			}
		}

		//update the switch node's active child, based on the handedness of the glove
		if (n->Handedness == RIGHT_HAND)
		{*/
			/* multiplying by:
			1	0	0	0
			0	-1	0	0
			0	0	1	0
			0	0	0	1
			should flip across the y axis */
		/*}
		else
		{

		}*/
		
	}

	GloveDeviceUpdater::GloveDeviceUpdater(Glove & glove) :
		_g(glove) {}

	void GloveDeviceUpdater::operator()(osg::Node* /*node*/, osg::NodeVisitor* /*nv*/) {
		_g.updateData();
	}

}
