// Copyright (c) 2024 xist.gg

#include "DiscordGameSubsystem.h"
#include "DiscordGame.h"

UDiscordGameSubsystem::UDiscordGameSubsystem(): ClientId(0)
{
#if EIKDISCORDACTIVE
	MinimumLogLevel = discord::LogLevel::Debug;
#endif
	CreateRetryTime = 5.0f;
}

bool UDiscordGameSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	TArray<UClass*> ChildClasses;
	GetDerivedClasses(GetClass(), OUT ChildClasses, false);

	UE_LOG(LogDiscord, Verbose, TEXT("Found %i derived classes when attemping to create DiscordGameSubsystem (%s)"), ChildClasses.Num(), *GetClass()->GetName());

	// Only create an instance if there is no override implementation defined elsewhere
	return ChildClasses.Num() == 0;
}

void UDiscordGameSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// Before we do anything else, grab a handle to the DiscordGame module
	DiscordGameModule = FDiscordGameModule::Get();

	// Only enable subsystem ticking if the SDK was successfully loaded
	if (IsDiscordSDKLoaded())
	{
		UE_LOG(LogDiscord, Log, TEXT("SDK loaded, enabling subsystem ticking"));

		SetTickEnabled(true);
	}
	else
	{
		UE_LOG(LogDiscord, Log, TEXT("SDK load failed, disabling Discord subsystem"));
	}
}

void UDiscordGameSubsystem::Deinitialize()
{
	// Stop ticking and reset the DiscordCore
	SetTickEnabled(false);
	ResetDiscordCore();
#if EIKDISCORDACTIVE
	DiscordGameModule = nullptr;
#endif
	Super::Deinitialize();
}

void UDiscordGameSubsystem::NativeOnDiscordCoreCreated()
{
#if EIKDISCORDACTIVE
	check(DiscordCorePtr);

	DiscordCorePtr->SetLogHook(MinimumLogLevel, [this](discord::LogLevel Level, const char* RawMessage)
	{
		const FString Message (UTF8_TO_TCHAR(RawMessage));
		NativeOnDiscordLogMessage(Level, Message);
	});
	DiscordCorePtr->UserManager().OnCurrentUserUpdate.Connect([this]()
	{
		UE_LOG(LogDiscord, Log, TEXT("Current User Updated"));
	});

	// In case we get disconnected and we need to try to reconnect,
	// make sure we'll log any future connection errors.
	bLogConnectionErrors = true;
#endif
}

void UDiscordGameSubsystem::NativeOnDiscordCoreReset()
{
}

#if EIKDISCORDACTIVE
void UDiscordGameSubsystem::NativeOnDiscordConnectError(discord::Result Result)
{
	switch (Result)
	{
	// The InternalError result is returned when the Discord App is not running.
	case discord::Result::InternalError:
		if (bLogConnectionErrors)
		{
			UE_LOG(LogDiscord, Warning, TEXT("Error(%i) Connecting to Discord; Discord App not running?"), Result);
			// Suppress subsequent repeated log messages since we expect this to recur
			// unless/until the Discord App is restarted by the player.
			bLogConnectionErrors = false;
		}
		else
		{
			// This message may get written every 5 seconds FOREVER, so make it VeryVerbose.
			// This way you can explicitly have it appear in the output log if you want,
			// but otherwise it doesn't spam you with "Discord STILL isn't running" messages.
			UE_LOG(LogDiscord, VeryVerbose, TEXT("Error(%i) Connecting to Discord; Discord App not running?"), Result);
		}
		break;

	default:
		// Not sure what this error is...  For now, spam the output log so you notice it.
		// Once you determine exactly what it is, update this code to handle it appropriately.
		UE_LOG(LogDiscord, Error, TEXT("Error(%i) Connecting to Discord; Unknown error"), Result);
		break;
	}
}

void UDiscordGameSubsystem::NativeOnDiscordLogMessage(discord::LogLevel Level, const FString& Message) const
{
	switch (Level)
	{
	case discord::LogLevel::Debug:
		UE_LOG(LogDiscord, Verbose, TEXT("Discord Internal Debug: %s"), *Message);
		break;
	case discord::LogLevel::Info:
		UE_LOG(LogDiscord, Log, TEXT("Discord Internal Message: %s"), *Message);
		break;
	case discord::LogLevel::Warn:
		UE_LOG(LogDiscord, Warning, TEXT("Discord Internal Warning: %s"), *Message);
		break;
	case discord::LogLevel::Error:
	default:
		UE_LOG(LogDiscord, Error, TEXT("Discord Internal Error: %s"), *Message);
		break;
	}
}

