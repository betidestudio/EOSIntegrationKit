// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSubsystemEIK/SdkFunctions/EIK_SharedFunctionFile.h"
#include "eos_p2p.h"
#include "Runtime/Launch/Resources/Version.h"
#include "eos_p2p_types.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "EIK_P2PSubsystem.generated.h"

USTRUCT(BlueprintType)
struct FEIK_P2P_OnIncomingPacketQueueFullInfo
{
	GENERATED_BODY()

	//The maximum size in bytes the incoming packet queue is allowed to use
	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit | SDK Functions | P2P Interface")
	int64 PacketQueueMaxSizeBytes;

	//The current size in bytes the incoming packet queue is currently using
	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit | SDK Functions | P2P Interface")
	int64 PacketQueueCurrentSizeBytes;

	//The Product User ID of the local user who is receiving the packet that would overflow the queue
	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit | SDK Functions | P2P Interface")
	FEIK_ProductUserId OverflowPacketLocalUserId;

	//The channel the incoming packet is for
	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit | SDK Functions | P2P Interface")
	int32 OverflowPacketChannel;

	//The size in bytes of the incoming packet (and related metadata) that would overflow the queue
	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit | SDK Functions | P2P Interface")
	int32 OverflowPacketSizeBytes;

	FEIK_P2P_OnIncomingPacketQueueFullInfo(): PacketQueueMaxSizeBytes(0), PacketQueueCurrentSizeBytes(0),
	                                          OverflowPacketChannel(0),
	                                          OverflowPacketSizeBytes(0)
	{
	}

	FEIK_P2P_OnIncomingPacketQueueFullInfo(const EOS_P2P_OnIncomingPacketQueueFullInfo& Data)
	{
		PacketQueueMaxSizeBytes = Data.PacketQueueMaxSizeBytes;
		PacketQueueCurrentSizeBytes = Data.PacketQueueCurrentSizeBytes;
		OverflowPacketLocalUserId = Data.OverflowPacketLocalUserId;
		OverflowPacketChannel = Data.OverflowPacketChannel;
		OverflowPacketSizeBytes = Data.OverflowPacketSizeBytes;
	}
};

DECLARE_DYNAMIC_DELEGATE_OneParam(FEIK_OnIncomingPacketQueueFull, const FEIK_P2P_OnIncomingPacketQueueFullInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_FourParams(FEIK_OnPeerConnectionClosed, const FEIK_ProductUserId&, LocalUserId, const FEIK_ProductUserId&, RemoteUserId, const FEIK_P2P_SocketId&, SocketId, const TEnumAsByte<EEIK_EConnectionClosedReason>&, Reason);
DECLARE_DYNAMIC_DELEGATE_FiveParams(FEIK_OnPeerConnectionEstablished, const FEIK_ProductUserId&, LocalUserId, const FEIK_ProductUserId&, RemoteUserId, const FEIK_P2P_SocketId&, SocketId, const TEnumAsByte<EEIK_EConnectionEstablishedType>&, ConnectionType, const TEnumAsByte<EEIK_ENetworkConnectionType>&, NetworkType);
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FEIK_OnPeerConnectionInterrupted, const FEIK_ProductUserId&, LocalUserId, const FEIK_ProductUserId&, RemoteUserId, const FEIK_P2P_SocketId&, SocketId);
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FEIK_OnIncomingConnectionRequest, const FEIK_ProductUserId&, LocalUserId, const FEIK_ProductUserId&, RemoteUserId, const FEIK_P2P_SocketId&, SocketId);

UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_P2PSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	
	//Accept or Request a connection with a specific peer on a specific Socket ID. If this connection was not already locally accepted, we will securely message the peer, and trigger a PeerConnectionRequest notification notifying them of the connection request. If the PeerConnectionRequest notification is not bound for all Socket IDs or for the requested Socket ID in particular, the request will be silently ignored. If the remote peer accepts the connection, a notification will be broadcast to the EOS_P2P_AddNotifyPeerConnectionEstablished when the connection is ready to send packets. If multiple Socket IDs are accepted with one peer, they will share one physical socket. Even if a connection is already locally accepted, EOS_EResult::EOS_Success will still be returned if the input was valid.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | P2P Interface", DisplayName="EOS_P2P_AcceptConnection")
	static const TEnumAsByte<EEIK_Result> EIK_P2P_AcceptConnection(FEIK_ProductUserId LocalUserId, FEIK_ProductUserId RemoteUserId, FEIK_P2P_SocketId SocketId);

	FEIK_OnIncomingPacketQueueFull OnIncomingPacketQueueFull;
	//Listen for when our packet queue has become full. This event gives an opportunity to read packets to make room for new incoming packets. If this event fires and no packets are read by calling EOS_P2P_ReceivePacket or the packet queue size is not increased by EOS_P2P_SetPacketQueueSize, any packets that are received after this event are discarded until there is room again in the queue.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | P2P Interface", DisplayName="EOS_P2P_AddNotifyIncomingPacketQueueFull")
	const FEIK_NotificationId EIK_P2P_AddNotifyIncomingPacketQueueFull(FEIK_ProductUserId LocalUserId, const FEIK_P2P_SocketId SocketId, const FEIK_OnIncomingPacketQueueFull& Callback);

	FEIK_OnPeerConnectionClosed OnPeerConnectionClosed;
	//Listen for when a previously accepted connection that was either open or pending is closed.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | P2P Interface", DisplayName="EOS_P2P_AddNotifyPeerConnectionClosed")
	const FEIK_NotificationId EIK_P2P_AddNotifyPeerConnectionClosed(FEIK_ProductUserId LocalUserId, const FEIK_P2P_SocketId SocketId, const FEIK_OnPeerConnectionClosed& Callback);

	FEIK_OnPeerConnectionEstablished OnPeerConnectionEstablished;
	//Listen for when a connection is established. This is fired when we first connect to a peer, when we reconnect to a peer after a connection interruption, and when our underlying network connection type changes (for example, from a direct connection to relay, or vice versa). Network Connection Type changes will always be broadcast with a EOS_CET_Reconnection connection type, even if the connection was not interrupted.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | P2P Interface", DisplayName="EOS_P2P_AddNotifyPeerConnectionEstablished")
	const FEIK_NotificationId EIK_P2P_AddNotifyPeerConnectionEstablished(FEIK_ProductUserId LocalUserId, const FEIK_P2P_SocketId SocketId, const FEIK_OnPeerConnectionEstablished& Callback);

	FEIK_OnPeerConnectionInterrupted OnPeerConnectionInterrupted;
	//Listen for when a previously opened connection is interrupted. The connection will automatically attempt to reestablish, but it may not be successful. If a connection reconnects, it will trigger the P2P PeerConnectionEstablished notification with the EOS_CET_Reconnection connection type. If a connection fails to reconnect, it will trigger the P2P PeerConnectionClosed notification.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | P2P Interface", DisplayName="EOS_P2P_AddNotifyPeerConnectionInterrupted")
	const FEIK_NotificationId EIK_P2P_AddNotifyPeerConnectionInterrupted(FEIK_ProductUserId LocalUserId, const FEIK_P2P_SocketId SocketId, const FEIK_OnPeerConnectionInterrupted& Callback);

	FEIK_OnIncomingConnectionRequest OnIncomingConnectionRequest;
	//Listen for incoming connection requests on a particular Socket ID, or optionally all Socket IDs. The bound function will only be called if the connection has not already been accepted.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | P2P Interface", DisplayName="EOS_P2P_AddNotifyPeerConnectionRequest")
	const FEIK_NotificationId EIK_P2P_AddNotifyPeerConnectionRequest(FEIK_ProductUserId LocalUserId, const FEIK_P2P_SocketId SocketId, const FEIK_OnIncomingConnectionRequest& Callback);

	//Clear queued incoming and outgoing packets.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | P2P Interface", DisplayName="EOS_P2P_ClearPacketQueue")
	static const TEnumAsByte<EEIK_Result> EIK_P2P_ClearPacketQueue(FEIK_ProductUserId LocalUserId, FEIK_ProductUserId RemoteUserId, FEIK_P2P_SocketId SocketId);

	//For all (or optionally one specific) Socket ID(s) with a specific peer: stop receiving packets, drop any locally queued packets, and if no other Socket ID is using the connection with the peer, close the underlying connection. If your application wants to migrate an existing connection with a peer it already connected to, it is recommended to call EOS_P2P_AcceptConnection with the new Socket ID first before calling EOS_P2P_CloseConnection, to prevent the shared physical socket from being torn down prematurely.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | P2P Interface", DisplayName="EOS_P2P_CloseConnection")
	static const TEnumAsByte<EEIK_Result> EIK_P2P_CloseConnection(FEIK_ProductUserId LocalUserId, FEIK_ProductUserId RemoteUserId, FEIK_P2P_SocketId SocketId);
	
	//Close any open Connections for a specific Peer Connection ID.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | P2P Interface", DisplayName="EOS_P2P_CloseConnections")
	static const TEnumAsByte<EEIK_Result> EIK_P2P_CloseConnections(FEIK_ProductUserId LocalUserId, FEIK_P2P_SocketId SocketId);
	
	//Get our last-queried NAT-type, if it has been successfully queried.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | P2P Interface", DisplayName="EOS_P2P_GetNATType")
	static const TEnumAsByte<EEIK_Result> EIK_P2P_GetNATType(TEnumAsByte<EEIK_ENATType>& OutNATType);

	//Gets the size of the packet that will be returned by ReceivePacket for a particular user, if there is any available packets to be retrieved.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | P2P Interface", DisplayName="EOS_P2P_GetNextReceivedPacketSize")
	static const TEnumAsByte<EEIK_Result> EIK_P2P_GetNextReceivedPacketSize(FEIK_ProductUserId LocalUserId, int32 Channel, int32& OutPacketSizeBytes);

	//Gets the current cached information related to the incoming and outgoing packet queues.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | P2P Interface", DisplayName="EOS_P2P_GetPacketQueueInfo")
	static const TEnumAsByte<EEIK_Result> EIK_P2P_GetPacketQueueInfo(FEIK_P2P_PacketQueueInfo& OutPacketQueueInfo);

	//Get the current chosen port and the amount of other ports to try above the chosen port if the chosen port is unavailable.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | P2P Interface", DisplayName="EOS_P2P_GetPortRange")
	static const TEnumAsByte<EEIK_Result> EIK_P2P_GetPortRange(int32& OutPort, int32& OutNumAdditionalPortsToTry);

	//Get the current relay control setting.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | P2P Interface", DisplayName="EOS_P2P_GetRelayControl")
	static const TEnumAsByte<EEIK_Result> EIK_P2P_GetRelayControl(TEnumAsByte<EEIK_ERelayControl>& OutControl);

	//Receive the next packet for the local user, and information associated with this packet, if it exists.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | P2P Interface", DisplayName="EOS_P2P_ReceivePacket")
	static const TEnumAsByte<EEIK_Result> EIK_P2P_ReceivePacket(FEIK_ProductUserId LocalUserId, int32 MaxDataSizeBytes, int32 RequestedChannel, FEIK_P2P_SocketId& OutSocketId, FEIK_ProductUserId& OutPeerId, int32& OutChannel, TArray<uint8>& OutData, int32& OutBytesRead);

	//Stop listening for full incoming packet queue events on a previously bound handler.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | P2P Interface", DisplayName="EOS_P2P_RemoveNotifyIncomingPacketQueueFull")
	static void EIK_P2P_RemoveNotifyIncomingPacketQueueFull(const FEIK_NotificationId& NotificationId);

	//Stop notifications for connections being closed on a previously bound handler.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | P2P Interface", DisplayName="EOS_P2P_RemoveNotifyPeerConnectionClosed")
	static void EIK_P2P_RemoveNotifyPeerConnectionClosed(const FEIK_NotificationId& NotificationId);

	//Stop notifications for connections being established on a previously bound handler.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | P2P Interface", DisplayName="EOS_P2P_RemoveNotifyPeerConnectionEstablished")
	static void EIK_P2P_RemoveNotifyPeerConnectionEstablished(const FEIK_NotificationId& NotificationId);

	//Stop notifications for connections being interrupted on a previously bound handler.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | P2P Interface", DisplayName="EOS_P2P_RemoveNotifyPeerConnectionInterrupted")
	static void EIK_P2P_RemoveNotifyPeerConnectionInterrupted(const FEIK_NotificationId& NotificationId);

	//Stop listening for connection requests on a previously bound handler.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | P2P Interface", DisplayName="EOS_P2P_RemoveNotifyPeerConnectionRequest")
	static void EIK_P2P_RemoveNotifyPeerConnectionRequest(const FEIK_NotificationId& NotificationId);

	//Send a packet to a peer at the specified address. If there is already an open connection to this peer, it will be sent immediately. If there is no open connection, an attempt to connect to the peer will be made. An EOS_Success result only means the data was accepted to be sent, not that it has been successfully delivered to the peer.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | P2P Interface", DisplayName="EOS_P2P_SendPacket")
	static const TEnumAsByte<EEIK_Result> EIK_P2P_SendPacket(FEIK_ProductUserId LocalUserId, FEIK_ProductUserId RemoteUserId, FEIK_P2P_SocketId SocketId, int32 Channel, const TArray<uint8>& Data, bool bAllowDelayedDelivery, bool bDisableAutoAcceptConnection, const TEnumAsByte<EEIK_EPacketReliability>& Reliability);

	//Sets the maximum packet queue sizes that packets waiting to be sent or received can use. If the packet queue size is made smaller than the current queue size while there are packets in the queue that would push this packet size over, existing packets are kept but new packets may not be added to the full queue until enough packets are sent or received.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | P2P Interface", DisplayName="EOS_P2P_SetPacketQueueSize")
	static const TEnumAsByte<EEIK_Result> EIK_P2P_SetPacketQueueSize(int64 IncomingPacketQueueMaxSizeBytes, int64 OutgoingPacketQueueMaxSizeBytes);

	//Set configuration options related to network ports.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | P2P Interface", DisplayName="EOS_P2P_SetPortRange")
	static const TEnumAsByte<EEIK_Result> EIK_P2P_SetPortRange(int32 Port, int32 MaxAdditionalPortsToTry);

	//Set how relay servers are to be used. This setting does not immediately apply to existing connections, but may apply to existing connections if the connection requires renegotiation.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | P2P Interface", DisplayName="EOS_P2P_SetRelayControl")
	static const TEnumAsByte<EEIK_Result> EIK_P2P_SetRelayControl(TEnumAsByte<EEIK_ERelayControl> Control);
};
