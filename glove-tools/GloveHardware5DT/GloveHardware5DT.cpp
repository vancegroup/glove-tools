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
#include <boost/algorithm/string.hpp>

#define LPVOID void *
#include <fglove.h>
#undef LPVOID

// Standard includes
#include <sstream>
#include <iostream>

namespace glove {
	namespace detail {
		struct GloveHardware5DTDevice {
			GloveHardware5DTDevice() : _fd(NULL) {}
			fdGlove * _fd;
		};
	}

	GloveHardwarePtr GloveHardware5DT::create(std::string const & option) {
		GloveHardwarePtr temp(new GloveHardware5DT(option));
		return temp;
	}

	static int _numUSBGloves() {
#define MAXUSB 4
		unsigned short aPID[MAXUSB];
		int numMax = MAXUSB;
#undef MAXUSB
		return fdScanUSB(aPID, numMax);
	}

	GloveHardware5DT::GloveHardware5DT(std::string const & option) :
			_d(new detail::GloveHardware5DTDevice),
			_raw(false) {
		// For the option parameter, please specify either the COM port or USB port of the device
		// Valid inputs are "COM1" through "COM8" for serial COM
		// and "USB0" through "USB3" for USB
		// Note, these values may be different under Linux

		std::string devname;
		std::vector<std::string> strs;
		boost::split(strs, option, boost::is_any_of("\t ;,"));
		for(unsigned int i = 0; i < strs.size(); ++i) {
			std::string opt = strs[i];
			std::string optCaps = boost::to_upper_copy(strs[i]);
			if (optCaps.find("USB") != std::string::npos || optCaps.find("COM") != std::string::npos) {
				devname = strs[i];
			} else if (optCaps == "RAW") {
				std::cout << "Switching GloveHardware5DT into RAW mode..." << std::endl;
				_raw = true;
			} else {
				std::cerr << "ERROR: Unrecognized option! '" << opt << std::endl;
				throw new InvalidGloveOptionError;
			}
		}

		if (devname.empty()) {
			/// By default, look on USB.
			static int nextUSB = 0;
			std::stringstream s;
			s << "USB" << nextUSB;
			devname = s.str();
			nextUSB++;
			std::cerr << "No port specified for GloveHardware5DT, defaulting to " << devname << std::endl;
		}

		if (devname.find("USB") != std::string::npos) {
			// First check and see if there are any USB devices available

			if (_numUSBGloves() == 0) {
				std::cerr << "ERROR: No 5DT USB gloves connected" << std::endl;
				throw new NoUSBGlove5DTFound;
			}

			// Try to open 5DT glove on USB port
			_d->_fd = fdOpen(const_cast<char*>(devname.c_str()));
			if (!_d->_fd) {
				std::cerr << "ERROR: Unable to open 5DT data glove on USB port" << std::endl;
				throw new USBGlove5DTConnectionFailed;
			}
		} else {
			// Try to open 5DT glove on serial port
			_d->_fd = fdOpen(const_cast<char*>(devname.c_str()));
			if (!_d->_fd) {
				std::cerr << "ERROR: Unable to open 5DT data glove on serial port" << std::endl;
				throw new SerialGlove5DTConnectionFailed;
			}
		}

		switch (fdGetGloveHand(_d->_fd)) {
			case FD_HAND_RIGHT:
				_setHand(RIGHT_HAND);
				break;
			case FD_HAND_LEFT:
				_setHand(LEFT_HAND);
				break;
			default:
				/// No idea what happened here
				throw new std::runtime_error("Should never happen: fdGetGloveHand didn't return a value we could handle!");
		}

		// Reset the calibration settings
		resetGloveCalibration();

		/// @todo do these vary significantly from glove to glove?
		const double varianceScale = 1.0;

		/// Tight fist, Raw input of 5000 steps
		std::vector<double> variance;
		variance.push_back(0.0000015335 * varianceScale);
		variance.push_back(0.000000324218 * varianceScale);
		variance.push_back(0.000000363943 * varianceScale);
		variance.push_back(0.000000280055 * varianceScale);
		variance.push_back(0.000000139414 * varianceScale);
		_setRawVariance(variance);
	}

