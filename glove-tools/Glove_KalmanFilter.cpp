/**
	@file Glove.cpp
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
#include <GloveToolsConfig.h>

#ifdef BUILD_WITH_EIGENKF
#include "Glove.h"
#include "GloveNode.h"
#include "GloveUpdater.h"
#include "IGloveHardware.h"

// Library/third-party includes
#include <eigenkf/KalmanFilter.h>
using namespace eigenkf;

// Standard includes
#include <ctime>
#include <iostream>

namespace glove {
	namespace detail {
	
		typedef SimpleState<5> state_t;
		typedef ConstantProcess<5, state_t> process_t;
		
		typedef Eigen::Matrix<double, 5, 1> Vector5d;
		
		struct GloveFilterContainer {
			GloveFilterContainer() :
				gotFirstTime(false) {}
			
			EIGEN_MAKE_ALIGNED_OPERATOR_NEW
			
			KalmanFilter<state_t, process_t> kf;
			
			bool gotFirstTime;
			std::clock_t lastTime;
			double getDt() {
				if (!gotFirstTime) {
					lastTime = clock();
					gotFirstTime = true;
					return -1;
				} else {
					std::clock_t now = clock();
					std::cout << "Clock: " << now << "     ";
					double dt = std::difftime(lastTime, now);
					lastTime = now;
					return dt;
				}
			}
		};
	}


	void Glove::_allocateFilter() {
		_filter = new detail::GloveFilterContainer;
		
		/// Set our process model's variance
		/// @todo tune the process model variance
		_filter->kf.processModel.sigma = detail::Vector5d::Constant(0.25);
	}

	void Glove::_destroyFilter() {
		delete _filter;
		_filter = NULL;
	}
	
	std::vector<double> Glove::_updateFilter(std::vector<double> const& calibBends) {
		double dt = _filter->getDt();
			std::cout << "DT: " << dt << std::endl;
		if (dt < 0) {
			/// This is the first update - we just set the initial state and belive it
			for (unsigned int i = 0; i < 5; ++i) {
				_filter->kf.state.x[i] = calibBends[i];
			}
		} else {
			/// Prediction step
			_filter->kf.predict(dt);

			/// Figure out the measurement details
			std::vector<double> rangesOrig = _calib.getRanges();
			
			detail::Vector5d ranges;
			AbsoluteMeasurement<detail::state_t> meas;

			for (unsigned int i = 0; i < 5; ++i) {
				meas.measurement[i] = calibBends[i];
				if (rangesOrig[i] <= 0) {
					ranges[i] = 1;
				} else {
					ranges[i] = rangesOrig[i];
				}
			}

			/// @todo this should be the device's raw variance
			detail::Vector5d rawVar(detail::Vector5d::Constant(0.25));
#if EIGEN_VERSION_AT_LEAST(2, 9, 0)
			detail::Vector5d scaledVar = rawVar.array() / ranges.array();
#else
			detail::Vector5d scaledVar = rawVar.cwise() / ranges.cwise();
#endif
			meas.covariance = scaledVar.asDiagonal();
			
			/// Correction step
			_filter->kf.correct(meas);
		}
		
		/// Return the state
		std::vector<double> ret;
		
		for (unsigned int i = 0; i < 5; ++i) {
			ret.push_back(_filter->kf.state.x[i]);
		}
		
		return ret;
	}
#endif


} // end of namespace glove

