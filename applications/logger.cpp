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
#include <glove-tools/IGloveHardware.h>
#include <glove-tools/GloveHardwareFactory.h>

// Library/third-party includes
#include <osg/ArgumentParser>

// Standard includes
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime>

static void sleepSeconds(double seconds) {
	std::clock_t done = clock() + seconds * CLOCKS_PER_SEC;
	while (clock() < done) {}
}

using namespace glove;

int main(int argc, char * argv[]) {

	// use an ArgumentParser object to manage the program arguments.
	osg::ArgumentParser arguments(&argc,argv);

	arguments.getApplicationUsage()->setApplicationName(arguments.getApplicationName());
	arguments.getApplicationUsage()->setDescription(arguments.getApplicationName()+" logs hand data to file.");
	arguments.getApplicationUsage()->setCommandLineUsage(arguments.getApplicationName()+" [options] ...");
	arguments.getApplicationUsage()->addCommandLineOption("--device <type>","Choose a different device type.");
	arguments.getApplicationUsage()->addCommandLineOption("--option <option>","Pass an option to the GloveHardware driver - like an address/port.");
	arguments.getApplicationUsage()->addCommandLineOption("--max <type>","Specify the max number of lines to write to the file.");
	arguments.getApplicationUsage()->addCommandLineOption("--file <type>","Specify the filename in CSV format.");

	unsigned int helpType = 0;
	if ((helpType = arguments.readHelpType()))
	{
		arguments.getApplicationUsage()->write(std::cout, helpType);
		return 1;
	}

	// Fake glove for testing
	std::string deviceType = "FakeFlexingGloveHardware";
	std::string deviceOption = "";
	std::string filename = "log.csv";
	unsigned int maxSamples = 300;

	while (arguments.read("--device", deviceType)) {}
	while (arguments.read("--option", deviceOption)) {}
	while (arguments.read("--max", maxSamples)) {}
	while (arguments.read("--file", filename)) {}

	// any option left unread are converted into errors to write out later.
	arguments.reportRemainingOptionsAsUnrecognized();

	// report any errors if they have occurred when parsing the program arguments.
	if (arguments.errors())
	{
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

	std::cout << "Connection successful! Startup is continuing..." << std::endl;
	Glove g(hardware);

	// Open file
	std::ofstream outfile (filename.c_str());
	if (!outfile.is_open())
		std::cerr << "Unable to open file for writing" << std::endl;
	
	outfile << "Timestep, Thumb, Index, Middle, Ring, Pinky\n";
	for (unsigned int i = 0; i < maxSamples; ++i) {
		hardware->updateData();
		g.updateData();
		/// log to file here in csv format
		outfile << i << ", " << g.getBend(Finger(THUMB)) << ", " << g.getBend(Finger(INDEX_FINGER)) << ", " 
		<< g.getBend(Finger(MIDDLE_FINGER)) << ", " << g.getBend(Finger(RING_FINGER)) << ", " << g.getBend(Finger(PINKY_FINGER)) << "\n";
		sleepSeconds(1.0/1000.0);
	}

	// Close file
	outfile.close();
	std::cout << "Finished writing to file." << std::endl;

	return 0;
}