/**
	@file GloveHardwareVRPN.cpp
	@brief

	@date 2011

	@author
	Ryan Pavlik
	<rpavlik@iastate.edu> and <abiryan@ryand.net>
	http://academic.cleardefinition.com/
	Iowa State University Virtual Reality Applications Center
	Human-Computer Interaction Graduate Program
*/

// Internal Includes
#include "GloveHardwareVRPN.h"

// Library/third-party includes
#include <vrpn_Analog.h>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

// Standard includes
#include <iostream>

namespace glove {
	namespace detail {
		struct GloveHardwareVRPNDevice {
			GloveHardwareVRPNDevice() : ana(NULL){}
			~GloveHardwareVRPNDevice() { delete ana; ana = NULL; }
			vrpn_Analog_Remote * ana;
		};
	}

	static void VRPN_CALLBACK handle_analog(void* userdata, const vrpn_ANALOGCB a) {
		std::vector<double> * v = static_cast<std::vector<double> *>(userdata);
		v->clear();
		for (unsigned int i = 0; i < a.num_channel; ++i) {
			v->push_back(static_cast<double>(a.channel[i]));
		}
	}

	GloveHardwarePtr GloveHardwareVRPN::create(std::string const & option) {
		GloveHardwarePtr temp(new GloveHardwareVRPN(option));
		return temp;
	}

	GloveHardwareVRPN::GloveHardwareVRPN(std::string const & option) :
			_d(new detail::GloveHardwareVRPNDevice),
			_minResponseSize(5),
			_max(1.0),
			_min(0.0) {
		// For the option parameter, specify:
		//  - the vrpn device name including machine (with @)
		//  - optionally the word "variance" follow by the raw variance for
		//    each digit starting with the thumb
		//  - optionally the word "channels" followed by the channel numbers for
		//    the digits starting with the thumb
		//  - optionally RIGHT or LEFT as desired.

		for (unsigned int i = 0; i < 5; ++i) {
			_channelMap.push_back(i);
		}

		std::string devname;
		std::vector<double> variance;

		std::vector<std::string> strs;
		boost::split(strs, option, boost::is_any_of("\t ;,"));
		for (unsigned int i = 0; i < strs.size(); ++i) {
			std::string opt = strs[i];
			std::string optCaps = boost::to_upper_copy(strs[i]);
			if (optCaps.find("@") != std::string::npos) {
				devname = strs[i];
			} else if (optCaps == "RIGHT") {
				_setHand(RIGHT_HAND);
			} else if (optCaps == "LEFT") {
				_setHand(LEFT_HAND);
			} else if (optCaps == "CHANNELS") {
				i++;
				int maxChan = 0;
				for (unsigned int j = 0; j < 5; ++j, ++i) {
					if (i >= strs.size()) {
						// got to specify them all!
						throw MissingGloveOptionError();
					}
					_channelMap[j] = boost::lexical_cast<int>(strs[i]);
					if (_channelMap[j] > maxChan) {
						maxChan = _channelMap[j];
					}
				}
				_minResponseSize = maxChan + 1;
			} else if (optCaps == "VARIANCE") {
				i++;
				for (unsigned int j = 0; j < 5; ++j, ++i) {
					if (i >= strs.size()) {
						// got to specify them all!
						throw MissingGloveOptionError();
					}
					variance.push_back(boost::lexical_cast<double>(strs[i]));
				}
			} else {
				std::cerr << "ERROR: Unrecognized option! '" << opt << std::endl;
				throw InvalidGloveOptionError();
			}
		}

		if (devname.empty()) {
			throw MissingGloveOptionError();
		}

		std::cout << "Connecting to vrpn analog device " << devname << std::endl;
		std::cout << "Channel mapping:" << std::endl;
		std::cout << "  - Thumb:  " << _channelMap[0] << std::endl;
		std::cout << "  - Index:  " << _channelMap[1] << std::endl;
		std::cout << "  - Middle: " << _channelMap[2] << std::endl;
		std::cout << "  - Ring:   " << _channelMap[3] << std::endl;
		std::cout << "  - Pinky:  " << _channelMap[4] << std::endl;
		std::cout << std::endl;
		std::cout << "with 0.0 bend mapped from value of " << _min;
		std::cout << " and 1.0 bend mapped from value of " << _max << std::endl;



		_d->ana = new vrpn_Analog_Remote(devname.c_str());
		if (!_d->ana) {
			throw GloveConnectionError();
		}
		_d->ana->register_change_handler(&_channels, &handle_analog);

		if (!variance.empty()) {
			_setRawVariance(variance);
		}
	}

	GloveHardwareVRPN::~GloveHardwareVRPN() {

		if (_d->ana) {
			_d->ana->unregister_change_handler(&_channels, &handle_analog);
		}

		delete _d;
		_d = NULL;
	}


	void GloveHardwareVRPN::updateData() {
		_d->ana->mainloop();
		if (!_channels.empty()) {
			if (_channels.size() < _minResponseSize) {
				std::cerr << "Got a response with " << _channels.size() << " channels but need " << _minResponseSize << std::endl;
				_channels.clear();
				return;
			}
			// Update stored bend values
			for (unsigned int i = 0; i < 5; ++i)
			{
				double data = _channels[_channelMap[i]];
				double bend = (data - _min) / (_max - _min);
				_setBend(Finger(i), bend, bend);
			}
			_channels.clear();
		}
	}
} // end of namespace glove
