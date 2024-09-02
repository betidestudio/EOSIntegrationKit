// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_P2PSubsystem.h"

#include "OnlineSubsystemEOS.h"
#include "OnlineSubsystemEIK/SdkFunctions/ConnectInterface/EIK_ConnectSubsystem.h"

const TEnumAsByte<EEIK_Result> UEIK_P2PSubsystem::EIK_P2P_AcceptConnection(FEIK_ProductUserId LocalUserId, FEIK_ProductUserId RemoteUserId, FEIK_P2P_SocketId SocketId)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			if(!EOSRef->SocketSubsystem)
			{
				UE_LOG(LogEIK, Error, TEXT("UEIK_P2PSubsystem::EIK_P2P_AcceptConnection: Failed to get EOS Socket subsystem"));
				return EEIK_Result::EOS_NotFound;
			}
			EOS_P2P_AcceptConnectionOptions Options = {};
			Options.ApiVersion = EOS_P2P_ACCEPTCONNECTION_API_LATEST;
			Options.LocalUserId = LocalUserId.GetValueAsEosType();
			Options.RemoteUserId = RemoteUserId.GetValueAsEosType();
			EOS_P2P_SocketId SocketIdData = SocketId.GetAsEosData();
			Options.SocketId = &SocketIdData;
			auto Result = EOS_P2P_AcceptConnection(EOSRef->SocketSubsystem->GetP2PHandle(), &Options);
			return static_cast<EEIK_Result>(Result);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("UEIK_P2PSubsystem::EIK_P2P_AcceptConnection: Failed to get EOS subsystem"));
	return EEIK_Result::EOS_NotFound;
}

const FEIK_NotificationId UEIK_P2PSubsystem::EIK_P2P_AddNotifyIncomingPacketQueueFull(FEIK_ProductUserId LocalUserId, const FEIK_P2P_SocketId SocketId, const FEIK_OnIncomingPacketQueueFull& Callback)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			if(!EOSRef->SocketSubsystem)
			{
				UE_LOG(LogEIK, Error, TEXT("UEIK_P2PSubsystem::EIK_P2P_AddNotifyIncomingPacketQueueFull: Failed to get EOS Socket subsystem"));
				return FEIK_NotificationId();
			}
			EOS_P2P_AddNotifyIncomingPacketQueueFullOptions Options = {};
			Options.ApiVersion = EOS_P2P_ADDNOTIFYINCOMINGPACKETQUEUEFULL_API_LATEST;
			EOS_P2P_AddNotifyIncomingPacketQueueFull(EOSRef->SocketSubsystem->GetP2PHandle(), &Options, this, [](const EOS_P2P_OnIncomingPacketQueueFullInfo* Data)
			{
				if(UEIK_P2PSubsystem* Subsystem = static_cast<UEIK_P2PSubsystem*>(Data->ClientData))
				{
						Subsystem->OnIncomingPacketQueueFull.ExecuteIfBound(*Data);
				}
			});
		}
	}
	UE_LOG(LogEIK, Error, TEXT("UEIK_P2PSubsystem::EIK_P2P_AddNotifyIncomingPacketQueueFull: Failed to get EOS subsystem"));
	return FEIK_NotificationId();
}

