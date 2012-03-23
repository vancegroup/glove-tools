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
	osg::ArgumentParser arguments(&argc, argv);

	arguments.getApplicationUsage()->setApplicationName(arguments.getApplicationName());
	arguments.getApplicationUsage()->setDescription(arguments.getApplicationName() + " shows a dummy hand.");
	arguments.getApplicationUsage()->setCommandLineUsage(arguments.getApplicationName() + " [options] ...");
	arguments.getApplicationUsage()->addCommandLineOption("--device <type>", "Choose a different device type.");
	arguments.getApplicationUsage()->addCommandLineOption("--option <option>", "Pass an option to the GloveHardware driver - like an address/port.");
	arguments.getApplicationUsage()->addCommandLineOption("--raw", "Use raw values.");
	arguments.getApplicationUsage()->addCommandLineOption("--calib", "Use values calibrated by glovetools.");
	arguments.getApplicationUsage()->addCommandLineOption("--filter", "Use values processed by the Kalman filter.");
	arguments.getApplicationUsage()->addCommandLineOption("--load-calib <filename>", "Load calibration values from a file (implies --calib).");

	osgViewer::Viewer viewer;

	unsigned int helpType = 0;
	if ((helpType = arguments.readHelpType())) {
		arguments.getApplicationUsage()->write(std::cout, helpType);
		return 1;
	}

	// Fake glove for testing visualization
	std::string deviceType = "FakeFlexingGloveHardware";
	std::string deviceOption = "";

	while (arguments.read("--device", deviceType)) {}
	while (arguments.read("--option", deviceOption)) {}

	Glove::ReportType r = Glove::REPORT_HWSCALED;

	std::string loadCalib;
	while (arguments.read("--load-calib", loadCalib)) {
		/// implies calib
		r = Glove::REPORT_CALIBRATED;
	}

	while (arguments.read("--raw")) {
		r = Glove::REPORT_RAW;
	}

	while (arguments.read("--calib")) {
		r = Glove::REPORT_CALIBRATED;
	}

	while (arguments.read("--filter")) {
		r = Glove::REPORT_FILTERED;
	}

	// any option left unread are converted into errors to write out later.
	arguments.reportRemainingOptionsAsUnrecognized();

	// report any errors if they have occurred when parsing the program arguments.
	if (arguments.errors()) {
		arguments.writeErrorMessages(std::cout);
		return 1;
	}


	/// The Important Part is Here!
	GloveHardwarePtr hardware;

	try {
		std::cout << "Trying to open glove hardware..." << std::endl;
		std::cout << "Device type: '" << deviceType << "'" << std::endl;
		std::cout << "Device option: '" << deviceOption << "'" << std::endl;
		GloveHardwarePtr tmp(GloveHardwareFactory::instance()->createByName(deviceType, deviceOption));
		hardware = tmp;
	} catch (std::exception & e) {
		std::cerr << "Could not open glove hardware!  Error was: " << e.what() << std::endl;
		exit(1);
	}

	std::cout << "Connection successful!" << std::endl;
	Glove g(hardware);

	bool didSetReport = g.setReportType(r);
	if (!didSetReport) {
		std::cerr << "Warning: could not set report type - usually this means you chose 'filter' but didn't build with Kalman filter turned on" << std::endl;
	}

	std::cout << "Glove will report ";
	switch (g.getReportType()) {
			/// raw values from the device
		case Glove::REPORT_RAW:
			std::cout << "raw values from the hardware" << std::endl;
			break;

			/// values scaled by the device's hardware/drivers
		case Glove::REPORT_HWSCALED:
			std::cout << "values scaled by the hardware/driver (default)." << std::endl;
			break;

			/// values scaled by glove-tools
		case Glove::REPORT_CALIBRATED:
			std::cout << "values calibrated by glovetools" << std::endl;
			break;

			/// values scaled and filtered by glove-tools
		case Glove::REPORT_FILTERED:
			std::cout << "values processed by the Kalman filter" << std::endl;
			break;
		default:
			std::cout << "an unknown report type - BUG - contact the glovetools authors!" << std::endl;
	}

	if (!loadCalib.empty()) {
		std::ifstream calibFile(loadCalib.c_str());
		if (calibFile.good()) {
			g.loadCalibration(calibFile);
		} else {
			std::cerr << "WARNING: could not load calibration file " << loadCalib << std::endl;
		}
	}


	osg::ref_ptr<osg::Group> root = new osg::Group();
	root->addChild(g.getNode());

	/// In an app this simple (no physics loop, etc) we can just update the Glove object (and hardware) during the scenegraph update.
	osg::ref_ptr<GloveDeviceUpdater> deviceUpdater = new GloveDeviceUpdater(g);
	root->setUpdateCallback(deviceUpdater.get());

	std::cout << "Running viewer..." << std::endl << std::endl;
	viewer.setSceneData(root.get());

	viewer.addEventHandler(new osgViewer::ScreenCaptureHandler);
	viewer.setUpViewInWindow(15, 15, 320, 240);
	viewer.realize();
	return viewer.run();
}