	GloveHardware5DT::~GloveHardware5DT() {
		if (_d->_fd) {
			// Close the 5DT glove
			if (fdClose(_d->_fd) == 0)
			{
				std::cerr << "WARNING: Unable to close 5DT data glove" << std::endl;
			}
		}
		delete _d;
		_d = NULL;
	}

	std::string GloveHardware5DT::returnGloveType()
	{
		if (_d->_fd) {
			// Returns a string of the 5DT device type
			if (fdGetGloveType(_d->_fd) == FD_GLOVE7)
			{
				return std::string("5DT Data Glove 5");
			}
			else if (fdGetGloveType(_d->_fd) == FD_GLOVE7W)
			{
				return std::string("Wireless 5DT Data Glove 5");
			}
			else if (fdGetGloveType(_d->_fd) == FD_GLOVE5U)
			{
				return std::string("5DT Data Glove 5 Ultra on Serial Port");
			}
			else if (fdGetGloveType(_d->_fd) == FD_GLOVE5UW)
			{
				return std::string("Wireless 5DT Data Glove 5 Ultra on Serial Port");
			}
			else if (fdGetGloveType(_d->_fd) == FD_GLOVE5U_USB)
			{
				return std::string("5DT Data Glove 5 Ultra on USB");
			}
			else if (fdGetGloveType(_d->_fd) == FD_GLOVE16)
			{
				return std::string("5DT Data Glove 16");
			}
			else if (fdGetGloveType(_d->_fd) == FD_GLOVE16W)
			{
				return std::string("Wireless 5DT Data Glove 16");
			}
			else if (fdGetGloveType(_d->_fd) == FD_GLOVE14U)
			{
				return std::string("5DT Data Glove 14 Ultra");
			}
			else if (fdGetGloveType(_d->_fd) == FD_GLOVE14UW)
			{
				return std::string("Wireless 5DT Data Glove 14 Ultra");
			}
			else if (fdGetGloveType(_d->_fd) == FD_GLOVE14U_USB)
			{
				return std::string("5DT Data Glove 14 Ultra on USB");
			}
			else if (fdGetGloveType(_d->_fd) == FD_GLOVENONE)
			{
				return std::string("No 5DT Glove");
			}
		}
		return NULL;
	}

	int GloveHardware5DT::returnNumSensors()
	{
		if (_d->_fd) {
			// This returns the number of sensors on the device
			// The 16 sensor model actually returns 18 since it also counts the tilt sensors
			return fdGetNumSensors(_d->_fd);
		}
		return NULL;
	}

	void GloveHardware5DT::resetGloveCalibration()
	{
		if (_d->_fd) {
			// Resets glove calibration to default settings
			fdResetCalibration(_d->_fd);
		}
	}

	void GloveHardware5DT::updateData() {
		// Update stored bend values
		if (_d->_fd) {
			if (fdGetGloveType(_d->_fd) == FD_GLOVE7 || fdGetGloveType(_d->_fd) == FD_GLOVE7W || fdGetGloveType(_d->_fd) == FD_GLOVE5U 
				|| fdGetGloveType(_d->_fd) == FD_GLOVE5UW || fdGetGloveType(_d->_fd) == FD_GLOVE5U_USB)
			{
				// 5 total sensors for all fingers
				for (unsigned int i = 0, j = 0; i <= 12; i+=3, j++)
				{
					unsigned short rawVal = fdGetSensorRaw(_d->_fd, i);
					/// Convert to a floating-point number by dividing by the max value from the sensor (12-bit unsigned -> 0 to 4095)
					double raw = static_cast<double>(rawVal)/4095.0;
					double bend = fdGetSensorScaled(_d->_fd, i);
					_setBend(Finger(j), bend, raw);
				}
			}
			else if (fdGetGloveType(_d->_fd) == FD_GLOVE16 || fdGetGloveType(_d->_fd) == FD_GLOVE16W || fdGetGloveType(_d->_fd) == FD_GLOVE14U 
				|| fdGetGloveType(_d->_fd) == FD_GLOVE14UW || fdGetGloveType(_d->_fd) == FD_GLOVE14U_USB)
			{
				// 14 total sensors for all fingers
				// @TODO: implement me, we don't have these gloves to test
				std::cerr << "WARNING: This device isn't implemented yet." << std::endl;
			}
		}
	}
}
