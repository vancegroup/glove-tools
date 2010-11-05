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
// - none

// Standard includes
// - none

namespace glove {
	GloveNode::GloveNode(Glove & g) :
			_g(g),
			_updater(new GloveUpdater(g)) {
		/// @todo load the model and set the updater here
	}

}
