#pragma once

#include "CoreMinimal.h"
#include "Interfaces/IPluginManager.h"
#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogEOSIntegrationKit, Log, All);
class FEOSIntegrationKitModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    void ConfigureOnlineSubsystemEIK() const;
    static void ConfigureDedicatedServerConfigEIK();
    virtual void ShutdownModule() override;
};
