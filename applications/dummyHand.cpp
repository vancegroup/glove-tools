/** @file	dummyHand.cpp
	@brief	Simple viewer app

	@date 	2010

	@author
	Ryan Pavlik ( <rpavlik@iastate.edu> http://academic.cleardefinition.com/ ),
	Iowa State University
	Virtual Reality Applications Center and
	Human-Computer Interaction Graduate Program
*/

// Internal Includes
#include <glove-tools/Glove.h>
#include <glove-tools/GloveUpdater.h>
#include <glove-tools/GloveHardwareFactory.h>

// Library/third-party includes
#include <osgUtil/Optimizer>

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

// Standard includes
#include <iostream>
#include <string>

using namespace glove;

int main(int argc, char * argv[]) {

	// use an ArgumentParser object to manage the program arguments.
	osg::ArgumentParser arguments(&argc,argv);

	arguments.getApplicationUsage()->setApplicationName(arguments.getApplicationName());
	arguments.getApplicationUsage()->setDescription(arguments.getApplicationName()+" shows a dummy hand.");
	arguments.getApplicationUsage()->setCommandLineUsage(arguments.getApplicationName()+" [options] ...");
	arguments.getApplicationUsage()->addCommandLineOption("--device <type>","Choose a different device type.");
	arguments.getApplicationUsage()->addCommandLineOption("--option <option>","Pass an option to the GloveHardware driver - like an address/port.");

	osgViewer::Viewer viewer;

	unsigned int helpType = 0;
	if ((helpType = arguments.readHelpType()))
	{
		arguments.getApplicationUsage()->write(std::cout, helpType);
		return 1;
	}

	std::string deviceType = "FakeFlexingGloveHardware";
	std::string deviceOption = "";

	while (arguments.read("--device", deviceType)) {}
	while (arguments.read("--option", deviceOption)) {}

	// any option left unread are converted into errors to write out later.
	arguments.reportRemainingOptionsAsUnrecognized();

	// report any errors if they have occurred when parsing the program arguments.
	if (arguments.errors())
	{
		arguments.writeErrorMessages(std::cout);
		return 1;
	}


	/// The Important Part is Here!	
	GloveHardwarePtr hardware(GloveHardwareFactory::instance()->createByName(deviceType, deviceOption));
	Glove g(hardware);

	osg::ref_ptr<osg::Group> root = new osg::Group();
	root->addChild(g.getNode());

	/// In an app this simple (no physics loop, etc) we can just update the Glove object (and hardware) during the scenegraph update.
	osg::ref_ptr<GloveDeviceUpdater> deviceUpdater = new GloveDeviceUpdater(g);
	root->setUpdateCallback(deviceUpdater.get());

	std::cout << "Running viewer..." << std::endl << std::endl;
	viewer.setSceneData(root.get());

	viewer.addEventHandler(new osgViewer::ScreenCaptureHandler);

	viewer.realize();
	return viewer.run();
}
