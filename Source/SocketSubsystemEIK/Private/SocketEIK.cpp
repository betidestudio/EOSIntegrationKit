// Copyright Epic Games, Inc. All Rights Reserved.

#include "SocketEIK.h"
#include "SocketTypes.h"
#include "SocketSubsystemEIK.h"

#if WITH_EOS_SDK
	#include "eos_p2p.h"
#endif

#if WANTS_NP_LOGGING

#include "Windows/AllowWindowsPlatformTypes.h"
THIRD_PARTY_INCLUDES_START
#include <Windows.h>
THIRD_PARTY_INCLUDES_END

const TCHAR* GetLogPrefix()
{
	static FString Prefix;
	if (Prefix.Len() == 0)
	{
		FParse::Value(FCommandLine::Get(), TEXT("LogPrefix="), Prefix);
		if (Prefix.Len() == 0)
		{
			Prefix = TEXT("Unknown");
		}
	}
	return *Prefix;
}

void NpLog(const TCHAR* Msg)
{
	static HWND EditWindow = NULL;
	// Get the edit window so we can send messages to it
	if (EditWindow == NULL)
	{
		HWND MainWindow = FindWindowW(NULL, L"Untitled - Notepad");
		if (MainWindow == NULL)
		{
			MainWindow = FindWindowW(NULL, L"*Untitled - Notepad");
		}
		if (MainWindow != NULL)
		{
			EditWindow = FindWindowExW(MainWindow, NULL, L"Edit", NULL);
		}
	}
	if (EditWindow != NULL)
	{
		SendMessageW(EditWindow, EM_REPLACESEL, TRUE, (LPARAM)Msg);
	}
}

#include "Windows/HideWindowsPlatformTypes.h"
#endif

FSocketEOS::FSocketEOS(FSocketSubsystemEIK& InSocketSubsystem, const FString& InSocketDescription)
	: FSocket(ESocketType::SOCKTYPE_Datagram, InSocketDescription, NAME_None)
	, SocketSubsystem(InSocketSubsystem)
	, bIsListening(false)
#if WITH_EOS_SDK
	, ConnectNotifyCallback(nullptr)
	, ConnectNotifyId(EOS_INVALID_NOTIFICATIONID)
	, ClosedNotifyCallback(nullptr)
	, ClosedNotifyId(EOS_INVALID_NOTIFICATIONID)
#endif
{
	CallbackAliveTracker = MakeShared<FCallbackBase>();
}

FSocketEOS::~FSocketEOS()
{
	Close();

	if (LocalAddress.IsValid())
	{
		SocketSubsystem.UnbindChannel(LocalAddress);
		LocalAddress = FInternetAddrEOS();
	}
	CallbackAliveTracker = nullptr;
}

bool FSocketEOS::Shutdown(ESocketShutdownMode Mode)
{
	SocketSubsystem.SetLastSocketError(ESocketErrors::SE_EOPNOTSUPP);
	return false;
}

