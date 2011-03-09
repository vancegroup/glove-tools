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
#include <boost/thread.hpp>

//#include <boost/thread/locks.hpp>

// Standard includes
#include <iostream>

namespace glove {
	namespace detail {
		struct GloveHardwareVRPNDevice {
			GloveHardwareVRPNDevice() : ana(NULL), stopflag(false), thread(NULL) {}
			~GloveHardwareVRPNDevice() {
				//ana->unregister_change_handler(this, &handle_analog);
				delete ana;
			}
			vrpn_Analog_Remote * ana;
			bool stopflag;
			std::vector<double> channels;
			void startSampleThread(std::string devname);
			void exitSampleThread();
			
			boost::mutex channelMutex;
			
			
		private:
			//static void vrpnSampleThreadEntry(GloveHardwareVRPNDevice * device, std::string devname);
			
			void vrpnSampleThread(std::string devname);
			
			static void VRPN_CALLBACK handle_analog(void* userdata, const vrpn_ANALOGCB a);
			boost::thread * thread;
		};
		
		void GloveHardwareVRPNDevice::startSampleThread(std::string devname) {
			ana = new vrpn_Analog_Remote(devname.c_str());
		
			if (!ana) {
				throw GloveConnectionError();
			}
			thread = new boost::thread(&GloveHardwareVRPNDevice::vrpnSampleThread, this, devname);
		
		}
		void GloveHardwareVRPNDevice::exitSampleThread() {
			if (thread) {
				stopflag = true;
				thread->join();
			}
		}
		/*
		void GloveHardwareVRPNDevice::vrpnSampleThreadEntry(GloveHardwareVRPNDevice * device, std::string devname) {
			device->vrpnSampleThread(devname);
		}
		*/
		
		void GloveHardwareVRPNDevice::vrpnSampleThread(std::string devname) {
			
			
			while (!stopflag) {
				ana->mainloop();
				vrpn_SleepMsecs(1);
			}
			ana->unregister_change_handler(this, &handle_analog);
		}
		
		void GloveHardwareVRPNDevice::handle_analog(void* userdata, const vrpn_ANALOGCB a) {
			std::cout << "VRPN sent a report!" << std::endl;
			GloveHardwareVRPNDevice * d = static_cast<GloveHardwareVRPNDevice *>(userdata);
			boost::lock_guard<boost::mutex> mutexLocker(d->channelMutex);
			d->channels.clear();
			for (unsigned int i = 0; i < a.num_channel; ++i) {
				d->channels.push_back(static_cast<double>(a.channel[i]));
			}
		}
	}
	
	static void handle_analog(void* userdata, const vrpn_ANALOGCB a) {
		std::cout << "VRPN sent a report!" << std::endl;
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
		// For the option parameter, specify the vrpn device name including machine (with @)
		// and optionally the word "channels" followed by the channel numbers for
		// the digits starting with the thumb. Specify RIGHT or LEFT as desired.

		for (unsigned int i = 0; i < 5; ++i) {
			_channelMap.push_back(i);
		}

		std::string devname;
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
			} else {
				std::cerr << "ERROR: Unrecognized option! '" << opt << std::endl;
				throw InvalidGloveOptionError();
			}
		}

		if (devname.empty()) {
			throw MissingGloveOptionError();
		}
		
		std::cout << "Connecting to vrpn analog device " << devname;
		std::cout << "with channel mapping:" << std::endl;
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
/*
		_d->_ana = new vrpn_Analog_Remote(devname.c_str());
		
		if (!_d->_ana) {
			throw GloveConnectionError();
		}
		_d->_ana->register_change_handler(this, &handle_analog);
		*/
/*
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
*/
	}

	GloveHardwareVRPN::~GloveHardwareVRPN() {
		//_d->exitSampleThread();
		
		if (_d->ana) {
			_d->ana->unregister_change_handler(&_channels, &handle_analog);
		}
		
		delete _d;
		_d = NULL;
	}


	void GloveHardwareVRPN::updateData() {
		//boost::lock_guard<boost::mutex> mutexLocker(_d->channelMutex);
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
