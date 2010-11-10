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

// Library/third-party includes
#include <osg/Group>
#include <osg/ref_ptr>
#include <osg/MatrixTransform>

// Standard includes
#include <vector>
#include <string>

namespace glove {
	class GloveNode : public osg::Group {
		public:
			GloveNode(Glove const & g);
			
			/// Called from GloveUpdater::operator()()
			void update();

		private:
			typedef std::vector< osg::ref_ptr<osg::MatrixTransform> > JointList;
			
			/// @name Helper routines
			/// @{
			void _updateFinger(Finger finger);
			
			static JointList _findJoints(osg::ref_ptr<osg::MatrixTransform> const& parent);
			static osg::ref_ptr<osg::MatrixTransform> _getChildTransform(osg::ref_ptr<osg::Group> const& parent);
			static osg::ref_ptr<osg::Node> _getNamedChild(osg::ref_ptr<osg::Group> const& parent, std::string const& name);
			/// @}
			
			Glove const & _g;
			osg::ref_ptr<osg::Switch> _leftyrighty;
			
			std::vector<JointList> _joints;
			
	};
}

#endif // INCLUDED_GloveNode_h_GUID_d7bdaeda_e402_4aa3_8746_923d38aebd7c

