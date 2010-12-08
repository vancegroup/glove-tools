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
#include <osgDB/ReadFile>
#include <osg/ref_ptr>
#include <osg/Switch>
#include <osg/MatrixTransform>

// Standard includes
#include <cassert>
#include <iostream>
#include <cmath>

namespace glove {
	osg::Vec3f const& GloveNode::_getJointAxis(Finger f, int joint) {
		static std::vector< std::vector<osg::Vec3f> > axes;
		static bool loaded = false;
		if (!loaded) {
			{
				// Thumb
				std::vector<osg::Vec3f> finger;
				osg::Vec3f thumb_oppose(0.1, 0, -1);
				thumb_oppose.normalize();
				finger.push_back(thumb_oppose);
				finger.push_back(osg::Vec3f(1/std::sqrt(2.0), 0, -1/std::sqrt(2.0)));
				finger.push_back(osg::Vec3f(1/std::sqrt(2.0), 0, -1/std::sqrt(2.0)));
				axes.push_back(finger);
			}
			for (unsigned int i = 0; i < 4; ++i) {
				// other fingers
				std::vector<osg::Vec3f> finger;
				finger.push_back(osg::Vec3f(1, 0, 0));
				finger.push_back(osg::Vec3f(1, 0, 0));
				finger.push_back(osg::Vec3f(1, 0, 0));
				axes.push_back(finger);
			}
			loaded = true;
		}
		return axes[f][joint];
	}
			

	GloveNode::GloveNode(Glove const & g) :
			_g(g),
			_leftyrighty(new osg::Switch) {
		/// load the model and set the updater here

		osg::ref_ptr<osg::Node> model = osgDB::readNodeFile("hand-structured.osg");
		assert(model.valid());
		
		/// Grab the meaningful parent node.
		osg::ref_ptr<osg::Group> hand = model->asGroup();
		hand->computeBound(); //calculate bound information since we need this before update is called
		while (hand->getName() != "hand") {
			assert(hand->getNumChildren() == 1);
			hand = hand->getChild(0)->asGroup(); // only has 1 child
		}

		osg::ref_ptr<osg::PositionAttitudeTransform> fingerBase;
		
		fingerBase = dynamic_cast<osg::PositionAttitudeTransform*>(_getNamedChild(hand, "thumb0").get());
		assert(fingerBase.valid());
		_joints.push_back(_findJoints(fingerBase));
		
		fingerBase = dynamic_cast<osg::PositionAttitudeTransform*>(_getNamedChild(hand, "index0").get());
		assert(fingerBase.valid());
		_joints.push_back(_findJoints(fingerBase));
		
		fingerBase = dynamic_cast<osg::PositionAttitudeTransform*>(_getNamedChild(hand, "middle0").get());
		assert(fingerBase.valid());
		_joints.push_back(_findJoints(fingerBase));
		
		fingerBase = dynamic_cast<osg::PositionAttitudeTransform*>(_getNamedChild(hand, "ring0").get());
		assert(fingerBase.valid());
		_joints.push_back(_findJoints(fingerBase));
		
		fingerBase = dynamic_cast<osg::PositionAttitudeTransform*>(_getNamedChild(hand, "pinky0").get());
		assert(fingerBase.valid());
		_joints.push_back(_findJoints(fingerBase));
		
		{
			osg::ref_ptr<osg::MatrixTransform> handedness = new osg::MatrixTransform;
			handedness->addChild(model);
			_leftyrighty->addChild(handedness);
		
			handedness = new osg::MatrixTransform;
			handedness->addChild(model);
			_leftyrighty->addChild(handedness);
		}
		
		{
			/// Model is right hand by default, so leave right-hand xform as identity.
			osg::ref_ptr<osg::MatrixTransform> leftXform = dynamic_cast<osg::MatrixTransform*>(_leftyrighty->getChild(Glove::LEFT_HAND));
		
			/* multiplying by:
			1	0	0	0
			0	-1	0	0
			0	0	1	0
			0	0	0	1
			should flip across the y axis */
			leftXform->setMatrix(
					osg::Matrix(1, 0, 0, 0,
					0, -1, 0, 0,
					0, 0, 1, 0,
					0, 0, 0, 1)
					);
		}
		
		/// Set up handedness right the first time
		_leftyrighty->setSingleChildOn(_g.getHand());

		this->addChild(_leftyrighty.get());
	}
	
	void GloveNode::doUpdate() {
		/// Update Handedness
		_leftyrighty->setSingleChildOn(_g.getHand());

		// using the data from _g, update the transforms for the joints
		_updateFinger(THUMB);
		_updateFinger(INDEX_FINGER);
		_updateFinger(MIDDLE_FINGER);
		_updateFinger(RING_FINGER);
		_updateFinger(PINKY_FINGER);
	}

	void GloveNode::_updateFinger(Finger finger) {
		/// @todo adjust scale here
		/// Scales chosen here are just based on "what looks right"
		double fingerAngle = _g.getBend(finger) * 1.5;
		if (finger == THUMB) {
			/// Thumb doesn't bend as far
			fingerAngle /= 1.5;
		}
		for (unsigned int i = 0; i < _joints[finger].size(); i++) {
			double jointAngle = fingerAngle;
			if (i == 0) {
				/// First joint on every finger doesn't bend as far
				jointAngle /= 1.3;
			}
			_joints[finger][i]->setAttitude(osg::Quat(jointAngle, _getJointAxis(finger, i))); //rotate around X axis
		}
	}
	
	GloveNode::JointList GloveNode::_findJoints(osg::ref_ptr<osg::PositionAttitudeTransform> const& parent) {
		JointList singleFinger;
		osg::ref_ptr<osg::PositionAttitudeTransform> joint = parent;
		
		while (joint.valid()) {
			//set pivot point so the joints don't jump around
			const osg::BoundingSphere & bs = joint->getBound();
			osg::Vec3d pivot(bs.center().x(),
				bs.center().y() + 0.01,
				bs.center().z() - 0.7 * bs.radius()); /// Scale radius to not pivot around the very bottom of the bounds.


			//std::cout << "Radius: " << bs.radius() <<  ", Pivot at " << pivot.x() << ", " << pivot.y() << ", " << pivot.z() << std::endl;
			joint->setPivotPoint(pivot);
			joint->setPosition(pivot);
			singleFinger.push_back(joint);
			joint = _getChildTransform(joint);
		}
		//std::cout << "Finger starting with " << parent->getName() << " has " << singleFinger.size() << " joints." << std::endl;
		return singleFinger;	
	}
	
	osg::ref_ptr<osg::PositionAttitudeTransform> GloveNode::_getChildTransform(osg::ref_ptr<osg::Group> const& parent) {
		osg::ref_ptr<osg::PositionAttitudeTransform> child;
		for (unsigned int i = 0; i < parent->getNumChildren(); ++i) {
			child = dynamic_cast<osg::PositionAttitudeTransform*>(parent->getChild(i));
			if (child.valid()) {
				return child;
			}
		}
		return child;
	}
	
	osg::ref_ptr<osg::Node> GloveNode::_getNamedChild(osg::ref_ptr<osg::Group> const& parent, std::string const& name) {
		osg::ref_ptr<osg::Node> nullPtr;
		if (!parent.valid()) {
			return nullPtr;
		}
		const unsigned int n = parent->getNumChildren();
		for (unsigned int i = 0; i < n; ++i) {
			osg::ref_ptr<osg::Node> child = parent->getChild(i);
			if (child->getName() == name) {
				return child;
			}
		}
	
		return nullPtr;
	}
}
