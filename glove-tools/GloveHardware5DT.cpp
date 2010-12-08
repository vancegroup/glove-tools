/**
	@file GloveHardware5DT.cpp
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
#include "GloveHardware5DT.h"

// Library/third-party includes
// - none

// Standard includes
#include <string>
#include <iostream>

namespace glove {
	GloveHardwarePtr GloveHardware5DT::create(std::string const & option) {
		GloveHardwarePtr temp(new GloveHardware5DT(option));
		return temp;
	}

	GloveHardware5DT::GloveHardware5DT(std::string const & option) :
			_fd(NULL) {
			// For the option parameter, please specify the COM port of the device
			// Valid inputs are COM1 through COM8
			// Note, this may be different under Linux

			// Open 5DT glove
			_fd = fdOpen(const_cast<std::string&>option.c_str()); //@todo <- how to remove constnes?
			if (!_fd)
			{
				std::cerr << "WARNING: Unable to open 5DT data glove" << std::endl;
			}

			// Reset the calibration settings
			fdResetCalibration(_fd);
	}

	GloveHardware5DT::~GloveHardware5DT() {
		if (_fd) {
			// Close the 5DT glove
			if (fdClose(_fd) == 0)
			{
				std::cerr << "WARNING: Unable to close 5DT data glove" << std::endl;
			}
		}
	}

	bool GloveHardware5DT::isRightHanded()
	{
		if (_fd) {
			// Determines the handedness of the glove and returns true if right handed
			if (FD_HAND_RIGHT == fdGetGloveHand(_fd))
				return true;
			else if (FD_HAND_LEFT == fdGetGloveHand(_fd))
				return false;
		}
		return NULL;
	}

	std::string GloveHardware5DT::returnGloveType()
	{
		if (_fd) {
			// Returns a string of the 5DT device type
			if (fdGetGloveType(_fd) == FD_GLOVE7)
			{
				return std::string("5DT Data Glove 5");
			}
			else if (fdGetGloveType(_fd) == FD_GLOVE7W)
			{
				return std::string("Wireless 5DT Data Glove 5");
			}
			else if (fdGetGloveType(_fd) == FD_GLOVE16)
			{
				return std::string("5DT Data Glove 16");
			}
			else if (fdGetGloveType(_fd) == FD_GLOVE16W)
			{
				return std::string("Wireless 5DT Data Glove 16");
			}
			else if (fdGetGloveType(_fd) == FD_GLOVENONE)
			{
				return std::string("No 5DT Glove");
			}
		}
		return NULL;
	}

	int GloveHardware5DT::returnNumSensors()
	{
		if (_fd) {
			// This returns the number of sensors on the device
			// The 16 sensor model actually returns 18 since it also counts the tilt sensors
			return fdGetNumSensors(_fd);
		}
		return NULL;
	}

	void GloveHardware5DT::resetGloveCalibration()
	{
		if (_fd) {
			// Resets glove calibration to default settings
			fdResetCalibration(_fd);
		}
	}

	void GloveHardware5DT::updateData() {
		// Update stored bend values
		for (unsigned int i = 0; i < 5; ++i)
		{
			//@TODO: do stuff
			// use float fdGetSensorScaled(fdGlove *pFG, int nSensor)
		}
	}
}