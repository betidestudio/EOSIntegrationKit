// Copyright Epic Games, Inc. All Rights Reserved.

#include "EIKCommands.h"

#define LOCTEXT_NAMESPACE "FEIKModule"

void FEIKCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "EIK", "Open Epic DevPortal", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
