//Copyright (c) 2023 Betide Studio. All Rights Reserved.

#include "EIKCommands.h"

#define LOCTEXT_NAMESPACE "FEIKModule"

void FEIKCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "EIK", "Execute EIK action", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
