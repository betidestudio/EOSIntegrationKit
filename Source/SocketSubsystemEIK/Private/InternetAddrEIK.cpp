// Copyright Epic Games, Inc. All Rights Reserved.

#include "InternetAddrEIK.h"
#if ENGINE_MAJOR_VERSION == 5
#include "Online/CoreOnline.h"
#else
#include "UObject/CoreOnline.h"
#endif
#include "EOSSharedTypes.h"
#include "Containers/StringView.h"

DEFINE_LOG_CATEGORY(LogSocketSubsystemEOS);

inline uint8 PortToChannel(int32 InPort)
{
	return InPort > 255 ? InPort % 256 : FMath::Clamp(InPort, 0, 255);
}

FInternetAddrEOS::FInternetAddrEOS()
	: LocalUserId(nullptr)
	, RemoteUserId(nullptr)
	, Channel(0)
{
	SocketName[0] = '\0';
}

FInternetAddrEOS::FInternetAddrEOS(const FString& InRemoteUserId, const FString& InSocketName, const int32 InChannel)
	: LocalUserId(nullptr)
	, RemoteUserId(nullptr)
{
#if WITH_EOS_SDK
	RemoteUserId = EOS_ProductUserId_FromString(TCHAR_TO_UTF8(*InRemoteUserId));
#endif
	FCStringAnsi::Strcpy(SocketName, TCHAR_TO_UTF8(*InSocketName));
	Channel = PortToChannel(InChannel);
}

#if WITH_EOS_SDK
FInternetAddrEOS::FInternetAddrEOS(const EOS_ProductUserId InRemoteUserId, const FString& InSocketName, const int32 InChannel)
	: LocalUserId(nullptr)
	, RemoteUserId(InRemoteUserId)
	, Channel(InChannel)
{
	FCStringAnsi::Strcpy(SocketName, TCHAR_TO_UTF8(*InSocketName));
	Channel = PortToChannel(InChannel);
}
#endif

void FInternetAddrEOS::SetIp(uint32)
{
	UE_LOG(LogSocketSubsystemEOS, Warning, TEXT("Calls to FInternetAddrEOS::SetIp are not valid"));
}

void FInternetAddrEOS::SetIp(const TCHAR* InAddr, bool& bIsValid)
{
	bIsValid = false;

	if (InAddr == nullptr)
	{
		return;
	}

	Channel = 0;

	TArray<FString> UrlParts;
	FString FullAddress = InAddr;
	FullAddress.ParseIntoArray(UrlParts, EOS_URL_SEPARATOR, false);
	// Expect URLs to look like "EOS:PUID:SocketName:Channel" and channel can be optional
	if (UrlParts.Num() < 3 || UrlParts.Num() > 4)
	{
		return;
	}
	if (UrlParts[0] != EOS_CONNECTION_URL_PREFIX)
	{
		return;
	}
#if WITH_EOS_SDK
	RemoteUserId = EOS_ProductUserId_FromString(TCHAR_TO_UTF8(*UrlParts[1]));
	if (EOS_ProductUserId_IsValid(RemoteUserId) == EOS_FALSE)
#endif
	{
		return;
	}
	if (UrlParts[2].Len() == 0)
	{
		return;
	}
	SetSocketName(UrlParts[2]);
	if (UrlParts.Num() == 4)
	{
		Channel = FCString::Atoi(*UrlParts[3]);
	}
	bIsValid = true;
}

void FInternetAddrEOS::GetIp(uint32& OutAddr) const
{
	OutAddr = 0u;

	UE_LOG(LogSocketSubsystemEOS, Warning, TEXT("Calls to FInternetAddrEOS::GetIp are not valid"));
}

void FInternetAddrEOS::SetPort(int32 InPort)
{
	Channel = PortToChannel(InPort);
}

int32 FInternetAddrEOS::GetPort() const
{
	return Channel;
}

void FInternetAddrEOS::SetRawIp(const TArray<uint8>& RawAddr)
{
	// Need auto here, as might give us different return type depending on size of TCHAR
	auto ConvertedTCHARData = StringCast<TCHAR>(reinterpret_cast<const ANSICHAR*>(RawAddr.GetData()), RawAddr.Num());
	const FString IpAsString(ConvertedTCHARData.Length(), ConvertedTCHARData.Get());

	bool bUnused;
	SetIp(*IpAsString, bUnused);
}

TArray<uint8> FInternetAddrEOS::GetRawIp() const
{
	// We could do this more efficiently, but this was much faster to write.
	const FString StringVersion = ToString(true);

	// Need auto here, as might give us different return type depending on size of TCHAR
	auto ConvertedANSIData = StringCast<ANSICHAR>(*StringVersion, StringVersion.Len());

	TArray<uint8> OutData;
	for (int32 Index = 0; Index < ConvertedANSIData.Length(); ++Index)
	{
		OutData.Add(ConvertedANSIData.Get()[Index]);
	}

	return OutData;
}

void FInternetAddrEOS::SetAnyAddress()
{
}

void FInternetAddrEOS::SetBroadcastAddress()
{
}

void FInternetAddrEOS::SetLoopbackAddress()
{
}

FString FInternetAddrEOS::ToString(bool bAppendPort) const
{
	char PuidBuffer[64];
	int32 BufferLen = 64;
#if WITH_EOS_SDK
	if (EOS_ProductUserId_ToString(RemoteUserId, PuidBuffer, &BufferLen) != EOS_EResult::EOS_Success)
#endif
	{
		PuidBuffer[0] = '\0';
	}

	if (bAppendPort)
	{
		return FString::Printf(TEXT("%s%s%s%s%s%s%u"), EOS_CONNECTION_URL_PREFIX, EOS_URL_SEPARATOR, UTF8_TO_TCHAR(PuidBuffer), EOS_URL_SEPARATOR, UTF8_TO_TCHAR(SocketName), EOS_URL_SEPARATOR, Channel);
	}

	return FString::Printf(TEXT("%s%s%s%s%s"), EOS_CONNECTION_URL_PREFIX, EOS_URL_SEPARATOR, UTF8_TO_TCHAR(PuidBuffer), EOS_URL_SEPARATOR, UTF8_TO_TCHAR(SocketName));
}

uint32 FInternetAddrEOS::GetTypeHash() const
{
#if ENGINE_MAJOR_VERSION ==5 && ENGINE_MINOR_VERSION >= 2
	return HashCombine(HashCombine(HashCombine(GetTypeHashHelper((void*)LocalUserId), GetTypeHashHelper((void*)RemoteUserId)), GetTypeHashHelper(FAnsiStringView(SocketName, EOS_SOCKET_NAME_SIZE))), Channel);
#else 
	return HashCombine(HashCombine(HashCombine(::GetTypeHash((void*)LocalUserId), ::GetTypeHash((void*)RemoteUserId)), ::GetTypeHash(FAnsiStringView(SocketName, EOS_SOCKET_NAME_SIZE))), Channel);
#endif
}

bool FInternetAddrEOS::IsValid() const
{
#if WITH_EOS_SDK
	return (EOS_ProductUserId_IsValid(LocalUserId) == EOS_TRUE || EOS_ProductUserId_IsValid(RemoteUserId) == EOS_TRUE) && FCStringAnsi::Strlen(SocketName) > 0;
#else
	return false;
#endif
}

TSharedRef<FInternetAddr> FInternetAddrEOS::Clone() const
{
	return MakeShared<FInternetAddrEOS>(*this);
}
