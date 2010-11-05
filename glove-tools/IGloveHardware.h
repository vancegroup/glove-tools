/** @file IGloveHardware.h
	@brief

	@date 2010

	@author
	Ryan Pavlik
	<rpavlik@iastate.edu> and <abiryan@ryand.net>
	http://academic.cleardefinition.com/
	Iowa State University Virtual Reality Applications Center
	Human-Computer Interaction Graduate Program
*/
#pragma once
#ifndef INCLUDED_IGloveHardware_h_GUID_70ba2c9c_5e00_44d8_b9c9_6a16cb9a2fb7
#define INCLUDED_IGloveHardware_h_GUID_70ba2c9c_5e00_44d8_b9c9_6a16cb9a2fb7

// Internal Includes
#include "Finger.h"
#include "GlovePointerTypes.h"

// Library/third-party includes
#include <boost/function.hpp>

// Standard includes
#include <string>
#include <map>
#include <stdexcept>

namespace glove {

	typedef boost::function<GloveHardwarePtr(std::string const &)> HardwareCreator;

	/// Class to allow drivers to register a creator function by name
	class DriverRegistration {
		public:
			DriverRegistration(std::string const & name, HardwareCreator creatorFunc);
	};

	struct UnregisteredHardwareTypeException : public std::runtime_error {
		UnregisteredHardwareTypeException(std::string const& name) :
			std::runtime_error("The following device type is not registered so could not be created: " + name) {}
	};

	class IGloveHardware {
		public:
			IGloveHardware() {}
			virtual ~IGloveHardware() {}

			/// Fetch the latest bend information for all fingers and normalize
			virtual void updateData() = 0;

			/// Access the bend data for the given finger, normalized into [0, 1]
			virtual double getBend(Finger finger) const = 0;

			static GloveHardwarePtr createByName(std::string const & name, std::string const & option = "");

			static void registerDriver(std::string const & name, HardwareCreator creatorFunc);

			
		private:
			static std::map<std::string, HardwareCreator> s_creators;
	};
}

#endif // INCLUDED_IGloveHardware_h_GUID_70ba2c9c_5e00_44d8_b9c9_6a16cb9a2fb7