bool FSocketEOS::Close()
{
	check(IsInGameThread() && "p2p does not support multithreading");

#if WITH_EOS_SDK
	if (ConnectNotifyId != EOS_INVALID_NOTIFICATIONID)
	{
		EOS_P2P_RemoveNotifyPeerConnectionRequest(SocketSubsystem.GetP2PHandle(), ConnectNotifyId);
	}
	delete ConnectNotifyCallback;
	ConnectNotifyCallback = nullptr;
	if (ClosedNotifyId != EOS_INVALID_NOTIFICATIONID)
	{
		EOS_P2P_RemoveNotifyPeerConnectionClosed(SocketSubsystem.GetP2PHandle(), ClosedNotifyId);
	}
	delete ClosedNotifyCallback;
	ClosedNotifyCallback = nullptr;

	if (LocalAddress.IsValid())
	{
		EOS_P2P_SocketId SocketId = { };
		SocketId.ApiVersion = EOS_P2P_SOCKETID_API_LATEST;
		FCStringAnsi::Strcpy(SocketId.SocketName, LocalAddress.GetSocketName());

		EOS_P2P_CloseConnectionsOptions Options = { };
		Options.ApiVersion = EOS_P2P_CLOSECONNECTIONS_API_LATEST;
		Options.LocalUserId = SocketSubsystem.GetLocalUserId();
		Options.SocketId = &SocketId;

		EOS_EResult Result = EOS_P2P_CloseConnections(SocketSubsystem.GetP2PHandle(), &Options);

		UE_LOG(LogSocketSubsystemEOS, Log, TEXT("Closing socket (%s) with result (%s)"), *LocalAddress.ToString(true), ANSI_TO_TCHAR(EOS_EResult_ToString(Result)));
		NP_LOG(TEXT("[%s] - Closing socket (%s) with result (%s)\r\n"), GetLogPrefix(), *LocalAddress.ToString(true), ANSI_TO_TCHAR(EOS_EResult_ToString(Result)));

		ClosedRemotes.Empty();
	}
#endif
	return true;
}

bool FSocketEOS::Bind(const FInternetAddr& Addr)
{
	check(IsInGameThread() && "p2p does not support multithreading");

	if (!Addr.IsValid())
	{
		UE_LOG(LogSocketSubsystemEOS, Warning, TEXT("Attempted to bind to invalid address. Address = (%s)"), *Addr.ToString(true));
		SocketSubsystem.SetLastSocketError(ESocketErrors::SE_EADDRNOTAVAIL);
		return false;
	}

	// Ensure we called Initialize so we know who we are
	if (LocalAddress.GetRemoteUserId() != nullptr)
	{
		UE_LOG(LogSocketSubsystemEOS, Warning, TEXT("Attempted to bind on a socket that was not initialized. Address = (%s)"), *Addr.ToString(true));
		SocketSubsystem.SetLastSocketError(ESocketErrors::SE_NOTINITIALISED);
		return false;
	}

	// If we have a remote user id, we're already bound
	if (LocalAddress.GetRemoteUserId() != nullptr)
	{
		UE_LOG(LogSocketSubsystemEOS, Warning, TEXT("Attempted to bind a socket that was already bound. ExistingAddress = (%s) NewAddress = (%s)"), *LocalAddress.ToString(true), *Addr.ToString(true));
		SocketSubsystem.SetLastSocketError(ESocketErrors::SE_EADDRINUSE);
		return false;
	}

	const FInternetAddrEOS& EOSAddr = static_cast<const FInternetAddrEOS&>(Addr);
	if (!SocketSubsystem.BindChannel(EOSAddr))
	{
		UE_LOG(LogSocketSubsystemEOS, Warning, TEXT("Attempted to bind a socket to a port in use. NewAddress = (%s)"), *Addr.ToString(true));
		// BindPort sets our LastSocketError
		return false;
	}

#if WITH_EOS_SDK
	EOS_ProductUserId LocalUserId = LocalAddress.GetLocalUserId();
#else
	void* LocalUserId = LocalAddress.GetLocalUserId();
#endif
	LocalAddress = EOSAddr;
	LocalAddress.SetLocalUserId(LocalUserId);

	UE_LOG(LogSocketSubsystemEOS, Verbose, TEXT("Successfully bound socket to address (%s)"), *LocalAddress.ToString(true));
	NP_LOG(TEXT("[%s] - Successfully bound socket to address (%s)\r\n"), GetLogPrefix(), *LocalAddress.ToString(true));
	return true;
}

bool FSocketEOS::Connect(const FInternetAddr& Addr)
{
	/** Not supported - connectionless (UDP) only */
	SocketSubsystem.SetLastSocketError(ESocketErrors::SE_EOPNOTSUPP);
	return false;
}

