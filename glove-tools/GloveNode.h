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
#include <osg/PositionAttitudeTransform>

// Standard includes
#include <vector>
#include <string>

namespace glove {
	class GloveNode : public osg::Group {
		public:
			GloveNode(Glove const & g);
			
			/// Called from GloveUpdater::operator()()
			void doUpdate();

		private:
			typedef std::vector< osg::ref_ptr<osg::PositionAttitudeTransform> > JointList;
			
			/// @name Helper routines
			/// @{
			void _updateFinger(Finger finger);
			
			static JointList _findJoints(osg::ref_ptr<osg::PositionAttitudeTransform> const& parent);
			static osg::ref_ptr<osg::PositionAttitudeTransform> _getChildTransform(osg::ref_ptr<osg::Group> const& parent);
			static osg::ref_ptr<osg::Node> _getNamedChild(osg::ref_ptr<osg::Group> const& parent, std::string const& name);
			/// @}
			
			Glove const & _g;
			osg::ref_ptr<osg::Switch> _leftyrighty;
			
			std::vector<JointList> _joints;
			static osg::Vec3f const& _getJointAxis(Finger f, int joint);
			
	};
}

#endif // INCLUDED_GloveNode_h_GUID_d7bdaeda_e402_4aa3_8746_923d38aebd7c

