// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_EngineSubsystem.h"

#include "Online.h"
#include "OnlineSubsystemModule.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineIdentityInterface.h"

#if WITH_EDITOR
	#include "Editor.h"
#endif

void UEIK_EngineSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
#if WITH_EDITOR
	FEditorDelegates::BeginPIE.AddUObject( this, &UEIK_EngineSubsystem::BeginPIE );
	Super::Initialize(Collection);
#endif
	
}

void UEIK_EngineSubsystem::BeginPIE(const bool bIsSimulating)
{
#if WITH_EDITOR
	static const FName OnlineSubsystemModuleName = TEXT("OnlineSubsystem");
	FOnlineSubsystemModule& OSSModule = FModuleManager::GetModuleChecked<FOnlineSubsystemModule>(OnlineSubsystemModuleName);
	OSSModule.ReloadDefaultSubsystem();
#endif
}
