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

// Standard includes
#include <cassert>
#include <iostream>

namespace glove {
	GloveNode::GloveNode(Glove const & g) :
			_g(g),
			_updater(new GloveUpdater(g)) {
		/// @todo load the model and set the updater here

		osg::ref_ptr<osg::Node> hand = osgDB::readNodeFile("hand-structured.osg");
		assert(hand.valid());
		
		/// Grab the meaningful parent node.
		osg::ref_ptr<osg::Group> root = hand->asGroup();
		while (root->getName() != "hand") {
			assert(root->getNumChildren() == 1);
			root = root->getChild(0)->asGroup(); // only has 1 child
		}

		osg::ref_ptr<osg::MatrixTransform> fingerBase;
		
		fingerBase = dynamic_cast<osg::MatrixTransform*>(_getNamedChild(root, "thumb0").get());
		assert(fingerBase.valid());
		_joints.push_back(_findJoints(fingerBase));
		
		fingerBase = dynamic_cast<osg::MatrixTransform*>(_getNamedChild(root, "index0").get());
		assert(fingerBase.valid());
		_joints.push_back(_findJoints(fingerBase));
		
		fingerBase = dynamic_cast<osg::MatrixTransform*>(_getNamedChild(root, "middle0").get());
		assert(fingerBase.valid());
		_joints.push_back(_findJoints(fingerBase));
		
		fingerBase = dynamic_cast<osg::MatrixTransform*>(_getNamedChild(root, "ring0").get());
		assert(fingerBase.valid());
		_joints.push_back(_findJoints(fingerBase));
		
		fingerBase = dynamic_cast<osg::MatrixTransform*>(_getNamedChild(root, "pinky0").get());
		assert(fingerBase.valid());
		_joints.push_back(_findJoints(fingerBase));

		hand->setUpdateCallback(_updater.get());

		this->addChild(hand);
	}
	
	void GloveNode::update() {
		/// @todo implement me!
		std::cout << "Implement this function! " << __FUNCTION__ << std::endl;
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