void UDiscordGameSubsystem::LogDiscordResult(discord::Result Result, const FString& RequestDescription) const
{
	switch (Result)
	{
	case discord::Result::Ok:
		UE_LOG(LogDiscord, Log, TEXT("Success %s"), *RequestDescription);
		break;
	default:
		UE_LOG(LogDiscord, Error, TEXT("Error(%i) %s"), Result, *RequestDescription);
		break;
	}
}
#endif
void UDiscordGameSubsystem::SetTickEnabled(bool bWantTicking)
{
#if EIKDISCORDACTIVE
	if (bWantTicking && !TickDelegateHandle.IsValid())
	{
		// Want to enable ticking and it is not currently enabled
		TickDelegateHandle = FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateUObject(this, &ThisClass::Tick));
	}
	else if (!bWantTicking && TickDelegateHandle.IsValid())
	{
		// Want to disable ticking and it is currently enabled
		FTSTicker::GetCoreTicker().RemoveTicker(TickDelegateHandle);
		TickDelegateHandle.Reset();
	}
#endif
}

bool UDiscordGameSubsystem::Tick(float DeltaTime)
{
#if EIKDISCORDACTIVE
	if (IsDiscordRunning())
	{
		const discord::Result Result = DiscordCorePtr->RunCallbacks();
		switch (Result)
		{
		case discord::Result::Ok:
			// The expected result; Discord is functioning normally
			break;

		case discord::Result::NotRunning:
			// Discord is no longer running, it is no longer usable and will never be again
			// unless/until we successfully initialize a new DiscordCore. 
			UE_LOG(LogDiscord, Warning, TEXT("Error(%i) Running Callbacks; Discord app is no longer running"), Result);
			ResetDiscordCore();
			break;

		default:
			// Unknown error, hopefully Discord can recover from this...
			// For now we just spam the log every tick with errors so you know what is happening.
			//
			// You will need to experiment with this to determine what errors are recoverable
			// and what errors are not, and manage them better than this:
			UE_LOG(LogDiscord, Error, TEXT("Error(%i) Running Callbacks"), Result);
			break;
		}
	}
	else if (IsDiscordSDKLoaded())
	{
		// Discord is not running, but we do have the SDK loaded.
		// Try to reconnect to DiscordCore.

		TryCreateDiscordCore(DeltaTime);
	}

	return true;
#else
	return false;
#endif
}

void UDiscordGameSubsystem::TryCreateDiscordCore(float DeltaTime)
{
	UE_LOG(LogDiscord, Log, TEXT("Attempting to create Discord Core"));
#if EIKDISCORDACTIVE
	RetryWaitRemaining -= DeltaTime;

	if (RetryWaitRemaining <= 0.f)
	{
		switch (const discord::Result Result = discord::Core::Create(ClientId, DiscordCreateFlags_NoRequireDiscord, &DiscordCorePtr))
		{
		case discord::Result::Ok:
			UE_LOG(LogDiscord, Log, TEXT("Created Discord Core"));
			NativeOnDiscordCoreCreated();
			break;

		default:
			NativeOnDiscordConnectError(Result);
			break;
		}

		// Don't try to create every single tick; wait some time
		RetryWaitRemaining = CreateRetryTime;
	}
#endif
}

void UDiscordGameSubsystem::ResetDiscordCore()
{
#if EIKDISCORDACTIVE
	if (DiscordCorePtr)
	{
		// Discord GameSDK doesn't provide any way to free memory !?
		//
		// It allocates memory with new() and it does not provide any way for us to free that memory.
		// If we explicitly delete, we sometimes get fatal Exception 0xc0000008, so we cannot do that.
		// Here we just have to hope that Discord isn't bad and they free the memory on fatal errors.
		// There may be a minor memory leak here, but this shouldn't happen often during the game.
		//
		//-- delete DiscordCorePtr;
		DiscordCorePtr = nullptr;

		// Allow child classes the opportunity to react to this event
		NativeOnDiscordCoreReset();
	}

	// Ensure that next tick we'll immediately try reconnecting (if still ticking)
	RetryWaitRemaining = -1;
#endif
}
