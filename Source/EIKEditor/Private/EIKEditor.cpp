// Copyright (c) 2024 Betide Studio. All Rights Reserved.

#include "EIKEditor.h"
#include "ToolMenus.h"
#include "Slate.h"
#include "SlateCore.h"
#include "EditorStyle.h"
#include "EditorStyleSet.h"
#include "EIKSettings.h"
#include "ILauncher.h"
#include "ILauncherServicesModule.h"
#include "ITargetDeviceServicesModule.h"
#include "EosIconStyle.h"
#include "Interfaces/IPluginManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Misc/InteractiveProcess.h"

#define LOCTEXT_NAMESPACE "FEIKEditorModule"

DEFINE_LOG_CATEGORY(LogEikEditor);

class FLauncherNotificationTask
{
public:
    FLauncherNotificationTask(TWeakPtr<SNotificationItem> InNotificationItemPtr, SNotificationItem::ECompletionState InCompletionState, const FText& InText)
        : CompletionState(InCompletionState)
        , NotificationItemPtr(InNotificationItemPtr)
        , Text(InText)
    { }

    void DoTask(ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent)
    {
        if (NotificationItemPtr.IsValid())
        {
            TSharedPtr<SNotificationItem> NotificationItem = NotificationItemPtr.Pin();
            NotificationItem->SetText(Text);
            NotificationItem->SetCompletionState(CompletionState);
            if (CompletionState == SNotificationItem::CS_Success || CompletionState == SNotificationItem::CS_Fail)
            {
                NotificationItem->ExpireAndFadeout();
            }
        }
    }

    static ESubsequentsMode::Type GetSubsequentsMode() { return ESubsequentsMode::TrackSubsequents; }
    ENamedThreads::Type GetDesiredThread() { return ENamedThreads::GameThread; }
    FORCEINLINE TStatId GetStatId() const
    {
        RETURN_QUICK_DECLARE_CYCLE_STAT(FLauncherNotificationTask, STATGROUP_TaskGraphTasks);
    }

private:
    SNotificationItem::ECompletionState CompletionState;
    TWeakPtr<SNotificationItem> NotificationItemPtr;
    FText Text;
};

void FEIKEditorModule::StartupModule()
{
    FEosIconStyle::Initialize();
    // Register a function to be called when the menu system is initialized
    UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(
        this, &FEIKEditorModule::RegisterMenuExtensions));
}

void FEIKEditorModule::ShutdownModule()
{
    // Unregister the startup function
    UToolMenus::UnRegisterStartupCallback(this);
 
    // Unregister all our menu extensions
    UToolMenus::UnregisterOwner(this);

    FEosIconStyle::Shutdown();
}

void FEIKEditorModule::BuildProcessCompleted(int I, bool bArg)
{
    UE_LOG(LogEikEditor, Warning, TEXT("Process Completed %d %d"), I, bArg);
}

void FEIKEditorModule::RegisterMenuExtensions()
{
    // Use the current object as the owner of the menus
    FToolMenuOwnerScoped OwnerScoped(this);
 
    // Extend the "ModesToolBar" section of the main toolbar
    UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
    FToolMenuSection& ToolbarSection = ToolbarMenu->FindOrAddSection("File");

    FUIAction ComboButtonAction;
    ComboButtonAction.ExecuteAction = FExecuteAction::CreateLambda([] {});
    FToolMenuSection &Section = ToolbarMenu->AddSection("EIKSection", LOCTEXT("EIKSection", "EIK"), FToolMenuInsert("File", EToolMenuInsertType::After));
    auto ComboButton = FToolMenuEntry::InitComboButton(
        "MyCustomDropdownName",
        ComboButtonAction,
        FOnGetContent::CreateRaw(this, &FEIKEditorModule::GenerateMenuContent),
        LOCTEXT("MyCustomDropdown", "EOS Integration Kit"),
        LOCTEXT("MyCustomDropdown_Tooltip", "Tooltip for my custom dropdown"),
        FSlateIcon(FAppStyle::GetAppStyleSetName(), "AboutScreen.EpicGamesLogo"),
        false,
        "MyCustomDropdownName"
    );
    ComboButton.StyleNameOverride = "CalloutToolbar";
    Section.AddEntry(ComboButton);
}

void FEIKEditorModule::OpenDevPortal()
{
    UEIKSettings* EIkSettings = GetMutableDefault<UEIKSettings>();
    FString ProductName = EIkSettings->ProductName;
    FString OrganizationId = EIkSettings->OrganizationName;
    FString URL = "https://dev.epicgames.com/portal/";
    if (!ProductName.IsEmpty() && !OrganizationId.IsEmpty())
    {
        URL = FString::Printf(TEXT("https://dev.epicgames.com/portal/en-US/%s/products/%s/"), *OrganizationId, *ProductName);
    }
    FPlatformProcess::LaunchURL(*URL, nullptr, nullptr);
}

