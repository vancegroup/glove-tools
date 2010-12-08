/** @file GloveHardwareFactory.h
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
#ifndef INCLUDED_GloveHardwareFactory_h_GUID_ab366177_3cfe_4451_87fb_a400cbce454c
#define INCLUDED_GloveHardwareFactory_h_GUID_ab366177_3cfe_4451_87fb_a400cbce454c

// Internal Includes
#include "GlovePointerTypes.h"

// Library/third-party includes
#include <boost/function.hpp>

// Standard includes
#include <string>
#include <map>
#include <stdexcept>


namespace glove {

	typedef boost::function<GloveHardwarePtr(std::string const &)> HardwareCreator;

	
	
	/// singleton for hardware create-by-name
	class GloveHardwareFactory {
		public:
			static GloveHardwareFactory * instance();
			GloveHardwarePtr createByName(std::string const & name, std::string const & option = "");
			
			bool registerHardwareCreator(std::string const & name, HardwareCreator creatorFunc);

			struct UnregisteredHardwareTypeException : public std::runtime_error {
				UnregisteredHardwareTypeException(std::string const& name) :
					std::runtime_error("The following device type is not registered so could not be created: " + name) {}
			};
			

		private:
			GloveHardwareFactory();

			std::map<std::string, HardwareCreator> _creators;

	};
}

#endif // INCLUDED_GloveHardwareFactory_h_GUID_ab366177_3cfe_4451_87fb_a400cbce454c