const FEIK_NotificationId UEIK_P2PSubsystem::EIK_P2P_AddNotifyPeerConnectionClosed(
	FEIK_ProductUserId LocalUserId, const FEIK_P2P_SocketId SocketId,
	const FEIK_OnPeerConnectionClosed& Callback)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			if(!EOSRef->SocketSubsystem)
			{
				UE_LOG(LogEIK, Error, TEXT("UEIK_P2PSubsystem::EIK_P2P_AddNotifyPeerConnectionClosed: Failed to get EOS Socket subsystem"));
				return FEIK_NotificationId();
			}
			EOS_P2P_AddNotifyPeerConnectionClosedOptions Options = {};
			Options.ApiVersion = EOS_P2P_ADDNOTIFYPEERCONNECTIONCLOSED_API_LATEST;
			EOS_P2P_AddNotifyPeerConnectionClosed(EOSRef->SocketSubsystem->GetP2PHandle(), &Options, this, [](const EOS_P2P_OnRemoteConnectionClosedInfo* Data)
			{
				if(UEIK_P2PSubsystem* Subsystem = static_cast<UEIK_P2PSubsystem*>(Data->ClientData))
				{
					FEIK_ProductUserId LocalUserId = Data->LocalUserId;
					FEIK_ProductUserId RemoteUserId = Data->RemoteUserId;
					FEIK_P2P_SocketId SocketId = *Data->SocketId;
					TEnumAsByte<EEIK_EConnectionClosedReason> Reason = static_cast<EEIK_EConnectionClosedReason>(Data->Reason);
					Subsystem->OnPeerConnectionClosed.ExecuteIfBound(LocalUserId, RemoteUserId, SocketId, Reason);
				}
			});
		}
	}
	UE_LOG(LogEIK, Error, TEXT("UEIK_P2PSubsystem::EIK_P2P_AddNotifyPeerConnectionClosed: Failed to get EOS subsystem"));
	return FEIK_NotificationId();
}

const FEIK_NotificationId UEIK_P2PSubsystem::EIK_P2P_AddNotifyPeerConnectionEstablished(
	FEIK_ProductUserId LocalUserId, const FEIK_P2P_SocketId SocketId,
	const FEIK_OnPeerConnectionEstablished& Callback)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			if(!EOSRef->SocketSubsystem)
			{
				UE_LOG(LogEIK, Error, TEXT("UEIK_P2PSubsystem::EIK_P2P_AddNotifyPeerConnectionEstablished: Failed to get EOS Socket subsystem"));
				return FEIK_NotificationId();
			}
			EOS_P2P_AddNotifyPeerConnectionEstablishedOptions Options = {};
			Options.ApiVersion = EOS_P2P_ADDNOTIFYPEERCONNECTIONESTABLISHED_API_LATEST;
			EOS_P2P_AddNotifyPeerConnectionEstablished(EOSRef->SocketSubsystem->GetP2PHandle(), &Options, this, [](const EOS_P2P_OnPeerConnectionEstablishedInfo* Data)
			{
				if(UEIK_P2PSubsystem* Subsystem = static_cast<UEIK_P2PSubsystem*>(Data->ClientData))
				{
					AsyncTask(ENamedThreads::GameThread, [Subsystem, Data]()
					{
						Subsystem->OnPeerConnectionEstablished.ExecuteIfBound(Data->LocalUserId, Data->RemoteUserId, *Data->SocketId, static_cast<EEIK_EConnectionEstablishedType>(Data->ConnectionType), static_cast<EEIK_ENetworkConnectionType>(Data->NetworkType));
					});
				}
			});
		}
	}
	UE_LOG(LogEIK, Error, TEXT("UEIK_P2PSubsystem::EIK_P2P_AddNotifyPeerConnectionEstablished: Failed to get EOS subsystem"));
	return FEIK_NotificationId();
}

const FEIK_NotificationId UEIK_P2PSubsystem::EIK_P2P_AddNotifyPeerConnectionInterrupted(
	FEIK_ProductUserId LocalUserId, const FEIK_P2P_SocketId SocketId,
	const FEIK_OnPeerConnectionInterrupted& Callback)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			if(!EOSRef->SocketSubsystem)
			{
				UE_LOG(LogEIK, Error, TEXT("UEIK_P2PSubsystem::EIK_P2P_AddNotifyPeerConnectionInterrupted: Failed to get EOS Socket subsystem"));
				return FEIK_NotificationId();
			}
			EOS_P2P_AddNotifyPeerConnectionInterruptedOptions Options = {};
			Options.ApiVersion = EOS_P2P_ADDNOTIFYPEERCONNECTIONINTERRUPTED_API_LATEST;
			EOS_P2P_AddNotifyPeerConnectionInterrupted(EOSRef->SocketSubsystem->GetP2PHandle(), &Options, this, [](const EOS_P2P_OnPeerConnectionInterruptedInfo* Data)
			{
				if(UEIK_P2PSubsystem* Subsystem = static_cast<UEIK_P2PSubsystem*>(Data->ClientData))
				{
					AsyncTask(ENamedThreads::GameThread, [Subsystem, Data]()
					{
						Subsystem->OnPeerConnectionInterrupted.ExecuteIfBound(Data->LocalUserId, Data->RemoteUserId, *Data->SocketId);
					});
				}
			});
		}
	}
	UE_LOG(LogEIK, Error, TEXT("UEIK_P2PSubsystem::EIK_P2P_AddNotifyPeerConnectionInterrupted: Failed to get EOS subsystem"));
	return FEIK_NotificationId();
}