bool FSocketEOS::Listen(int32)
{
	check(IsInGameThread() && "p2p does not support multithreading");

	if (!LocalAddress.IsValid())
	{
		UE_LOG(LogSocketSubsystemEOS, Warning, TEXT("Attempted to listen without a bound address. Address = (%s)"), *LocalAddress.ToString(true));
		SocketSubsystem.SetLastSocketError(ESocketErrors::SE_EADDRINUSE);
		return false;
	}

#if WITH_EOS_SDK
	// Add listener for inbound connections
	EOS_P2P_SocketId SocketId = { };
	SocketId.ApiVersion = EOS_P2P_SOCKETID_API_LATEST;
	FCStringAnsi::Strcpy(SocketId.SocketName, LocalAddress.GetSocketName());

	EOS_P2P_AddNotifyPeerConnectionRequestOptions Options = { };
	Options.ApiVersion = EOS_P2P_ADDNOTIFYPEERCONNECTIONREQUEST_API_LATEST;
	Options.LocalUserId = LocalAddress.GetLocalUserId();
	Options.SocketId = &SocketId;

#if ENGINE_MAJOR_VERSION == 5
	ConnectNotifyCallback = new FConnectNotifyCallback(CallbackAliveTracker);
#else
	ConnectNotifyCallback = new FConnectNotifyCallback();
#endif
	ConnectNotifyCallback->CallbackLambda = [this](const EOS_P2P_OnIncomingConnectionRequestInfo* Info)
	{
		char PuidBuffer[64];
		int32 BufferLen = 64;
		if (EOS_ProductUserId_ToString(Info->RemoteUserId, PuidBuffer, &BufferLen) != EOS_EResult::EOS_Success)
		{
			PuidBuffer[0] = '\0';
		}
		FString RemoteUser(PuidBuffer);

		if (Info->LocalUserId == LocalAddress.GetLocalUserId() && FCStringAnsi::Stricmp(Info->SocketId->SocketName, LocalAddress.GetSocketName()) == 0)
		{
			// In case they disconnected and then reconnected, remove them from our closed list
			FInternetAddrEOS RemoteAddress(Info->RemoteUserId, Info->SocketId->SocketName, LocalAddress.GetChannel());
			RemoteAddress.SetLocalUserId(LocalAddress.GetLocalUserId());
			ClosedRemotes.Remove(RemoteAddress);

			EOS_P2P_SocketId SocketId = { };
			SocketId.ApiVersion = EOS_P2P_SOCKETID_API_LATEST;
			FCStringAnsi::Strcpy(SocketId.SocketName, Info->SocketId->SocketName);

			EOS_P2P_AcceptConnectionOptions Options = { };
			Options.ApiVersion = EOS_P2P_ACCEPTCONNECTION_API_LATEST;
			Options.LocalUserId = LocalAddress.GetLocalUserId();
			Options.RemoteUserId = Info->RemoteUserId;
			Options.SocketId = &SocketId;
			EOS_EResult AcceptResult = EOS_P2P_AcceptConnection(SocketSubsystem.GetP2PHandle(), &Options);
			if (AcceptResult == EOS_EResult::EOS_Success)
			{
				UE_LOG(LogSocketSubsystemEOS, Verbose, TEXT("Accepting connection request from (%s) on socket (%s)"), *RemoteUser, UTF8_TO_TCHAR(Info->SocketId->SocketName));
				NP_LOG(TEXT("[%s] - Accepting connection request from (%s) on socket (%s)\r\n"), GetLogPrefix(), *RemoteUser, UTF8_TO_TCHAR(Info->SocketId->SocketName));
			}
			else
			{
				UE_LOG(LogSocketSubsystemEOS, Error, TEXT("EOS_P2P_AcceptConnection from (%s) on socket (%s) failed with (%s)"), *RemoteUser, UTF8_TO_TCHAR(Info->SocketId->SocketName), ANSI_TO_TCHAR(EOS_EResult_ToString(AcceptResult)));
				NP_LOG(TEXT("[%s] - EOS_P2P_AcceptConnection from (%s) on socket (%s) failed with (%s)\r\n"), GetLogPrefix(), *RemoteUser, UTF8_TO_TCHAR(Info->SocketId->SocketName), ANSI_TO_TCHAR(EOS_EResult_ToString(AcceptResult)));
			}
		}
		else
		{
			UE_LOG(LogSocketSubsystemEOS, Warning, TEXT("Ignoring connection request from (%s) on socket (%s)"), *RemoteUser, UTF8_TO_TCHAR(Info->SocketId->SocketName));
		}
	};
	ConnectNotifyId = EOS_P2P_AddNotifyPeerConnectionRequest(SocketSubsystem.GetP2PHandle(), &Options, ConnectNotifyCallback, ConnectNotifyCallback->GetCallbackPtr());

	// Need to handle closures too
	RegisterClosedNotification();
#endif

	bIsListening = true;

	return true;
}

