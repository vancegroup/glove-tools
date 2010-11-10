/** @file GloveNode.h
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
#ifndef INCLUDED_GloveNode_h_GUID_d7bdaeda_e402_4aa3_8746_923d38aebd7c
#define INCLUDED_GloveNode_h_GUID_d7bdaeda_e402_4aa3_8746_923d38aebd7c

// Internal Includes
#include "Glove.h"
#include "GloveUpdater.h"

// Library/third-party includes
#include <osg/Group>
#include <osg/ref_ptr>
#include <osg/MatrixTransform>

// Standard includes
#include <vector>

namespace glove {
	class GloveNode : public osg::Group {
		public:
			GloveNode(Glove const & g);

		private:
			Glove const & _g;
			osg::ref_ptr<GloveUpdater> _updater;
			typedef std::vector< osg::ref_ptr<osg::MatrixTransform> > JointList;
			std::vector<JointList> _joints;
			
	};
}

#endif // INCLUDED_GloveNode_h_GUID_d7bdaeda_e402_4aa3_8746_923d38aebd7c

