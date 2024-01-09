// Copyright Epic Games, Inc. All Rights Reserved.

// This file is not intended to be included directly. Include eos_ui_types.h instead.

/** No buttons */
EOS_UI_KEY_ENTRY_FIRST(EOS_UISBF_, None, 0)
/** Controller directional pad left */
EOS_UI_KEY_MODIFIER(EOS_UISBF_, DPad_Left, (1 << 0))
/** Controller directional pad right */
EOS_UI_KEY_MODIFIER(EOS_UISBF_, DPad_Right, (1 << 1))
/** Controller directional pad down */
EOS_UI_KEY_MODIFIER(EOS_UISBF_, DPad_Down, (1 << 2))
/** Controller directional pad up */
EOS_UI_KEY_MODIFIER(EOS_UISBF_, DPad_Up, (1 << 3))
/** Controller left main face button */
EOS_UI_KEY_MODIFIER(EOS_UISBF_, FaceButton_Left, (1 << 4))
/** Controller right main face button */
EOS_UI_KEY_MODIFIER(EOS_UISBF_, FaceButton_Right, (1 << 5))
/** Controller bottom main face button */
EOS_UI_KEY_MODIFIER(EOS_UISBF_, FaceButton_Bottom, (1 << 6))
/** Controller top main face button */
EOS_UI_KEY_MODIFIER(EOS_UISBF_, FaceButton_Top, (1 << 7))
/** Controller left upper shoulder button. */
EOS_UI_KEY_MODIFIER(EOS_UISBF_, LeftShoulder, (1 << 8))
/** Controller right upper shoulder button. */
EOS_UI_KEY_MODIFIER(EOS_UISBF_, RightShoulder, (1 << 9))
/** Controller left lower trigger button. */
EOS_UI_KEY_MODIFIER(EOS_UISBF_, LeftTrigger, (1 << 10))
/** Controller right lower trigger button. */
EOS_UI_KEY_MODIFIER(EOS_UISBF_, RightTrigger, (1 << 11))
/** Controller special button on left. */
EOS_UI_KEY_MODIFIER(EOS_UISBF_, Special_Left, (1 << 12))
/** Controller special button on right. */
EOS_UI_KEY_MODIFIER(EOS_UISBF_, Special_Right, (1 << 13))
/** Controller left thumbstick as a button. */
EOS_UI_KEY_MODIFIER(EOS_UISBF_, LeftThumbstick, (1 << 14))
/** Controller right thumbstick as a button. */
EOS_UI_KEY_MODIFIER_LAST(EOS_UISBF_, RightThumbstick, (1 << 15))