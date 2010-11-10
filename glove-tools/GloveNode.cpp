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

// Standard includes
#include <cassert>
#include <iostream>

namespace glove {
	GloveNode::GloveNode(Glove const & g) :
			_g(g),
			_updater(new GloveUpdater(g)),
			_leftyrighty(new osg::Switch) {
		/// load the model and set the updater here

		osg::ref_ptr<osg::Node> model = osgDB::readNodeFile("hand-structured.osg");
		assert(model.valid());
		
		/// Grab the meaningful parent node.
		osg::ref_ptr<osg::Group> hand = model->asGroup();
		while (hand->getName() != "hand") {
			assert(hand->getNumChildren() == 1);
			hand = hand->getChild(0)->asGroup(); // only has 1 child
		}

		osg::ref_ptr<osg::MatrixTransform> fingerBase;
		
		fingerBase = dynamic_cast<osg::MatrixTransform*>(_getNamedChild(hand, "thumb0").get());
		assert(fingerBase.valid());
		_joints.push_back(_findJoints(fingerBase));
		
		fingerBase = dynamic_cast<osg::MatrixTransform*>(_getNamedChild(hand, "index0").get());
		assert(fingerBase.valid());
		_joints.push_back(_findJoints(fingerBase));
		
		fingerBase = dynamic_cast<osg::MatrixTransform*>(_getNamedChild(hand, "middle0").get());
		assert(fingerBase.valid());
		_joints.push_back(_findJoints(fingerBase));
		
		fingerBase = dynamic_cast<osg::MatrixTransform*>(_getNamedChild(hand, "ring0").get());
		assert(fingerBase.valid());
		_joints.push_back(_findJoints(fingerBase));
		
		fingerBase = dynamic_cast<osg::MatrixTransform*>(_getNamedChild(hand, "pinky0").get());
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

		this->setUpdateCallback(_updater.get());

		this->addChild(_leftyrighty.get());
	}
	
	void GloveNode::update() {
		std::cout << "Implement this function! " << __FUNCTION__ << std::endl;

		/// Update Handedness
		_leftyrighty->setSingleChildOn(_g.getHand());

		// using the data from _g, update the transforms for the joints
		updateFinger(THUMB);
		updateFinger(INDEX_FINGER);
		updateFinger(MIDDLE_FINGER);
		updateFinger(RING_FINGER);
		updateFinger(PINKY_FINGER);
	}

	void GloveNode::updateFinger(Finger finger) {
		/*double fingerAngle = _g.getBend(finger) * 0.5;
		for (unsigned int i = 0; i < _joints[finger].size(); i++) {
			osg::Matrix m(osg::Matrix::rotate(fingerAngle, osg::Vec3f(1.0f, 0.0f, 0.0f))); //rotate around X axis
			_joints[finger][i]->setMatrix(m);
		}*/
	}
	
	GloveNode::JointList GloveNode::_findJoints(osg::ref_ptr<osg::MatrixTransform> const& parent) {
		JointList singleFinger;
		osg::ref_ptr<osg::MatrixTransform> joint = parent;
		
		while (joint.valid()) {
			singleFinger.push_back(joint);
			joint = _getChildTransform(joint);
		}
		std::cout << "Finger starting with " << parent->getName() << " has " << singleFinger.size() << " joints." << std::endl;
		return singleFinger;	
	}
	
	osg::ref_ptr<osg::MatrixTransform> GloveNode::_getChildTransform(osg::ref_ptr<osg::Group> const& parent) {
		osg::ref_ptr<osg::MatrixTransform> child;
		for (unsigned int i = 0; i < parent->getNumChildren(); ++i) {
			child = dynamic_cast<osg::MatrixTransform*>(parent->getChild(i));
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
