// Copyright (c) 2024 xist.gg

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Launch/Resources/Version.h"
#if EIKDISCORDACTIVE
#include "discord-cpp/discord.h"
#endif
#include "DiscordGame.h"
#include "Subsystems/EngineSubsystem.h"
#include "Engine/Engine.h"
#include "Containers/Ticker.h"
#include "DiscordGameSubsystem.generated.h"

/**
 * Discord Game Subsystem
 *
 * Base subsystem to manage the Discord GameSDK.
 *
 * This handles loading the Discord GameSDK DLLs and automatically reconnecting
 * to Discord as needed. This doesn't actually DO anything with the GameSDK
 * other than load it and actively manage the connection.
 *
 * You are expected to create your own derivative subsystem to implement any
 * functionality you wish to have in your game. For an example, see
 * `UCustomDiscordGameSubsystem` in the sample project.
 * 
 * @see https://github.com/XistGG/DiscordGameSample/Source/DiscordGameSample/CustomDiscordGameSubsystem.h
 * @see https://github.com/XistGG/DiscordGameSample/Source/DiscordGameSample/CustomDiscordGameSubsystem.cpp
 *
 * If you prefer, you can configure this in DefaultGame.ini by setting some INI such as:
 *
 *   [/Script/DiscordGame.DiscordGameSubsystem]
 *   ClientId=1192487163825246269
 *   CreateRetryTime=5.0
 */
UCLASS(Config=Game)
class DISCORDGAME_API UDiscordGameSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()

public:
	/** Get this subsystem, if the Engine exists, else nullptr */
	static UDiscordGameSubsystem* Get() { return GEngine ? GEngine->GetEngineSubsystem<UDiscordGameSubsystem>() : nullptr; }

	// Set Class Defaults
	UDiscordGameSubsystem();

	//~USubsystem interface
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	//~End of USubsystem interface

	/**
	 * Have we successfully loaded the Discord GameSDK DLL?
	 *
	 * We'll never be able to manage a Discord connection if we haven't already
	 * loaded the DLL.  If this returns TRUE it DOES NOT MEAN that Discord is
	 * actually installed and running on the current machine.  This only tells
	 * us whether or not the DLL has been loaded.
	 * 
	 * @return TRUE if the Discord GameSDK DLL has been successfully loaded into the Engine, else FALSE
	 */
	
	bool IsDiscordSDKLoaded() const
	{
#if EIKDISCORDACTIVE
		return DiscordGameModule && DiscordGameModule->IsDiscordSDKLoaded();
#else
		UE_LOG(LogTemp, Warning, TEXT("Discord SDK Not Loaded"));
		return false;
#endif
	}

	/**
	 * Is Discord currently running on the local machine?
	 *
	 * This tells us whether or not we can attempt to send commands to Discord.
	 * If this returns FALSE you MUST NOT try to obtain a reference to DiscordCore().
	 * If this returns TRUE then you can use DiscordCore().
	 *
	 * @return TRUE if we have a connection to Discord, else FALSE
	 */
	UFUNCTION(BlueprintCallable, Category = "Discord")
	bool IsDiscordRunning() const
	{
#if EIKDISCORDACTIVE
		return DiscordCorePtr != nullptr;
#else
		return false;
#endif
	}

	UFUNCTION(BlueprintCallable, Category = "Discord")
	FString GetDiscordDisplayName()
	{
#if EIKDISCORDACTIVE
		if(IsDiscordRunning() && IsDiscordSDKLoaded())
		{
			discord::User CurrentUser;
			auto Rsult = DiscordCore().UserManager().GetCurrentUser(&CurrentUser);
			if(Rsult == discord::Result::Ok)
			{
				return UTF8_TO_TCHAR(CurrentUser.GetUsername());
			}
			return TEXT("Failed to get Discord User");
		}
		return TEXT("Discord is not running");
#else
		return TEXT("Discord setup files are not included in the project");
#endif
	}

	UFUNCTION(BlueprintCallable, Category = "Discord")
	int64 GetDiscordUserId()
	{
#if EIKDISCORDACTIVE
		if(IsDiscordRunning() && IsDiscordSDKLoaded())
		{
			discord::User CurrentUser;
			auto Rsult = DiscordCore().UserManager().GetCurrentUser(&CurrentUser);
			if(Rsult == discord::Result::Ok)
			{
				return CurrentUser.GetId();
			}
			UE_LOG(LogDiscord, Error, TEXT("Failed to get Discord Auth Token due to error: %d"), static_cast<int32>(Rsult));
			return -1;
		}
		return -1;
#else
		return -1;
#endif
	}

	UFUNCTION(BlueprintCallable, Category = "Discord")
	void GetDiscordAvatarUrl(FString& Hash, FString& URL)
	{
#if EIKDISCORDACTIVE
		if(IsDiscordRunning() && IsDiscordSDKLoaded())
		{
			discord::User CurrentUser;
			auto Rsult = DiscordCore().UserManager().GetCurrentUser(&CurrentUser);
			if(Rsult == discord::Result::Ok)
			{
				Hash =  UTF8_TO_TCHAR(CurrentUser.GetAvatar());
				URL = FString::Printf(TEXT("https://cdn.discordapp.com/avatars/%lld/%s.png"), CurrentUser.GetId(), *Hash);
				return;
			}
			UE_LOG(LogDiscord, Error, TEXT("Failed to get Discord Auth Token due to error: %d"), static_cast<int32>(Rsult));
			return;
		}
		return;
#else
		UE_LOG(LogDiscord, Error, TEXT("Discord setup files are not included in the project"));
		return;
#endif
	}
