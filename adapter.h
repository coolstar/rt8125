#pragma once

#include "bsd.h"
#include "if_rereg.h"
#include "bsdexport.h"

// multicast list size
#define RT_MAX_MCAST_LIST 32

// supported filters
#define RT_SUPPORTED_FILTERS (          \
    NetPacketFilterFlagDirected    | \
    NetPacketFilterFlagMulticast   | \
    NetPacketFilterFlagBroadcast   | \
    NetPacketFilterFlagPromiscuous | \
    NetPacketFilterFlagAllMulticast)

#define RT_MAX_TX_QUEUES (2)
#define RT_MAX_RX_QUEUES (4)
#define RT_MAX_QUEUES RT_MAX_RX_QUEUES

#define RT_GSO_OFFLOAD_MAX_SIZE 64000
#define RT_GSO_OFFLOAD_MIN_SEGMENT_COUNT 2
#define RT_GSO_OFFLOAD_LAYER_4_HEADER_OFFSET_LIMIT 127
#define RT_CHECKSUM_OFFLOAD_LAYER_4_HEADER_OFFSET_LIMIT 1023

typedef enum REG_SPEED_SETTING {
    RtSpeedDuplexModeAutoNegotiation = 0,
    RtSpeedDuplexMode10MHalfDuplex = 1,
    RtSpeedDuplexMode10MFullDuplex = 2,
    RtSpeedDuplexMode100MHalfDuplex = 3,
    RtSpeedDuplexMode100MFullDuplex = 4,
    RtSpeedDuplexMode1GHalfDuplex = 5,
    RtSpeedDuplexMode1GFullDuplex = 6,
    RtSpeedDuplexMode2GFullDuplex = 7,
    RtSpeedDuplexMode5GFullDuplex = 8
} REG_SPEED_SETTING;

typedef enum _FLOW_CTRL {
    NoFlowControl = 0,
    FlowControlTxOnly = 1,
    FlowControlRxOnly = 2,
    FlowControlTxRx = 3
} FLOW_CTRL;

typedef struct _RT_TAG_802_1Q
{
    union
    {
        struct
        {
            USHORT VLanID1 : 4;
            USHORT CFI : 1;
            USHORT Priority : 3;
            USHORT VLanID2 : 8;
        } TagHeader;

        USHORT Value;
    };
} RT_TAG_802_1Q;

typedef struct _RT_ADAPTER
{
    // WDF handles associated with this context
    NETADAPTER NetAdapter;
    WDFDEVICE WdfDevice;

    //Handle to default Tx and Rx Queues
    NETPACKETQUEUE TxQueues[RT_MAX_TX_QUEUES];
    NETPACKETQUEUE RxQueues[RT_MAX_RX_QUEUES];

    // spin locks
    WDFSPINLOCK Lock;

    WDFDMAENABLER DmaEnabler;

    // MMIO
    PVOID MMIOAddress;
    SIZE_T MMIOSize;
    BUS_INTERFACE_STANDARD PciConfig;

    // Pointer to interrupt object
    RT_INTERRUPT* Interrupt;

    // Multicast list
    NET_PACKET_FILTER_FLAGS PacketFilterFlags;
    UINT MCAddressCount;
    NET_ADAPTER_LINK_LAYER_ADDRESS MCList[RT_MAX_MCAST_LIST];

    // Configuration
    REG_SPEED_SETTING SpeedDuplex;
    NET_ADAPTER_LINK_LAYER_ADDRESS PermanentAddress;
    NET_ADAPTER_LINK_LAYER_ADDRESS CurrentAddress;
    BOOLEAN OverrideAddress;
    FLOW_CTRL FlowControl;
    UINT16 VlanID;

    BOOLEAN isRTL8125;
    ULONG64 MaxSpeed;

    BOOLEAN TxIpHwChkSum;
    BOOLEAN TxTcpHwChkSum;
    BOOLEAN TxUdpHwChkSum;

    BOOLEAN RxIpHwChkSum;
    BOOLEAN RxTcpHwChkSum;
    BOOLEAN RxUdpHwChkSum;

    BOOLEAN LSOv4;
    BOOLEAN LSOv6;

    BOOLEAN EEEEnable;

    struct re_softc bsdData;
} RT_ADAPTER, * PRT_ADAPTER;

WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(RT_ADAPTER, RtGetAdapterContext);

EVT_NET_ADAPTER_CREATE_TXQUEUE   EvtAdapterCreateTxQueue;
EVT_NET_ADAPTER_CREATE_RXQUEUE   EvtAdapterCreateRxQueue;

NTSTATUS
RtInitializeAdapterContext(
    _In_ RT_ADAPTER* adapter,
    _In_ WDFDEVICE device,
    _In_ NETADAPTER netAdapter);

NTSTATUS
RtAdapterStart(
    _In_ RT_ADAPTER* adapter);

UINT8 ConfigRead8(_In_ RT_ADAPTER* adapter, UINT32 reg);
UINT16 ConfigRead16(_In_ RT_ADAPTER* adapter, UINT32 reg);
void ConfigWrite8(_In_ RT_ADAPTER* adapter, UINT32 reg, UINT8 val);
void ConfigWrite16(_In_ RT_ADAPTER* adapter, UINT32 reg, UINT16 val);

void RtResetQueues(_In_ RT_ADAPTER* adapter);

#ifdef __cplusplus
extern "C" {
#endif

void
GetMulticastBit(
    _In_ NET_ADAPTER_LINK_LAYER_ADDRESS const* address,
    _Out_ _Post_satisfies_(*byte < MAX_NIC_MULTICAST_REG) UCHAR* byte,
    _Out_ UCHAR* value
);

#ifdef __cplusplus
}
#endif
