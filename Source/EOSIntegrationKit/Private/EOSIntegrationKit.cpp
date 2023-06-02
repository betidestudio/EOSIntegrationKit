#include "EOSIntegrationKit.h"

#define LOCTEXT_NAMESPACE "FEOSIntegrationKitModule"

void FEOSIntegrationKitModule::StartupModule()
{
    UE_LOG(LogTemp, Warning, TEXT("EOSIntegrationKit: StartupModule"));
}

void FEOSIntegrationKitModule::ShutdownModule()
{
    
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FEOSIntegrationKitModule, EOSIntegrationKit)