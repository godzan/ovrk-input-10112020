#pragma once

#include <openvr.h>
#include "logging.h"

namespace keyboardinput {

	class DomHandMultiplier {
	public:
		DomHandMultiplier();
		~DomHandMultiplier();
		void init(vr::TrackedDeviceIndex_t);
		bool initialized();
		void update();
		float multiplier();
		bool gate();
	private:
		void reduceMultiplier();
		void setMultiplierMax();
		void toggleGate();

		vr::TrackedDeviceIndex_t _domControllerIndex;
		uint32_t _joyAxisIndex;
		uint64_t _gateButtonMask;

		// "speed" of multiplier we are currently on. 0 is slowest. NUM_MULTIPLIERS - 1 is fastest.
		int _currMultiplierLevel;
		float _lastYAxisValue;
		bool _gate;
		uint64_t _lastGateButtonBool;

		// class members instead of local vars to conserve memory
		vr::VRControllerState_t _controllerState;
		uint64_t _gateButtonBool;
		float _yAxisValue;

		// constants
		static const int NUM_MULTIPLIERS = 4;
		static const float MULTIPLIERS[NUM_MULTIPLIERS];
		static const float THUMBSTICK_THRESHOLD;
	};

}