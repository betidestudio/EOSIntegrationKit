#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FEOSIntegrationKitModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    void ConfigureOnlineSubsystemEIK() const;
    static void ConfigureDedicatedServerConfigEIK();
    virtual void ShutdownModule() override;
};
