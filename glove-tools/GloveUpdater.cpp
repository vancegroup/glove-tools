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

// Library/third-party includes
// - none

// Standard includes
// - none

namespace glove {

	GloveUpdater::GloveUpdater(Glove & g) :
		_g(g) {}

	void GloveUpdater::operator()(osg::Node* node, osg::NodeVisitor* nv) {
		 /// @todo using the data from _g, update the joints
	}

}