const FEIK_NotificationId UEIK_P2PSubsystem::EIK_P2P_AddNotifyPeerConnectionRequest(
	FEIK_ProductUserId LocalUserId, const FEIK_P2P_SocketId SocketId,
	const FEIK_OnIncomingConnectionRequest& Callback)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			if(!EOSRef->SocketSubsystem)
			{
				UE_LOG(LogEIK, Error, TEXT("UEIK_P2PSubsystem::EIK_P2P_AddNotifyPeerConnectionRequest: Failed to get EOS Socket subsystem"));
				return FEIK_NotificationId();
			}
			EOS_P2P_AddNotifyPeerConnectionRequestOptions Options = {};
			Options.ApiVersion = EOS_P2P_ADDNOTIFYPEERCONNECTIONREQUEST_API_LATEST;
			EOS_P2P_AddNotifyPeerConnectionRequest(EOSRef->SocketSubsystem->GetP2PHandle(), &Options, this, [](const EOS_P2P_OnIncomingConnectionRequestInfo* Data)
			{
				if(UEIK_P2PSubsystem* Subsystem = static_cast<UEIK_P2PSubsystem*>(Data->ClientData))
				{
					AsyncTask(ENamedThreads::GameThread, [Subsystem, Data]()
					{
						Subsystem->OnIncomingConnectionRequest.ExecuteIfBound(Data->LocalUserId, Data->RemoteUserId, *Data->SocketId);
					});
				}
			});
		}
	}
	UE_LOG(LogEIK, Error, TEXT("UEIK_P2PSubsystem::EIK_P2P_AddNotifyPeerConnectionRequest: Failed to get EOS subsystem"));
	return FEIK_NotificationId();
}

const TEnumAsByte<EEIK_Result> UEIK_P2PSubsystem::EIK_P2P_ClearPacketQueue(FEIK_ProductUserId LocalUserId,
	FEIK_ProductUserId RemoteUserId, FEIK_P2P_SocketId SocketId)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			if(!EOSRef->SocketSubsystem)
			{
				UE_LOG(LogEIK, Error, TEXT("UEIK_P2PSubsystem::EIK_P2P_ClearPacketQueue: Failed to get EOS Socket subsystem"));
				return EEIK_Result::EOS_NotFound;
			}
			EOS_P2P_ClearPacketQueueOptions Options = {};
			Options.ApiVersion = EOS_P2P_CLEARPACKETQUEUE_API_LATEST;
			Options.LocalUserId = LocalUserId.GetValueAsEosType();
			Options.RemoteUserId = RemoteUserId.GetValueAsEosType();
			EOS_P2P_SocketId SocketIdData = SocketId.GetAsEosData();
			Options.SocketId = &SocketIdData;
			auto Result = EOS_P2P_ClearPacketQueue(EOSRef->SocketSubsystem->GetP2PHandle(), &Options);
			return static_cast<EEIK_Result>(Result);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("UEIK_P2PSubsystem::EIK_P2P_ClearPacketQueue: Failed to get EOS subsystem"));
	return EEIK_Result::EOS_NotFound;
}

