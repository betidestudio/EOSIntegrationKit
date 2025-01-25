 // Copyright (c) 2024 Betide Studio. All Rights Reserved.

#include "EIKEditor.h"

#include "DesktopPlatformModule.h"
#include "ToolMenus.h"
#include "SlateCore.h"
#include "EditorStyle.h"
#include "EditorStyleSet.h"
#include "EIKSettings.h"
#include "ILauncher.h"
#include "ILauncherServicesModule.h"
#include "ITargetDeviceServicesModule.h"
#include "EosIconStyle.h"
#include "Misc/Paths.h"
#include "ISettingsModule.h"
#include "Framework/Notifications/NotificationManager.h"
#include "Interfaces/IPluginManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Misc/InteractiveProcess.h"
#include "Widgets/Notifications/SNotificationList.h"

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
    AsyncTask(ENamedThreads::GameThread, [this, bArg]()
    {
        if (NotificationItem.IsValid())
        {
            if (bArg)
            {
                NotificationItem->SetText(LOCTEXT("PackageSuccess", "Package Success"));
                NotificationItem->SetCompletionState(SNotificationItem::CS_Success);
            }
            else
            {
                NotificationItem->SetText(LOCTEXT("PackageFailed", "Package Failed"));
                NotificationItem->SetCompletionState(SNotificationItem::CS_Fail);
            }
            NotificationItem->ExpireAndFadeout();
        }
    });
}

void FEIKEditorModule::HandleProcessCanceled()
{
    AsyncTask(ENamedThreads::GameThread, [this]()
    {
        if (NotificationItem.IsValid())
        {
            NotificationItem->SetText(LOCTEXT("Package Failed", "Package Failed"));
            NotificationItem->SetCompletionState(SNotificationItem::CS_Fail);
            NotificationItem->ExpireAndFadeout();
        }
    });
}

void FEIKEditorModule::HandleProcessOutput(const FString& String)
{
}

void FEIKEditorModule::RegisterMenuExtensions()
{
    // Use the current object as the owner of the menus
    FToolMenuOwnerScoped OwnerScoped(this);
 
    // Extend the "ModesToolBar" section of the main toolbar
#if ENGINE_MAJOR_VERSION == 5
    UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
#else
    UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
#endif
    if (!ToolbarMenu)
    {
        UE_LOG(LogTemp, Warning, TEXT("ToolbarMenu not found!"));
        return;
    }
    FToolMenuSection& ToolbarSection = ToolbarMenu->FindOrAddSection("File");

    FUIAction ComboButtonAction;
    ComboButtonAction.ExecuteAction = FExecuteAction::CreateLambda([] {});
    FToolMenuSection &Section = ToolbarMenu->AddSection("EIKSection", LOCTEXT("EIKSection", "EIK"), FToolMenuInsert("File", EToolMenuInsertType::After));
    auto ComboButton = FToolMenuEntry::InitComboButton(
        "MyCustomDropdownName",
        ComboButtonAction,
        FOnGetContent::CreateRaw(this, &FEIKEditorModule::GenerateMenuContent),
        LOCTEXT("MyCustomDropdown", "EOS Integration Kit"),
        LOCTEXT("MyCustomDropdown_Tooltip", "EOS Integration Kit Menu"),
#if ENGINE_MAJOR_VERSION == 5
        FSlateIcon(FAppStyle::GetAppStyleSetName(), "AboutScreen.EpicGamesLogo"),
#else
        FSlateIcon(FEditorStyle::GetStyleSetName(), "AboutScreen.EpicGames"),
#endif
        false,
        "MyCustomDropdownName"
    );
#if ENGINE_MAJOR_VERSION == 5
    ComboButton.StyleNameOverride = "CalloutToolbar";
#endif
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

void FEIKEditorModule::OpenDevTool()
{
    IPluginManager& PluginManager = IPluginManager::Get();
    TSharedPtr<IPlugin> EOSPlugin = PluginManager.FindPlugin(TEXT("EOSIntegrationKit"));
    if (!EOSPlugin.IsValid())
    {
        UE_LOG(LogEikEditor, Error, TEXT("EOSIntegrationKit plugin not found"));
        return;
    }

    FString PluginRoot = EOSPlugin->GetBaseDir();
    FString DevToolPath;

#if PLATFORM_WINDOWS
    FString MainModulePath = FPaths::Combine(*PluginRoot, TEXT("Source/ThirdParty/EIKSDK/Tools/EOS_DevAuthTool-win32-x64-1.2.1"));
    DevToolPath = FPaths::Combine(*MainModulePath, TEXT("EOS_DevAuthTool.exe"));
    if (!FPaths::FileExists(DevToolPath))
    {
        UE_LOG(LogEikEditor, Error, TEXT("DevTool not found at %s"), *DevToolPath);
        FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("DevToolNotFound", "DevTool not found. Ensure the devtool is downloaded and placed in the correct directory and unzipped."));
        return;
    }
#elif PLATFORM_MAC
    DevToolPath = FPaths::Combine(*PluginRoot, TEXT("Source/ThirdParty/EIKSDK/Tools/EOS_DevAuthTool-darwin-x64-1.2.1/EOS_DevAuthTool.app"));
#else
    UE_LOG(LogEikEditor, Error, TEXT("Unsupported platform"));
    FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("UnsupportedPlatform", "Unsupported platform"));
    return;
