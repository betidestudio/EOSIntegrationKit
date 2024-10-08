// Copyright Epic Games, Inc. All Rights Reserved.

#include "EosIconStyle.h"
#include "Framework/Application/SlateApplication.h"
#include "Styling/SlateStyleRegistry.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"

#define RootToContentDir Style->RootToContentDir

#if ENGINE_MAJOR_VERSION == 4
#define IMAGE_BRUSH( RelativePath, ... ) FSlateImageBrush( FPaths::ProjectContentDir() / "UI" / RelativePath + TEXT(".png"), __VA_ARGS__)
#else
#include "Styling/SlateStyleMacros.h"
#endif

TSharedPtr<FSlateStyleSet> FEosIconStyle::StyleInstance = nullptr;

void FEosIconStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FEosIconStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FEosIconStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("EIKStyle"));
	return StyleSetName;
}


const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);

TSharedRef< FSlateStyleSet > FEosIconStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("EIKStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("EosIntegrationKit")->GetBaseDir() / TEXT("Resources"));

	Style->Set("EIK.PluginAction", new IMAGE_BRUSH(TEXT("NewButtonIcon"), Icon20x20));
	return Style;
}

void FEosIconStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FEosIconStyle::Get()
{
	return *StyleInstance;
}