bool FSocketEOS::WaitForPendingConnection(bool& bHasPendingConnection, const FTimespan& WaitTime)
{
	/** Not supported - connectionless (UDP) only */
	SocketSubsystem.SetLastSocketError(ESocketErrors::SE_EOPNOTSUPP);
	return false;
}

bool FSocketEOS::HasPendingData(uint32& PendingDataSize)
{
	check(IsInGameThread() && "p2p does not support multithreading");

	PendingDataSize = 0;

#if WITH_EOS_SDK
	EOS_P2P_GetNextReceivedPacketSizeOptions Options = { };
	Options.ApiVersion = EOS_P2P_GETNEXTRECEIVEDPACKETSIZE_API_LATEST;
	Options.LocalUserId = LocalAddress.GetLocalUserId();
	uint8 Channel = LocalAddress.GetChannel();
	Options.RequestedChannel = &Channel;

	EOS_EResult Result = EOS_P2P_GetNextReceivedPacketSize(SocketSubsystem.GetP2PHandle(), &Options, &PendingDataSize);
	if (Result == EOS_EResult::EOS_NotFound)
	{
		return false;
	}
	if (Result != EOS_EResult::EOS_Success)
	{
		UE_LOG(LogSocketSubsystemEOS, Warning, TEXT("Unable to check for data on address (%s) result code = (%s)"), *LocalAddress.ToString(true), ANSI_TO_TCHAR(EOS_EResult_ToString(Result)));

		// @todo joeg - map EOS codes to UE4's
		SocketSubsystem.SetLastSocketError(ESocketErrors::SE_EINVAL);
		return false;
	}

	return true;
#else
	return false;
#endif
}

FSocket* FSocketEOS::Accept(const FString& InSocketDescription)
{
	/** Not supported - connectionless (UDP) only */
	SocketSubsystem.SetLastSocketError(ESocketErrors::SE_EOPNOTSUPP);
	return nullptr;
}

FSocket* FSocketEOS::Accept(FInternetAddr& OutAddr, const FString& InSocketDescription)
{
	/** Not supported - connectionless (UDP) only */
	SocketSubsystem.SetLastSocketError(ESocketErrors::SE_EOPNOTSUPP);
	return nullptr;
}