#endif

#if PLATFORM_WINDOWS
    // Print the path to the DevTool & Length
    UE_LOG(LogEikEditor, Log, TEXT("DevToolPath: %s | Length: %d"), *DevToolPath, DevToolPath.Len());
    FString DevToolArgs = TEXT("");
    FPlatformProcess::CreateProc(*DevToolPath, *DevToolArgs, true, false, false, nullptr, 0, nullptr, nullptr);
#elif PLATFORM_MAC
    FString OpenCommand = TEXT("/usr/bin/open");
    FString DevToolArgs = FString::Printf(TEXT("\"%s\""), *DevToolPath);
    FPlatformProcess::CreateProc(*OpenCommand, *DevToolArgs, true, false, false, nullptr, 0, nullptr, nullptr);
#endif
}

void FEIKEditorModule::OpenRedistributableInstallerTool()
{
    TArray<FString> OutFileNames;
    IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
    if (DesktopPlatform)
    {
        void* ParentWindowHandle = const_cast<void*>(FSlateApplication::Get().FindBestParentWindowHandleForDialogs(nullptr));
        
        const FString FileTypes = TEXT("Executable Files (*.exe)|*.exe|All Files (*.*)|*.*");
        const FString DefaultPath = FPaths::ProjectDir(); // Default path to start from

        bool bFileSelected = DesktopPlatform->OpenFileDialog(
            ParentWindowHandle,
            TEXT("Select Redistributable Installer EXE"),
            DefaultPath,
            TEXT(""),
            FileTypes,
            EFileDialogFlags::None,
            OutFileNames
        );

        if (bFileSelected && OutFileNames.Num() > 0)
        {
            // Get the selected file path
            const FString SelectedFilePath = OutFileNames[0];
            UE_LOG(LogTemp, Log, TEXT("Selected EXE Path: %s"), *SelectedFilePath);

            // Extract directory and executable name
            FString SelectedFileName = FPaths::GetCleanFilename(SelectedFilePath);
            FString SelectedFileDir = FPaths::GetPath(SelectedFilePath);

            // Locate the EOSBootstrapperTool
            IPluginManager& PluginManager = IPluginManager::Get();
            FString PluginDir = PluginManager.FindPlugin("EOSIntegrationKit")->GetBaseDir();
            FString EOSBootstrapperToolPath = PluginDir / TEXT("Source/ThirdParty/EIKSDK/Tools/EOSBootstrapperTool.exe");

            //Show error if the tool is not found
            if (!FPaths::FileExists(EOSBootstrapperToolPath))
            {
                UE_LOG(LogTemp, Error, TEXT("EOSBootstrapperTool not found at %s"), *EOSBootstrapperToolPath);
                FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("EOSBootstrapperToolNotFound", "EOSBootstrapperTool not found. Ensure the tool is downloaded and placed in the correct directory and unzipped."));
                return;
            }
            // Output path for EOSBootstrapper.exe
            FString EOSBootstrapperOutputPath = SelectedFileDir / TEXT("EOSBootstrapper.exe");

            // Command-line arguments for EOSBootstrapperTool
            FString CommandLineArgs = FString::Printf(
                TEXT("\"%s\" -o \"%s\" -a \"%s\""),
                *EOSBootstrapperToolPath,
                *EOSBootstrapperOutputPath,
                *SelectedFileName
            );

            // Launch the command prompt with the tool and arguments
            FString Command = FString::Printf(TEXT("cmd.exe /K %s"), *CommandLineArgs);
            UE_LOG(LogTemp, Log, TEXT("Command: %s"), *Command);
            FProcHandle ProcessHandle = FPlatformProcess::CreateProc(
                TEXT("cmd.exe"),
                *FString::Printf(TEXT("/K \"%s\""), *CommandLineArgs),
                true,   // bLaunchDetached
                false,  // bLaunchHidden
                false,  // bLaunchReallyHidden
                nullptr, // OutProcessId
                0,      // PriorityModifier
                nullptr,// OptionalWorkingDirectory
                nullptr // Pipe for output
            );

            if (!ProcessHandle.IsValid())
            {
                UE_LOG(LogTemp, Error, TEXT("Failed to launch the EOSBootstrapperTool in the command prompt."));
                //Show a message box
                FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("FailedToLaunchEOSBootstrapperTool", "Failed to launch the EOSBootstrapperTool in the command prompt."));
            }
            else
            {
                UE_LOG(LogTemp, Log, TEXT("EOSBootstrapperTool launched successfully."));
                //Show a message box
                FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("EOSBootstrapperToolLaunched", "EOSBootstrapperTool launched successfully."));
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("No file selected or operation canceled."));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to access the Desktop Platform Module."));
    }
}

 TSharedRef<SWidget> FEIKEditorModule::GenerateMenuContent()
{
    FMenuBuilder MenuBuilder(true, nullptr);

    MenuBuilder.AddMenuEntry(
        LOCTEXT("Option2", "Epic DevPortal"),
        LOCTEXT("Option2_Tooltip", "Open the Epic's DevPortal in the browser"),
#if ENGINE_MAJOR_VERSION == 5
        FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.OpenInBrowser"),
#else
        FSlateIcon(FEditorStyle::GetStyleSetName(), "ContentBrowser.ImportIcon"),
#endif
        FUIAction(FExecuteAction::CreateRaw(this, &FEIKEditorModule::OpenDevPortal))
    );
    
    MenuBuilder.AddMenuEntry(
    LOCTEXT("Option3", "Launch DevTool"),
    LOCTEXT("Option3_Tooltip", "Open the Epic's DevPortal in the browser"),
#if ENGINE_MAJOR_VERSION == 5
    FSlateIcon(FAppStyle::GetAppStyleSetName(), "DeveloperTools.MenuIcon"),
#else
    FSlateIcon(FEditorStyle::GetStyleSetName(), "DeveloperTools.MenuIcon"),
#endif
    FUIAction(FExecuteAction::CreateRaw(this, &FEIKEditorModule::OpenDevTool))
);

    MenuBuilder.AddMenuEntry(
        LOCTEXT("Option5", "Redistributable Installer Tool"),
        LOCTEXT("Option5_Tooltip", "Add the redistributable installer tool to a build"),
#if ENGINE_MAJOR_VERSION == 5
        FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.Install"),
#else
        FSlateIcon(FEditorStyle::GetStyleSetName(), "ContentBrowser.ImportIcon"),
#endif
        FUIAction(FExecuteAction::CreateRaw(this, &FEIKEditorModule::OpenRedistributableInstallerTool))
        );

    MenuBuilder.BeginSection(NAME_None, LOCTEXT("OneClickDeployHeader", "One Click Deploy"));

    MenuBuilder.AddMenuEntry(
        LOCTEXT("Option1", "Package and Deploy"),
        LOCTEXT("Option1_Tooltip", "Package and deploy the game to Epic Games"),
#if ENGINE_MAJOR_VERSION == 5
FSlateIcon(FAppStyle::GetAppStyleSetName(), "PlayWorld.RepeatLastLaunch"),
#else
        FSlateIcon(FEditorStyle::GetStyleSetName(), "PlayWorld.RepeatLastLaunch"),
#endif
        FUIAction(FExecuteAction::CreateRaw(this, &FEIKEditorModule::OnPackageAndDeploySelected))
    );


    MenuBuilder.AddMenuEntry(
        LOCTEXT("Option4", "Create/Reset Packaging Profile"),
        LOCTEXT("Option4_Tooltip", "Create or reset the packaging profile"),
#if ENGINE_MAJOR_VERSION == 5
        FSlateIcon(FAppStyle::GetAppStyleSetName(), "MainFrame.PackageProject"),
#else
        FSlateIcon(FEditorStyle::GetStyleSetName(), "MainFrame.PackageProject"),
#endif
        FUIAction(FExecuteAction::CreateLambda([]
        {
            UEIKSettings* Settings = GetMutableDefault<UEIKSettings>();
            ILauncherServicesModule& LauncherServicesModule = FModuleManager::LoadModuleChecked<ILauncherServicesModule>(TEXT("LauncherServices"));
            ILauncherProfileManagerRef LauncherProfileManagerRef = LauncherServicesModule.GetProfileManager();
            if (auto Profile = LauncherProfileManagerRef->FindProfile("EIKProfile"))
            {
                LauncherProfileManagerRef->RemoveProfile(Profile.ToSharedRef());
            }
            ILauncherProfilePtr LauncherProfilePtr = LauncherProfileManagerRef->AddNewProfile();
            LauncherProfilePtr->SetName("EIKProfile");
            LauncherProfilePtr->SetBuildConfiguration(static_cast<EBuildConfiguration>(Settings->OneClick_BuildConfiguration.GetValue()));
            LauncherProfilePtr->SetDeploymentMode(ELauncherProfileDeploymentModes::DoNotDeploy);
            LauncherProfilePtr->SetCookMode(ELauncherProfileCookModes::ByTheBook);
            LauncherProfilePtr->AddCookedPlatform("Windows");
            LauncherProfilePtr->SetPackagingMode(ELauncherProfilePackagingModes::Locally);
            LauncherProfilePtr->SetLaunchMode(ELauncherProfileLaunchModes::DoNotLaunch);
            LauncherProfilePtr->SetArchive(true);            
            LauncherProfilePtr->SetArchiveDirectory(FPaths::ConvertRelativePathToFull(FPaths::ProjectDir() / TEXT("EosBuilds") / TEXT("Windows")));
            LauncherProfileManagerRef->SaveProfile(LauncherProfilePtr.ToSharedRef());
            LauncherProfileManagerRef->SaveSettings();
            FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("ProfileCreated", "Packaging profile created successfully. Please configure the profile in Project Launcher as needed."));
        }))
    );

    MenuBuilder.BeginSection(NAME_None, LOCTEXT("PluginInfoHeader", "Plugin Info"));

    MenuBuilder.AddMenuEntry(
        LOCTEXT("Settings", "Settings"),
        LOCTEXT("Settings_Tooltip", "Open the plugin settings"),
#if ENGINE_MAJOR_VERSION == 5
        FSlateIcon(FAppStyle::GetAppStyleSetName(), "SourceControl.Actions.ChangeSettings"),
#else
        FSlateIcon(FEditorStyle::GetStyleSetName(), "SourceControl.Actions.ChangeSettings"),
#endif
        FUIAction(FExecuteAction::CreateLambda([]
        {
            FModuleManager::LoadModuleChecked<ISettingsModule>("Settings").ShowViewer("Project", "Game", "Online Subsystem EIK");
        }))
        );

    MenuBuilder.AddMenuEntry(
    LOCTEXT("Documentation", "Documentation"),
    LOCTEXT("PluginDocumentation_Tooltip", "Open the plugin documentation"),
#if ENGINE_MAJOR_VERSION == 5
    FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.Documentation"),
#else
    FSlateIcon(FEditorStyle::GetStyleSetName(), "GraphEditor.GoToDocumentation"),
#endif
    FUIAction(FExecuteAction::CreateLambda([]
    {
        UKismetSystemLibrary::LaunchURL("eik.betide.studio");
    }))
);

    MenuBuilder.AddMenuEntry(
        LOCTEXT("Marketplace", "Marketplace"),
        LOCTEXT("PluginMarketplace_Tooltip", "Open the plugin marketplace page"),
#if ENGINE_MAJOR_VERSION == 5
        FSlateIcon(FAppStyle::GetAppStyleSetName(), "MainFrame.OpenMarketplace"),
#else
        FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.OpenMarketplace"),
#endif
        FUIAction(FExecuteAction::CreateLambda([]
        {
            UKismetSystemLibrary::LaunchURL("https://www.unrealengine.com/marketplace/en-US/profile/Betide+Studio");
        }))
    );
    
    return MenuBuilder.MakeWidget();
}



