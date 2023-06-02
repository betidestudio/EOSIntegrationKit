//Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "EIKStyle.h"

class FEIKCommands : public TCommands<FEIKCommands>
{
public:

	FEIKCommands()
		: TCommands<FEIKCommands>(TEXT("EIK"), NSLOCTEXT("Contexts", "EIK", "EIK Plugin"), NAME_None, FEIKStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > PluginAction;
};
