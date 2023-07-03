//Copyright (c) 2023 Betide Studio. All Rights Reserved.

#include "..\Public\EIKCommands.h"

#define LOCTEXT_NAMESPACE "FEIKModule"

void FEIKCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "EIK", "Execute EIK action", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(CreateNewMasterSequenceInLevel, "Add Master Sequence", "Create a new master sequence asset, and place an instance of it in this level", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(ToggleCinematicViewportCommand, "Cinematic Viewport", "A viewport layout tailored to cinematic preview", EUserInterfaceActionType::RadioButton, FInputChord());

}

#undef LOCTEXT_NAMESPACE