void FEIKEditorModule::OnPackageAndDeploySelected()
{
#if PLATFORM_WINDOWS
    UEIKSettings* Settings = GetMutableDefault<UEIKSettings>();
    FEOSArtifactSettings ArtifactSettings;
    if (Settings->OneClick_ArtifactId.IsEmpty() || Settings->OneClick_ClientId.IsEmpty() || Settings->OneClick_ClientSecret.IsEmpty() || Settings->OneClick_OrganizationId.IsEmpty() || Settings->OneClick_ProductId.IsEmpty() || Settings->OneClick_CloudDirOverride.IsEmpty())
    {
        FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("BuildConfigurationNotSet", "Please set the Artifact ID, Client ID, Client Secret, Organization ID, and Product ID in the settings. Eeither the settings are empty or not saved."));
        return;
    }
    // Create the notification item
    FNotificationInfo Info(LOCTEXT("PackagingInProgress", "Packaging in progress..."));
    Info.bFireAndForget = false;
    Info.FadeOutDuration = 0.5f;
    Info.ExpireDuration = 3.0f;
    Info.ButtonDetails.Add(FNotificationButtonInfo(
        LOCTEXT("CancelPackage", "Cancel"),
        LOCTEXT("CancelPackage_Tooltip", "Cancel the packaging process"),
        FSimpleDelegate::CreateLambda([this]() {
            if (GEditor->LauncherWorker.IsValid())
            {
                GEditor->LauncherWorker->Cancel();
                AsyncTask(ENamedThreads::GameThread, [&]()
                {
                    if (NotificationItem.IsValid())
                    {
                        NotificationItem->SetText(LOCTEXT("PackageCanceled", "Packaging process canceled"));
                        NotificationItem->SetCompletionState(SNotificationItem::CS_Fail);
                        NotificationItem->ExpireAndFadeout();
                    }
                });
            }
            else
            {
                AsyncTask(ENamedThreads::GameThread, [&]()
                {
                    if (NotificationItem.IsValid())
                    {
                        NotificationItem->SetText(LOCTEXT("PackageCanceled", "Packaging process canceled"));
                        NotificationItem->SetCompletionState(SNotificationItem::CS_Fail);
                        NotificationItem->ExpireAndFadeout();
                    }
                });
            }
        })
    ));
    NotificationItem = FSlateNotificationManager::Get().AddNotification(Info);
    if (NotificationItem.IsValid())
    {
        NotificationItem->SetCompletionState(SNotificationItem::CS_Pending);
    }
    UEIKSettings* EIkSettings = GetMutableDefault<UEIKSettings>();
    ILauncherServicesModule& LauncherServicesModule = FModuleManager::LoadModuleChecked<ILauncherServicesModule>(TEXT("LauncherServices"));
    ILauncherProfileManagerRef LauncherProfileManagerRef = LauncherServicesModule.GetProfileManager();
    auto LauncherProfilePtr = LauncherProfileManagerRef->FindProfile("EIKProfile");
    if (!LauncherProfilePtr.IsValid())
    {
        FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("ProfileNotCreated", "Packaging profile not created. Please create a profile first."));
        AsyncTask(ENamedThreads::GameThread, [&]()
        {
            if (NotificationItem.IsValid())
            {
                NotificationItem->SetText(LOCTEXT("PackageCanceled", "Packaging process canceled"));
                NotificationItem->SetCompletionState(SNotificationItem::CS_Fail);
                NotificationItem->ExpireAndFadeout();
            }
        });
        return;
    }
    ILauncherRef LauncherRef = LauncherServicesModule.CreateLauncher();
    ITargetDeviceServicesModule& TargetDeviceServicesModule = FModuleManager::LoadModuleChecked<ITargetDeviceServicesModule>("TargetDeviceServices");
    GEditor->LauncherWorker = LauncherRef->Launch(TargetDeviceServicesModule.GetDeviceProxyManager(), LauncherProfilePtr.ToSharedRef());

    GEditor->LauncherWorker->OnOutputReceived().AddLambda([](const FString& String) {
        UE_LOG(LogEikEditor, Log, TEXT("%s"), *String);
    });

    GEditor->LauncherWorker->OnStageStarted().AddLambda([](const FString& String) {
    });
    GEditor->LauncherWorker->OnCanceled().AddLambda([this](double X)
    {
    if (NotificationItem.IsValid())
    {
        AsyncTask(ENamedThreads::GameThread, [&]()
        {
            NotificationItem->SetText(LOCTEXT("PackageCanceled", "Packaging process canceled"));
            NotificationItem->SetCompletionState(SNotificationItem::CS_Fail);
            NotificationItem->ExpireAndFadeout();
        });
    }
});
    GEditor->LauncherWorker->OnCompleted().AddLambda([this, Settings](bool bArg, double X, int I)
{
    if (bArg)
    {
        IPluginManager& PluginManager = IPluginManager::Get();
        FString PluginDir = PluginManager.FindPlugin("EOSIntegrationKit")->GetBaseDir();
        FString BuildPatchToolDir = PluginDir / TEXT("Source/ThirdParty/EIKSDK/Tools/BuildPatchTool_1.6.0/Engine/Binaries/Win64");
        FString BuildPatchToolExe = BuildPatchToolDir / TEXT("BuildPatchTool.exe");
        FString FilePath = Settings->OneClick_BuildRootOverride.IsEmpty() ? FPaths::ConvertRelativePathToFull(FPaths::ProjectDir() / TEXT("EosBuilds") / TEXT("Windows")) : Settings->OneClick_BuildRootOverride;
        FString VersionNumber = Settings->OneClick_BuildVersionOverride.IsEmpty() ? FString::Printf(TEXT("%s_%s"), FApp::GetProjectName(), *FDateTime::Now().ToString(TEXT("%Y%m%d%H%M%S"))) : Settings->OneClick_BuildVersionOverride;
        FString CloudDir = Settings->OneClick_CloudDirOverride.IsEmpty() ? TEXT("\\\"\\\"")  : Settings->OneClick_CloudDirOverride;
        FString AppArgs = Settings->OneClick_AppArgsOverride.IsEmpty() ? TEXT("\\\"\\\"")  : Settings->OneClick_AppArgsOverride;
        FString AppLaunch = Settings->OneClick_AppLaunchOverride.IsEmpty() ? FString::Printf(TEXT("%s.exe"), FApp::GetProjectName()) : Settings->OneClick_AppLaunchOverride;
        FString ParametersForBuildPatchTool = FString::Printf(TEXT("-ClientId=%s -ClientSecret=%s -OrganizationId=%s -ProductId=%s -ArtifactId=%s -mode=UploadBinary -BuildRoot=%s -CloudDir=%s -BuildVersion=%s -AppArgs=%s -AppLaunch=%s"),
            *Settings->OneClick_ClientId, *Settings->OneClick_ClientSecret, *Settings->OneClick_OrganizationId, *Settings->OneClick_ProductId, *Settings->OneClick_ArtifactId, *FilePath, *CloudDir, *VersionNumber, *AppArgs, *AppLaunch);
        UE_LOG(LogEikEditor, Log, TEXT("Build Patch Tool Parameters: %s"), *ParametersForBuildPatchTool);
        InteractiveProcess = MakeShareable(new FInteractiveProcess(*BuildPatchToolExe, ParametersForBuildPatchTool, false, true));
        InteractiveProcess->OnCompleted().BindRaw(this, &FEIKEditorModule::BuildProcessCompleted);
        InteractiveProcess->OnCanceled().BindRaw(this, &FEIKEditorModule::HandleProcessCanceled);
        InteractiveProcess->OnOutput().BindRaw(this, &FEIKEditorModule::HandleProcessOutput);
        InteractiveProcess->Launch();
    }
    else
    {
        AsyncTask(ENamedThreads::GameThread, [&]()
        {
            if (NotificationItem.IsValid())
            {
                NotificationItem->SetText(LOCTEXT("Package Failed", "Package Failed"));
                NotificationItem->SetCompletionState(SNotificationItem::CS_Fail);
                NotificationItem->ExpireAndFadeout();
            }
        });
    }
});
#else
    FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("PlatformNotSupported", "Platform not supported"));
#endif
}



#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FEIKEditorModule, EIKEditor)
