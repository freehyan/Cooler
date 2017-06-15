#pragma once
#include <stdint.h>

namespace Cooler
{
	/// Pointer input action values.
	struct SPointerActionType
	{
		enum Enum
		{
			UP = 0, ///< touch or button release
			DOWN = 1, ///< touch or button press
			MOTION = 2, ///< touch or mouse pointer movement
			EXTRA_DOWN = 4, ///< multitouch additional touch press
			EXTRA_UP = 8 ///< multitouch additional touch release
		};
	};

	/// Key input action values.
	struct SKeyActionType
	{
		enum Enum
		{
			UP = 0, ///< key release
			DOWN = 1, ///< key initial press
			REPEAT = 2 ///< key auto-repeat (held)
		};
	};

	/// Type of pointer input devices.
	struct SInputDeviceType
	{
		enum Enum
		{
			MOUSE, ///< Mouse (possibly multi-button)
			TOUCH, ///< Touch (possibly multi-touch)
			STYLUS ///< Stylus (possibly multiple "point" types)
		};
	};

	/// Mouse button masks.
	/// The button indices are bitfields.  i.e., button-3 == middle == 1<<3.
	struct SMouseButton
	{
		const static uint32_t LEFT = 0x00000001; ///< Left button
		const static uint32_t RIGHT = 0x00000002; ///< Right button
		const static uint32_t MIDDLE = 0x00000004; ///< Middle button
	};

	/// Basic single-point pointer event.
	class CPointerEvent
	{
	public:
		float m_x; ///< x value in pixel-space
		float m_y; ///< y value in pixel-space
		int m_id; ///< Unique ID for tracking multiple touches
		SInputDeviceType::Enum m_device; ///< Device type for this pointer event
	};

	/// Input handling callback interfaces.
	class CInputCallbacks
	{
	public:
		virtual bool pointerInput(SInputDeviceType::Enum device, SPointerActionType::Enum action, uint32_t modifiers, int32_t count, CPointerEvent* points, int64_t timestamp = 0) = 0;

 		virtual bool keyInput(uint32_t code, SKeyActionType::Enum action) = 0;
// 
// 		/// Character input event
// 		/// Called when a keypressed, release or hold is unhandled and maps to a character
// 		/// param[in] c the ASCII character of the event
// 		/// \return true if the recipient handled the event, false if the recipient wishes
// 		/// the caller to handle the event
// 		virtual bool characterInput(uint8_t c) = 0;
// 
// 		/// Gamepad input event
// 		/// Called when any button or axis value on any active gamepad changes
// 		/// \param[in] changedPadFlags a mask of the changed pad indices.  For each gamepad
// 		/// i that has changed, bit (1<<i) will be set.
// 		/// \return true if the recipient handled the event, false if the recipient wishes
// 		/// the caller to handle the event
// 		virtual bool gamepadChanged(uint32_t changedPadFlags) = 0;
	};
}