#if EIKDISCORDACTIVE

	/**
	 * Get a reference to the Discord Core.
	 *
	 * Before you call this, you MUST check IsDiscordRunning().
	 *
	 * If you try to call this when discord is not running, it will throw an exception,
	 * which will break your game.
	 *
	 * @return DiscordCore reference
	 */
	FORCEINLINE_DEBUGGABLE discord::Core& DiscordCore() const
	{
		checkf(DiscordCorePtr, TEXT("Discord is not running"));
		return *DiscordCorePtr;
	}
#endif

	UFUNCTION(BlueprintCallable, Category = "Discord")
	bool IsClientIdValid() const { return ClientId != 0; }

protected:
	/**
	 * Called any time we gain a new connection to Discord running on the local machine.
	 *
	 * Override this in derived subsystems to hook into this event.
	 */
	virtual void NativeOnDiscordCoreCreated();

	/**
	 * Called any time the DiscordCore is reset.
	 *
	 * This may happen during gameplay if for example Discord goes away during the game.
	 *
	 * Override this in derived subsystems to hook into this event.
	 */
	virtual void NativeOnDiscordCoreReset();

#if EIKDISCORDACTIVE

	/**
	 * Called every time we fail to connect to Discord.
	 *
	 * If you want to implement a more sophisticated error handling/retry system, for
	 * example stopping the ticking after X number of failures etc, this is the place
	 * to add that logic.
	 *
	 * @param Result The error result from discord::Core::Create
	 */
	virtual void NativeOnDiscordConnectError(discord::Result Result);

	/**
	 * This is registered as the Discord "Log Hook"
	 *
	 * Every message Discord wants to log is sent to this method.
	 *
	 * @param Level Discord Log Message Level
	 * @param Message The Message that Discord is trying to log
	 */
	virtual void NativeOnDiscordLogMessage(discord::LogLevel Level, const FString& Message) const;

	/**
	 * Helper method to log the result of an asynchronous Discord event result
	 * 
	 * @param Result The Result of the async event
	 * @param RequestDescription Description of what async event this result is for
	 */
	void LogDiscordResult(discord::Result Result, const FString& RequestDescription) const;
#endif

	/**
	 * Enable or Disable Ticking for this Subsystem
	 * @param bWantTicking True if we want the subsystem to start Ticking; False if we want it to stop
	 */
	void SetTickEnabled(bool bWantTicking);

	/**
	 * Your Game's Discord Application ID (or Client ID)
	 *
	 * You should override this in your custom Subsystem's constructor.
	 * To get your own Application ID:
	 * @see https://discord.com/developers/applications
	 */
	UPROPERTY(Config, EditDefaultsOnly, Category = "Discord")
	uint64 ClientId {0};

#if EIKDISCORDACTIVE

	/**
	 * The minimum log level for Discord's internal logs that we want to see.
	 *
	 * This determines which logs result in calls to NativeOnDiscordLogMessage,
	 * and which do not.
	 */
	discord::LogLevel MinimumLogLevel;
#endif

	/**
	 * The minimum number of seconds to wait between DiscordCore connection retries.
	 *
	 * Set this too low and you'll waste a lot of CPU whenever Discord isn't running.
	 *
	 * Set this too high and it will take unreasonably long for the game to connect
	 * to Discord if/when Discord starts after the game.
	 */
	UPROPERTY(Config, EditDefaultsOnly, Category = "Discord")
	float CreateRetryTime;

private:
	/**
	 * Subsystem Tick Function
	 *
	 * Discord requires ticking to maintain its connection.  This method allows it to do so.
	 * If/when Discord is not currently running, this instead actively attempts to reconnect
	 * to Discord.
	 *
	 * @param DeltaTime Tick DeltaTime
	 * @return True
	 */
	bool Tick(float DeltaTime);

	/**
	 * Called by Tick if/when Discord is not currently connected, this method is responsible
	 * for trying to reconnect to Discord.
	 * 
	 * @param DeltaTime Tick DeltaTime
	 */
	void TryCreateDiscordCore(float DeltaTime);

	/**
	 * Explicitly Reset (and possibly disconnect from) DiscordCore.
	 *
	 * This will be called automatically when DiscordCore experiences fatal errors, for example.
	 */
	void ResetDiscordCore();

	/** Pointer to DiscordGame plugin module */
	FDiscordGameModule* DiscordGameModule {nullptr};

#if EIKDISCORDACTIVE
	/** Currently-connected DiscordCore, if any */
	discord::Core* DiscordCorePtr {nullptr};
#endif
	/** Tick delegate, if ticking is currently enabled */
#if ENGINE_MAJOR_VERSION == 5
	FTSTicker::FDelegateHandle TickDelegateHandle;
#else
	FDelegateHandle TickDelegateHandle;
#endif

	/** Amount of time (seconds) we will wait until trying to reconnect to Discord, if positive */
	float RetryWaitRemaining {-1.f};

	/** Toggles if/when we want to log connection errors (e.g. not repeatedly) */
	bool bLogConnectionErrors {true};

};
