/** @file GlovePointerTypes.h
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
#ifndef INCLUDED_GlovePointerTypes_h_GUID_82e91079_b61c_4228_810c_2a13ac158f79
#define INCLUDED_GlovePointerTypes_h_GUID_82e91079_b61c_4228_810c_2a13ac158f79

// Internal Includes
// - none

// Library/third-party includes
#include <boost/shared_ptr.hpp>

// Standard includes
// - none

class IGloveHardware;
typedef boost::shared_ptr<IGloveHardware> GloveHardwarePtr;

#endif // INCLUDED_GlovePointerTypes_h_GUID_82e91079_b61c_4228_810c_2a13ac158f79