bool FSocketEOS::SendTo(const uint8* Data, int32 Count, int32& OutBytesSent, const FInternetAddr& Destination)
{
	check(IsInGameThread() && "p2p does not support multithreading");

	OutBytesSent = 0;

	if (!Destination.IsValid())
	{
		UE_LOG(LogSocketSubsystemEOS, Warning, TEXT("Unable to send data, invalid destination address. DestinationAddress = (%s)"), *Destination.ToString(true));

		SocketSubsystem.SetLastSocketError(ESocketErrors::SE_EADDRNOTAVAIL);
		return false;
	}

#if WITH_EOS_SDK
	if (Count > EOS_P2P_MAX_PACKET_SIZE)
	{
		UE_LOG(LogSocketSubsystemEOS, Warning, TEXT("Unable to send data, data over maximum size. Amount=[%d/%d] DestinationAddress = (%s)"), Count, EOS_P2P_MAX_PACKET_SIZE, *Destination.ToString(true));

		SocketSubsystem.SetLastSocketError(ESocketErrors::SE_EMSGSIZE);
		return false;
	}

	if (Count < 0)
	{
		UE_LOG(LogSocketSubsystemEOS, Warning, TEXT("Unable to send data, data invalid. Amount=[%d/%d] DestinationAddress = (%s)"), Count, EOS_P2P_MAX_PACKET_SIZE, *Destination.ToString(true));

		SocketSubsystem.SetLastSocketError(ESocketErrors::SE_EINVAL);
		return false;
	}
#endif 

	if (Data == nullptr && Count != 0)
	{
		UE_LOG(LogSocketSubsystemEOS, Warning, TEXT("Unable to send data, data invalid. DestinationAddress = (%s)"), *Destination.ToString(true));

		SocketSubsystem.SetLastSocketError(ESocketErrors::SE_EINVAL);
		return false;
	}

	if (!LocalAddress.IsValid())
	{
		UE_LOG(LogSocketSubsystemEOS, Warning, TEXT("Unable to send data, socket was not initialized. DestinationAddress = (%s)"), *Destination.ToString(true));

		SocketSubsystem.SetLastSocketError(ESocketErrors::SE_NOTINITIALISED);
		return false;
	}

	const FInternetAddrEOS& DestinationAddress = static_cast<const FInternetAddrEOS&>(Destination);
	if (LocalAddress == DestinationAddress)
	{
		UE_LOG(LogSocketSubsystemEOS, Warning, TEXT("Unable to send data, unable to send data to ourselves. DestinationAddress = (%s)"), *Destination.ToString(true));

		SocketSubsystem.SetLastSocketError(ESocketErrors::SE_ECONNREFUSED);
		return false;
	}

	// Check for sending to an address we explicitly closed
	if (WasClosed(DestinationAddress))
	{
		UE_LOG(LogSocketSubsystemEOS, Warning, TEXT("Unable to send data to closed connection. DestinationAddress = (%s)"), *Destination.ToString(true));

		SocketSubsystem.SetLastSocketError(ESocketErrors::SE_ECONNREFUSED);
		return false;
	}

#if WITH_EOS_SDK
	// Need to handle closures if we are a client and the server closes down on us
	RegisterClosedNotification();

	EOS_P2P_SocketId SocketId = { };
	SocketId.ApiVersion = EOS_P2P_SOCKETID_API_LATEST;
	FCStringAnsi::Strcpy(SocketId.SocketName, DestinationAddress.GetSocketName());

	EOS_P2P_SendPacketOptions Options = { };
	Options.ApiVersion = EOS_P2P_SENDPACKET_API_LATEST;
	Options.LocalUserId = LocalAddress.GetLocalUserId();
	Options.RemoteUserId = DestinationAddress.GetRemoteUserId();
	Options.SocketId = &SocketId;
	Options.bAllowDelayedDelivery = EOS_TRUE;
	Options.Reliability = EOS_EPacketReliability::EOS_PR_UnreliableUnordered;
	Options.Channel = DestinationAddress.GetChannel();
	Options.DataLengthBytes = Count;
	Options.Data = Data;
	EOS_EResult Result = EOS_P2P_SendPacket(SocketSubsystem.GetP2PHandle(), &Options);
	NP_LOG(TEXT("[%s] - EOS_P2P_SendPacket() to (%s) result code = (%s)\r\n"), GetLogPrefix(), *Destination.ToString(true), ANSI_TO_TCHAR(EOS_EResult_ToString(Result)));
	if (Result != EOS_EResult::EOS_Success)
	{
		UE_LOG(LogSocketSubsystemEOS, Error, TEXT("Unable to send data to (%s) result code = (%s)"), *Destination.ToString(true), ANSI_TO_TCHAR(EOS_EResult_ToString(Result)));

		// @todo joeg - map EOS codes to UE4's
		SocketSubsystem.SetLastSocketError(ESocketErrors::SE_EINVAL);
		return false;
	}
	OutBytesSent = Count;
	return true;
#else
	return false;
#endif
}

