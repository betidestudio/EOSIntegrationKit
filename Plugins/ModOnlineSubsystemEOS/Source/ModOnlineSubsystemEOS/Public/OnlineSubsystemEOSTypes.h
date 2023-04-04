// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EOSSharedTypes.h"

/**
 * 
 */
class MODONLINESUBSYSTEMEOS_API OnlineSubsystemEIKTypes
{
public:
	OnlineSubsystemEIKTypes();
	~OnlineSubsystemEIKTypes();
};


/** Class to handle all callbacks generically using a lambda to process callback results */
template<typename CallbackFuncType, typename CallbackType>
class TEOSCallback :
	public FCallbackBase
{
public:
	TFunction<void(const CallbackType*)> CallbackLambda;

	TEOSCallback()
	{

	}
	virtual ~TEOSCallback() = default;


	CallbackFuncType GetCallbackPtr()
	{
		return &CallbackImpl;
	}

private:
	static void EOS_CALL CallbackImpl(const CallbackType* Data)
	{
		if (EOS_EResult_IsOperationComplete(Data->ResultCode) == EOS_FALSE)
		{
			// Ignore
			return;
		}
		check(IsInGameThread());

		TEOSCallback* CallbackThis = (TEOSCallback*)Data->ClientData;
		check(CallbackThis);

		if (FCallbackBase::ShouldCancelAllCallbacks())
		{
			delete CallbackThis;
			return;
		}

		check(CallbackThis->CallbackLambda);
		CallbackThis->CallbackLambda(Data);

		delete CallbackThis;
	}
};