void FEIKEditorModule::OnOption1Click()
{
}

void FEIKEditorModule::OpenDevTool()
{
    IPluginManager& PluginManager = IPluginManager::Get();
    TSharedPtr<IPlugin> EOSPlugin = PluginManager.FindPlugin(TEXT("EOSIntegrationKit"));
    if(!EOSPlugin.IsValid())
    {
        UE_LOG(LogEikEditor, Error, TEXT("EOSIntegrationKit plugin not found"));
        return;
    }
    FString PluginRoot = EOSPlugin->GetBaseDir();
    auto MainModulePath = FPaths::Combine(*PluginRoot, TEXT("Source/ThirdParty/EIKSDK/Tools/EOS_DevAuthTool-win32-x64-1.2.0"));
    FString DevToolPath = FPaths::Combine(*MainModulePath, TEXT("EOS_DevAuthTool.exe"));
    FString DevToolArgs = TEXT("");
    FPlatformProcess::CreateProc(*DevToolPath, *DevToolArgs, true, false, false, nullptr, 0, nullptr, nullptr);
}


TSharedRef<SWidget> FEIKEditorModule::GenerateMenuContent()
{
    FMenuBuilder MenuBuilder(true, nullptr);

    MenuBuilder.AddMenuEntry(
        LOCTEXT("Option2", "Epic DevPortal"),
        LOCTEXT("Option2_Tooltip", "Open the Epic's DevPortal in the browser"),
        FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.OpenInBrowser"),
        FUIAction(FExecuteAction::CreateRaw(this, &FEIKEditorModule::OpenDevPortal))
    );
    
    MenuBuilder.AddMenuEntry(
    LOCTEXT("Option3", "Launch DevTool"),
    LOCTEXT("Option3_Tooltip", "Open the Epic's DevPortal in the browser"),
    FSlateIcon(FAppStyle::GetAppStyleSetName(), "DeveloperTools.MenuIcon"),
    FUIAction(FExecuteAction::CreateRaw(this, &FEIKEditorModule::OpenDevTool))
);

    MenuBuilder.BeginSection(NAME_None, LOCTEXT("OneClickDeployHeader", "One Click Deploy"));

    MenuBuilder.AddMenuEntry(
        LOCTEXT("Option1", "Package and Deploy"),
        LOCTEXT("Option1_Tooltip", "Package and deploy the game to Steamworks"),
        FSlateIcon(FAppStyle::GetAppStyleSetName(), "PlayWorld.RepeatLastLaunch"),
        FUIAction(FExecuteAction::CreateRaw(this, &FEIKEditorModule::OnOption1Click))
    );

    UEIKSettings* Settings = GetMutableDefault<UEIKSettings>();

    MenuBuilder.AddMenuEntry(
        LOCTEXT("Option3", "Login to Epic Games"),
        LOCTEXT("Option3_Tooltip", "Login to Epic Games to authenticate the user account"),
        FSlateIcon(FAppStyle::GetAppStyleSetName(), "ShowFlagsMenu.SubMenu.Developer"),
        FUIAction(FExecuteAction::CreateLambda([this]()
        {
        }))
    );



    MenuBuilder.AddMenuEntry(
        LOCTEXT("Option4", "Create/Reset Packaging Profile"),
        LOCTEXT("Option4_Tooltip", "Create or reset the packaging profile"),
        FSlateIcon(FAppStyle::GetAppStyleSetName(), "MainFrame.PackageProject"),
        FUIAction(FExecuteAction::CreateLambda([]
        {
           
        }))
    );

    MenuBuilder.BeginSection(NAME_None, LOCTEXT("PluginInfoHeader", "Plugin Info"));

    MenuBuilder.AddMenuEntry(
    LOCTEXT("Documentation", "Documentation"),
    LOCTEXT("PluginDocumentation_Tooltip", "Open the plugin documentation"),
    FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.Documentation"),
    FUIAction(FExecuteAction::CreateLambda([]
    {
        UKismetSystemLibrary::LaunchURL("eik-new.betide.studio");
    }))
);

    MenuBuilder.AddMenuEntry(
        LOCTEXT("Marketplace", "Marketplace"),
        LOCTEXT("PluginMarketplace_Tooltip", "Open the plugin marketplace page"),
        FSlateIcon(FAppStyle::GetAppStyleSetName(), "MainFrame.OpenMarketplace"),
        FUIAction(FExecuteAction::CreateLambda([]
        {
            UKismetSystemLibrary::LaunchURL("https://www.unrealengine.com/marketplace/en-US/profile/Betide+Studio");
        }))
    );
    
    return MenuBuilder.MakeWidget();
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FEIKEditorModule, EikEditor)
