/** @file GloveUpdater.h
	@brief

	@date 2010

	@author
	Ryan Pavlik
	<rpavlik@iastate.edu> and <abiryan@ryand.net>
	http://academic.cleardefinition.com/
	Iowa State University Virtual Reality Applications Center
	Human-Computer Interaction Graduate Program
*/
#pragma once
#ifndef INCLUDED_GloveUpdater_h_GUID_a6db12e1_46e0_42c7_bb69_ba82a5d6ec5d
#define INCLUDED_GloveUpdater_h_GUID_a6db12e1_46e0_42c7_bb69_ba82a5d6ec5d

// Internal Includes
#include "Glove.h"
#include "GloveUpdater.h"

// Library/third-party includes
#include <osg/NodeCallback>
#include <osg/Node>

// Standard includes
// - none

namespace glove {
	class GloveUpdater : public osg::NodeCallback {
		public:
			GloveUpdater(Glove const & g);
			void operator()(osg::Node* node, osg::NodeVisitor* nv);

		private:
			Glove const & _g;

	};


}

#endif // INCLUDED_GloveUpdater_h_GUID_a6db12e1_46e0_42c7_bb69_ba82a5d6ec5d

