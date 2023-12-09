//Copyright (c) 2023 Betide Studio. All Rights Reserved.

#include "EIKStyle.h"
#include "OnlineSubsystemEOS.h"
#include "Framework/Application/SlateApplication.h"
#include "Styling/SlateStyleRegistry.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FEIKStyle::StyleInstance = nullptr;

void FEIKStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FEIKStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FEIKStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("EIKStyle"));
	return StyleSetName;
}


const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);

TSharedRef< FSlateStyleSet > FEIKStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("EIKStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("EOSIntegrationKit")->GetBaseDir() / TEXT("Resources"));

	Style->Set("EIK.PluginAction", new IMAGE_BRUSH(TEXT("PlaceholderButtonIcon"), Icon20x20));
	return Style;
}

void FEIKStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FEIKStyle::Get()
{
	return *StyleInstance;
}
