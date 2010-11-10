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
#include <vector>

namespace glove {
	GloveNode::GloveNode(Glove const & g) :
			_g(g),
			_updater(new GloveUpdater(g)) {
		/// @todo load the model and set the updater here

		osg::ref_ptr<osg::Node> hand = osgDB::readNodeFile("hand-structured.osg");
		assert(hand.valid());

		// Setup the vector
		/*osg::ref_ptr<osg::MatrixTransform> empty;
		_joints[THUMB].push_back(empty);
		_joints[INDEX_FINGER].push_back(empty);
		_joints[MIDDLE_FINGER].push_back(empty);
		_joints[RING_FINGER].push_back(empty);
		_joints[PINKY_FINGER].push_back(empty);*/

		/*for (unsigned int i = 0; i < 5; i++)
		{
			std::vector<osg::ref_ptr<MatrixTransform> > singleFinger;
			osg::ref_ptr<osg::MatrixTransform> joint;

			joint = dynamic_cast<osg::MatrixTransform*>(hand->getChild(i));
			singleFinger.push_back(joint);
			joint = dynamic_cast<osg::MatrixTransform*>(hand->getChild(i)->getChild(0));
			singleFinger.push_back(joint);
			joint = dynamic_cast<osg::MatrixTransform*>(hand->getChild(i)->getChild(0)->getChild(0));
			singleFinger.push_back(joint);

			_joints.push_back(singleFinger);

			singleFinger.clear()
		}*/

		hand->setUpdateCallback(_updater.get());

		this->addChild(hand);
	}

}
