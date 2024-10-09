// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreGlobals.h"
#include "Runtime/Launch/Resources/Version.h"
#include "Templates/SharedPointer.h"

#if WITH_EOS_SDK
#if defined(EOS_PLATFORM_BASE_FILE_NAME)
#include EOS_PLATFORM_BASE_FILE_NAME
#endif

#include "eos_common.h"
#endif

// Expect URLs to look like "EOS:PUID:SocketName:Channel"
#define EOS_CONNECTION_URL_PREFIX TEXT("EOS")
#define EOS_URL_SEPARATOR TEXT(":")

/** Used to store a pointer to the EOS callback object without knowing type */
class EIKSHARED_API FCallbackBase
{
public:
	virtual ~FCallbackBase() {}
};
#if WITH_EOS_SDK

/** Class to handle all callbacks generically using a lambda to process callback results */
#if ENGINE_MAJOR_VERSION == 5
template<typename CallbackFuncType, typename CallbackParamType, typename OwningType, typename CallbackReturnType = void, typename... CallbackExtraParams>
#else
template<typename CallbackFuncType, typename CallbackType>
#endif
class TEIKGlobalCallback :
	public FCallbackBase
{
public:
#if ENGINE_MAJOR_VERSION == 5
	TFunction<CallbackReturnType(const CallbackParamType*, CallbackExtraParams... ExtraParams)> CallbackLambda;
	TEIKGlobalCallback(TWeakPtr<OwningType> InOwner)
		: FCallbackBase()
		, Owner(InOwner)
	{
	}
#else
	TFunction<void(const CallbackType*)> CallbackLambda;
	TEIKGlobalCallback() = default;
#endif
	virtual ~TEIKGlobalCallback() = default;

	CallbackFuncType GetCallbackPtr()
	{
		return &CallbackImpl;
	}

	/** Is this callback intended for the game thread */
	bool bIsGameThreadCallback = true;

private:

#if ENGINE_MAJOR_VERSION == 5

	/** The object that needs to be checked for lifetime before calling the callback */
	TWeakPtr<OwningType> Owner;

	static CallbackReturnType EOS_CALL CallbackImpl(const CallbackParamType* Data, CallbackExtraParams... ExtraParams)
	{
		TEIKGlobalCallback* CallbackThis = (TEIKGlobalCallback*)Data->ClientData;
		check(CallbackThis);

		if (CallbackThis->bIsGameThreadCallback)
		{
			check(IsInGameThread());
		}

		if (CallbackThis->Owner.IsValid())
		{
			check(CallbackThis->CallbackLambda);

			if constexpr (std::is_void<CallbackReturnType>::value)
			{
				CallbackThis->CallbackLambda(Data, ExtraParams...);
			}
			else
			{
				return CallbackThis->CallbackLambda(Data, ExtraParams...);
			}
		}
		
		if constexpr (!std::is_void<CallbackReturnType>::value)
		{
			// we need to return _something_ to compile.
			return CallbackReturnType{};
		}
	}

#else

	static void EOS_CALL CallbackImpl(const CallbackType* Data)
	{
		check(IsInGameThread());

		TEIKGlobalCallback* CallbackThis = (TEIKGlobalCallback*)Data->ClientData;
		check(CallbackThis);

		check(CallbackThis->CallbackLambda);
		CallbackThis->CallbackLambda(Data);
	}
#endif
};

#endif