const TEnumAsByte<EEIK_Result> UEIK_P2PSubsystem::EIK_P2P_CloseConnection(FEIK_ProductUserId LocalUserId,
	FEIK_ProductUserId RemoteUserId, FEIK_P2P_SocketId SocketId)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			if(!EOSRef->SocketSubsystem)
			{
				UE_LOG(LogEIK, Error, TEXT("UEIK_P2PSubsystem::EIK_P2P_CloseConnection: Failed to get EOS Socket subsystem"));
				return EEIK_Result::EOS_NotFound;
			}
			EOS_P2P_CloseConnectionOptions Options = {};
			Options.ApiVersion = EOS_P2P_CLOSECONNECTION_API_LATEST;
			Options.LocalUserId = LocalUserId.GetValueAsEosType();
			Options.RemoteUserId = RemoteUserId.GetValueAsEosType();
			EOS_P2P_SocketId SocketIdData = SocketId.GetAsEosData();
			Options.SocketId = &SocketIdData;
			auto Result = EOS_P2P_CloseConnection(EOSRef->SocketSubsystem->GetP2PHandle(), &Options);
			return static_cast<EEIK_Result>(Result);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("UEIK_P2PSubsystem::EIK_P2P_CloseConnection: Failed to get EOS subsystem"));
	return EEIK_Result::EOS_NotFound;
}

const TEnumAsByte<EEIK_Result> UEIK_P2PSubsystem::EIK_P2P_CloseConnections(FEIK_ProductUserId LocalUserId,
	FEIK_P2P_SocketId SocketId)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			if(!EOSRef->SocketSubsystem)
			{
				UE_LOG(LogEIK, Error, TEXT("UEIK_P2PSubsystem::EIK_P2P_CloseConnections: Failed to get EOS Socket subsystem"));
				return EEIK_Result::EOS_NotFound;
			}
			EOS_P2P_CloseConnectionsOptions Options = {};
			Options.ApiVersion = EOS_P2P_CLOSECONNECTIONS_API_LATEST;
			Options.LocalUserId = LocalUserId.GetValueAsEosType();
			EOS_P2P_SocketId SocketIdData = SocketId.GetAsEosData();
			Options.SocketId = &SocketIdData;
			auto Result = EOS_P2P_CloseConnections(EOSRef->SocketSubsystem->GetP2PHandle(), &Options);
			return static_cast<EEIK_Result>(Result);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("UEIK_P2PSubsystem::EIK_P2P_CloseConnections: Failed to get EOS subsystem"));
	return EEIK_Result::EOS_NotFound;
}

const TEnumAsByte<EEIK_Result> UEIK_P2PSubsystem::EIK_P2P_GetNATType(TEnumAsByte<EEIK_ENATType>& OutNATType)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			if(!EOSRef->SocketSubsystem)
			{
				UE_LOG(LogEIK, Error, TEXT("UEIK_P2PSubsystem::EIK_P2P_GetNATType: Failed to get EOS Socket subsystem"));
				return EEIK_Result::EOS_NotFound;
			}
			EOS_P2P_GetNATTypeOptions Options = {};
			Options.ApiVersion = EOS_P2P_GETNATTYPE_API_LATEST;
			EOS_ENATType EosNATType;
			auto Result = EOS_P2P_GetNATType(EOSRef->SocketSubsystem->GetP2PHandle(), &Options, &EosNATType);
			OutNATType = static_cast<EEIK_ENATType>(EosNATType);
			return static_cast<EEIK_Result>(Result);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("UEIK_P2PSubsystem::EIK_P2P_GetNATType: Failed to get EOS subsystem"));
	return EEIK_Result::EOS_NotFound;
}

const TEnumAsByte<EEIK_Result> UEIK_P2PSubsystem::EIK_P2P_GetNextReceivedPacketSize(
	FEIK_ProductUserId LocalUserId, int32 Channel, int32& OutPacketSizeBytes)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			if(!EOSRef->SocketSubsystem)
			{
				UE_LOG(LogEIK, Error, TEXT("UEIK_P2PSubsystem::EIK_P2P_GetNextReceivedPacketSize: Failed to get EOS Socket subsystem"));
				return EEIK_Result::EOS_NotFound;
			}
			EOS_P2P_GetNextReceivedPacketSizeOptions Options = {};
			Options.ApiVersion = EOS_P2P_GETNEXTRECEIVEDPACKETSIZE_API_LATEST;
			Options.LocalUserId = LocalUserId.GetValueAsEosType();
			if(Channel != -1)
			{
				uint8_t LocalChannel = static_cast<uint8_t>(Channel);
				Options.RequestedChannel = &LocalChannel;
			}
			else
			{
				Options.RequestedChannel = nullptr;
			}
			uint32 PacketSizeBytes;
			auto Result = EOS_P2P_GetNextReceivedPacketSize(EOSRef->SocketSubsystem->GetP2PHandle(), &Options, &PacketSizeBytes);
			OutPacketSizeBytes = PacketSizeBytes;
			return static_cast<EEIK_Result>(Result);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("UEIK_P2PSubsystem::EIK_P2P_GetNextReceivedPacketSize: Failed to get EOS subsystem"));
	return EEIK_Result::EOS_NotFound;
}