bool FSocketEOS::Send(const uint8* Data, int32 Count, int32& BytesSent)
{
	/** Not supported - connectionless (UDP) only */
	SocketSubsystem.SetLastSocketError(ESocketErrors::SE_EOPNOTSUPP);
	BytesSent = 0;
	return false;
}

bool FSocketEOS::RecvFrom(uint8* Data, int32 BufferSize, int32& BytesRead, FInternetAddr& Source, ESocketReceiveFlags::Type Flags)
{
	check(IsInGameThread() && "p2p does not support multithreading");
	BytesRead = 0;

	if (BufferSize < 0)
	{
		UE_LOG(LogSocketSubsystemEOS, Error, TEXT("Unable to receive data, receiving buffer was invalid. BufferSize = (%d)"), BufferSize);

		SocketSubsystem.SetLastSocketError(ESocketErrors::SE_EINVAL);
		return false;
	}

	if (Flags != ESocketReceiveFlags::None)
	{
		// We do not support peaking / blocking until a packet comes
		UE_LOG(LogSocketSubsystemEOS, Error, TEXT("Socket receive flags (%d) are not supported"), int32(Flags));

		SocketSubsystem.SetLastSocketError(ESocketErrors::SE_EOPNOTSUPP);
		return false;
	}

#if WITH_EOS_SDK
	EOS_P2P_ReceivePacketOptions Options = { };
	Options.ApiVersion = EOS_P2P_RECEIVEPACKET_API_LATEST;
	Options.LocalUserId = LocalAddress.GetLocalUserId();
	Options.MaxDataSizeBytes = BufferSize;
	uint8 Channel = LocalAddress.GetChannel();
	Options.RequestedChannel = &Channel;

	EOS_ProductUserId RemoteUserId = nullptr;
	EOS_P2P_SocketId SocketId;
	
	EOS_EResult Result = EOS_P2P_ReceivePacket(SocketSubsystem.GetP2PHandle(), &Options, &RemoteUserId, &SocketId, &Channel, Data, (uint32*)&BytesRead);
	NP_LOG(TEXT("[%s] - EOS_P2P_ReceivePacket() for user (%s) and channel (%d) with result code = (%s)\r\n"), GetLogPrefix(), *MakeStringFromProductUserId(LocalAddress.GetLocalUserId()), Channel, ANSI_TO_TCHAR(EOS_EResult_ToString(Result)));
	if (Result == EOS_EResult::EOS_NotFound)
	{
		// No data to read
		SocketSubsystem.SetLastSocketError(ESocketErrors::SE_EWOULDBLOCK);
		return false;
	}
	else if (Result != EOS_EResult::EOS_Success)
	{
		UE_LOG(LogSocketSubsystemEOS, Error, TEXT("Unable to receive data result code = (%s)"), ANSI_TO_TCHAR(EOS_EResult_ToString(Result)));

		// @todo joeg - map EOS codes to UE4's
		SocketSubsystem.SetLastSocketError(ESocketErrors::SE_EINVAL);
		return false;
	}

	FInternetAddrEOS& SourceAddress = static_cast<FInternetAddrEOS&>(Source);
	SourceAddress.SetLocalUserId(LocalAddress.GetLocalUserId());
	SourceAddress.SetRemoteUserId(RemoteUserId);
	SourceAddress.SetSocketName(SocketId.SocketName);
	SourceAddress.SetChannel(Channel);

	NP_LOG(TEXT("[%s] - EOS_P2P_ReceivePacket() of size (%d) from (%s)\r\n"), GetLogPrefix(), BytesRead, *SourceAddress.ToString(true));
	return true;
#else
	return false;
#endif
}

