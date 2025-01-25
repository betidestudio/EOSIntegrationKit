// Copyright (c) 2024 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Misc/InteractiveProcess.h"
#include "Editor/UnrealEd/Public/Editor.h"
#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogEikEditor, Log, All);
class FEIKEditorModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
    void BuildProcessCompleted(int I, bool bArg);
    void HandleProcessCanceled();
    void HandleProcessOutput(const FString& String);

private:
    void RegisterMenuExtensions();
    void OpenDevPortal();
    void OnPackageAndDeploySelected();
    void OpenDevTool();
    void OpenRedistributableInstallerTool();
    TSharedRef<SWidget> GenerateMenuContent();
    TSharedPtr<FInteractiveProcess> InteractiveProcess;
    TSharedPtr<FInteractiveProcess> InteractiveProcessLogin;
    TSharedPtr<SEditableTextBox> SteamGuardTextBox;
    TSharedPtr<SWindow> SteamGuardWindow;
    TSharedPtr<SNotificationItem> NotificationItem;
};