const TEnumAsByte<EEIK_Result> UEIK_P2PSubsystem::EIK_P2P_GetPacketQueueInfo(
	FEIK_P2P_PacketQueueInfo& OutPacketQueueInfo)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			if(!EOSRef->SocketSubsystem)
			{
				UE_LOG(LogEIK, Error, TEXT("UEIK_P2PSubsystem::EIK_P2P_GetPacketQueueInfo: Failed to get EOS Socket subsystem"));
				return EEIK_Result::EOS_NotFound;
			}
			EOS_P2P_GetPacketQueueInfoOptions Options = {};
			Options.ApiVersion = EOS_P2P_GETPACKETQUEUEINFO_API_LATEST;
			EOS_P2P_PacketQueueInfo EosPacketQueueInfo;
			auto Result = EOS_P2P_GetPacketQueueInfo(EOSRef->SocketSubsystem->GetP2PHandle(), &Options, &EosPacketQueueInfo);
			OutPacketQueueInfo = EosPacketQueueInfo;
			return static_cast<EEIK_Result>(Result);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("UEIK_P2PSubsystem::EIK_P2P_GetPacketQueueInfo: Failed to get EOS subsystem"));
	return EEIK_Result::EOS_NotFound;
}

const TEnumAsByte<EEIK_Result> UEIK_P2PSubsystem::EIK_P2P_GetPortRange(int32& OutPort,
	int32& OutNumAdditionalPortsToTry)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			if(!EOSRef->SocketSubsystem)
			{
				UE_LOG(LogEIK, Error, TEXT("UEIK_P2PSubsystem::EIK_P2P_GetPortRange: Failed to get EOS Socket subsystem"));
				return EEIK_Result::EOS_NotFound;
			}
			EOS_P2P_GetPortRangeOptions Options = {};
			Options.ApiVersion = EOS_P2P_GETPORTRANGE_API_LATEST;
			uint16_t TempOutPort;
			uint16_t TempOutNumAdditionalPortsToTry;
			auto Result = EOS_P2P_GetPortRange(EOSRef->SocketSubsystem->GetP2PHandle(), &Options, &TempOutPort, &TempOutNumAdditionalPortsToTry);
			OutPort = TempOutPort;
			OutNumAdditionalPortsToTry = TempOutNumAdditionalPortsToTry;
			return static_cast<EEIK_Result>(Result);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("UEIK_P2PSubsystem::EIK_P2P_GetPortRange: Failed to get EOS subsystem"));
	return EEIK_Result::EOS_NotFound;
}

const TEnumAsByte<EEIK_Result> UEIK_P2PSubsystem::EIK_P2P_GetRelayControl(TEnumAsByte<EEIK_ERelayControl>& OutControl)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			if (!EOSRef->SocketSubsystem)
			{
				UE_LOG(LogEIK, Error, TEXT("UEIK_P2PSubsystem::EIK_P2P_GetRelayControl: Failed to get EOS Socket subsystem"));
				return EEIK_Result::EOS_NotFound;
			}
			EOS_P2P_GetRelayControlOptions Options = {};
			Options.ApiVersion = EOS_P2P_GETRELAYCONTROL_API_LATEST;
			EOS_ERelayControl EosControl;
			auto Result = EOS_P2P_GetRelayControl(EOSRef->SocketSubsystem->GetP2PHandle(), &Options, &EosControl);
			OutControl = static_cast<EEIK_ERelayControl>(EosControl);
			return static_cast<EEIK_Result>(Result);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("UEIK_P2PSubsystem::EIK_P2P_GetRelayControl: Failed to get EOS subsystem"));
	return EEIK_Result::EOS_NotFound;
}

