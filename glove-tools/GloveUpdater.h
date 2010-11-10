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

// Library/third-party includes
#include <osg/NodeCallback>

// Standard includes
// - none

namespace glove {
	/// Updates the GloveNode based on the data reported by Glove
	class GloveUpdater : public osg::NodeCallback {
			virtual void operator()(osg::Node* node, osg::NodeVisitor* nv);
	};

	/// Optional node callback to perform a full device update during
	/// the scenegraph update traversal - updates Glove/GloveHardware
	class GloveDeviceUpdater : public osg::NodeCallback {
		public:
			GloveDeviceUpdater(Glove & glove);
			virtual void operator()(osg::Node* node, osg::NodeVisitor* nv);
		private:
			Glove & _g;
	};

}

#endif // INCLUDED_GloveUpdater_h_GUID_a6db12e1_46e0_42c7_bb69_ba82a5d6ec5d

