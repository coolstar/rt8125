#pragma once

#define __NO_STRICT_ALIGNMENT

// Driver Options

#define s0_magic_packet 0
#define config_soc_lan 0
#define interrupt_mitigation 1
#define phy_power_saving 1
#define phy_mdix_mode RE_ETH_PHY_AUTO_MDI_MDIX

#define RE_CSUM_FEATURES_IPV4    (CSUM_IP | CSUM_TCP | CSUM_UDP)
#define RE_CSUM_FEATURES_IPV6    (CSUM_TCP_IPV6 | CSUM_UDP_IPV6)
#define RE_CSUM_FEATURES    (RE_CSUM_FEATURES_IPV4 | RE_CSUM_FEATURES_IPV6)

// BSD Compat

#define __FBSDID(id)

#define	MJUM9BYTES	(9 * 1024)	/* jumbo cluster 9k */
#define	MJUM16BYTES	(16 * 1024)	/* jumbo cluster 16k */

#define __P(x) x

#define DELAY(x) KeStallExecutionProcessor(x)

#define ARRAY_SIZE(x) ARRAYSIZE(x)

#define htole16(x) x
#define htole32(x) x
#define htole64(x) x

#define ntohs(x) RtlUshortByteSwap(x)

#define	ENXIO		6		/* Device not configured */
#define	EOPNOTSUPP	45		/* Operation not supported */

#define Add2Ptr(Ptr, Value) ((PVOID)((PUCHAR)(Ptr) + (Value)))

#define MmioAddr(sc, Reg) Add2Ptr(sc->dev->MMIOAddress, Reg)

#if DEBUG
#define device_printf(dev, x, ...) DbgPrint(x, __VA_ARGS__)
#else
#define device_printf(dev, x, ...) __nop()
#endif

#if !defined(_ARM_) && !defined(_ARM64_)
#define WRITE_REGISTER_NOFENCE_ULONG WRITE_REGISTER_ULONG
#define WRITE_REGISTER_NOFENCE_USHORT WRITE_REGISTER_USHORT
#define WRITE_REGISTER_NOFENCE_UCHAR WRITE_REGISTER_UCHAR

#define READ_REGISTER_NOFENCE_ULONG READ_REGISTER_ULONG
#define READ_REGISTER_NOFENCE_USHORT READ_REGISTER_USHORT
#define READ_REGISTER_NOFENCE_UCHAR READ_REGISTER_UCHAR
#endif

#include "mii.h"
#include "ethernet.h"
#include "if.h"
#include "mbuf.h"

typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef signed short int16_t;
typedef unsigned short uint16_t;
typedef signed int int32_t;
typedef unsigned int uint32_t;
typedef signed long long int64_t;
typedef unsigned long long uint64_t;

typedef uint8_t u_int8_t;
typedef uint16_t u_int16_t;
typedef uint32_t u_int32_t;
typedef uint64_t u_int64_t;
typedef uintptr_t caddr_t;

struct ifnet {
    uint16_t if_mtu;
    int if_capenable;
    int if_hwassist;
};

// Placeholder types - fields are not used on WIN32.

typedef char bus_dma_tag_t;
typedef char bus_dmamap_t;
typedef char bus_dma_segment_t;
typedef char bus_addr_t;
struct bus_dma_tag_common { char x; };