bool FSocketEOS::Recv(uint8* Data, int32 BufferSize, int32& BytesRead, ESocketReceiveFlags::Type Flags)
{
	BytesRead = 0;
	SocketSubsystem.SetLastSocketError(ESocketErrors::SE_EOPNOTSUPP);
	return false;
}

bool FSocketEOS::Wait(ESocketWaitConditions::Type Condition, FTimespan WaitTime)
{
	SocketSubsystem.SetLastSocketError(ESocketErrors::SE_EOPNOTSUPP);
	return false;
}

ESocketConnectionState FSocketEOS::GetConnectionState()
{
	return ESocketConnectionState::SCS_NotConnected;
}

void FSocketEOS::GetAddress(FInternetAddr& OutAddr)
{
	OutAddr = LocalAddress;
}

bool FSocketEOS::GetPeerAddress(FInternetAddr& OutAddr)
{
	SocketSubsystem.SetLastSocketError(ESocketErrors::SE_EOPNOTSUPP);
	return false;
}

bool FSocketEOS::SetNonBlocking(bool bIsNonBlocking)
{
	return true;
}

bool FSocketEOS::SetBroadcast(bool bAllowBroadcast)
{
	return true;
}

bool FSocketEOS::SetNoDelay(bool bIsNoDelay)
{
	return true;
}

bool FSocketEOS::JoinMulticastGroup(const FInternetAddr& GroupAddress)
{
	SocketSubsystem.SetLastSocketError(ESocketErrors::SE_EOPNOTSUPP);
	return false;
}

bool FSocketEOS::LeaveMulticastGroup(const FInternetAddr& GroupAddress)
{
	SocketSubsystem.SetLastSocketError(ESocketErrors::SE_EOPNOTSUPP);
	return false;
}

bool FSocketEOS::SetMulticastLoopback(bool bLoopback)
{
	SocketSubsystem.SetLastSocketError(ESocketErrors::SE_EOPNOTSUPP);
	return false;
}

bool FSocketEOS::SetMulticastTtl(uint8 TimeToLive)
{
	SocketSubsystem.SetLastSocketError(ESocketErrors::SE_EOPNOTSUPP);
	return false;
}

bool FSocketEOS::JoinMulticastGroup(const FInternetAddr& GroupAddress, const FInternetAddr& InterfaceAddress)
{
	SocketSubsystem.SetLastSocketError(ESocketErrors::SE_EOPNOTSUPP);
	return false;
}

bool FSocketEOS::LeaveMulticastGroup(const FInternetAddr& GroupAddress, const FInternetAddr& InterfaceAddress)
{
	SocketSubsystem.SetLastSocketError(ESocketErrors::SE_EOPNOTSUPP);
	return false;
}

bool FSocketEOS::SetMulticastInterface(const FInternetAddr& InterfaceAddress)
{
	SocketSubsystem.SetLastSocketError(ESocketErrors::SE_EOPNOTSUPP);
	return false;
}

bool FSocketEOS::SetReuseAddr(bool bAllowReuse)
{
	return true;
}

bool FSocketEOS::SetLinger(bool bShouldLinger, int32 Timeout)
{
	return true;
}

bool FSocketEOS::SetRecvErr(bool bUseErrorQueue)
{
	return true;
}

bool FSocketEOS::SetSendBufferSize(int32 Size, int32& NewSize)
{
	return true;
}

bool FSocketEOS::SetReceiveBufferSize(int32 Size, int32& NewSize)
{
	return true;
}

int32 FSocketEOS::GetPortNo()
{
	return LocalAddress.GetChannel();
}

void FSocketEOS::SetLocalAddress(const FInternetAddrEOS& InLocalAddress)
{
	LocalAddress = InLocalAddress;
}

