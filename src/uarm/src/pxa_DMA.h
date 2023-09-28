//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#ifndef _PXA_DMA_H_
#define _PXA_DMA_H_

#include "soc_DMA.h"

// common
#define DMA_CMR_DREQ_0 0
#define DMA_CMR_DREQ_1 1
#define DMA_CMR_I2S_RX 2
#define DMA_CMR_I2S_TX 3
#define DMA_CMR_BTUART_RX 4
#define DMA_CMR_BTUART_TX 5
#define DMA_CMR_FFUART_RX 6
#define DMA_CMR_FFUART_TX 7
#define DMA_CMR_AC97_MIC 8
#define DMA_CMR_AC97_MODEM_RX 9
#define DMA_CMR_AC97_MODEM_TX 10
#define DMA_CMR_AC97_AUDIO_RX 11
#define DMA_CMR_AC97_AUDIO_TX 12
#define DMA_CMR_SSP_RX 13
#define DMA_CMR_SSP_TX 14
#define DMA_CMR_FICP_RX 17
#define DMA_CMR_FICP_TX 18
#define DMA_CMR_STUART_RX 19
#define DMA_CMR_STUART_TX 20
#define DMA_CMR_MMC_RX 21
#define DMA_CMR_MMC_TX 22

// PXA25x and PXA26x
#define DMA_CMR_NSSP_RX 15
#define DMA_CMR_NSSP_TX 16
#define DMA_CMR_USB_EP1_TX 25
#define DMA_CMR_USB_EP2_RX 26
#define DMA_CMR_USB_EP3_TX 27
#define DMA_CMR_USB_EP4_RX 28
#define DMA_CMR_USB_EP6_TX 30
#define DMA_CMR_USB_EP7_RX 31
#define DMA_CMR_USB_EP8_TX 32
#define DMA_CMR_USB_EP9_RX 33
#define DMA_CMR_USB_EP11_TX 35
#define DMA_CMR_USB_EP12_RX 36
#define DMA_CMR_USB_EP13_TX 37
#define DMA_CMR_USB_EP14_RX 38

// PXA26x
#define DMA_CMR_ASSP_RX 23
#define DMA_CMR_ASSP_TX 24

// PXA27x
#define DMA_CMR_SSP2_RX 15
#define DMA_CMR_SSP2_TX 16
#define DMA_CMR_USB_EP0 24
#define DMA_CMR_USB_EPA 25
#define DMA_CMR_USB_EPB 26
#define DMA_CMR_USB_EPC 27
#define DMA_CMR_USB_EPD 28
#define DMA_CMR_USB_EPE 29
#define DMA_CMR_USB_EPF 30
#define DMA_CMR_USB_EPG 31
#define DMA_CMR_USB_EPH 32
#define DMA_CMR_USB_EPI 33
#define DMA_CMR_USB_EPJ 34
#define DMA_CMR_USB_EPK 35
#define DMA_CMR_USB_EPL 36
#define DMA_CMR_USB_EPM 37
#define DMA_CMR_USB_EPN 38
#define DMA_CMR_USB_EPP 39
#define DMA_CMR_USB_EPQ 40
#define DMA_CMR_USB_EPR 41
#define DMA_CMR_USB_EPS 42
#define DMA_CMR_USB_EPT 43
#define DMA_CMR_USB_EPU 44
#define DMA_CMR_USB_EPV 45
#define DMA_CMR_USB_EPW 46
#define DMA_CMR_USB_EPX 47
#define DMA_CMR_MSL_1_RX 48
#define DMA_CMR_MSL_1_TX 49
#define DMA_CMR_MSL_2_RX 50
#define DMA_CMR_MSL_2_TX 51
#define DMA_CMR_MSL_3_RX 52
#define DMA_CMR_MSL_3_TX 53
#define DMA_CMR_MSL_4_RX 54
#define DMA_CMR_MSL_4_TX 55
#define DMA_CMR_MSL_5_RX 56
#define DMA_CMR_MSL_5_TX 57
#define DMA_CMR_MSL_6_RX 58
#define DMA_CMR_MSL_6_TX 59
#define DMA_CMR_MSL_7_RX 60
#define DMA_CMR_MSL_7_TX 61
#define DMA_CMR_USIM_RX 62
#define DMA_CMR_USIM_TX 63
#define DMA_CMR_MS_RX 64
#define DMA_CMR_MS_TX 65
#define DMA_CMR_SSP3_RX 66
#define DMA_CMR_SSP3_TX 67
#define DMA_CMR_QCIF_RX_0 68
#define DMA_CMR_QCIF_RX_1 69
#define DMA_CMR_QCIF_RX_2 70
#define DMA_CMR_DREQ_2 74

#endif