const TEnumAsByte<EEIK_Result> UEIK_P2PSubsystem::EIK_P2P_ReceivePacket(FEIK_ProductUserId LocalUserId,
	int32 MaxDataSizeBytes, int32 RequestedChannel, FEIK_P2P_SocketId& OutSocketId, FEIK_ProductUserId& OutPeerId,
	int32& OutChannel, TArray<uint8>& OutData, int32& OutBytesRead)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			OutData.SetNum(MaxDataSizeBytes);
			if(!EOSRef->SocketSubsystem)
			{
				UE_LOG(LogEIK, Error, TEXT("UEIK_P2PSubsystem::EIK_P2P_ReceivePacket: Failed to get EOS Socket subsystem"));
				return EEIK_Result::EOS_NotFound;
			}
			EOS_P2P_ReceivePacketOptions Options = {};
			Options.ApiVersion = EOS_P2P_RECEIVEPACKET_API_LATEST;
			Options.LocalUserId = LocalUserId.GetValueAsEosType();
			Options.MaxDataSizeBytes = MaxDataSizeBytes;
			if(RequestedChannel != -1)
			{
				uint8_t LocalChannel = static_cast<uint8_t>(RequestedChannel);
				Options.RequestedChannel = &LocalChannel;
			}
			else
			{
				Options.RequestedChannel = nullptr;
			}
			EOS_P2P_SocketId SocketId;
			EOS_ProductUserId PeerId;
			uint8_t Channel;
			uint32 BytesRead;
			auto Result = EOS_P2P_ReceivePacket(EOSRef->SocketSubsystem->GetP2PHandle(), &Options, &PeerId, &SocketId, &Channel, OutData.GetData(), &BytesRead);
			OutSocketId = SocketId;
			OutPeerId = PeerId;
			OutChannel = Channel;
			OutData.SetNum(BytesRead);
			OutBytesRead = BytesRead;
			return static_cast<EEIK_Result>(Result);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("UEIK_P2PSubsystem::EIK_P2P_ReceivePacket: Failed to get EOS subsystem"));
	return EEIK_Result::EOS_NotFound;
}

void UEIK_P2PSubsystem::EIK_P2P_RemoveNotifyIncomingPacketQueueFull(const FEIK_NotificationId& NotificationId)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_P2P_RemoveNotifyIncomingPacketQueueFull(EOSRef->SocketSubsystem->GetP2PHandle(), NotificationId.NotificationId);
		}
	}
}

void UEIK_P2PSubsystem::EIK_P2P_RemoveNotifyPeerConnectionClosed(const FEIK_NotificationId& NotificationId)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_P2P_RemoveNotifyPeerConnectionClosed(EOSRef->SocketSubsystem->GetP2PHandle(), NotificationId.NotificationId);
		}
	}
}

void UEIK_P2PSubsystem::EIK_P2P_RemoveNotifyPeerConnectionEstablished(const FEIK_NotificationId& NotificationId)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_P2P_RemoveNotifyPeerConnectionEstablished(EOSRef->SocketSubsystem->GetP2PHandle(), NotificationId.NotificationId);
		}
	}
}

void UEIK_P2PSubsystem::EIK_P2P_RemoveNotifyPeerConnectionInterrupted(const FEIK_NotificationId& NotificationId)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_P2P_RemoveNotifyPeerConnectionInterrupted(EOSRef->SocketSubsystem->GetP2PHandle(), NotificationId.NotificationId);
		}
	}
}

void UEIK_P2PSubsystem::EIK_P2P_RemoveNotifyPeerConnectionRequest(const FEIK_NotificationId& NotificationId)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_P2P_RemoveNotifyPeerConnectionRequest(EOSRef->SocketSubsystem->GetP2PHandle(), NotificationId.NotificationId);
		}
	}
}