bool FSocketEOS::Close(const FInternetAddrEOS& RemoteAddress)
{
	check(IsInGameThread() && "p2p does not support multithreading");

	if (!RemoteAddress.IsValid())
	{
		UE_LOG(LogSocketSubsystemEOS, Error, TEXT("Unable to close socket with remote address as it is invalid RemoteAddress = (%s)"), *RemoteAddress.ToString(true));
		SocketSubsystem.SetLastSocketError(ESocketErrors::SE_EADDRNOTAVAIL);
		return false;
	}

#if WITH_EOS_SDK
	// So we don't reopen a connection by sending to it
	ClosedRemotes.Add(RemoteAddress);

	EOS_P2P_SocketId SocketId = { };
	SocketId.ApiVersion = EOS_P2P_SOCKETID_API_LATEST;
	FCStringAnsi::Strcpy(SocketId.SocketName, RemoteAddress.GetSocketName());

	EOS_P2P_CloseConnectionOptions Options = { };
	Options.ApiVersion = EOS_P2P_CLOSECONNECTION_API_LATEST;
	Options.LocalUserId = LocalAddress.GetLocalUserId();
	Options.RemoteUserId = RemoteAddress.GetRemoteUserId();
	Options.SocketId = &SocketId;

	EOS_EResult Result = EOS_P2P_CloseConnection(SocketSubsystem.GetP2PHandle(), &Options);
	NP_LOG(TEXT("[%s] - EOS_P2P_CloseConnection() with remote address RemoteAddress (%s) result code (%s)\r\n"), GetLogPrefix(), *RemoteAddress.ToString(true), ANSI_TO_TCHAR(EOS_EResult_ToString(Result)));
	if (Result != EOS_EResult::EOS_Success)
	{
		UE_LOG(LogSocketSubsystemEOS, Error, TEXT("Unable to close socket with remote address RemoteAddress (%s) due to error (%s)"), *RemoteAddress.ToString(true), ANSI_TO_TCHAR(EOS_EResult_ToString(Result)));

		// @todo joeg - map EOS codes to UE4's
		SocketSubsystem.SetLastSocketError(ESocketErrors::SE_EINVAL);
		return false;
	}
	return true;
#else
	return false;
#endif
}

void FSocketEOS::RegisterClosedNotification()
{
#if WITH_EOS_SDK
	if (ClosedNotifyId != EOS_INVALID_NOTIFICATIONID)
	{
		// Already listening for these events so ignore
		return;
	}
	
	EOS_P2P_SocketId SocketId = { };
	SocketId.ApiVersion = EOS_P2P_SOCKETID_API_LATEST;
	FCStringAnsi::Strcpy(SocketId.SocketName, LocalAddress.GetSocketName());

	EOS_P2P_AddNotifyPeerConnectionClosedOptions Options = { };
	Options.ApiVersion = EOS_P2P_ADDNOTIFYPEERCONNECTIONCLOSED_API_LATEST;
	Options.LocalUserId = LocalAddress.GetLocalUserId();
	Options.SocketId = &SocketId;

#if ENGINE_MAJOR_VERSION == 5
	ClosedNotifyCallback = new FClosedNotifyCallback(CallbackAliveTracker);
#else
	ClosedNotifyCallback = new FClosedNotifyCallback();
#endif
	ClosedNotifyCallback->CallbackLambda = [this](const EOS_P2P_OnRemoteConnectionClosedInfo* Info)
	{
		// Add this connection to the list of closed ones
		FInternetAddrEOS RemoteAddress(Info->RemoteUserId, Info->SocketId->SocketName, LocalAddress.GetChannel());
		RemoteAddress.SetLocalUserId(LocalAddress.GetLocalUserId());
		ClosedRemotes.Add(RemoteAddress);
		NP_LOG(TEXT("[%s] - Close connection received for remote address (%s)\r\n"), GetLogPrefix(), *RemoteAddress.ToString(true));
	};
	ClosedNotifyId = EOS_P2P_AddNotifyPeerConnectionClosed(SocketSubsystem.GetP2PHandle(), &Options, ClosedNotifyCallback, ClosedNotifyCallback->GetCallbackPtr());
#endif
}
