// Copyright (c) 2024 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Misc/InteractiveProcess.h"
#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogEikEditor, Log, All);
class FEIKEditorModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
    void BuildProcessCompleted(int I, bool bArg);

private:
    void RegisterMenuExtensions();
    void OpenDevPortal();
    void OnOption1Click();
    void OpenDevTool();
    TSharedRef<SWidget> GenerateMenuContent();
    TSharedPtr<FInteractiveProcess> InteractiveProcess;
    TSharedPtr<FInteractiveProcess> InteractiveProcessLogin;
    TSharedPtr<SEditableTextBox> SteamGuardTextBox;
    TSharedPtr<SWindow> SteamGuardWindow;
    TSharedPtr<SNotificationItem> NotificationItem;
};