const TEnumAsByte<EEIK_Result> UEIK_P2PSubsystem::EIK_P2P_SendPacket(FEIK_ProductUserId LocalUserId,
	FEIK_ProductUserId RemoteUserId, FEIK_P2P_SocketId SocketId, int32 Channel, const TArray<uint8>& Data,
	bool bAllowDelayedDelivery, bool bDisableAutoAcceptConnection,
	const TEnumAsByte<EEIK_EPacketReliability>& Reliability)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			if(!EOSRef->SocketSubsystem)
			{
				UE_LOG(LogEIK, Error, TEXT("UEIK_P2PSubsystem::EIK_P2P_SendPacket: Failed to get EOS Socket subsystem"));
				return EEIK_Result::EOS_NotFound;
			}
			EOS_P2P_SendPacketOptions Options = {};
			Options.ApiVersion = EOS_P2P_SENDPACKET_API_LATEST;
			Options.LocalUserId = LocalUserId.GetValueAsEosType();
			Options.RemoteUserId = RemoteUserId.GetValueAsEosType();
			EOS_P2P_SocketId SocketIdData = SocketId.GetAsEosData();
			Options.SocketId = &SocketIdData;
			Options.Channel = static_cast<uint8_t>(Channel);
			Options.DataLengthBytes = Data.Num();
			Options.Data = Data.GetData();
			Options.bAllowDelayedDelivery = bAllowDelayedDelivery;
			Options.bDisableAutoAcceptConnection = bDisableAutoAcceptConnection;
			Options.Reliability = static_cast<EOS_EPacketReliability>(Reliability.GetValue());
			auto Result = EOS_P2P_SendPacket(EOSRef->SocketSubsystem->GetP2PHandle(), &Options);
			return static_cast<EEIK_Result>(Result);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("UEIK_P2PSubsystem::EIK_P2P_SendPacket: Failed to get EOS subsystem"));
	return EEIK_Result::EOS_NotFound;
}

const TEnumAsByte<EEIK_Result> UEIK_P2PSubsystem::EIK_P2P_SetPacketQueueSize(int64 IncomingPacketQueueMaxSizeBytes,
	int64 OutgoingPacketQueueMaxSizeBytes)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_P2P_SetPacketQueueSizeOptions Options = {};
			Options.ApiVersion = EOS_P2P_SETPACKETQUEUESIZE_API_LATEST;
			Options.IncomingPacketQueueMaxSizeBytes = IncomingPacketQueueMaxSizeBytes;
			Options.OutgoingPacketQueueMaxSizeBytes = OutgoingPacketQueueMaxSizeBytes;
			auto Result = EOS_P2P_SetPacketQueueSize(EOSRef->SocketSubsystem->GetP2PHandle(), &Options);
			return static_cast<EEIK_Result>(Result);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("UEIK_P2PSubsystem::EIK_P2P_SetPacketQueueSize: Failed to get EOS subsystem"));
	return EEIK_Result::EOS_NotFound;
}

const TEnumAsByte<EEIK_Result> UEIK_P2PSubsystem::EIK_P2P_SetPortRange(int32 Port, int32 MaxAdditionalPortsToTry)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_P2P_SetPortRangeOptions Options = {};
			Options.ApiVersion = EOS_P2P_SETPORTRANGE_API_LATEST;
			Options.Port = static_cast<uint16_t>(Port);
			Options.MaxAdditionalPortsToTry = static_cast<uint16_t>(MaxAdditionalPortsToTry);
			auto Result = EOS_P2P_SetPortRange(EOSRef->SocketSubsystem->GetP2PHandle(), &Options);
			return static_cast<EEIK_Result>(Result);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("UEIK_P2PSubsystem::EIK_P2P_SetPortRange: Failed to get EOS subsystem"));
	return EEIK_Result::EOS_NotFound;
}

const TEnumAsByte<EEIK_Result> UEIK_P2PSubsystem::EIK_P2P_SetRelayControl(TEnumAsByte<EEIK_ERelayControl> Control)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_P2P_SetRelayControlOptions Options = {};
			Options.ApiVersion = EOS_P2P_SETRELAYCONTROL_API_LATEST;
			Options.RelayControl = static_cast<EOS_ERelayControl>(Control.GetValue());
			auto Result = EOS_P2P_SetRelayControl(EOSRef->SocketSubsystem->GetP2PHandle(), &Options);
			return static_cast<EEIK_Result>(Result);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("UEIK_P2PSubsystem::EIK_P2P_SetRelayControl: Failed to get EOS subsystem"));
	return EEIK_Result::EOS_NotFound;
}