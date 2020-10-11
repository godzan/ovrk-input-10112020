#include "DomHandMultiplier.h"

namespace keyboardinput {

	DomHandMultiplier::DomHandMultiplier() {
		_domControllerIndex = 0; // not valid value. HMD has index 0
		// remaining members are indeterminate. they are defined in init()

		_currMultiplierLevel = NUM_MULTIPLIERS - 1;
		_lastYAxisValue = 0;
		_gate = true;
		_lastGateButtonBool = 0;

		// _controllerState is indeterminate. it is defined in update()
		_gateButtonBool = 0;
		_yAxisValue = 0;
	}

	DomHandMultiplier::~DomHandMultiplier() {
	}

	void DomHandMultiplier::init(vr::TrackedDeviceIndex_t index) { // index cannot be 0 because HMD is always index 0.
		if (index > 0) {
			_domControllerIndex = index;

			// find and store joystick axis index
			int32_t joyAxisIndex = 0;
			vr::TrackedPropertyError error = vr::TrackedProp_Success;
			int32_t axisType = vr::k_eControllerAxis_None;
			while (1) {
				if (joyAxisIndex >= vr::k_unControllerStateAxisCount) {
					LOG(ERROR) << "Unable to find right controller joystick axis index.";
					break;
				}
				axisType = vr::VRSystem()->GetInt32TrackedDeviceProperty(_domControllerIndex, vr::TrackedDeviceProperty(vr::Prop_Axis0Type_Int32 + joyAxisIndex), &error);
				if (error != vr::TrackedProp_Success) {
					LOG(ERROR) << "Failed to access right controller axis at index " << joyAxisIndex << ".";
					break;
				}
				if (axisType == vr::k_eControllerAxis_Joystick) {
					_joyAxisIndex = joyAxisIndex;
					LOG(INFO) << "Right controller joystick-axis index is " << joyAxisIndex << ".";
					break;
				}
				joyAxisIndex++;
			}
			_gateButtonMask = vr::ButtonMaskFromId(vr::EVRButtonId(vr::k_EButton_Axis0 + _joyAxisIndex));
		} else {
			LOG(ERROR) << "DomHandMultiplier device index must be greater than 0.";
		}
	}

	bool DomHandMultiplier::initialized() {
		// _domControllerIndex will be greater than 0 if init ran successfully.
		return _domControllerIndex > 0;
	}

	void DomHandMultiplier::update() {
		if (initialized()) { 
			if (vr::VRSystem()->GetControllerState(_domControllerIndex, &_controllerState, sizeof(vr::VRControllerState_t))) {

				// check toggle gate button
				_gateButtonBool = _controllerState.ulButtonPressed & _gateButtonMask;
				if (_gateButtonBool != _lastGateButtonBool) {
					if (_gateButtonBool) {
						toggleGate();
					}
					_lastGateButtonBool = _gateButtonBool;
				}

				// check joystick and change multiplier accordingly
				_yAxisValue = _controllerState.rAxis[_joyAxisIndex].y;
				if (_yAxisValue > THUMBSTICK_THRESHOLD) {
					if (_lastYAxisValue <= THUMBSTICK_THRESHOLD) {
						setMultiplierMax();
					}
				} else if (_yAxisValue < -1 * THUMBSTICK_THRESHOLD) {
					if (_lastYAxisValue >= -1 * THUMBSTICK_THRESHOLD) {
						reduceMultiplier();
					}
				}
				_lastYAxisValue = _yAxisValue;

				/*
				// DEBUGGING
				OutputDebugString(L"\n gateButtonBool: ");
				OutputDebugString(std::to_wstring(_gateButtonBool).c_str());

				OutputDebugString(L"\n _currMultiplierLevel: ");
				OutputDebugString(std::to_wstring(_currMultiplierLevel).c_str());

				OutputDebugString(L"\n _gate: ");
				OutputDebugString(std::to_wstring(_gate).c_str());

				OutputDebugString(L"\n multiplier(): ");
				OutputDebugString(std::to_wstring(multiplier()).c_str());
				*/

			} else {
				LOG(ERROR) << "Right controller device index invalid or controller state invalid.";
			}
		} else {
			LOG(ERROR) << "DomHandMultiplier.update() was called before init().";
		}
	}

	float DomHandMultiplier::multiplier() {
		return MULTIPLIERS[_currMultiplierLevel];
	}

	bool DomHandMultiplier::gate() {
		return _gate;
	}

	void DomHandMultiplier::reduceMultiplier() {
		if (_currMultiplierLevel > 0) {
			_currMultiplierLevel--;
		}
	}

	void DomHandMultiplier::setMultiplierMax() {
		_currMultiplierLevel = NUM_MULTIPLIERS - 1;
	}

	void DomHandMultiplier::toggleGate() {
		_gate = !_gate;
		// when gate is toggled on, set multiplier to max
		if (_gate) {
			setMultiplierMax();
		}
	}

	// constant multiplier array
	const float DomHandMultiplier::MULTIPLIERS[NUM_MULTIPLIERS] = { 0.2f, 0.4f, 0.6f, 1.0f };
	const float DomHandMultiplier::THUMBSTICK_THRESHOLD = 0.56f;
}
