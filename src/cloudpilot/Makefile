-include ../Makefile.local

CFLAGS_COMMON = -Wextra -Wall -Wno-unused-parameter -Wno-pragma-pack -Wno-multichar -Wno-unknown-pragmas \
	-Wno-missing-field-initializers -DEMULATION_LEVEL=EMULATION_UNIX

CFLAGS_EMCC_EXTRA = -D_POSIX_SOURCE

LDFLAGS_TEST_EXTRA ?= -lgtest_main

EMCC_INITIAL_MEMORY = 67108864

WEBIDL_BINDING_SRC = \
	$(WEBIDL_BINDING_DIR)/cloudpilot.idl \
	../common/web/gunzip.idl \
	../common/web/zipfile_walker.idl \
	../common/web/session_file5.idl

INCLUDE = \
	-I../common \
	-I../common/zip \
	-I../skins \
	-I./emulator/hardware \
	-I./emulator/hardware/clie \
	-I./emulator/hardware/handera \
	-I./emulator/hardware/acer \
	-I./emulator/uae \
	-I./emulator/patch \
	-I./emulator/patch/clie \
	-I./emulator/palm/Device \
	-I./emulator/palm/Platform \
	-I./emulator/palm/Platform/Core/Hardware/IncsPrv \
	-I./emulator/palm/Platform/Core/System/IncsPrv \
	-I./emulator/palm/Platform/Incs \
	-I./emulator/palm/Platform/Incs/Core \
	-I./emulator/palm/Platform/Incs/Core/Hardware \
	-I./emulator/palm/Platform/Incs/Core/System \
	-I./emulator/palm/Platform/Incs/Core/UI \
	-I./emulator/palm/Platform/Incs/Libraries \
	-I./emulator/suspend \
	-I./emulator \
	-I./nanopb \
	-I./proto \
	-I../argparse \
	-Inative/network-backend

SOURCE_C = \
	emulator/uae/cpudefs.c \
	emulator/uae/cpuemu.c \
	emulator/uae/cpustbl.c \
	emulator/palm/Platform/Core/System/Src/Crc.c \
	nanopb/pb_common.c \
	nanopb/pb_encode.c \
	nanopb/pb_decode.c \
	proto/networking.pb.c

SOURCE_CXX = \
	emulator/uae/readcpu.cpp \
	emulator/hardware/EmMemory.cpp \
	emulator/hardware/EmHAL.cpp \
	emulator/hardware/EmCPU.cpp \
	emulator/hardware/EmCPU68K.cpp \
	emulator/hardware/EmBankSRAM.cpp \
	emulator/hardware/EmBankDRAM.cpp \
	emulator/hardware/EmBankDummy.cpp \
	emulator/hardware/EmBankMapped.cpp \
	emulator/hardware/EmRegs.cpp \
	emulator/hardware/EmBankRegs.cpp \
	emulator/hardware/EmSPISlave.cpp \
	emulator/hardware/EmUARTDragonball.cpp \
	emulator/hardware/EmRegsEZ.cpp \
	emulator/hardware/EmRegsVZ.cpp \
	emulator/hardware/EmRegsSZ.cpp \
	emulator/hardware/EmRegsESRAM.cpp \
	emulator/hardware/EmSPISlaveADS784x.cpp \
	emulator/hardware/EmRegsUSBPhilipsPDIUSBD12.cpp \
	emulator/hardware/EmRegsSED1376.cpp \
	emulator/hardware/EmRegsSED1375.cpp \
	emulator/hardware/EmRegsFrameBuffer.cpp \
	emulator/hardware/EmRegsVZPalmM500.cpp \
	emulator/hardware/EmRegsVZPalmM505.cpp \
	emulator/hardware/EmRegsEZPalmV.cpp \
	emulator/hardware/EmRegsEZPalmIIIc.cpp \
	emulator/hardware/EmRegsMediaQ11xx.cpp \
	emulator/hardware/EmRegsVZPalmM125.cpp \
	emulator/hardware/EmRegsVZPalmM130.cpp \
	emulator/hardware/EmRegsVZPalmI705.cpp \
	emulator/hardware/EmRegsPLDPalmI705.cpp \
	emulator/hardware/EmRegsEZPalmM100.cpp \
	emulator/hardware/EmRegsPLDAtlantiC.cpp \
	emulator/hardware/EmRegsVZAtlantiC.cpp \
	emulator/hardware/EmRegsPLDPacifiC.cpp \
	emulator/hardware/EmRegsMediaQ11xxPacifiC.cpp \
	emulator/hardware/EmRegs328.cpp \
	emulator/hardware/EmRegs328PalmPilot.cpp \
	emulator/hardware/EmRegsEZPalmVIIx.cpp \
	emulator/hardware/EmRegsEZPalmVII.cpp \
	emulator/hardware/EmRegsPLDPalmVIIEZ.cpp \
	emulator/hardware/EmRegsSZLP168.cpp \
	emulator/hardware/EmBankROM.cpp \
	emulator/hardware/EmUAEGlue.cpp \
	emulator/hardware/EmSPISlaveSD.cpp \
	emulator/hardware/EmRegsMB86189.cpp \
	emulator/hardware/MemoryStick.cpp \
	emulator/hardware/clie/EmRegsUsbCLIE.cpp \
	emulator/hardware/clie/EmRegsFMSound.cpp \
	emulator/hardware/clie/EmRegsEzPegS300.cpp \
	emulator/hardware/clie/EmRegsEzPegS500C.cpp \
	emulator/hardware/clie/EmRegsVZPegNasca.cpp \
	emulator/hardware/clie/EmRegsVZPegVenice.cpp \
	emulator/hardware/clie/EmRegsVZPegYellowStone.cpp \
	emulator/hardware/clie/EmRegsVZPegModena.cpp \
	emulator/hardware/clie/EmRegsSZNaples.cpp \
	emulator/hardware/clie/EmRegsSZRedwood.cpp \
	emulator/hardware/clie/EmRegsVZPegN700C.cpp \
	emulator/hardware/clie/EmRegsLCDCtrlT2.cpp \
	emulator/hardware/clie/EmRegsSonyDSP.cpp \
	emulator/hardware/handera/EmRegs330CPLD.cpp \
	emulator/hardware/handera/EmRegsVZHandEra330.cpp \
	emulator/hardware/handera/EmRegsVZHandEra330c.cpp \
	emulator/hardware/handera/EmSPISlave330Current.cpp \
	emulator/hardware/handera/EmRegs330CPLDMirror.cpp \
	emulator/hardware/handera/EmTRGATA.cpp \
	emulator/hardware/handera/EmTRGCF.cpp \
	emulator/hardware/handera/EmTRGCFIO.cpp \
	emulator/hardware/handera/EmTRGCFMem.cpp \
	emulator/hardware/handera/EmTRGDiskIO.cpp \
	emulator/hardware/acer/EmRegsAcerUSBStub.cpp \
	emulator/hardware/acer/EmRegsAcerDSPStub.cpp \
	emulator/hardware/acer/EmRegsVZAcerS1x.cpp \
	emulator/patch/EmPatchModule.cpp \
	emulator/patch/EmPatchModuleSys.cpp \
	emulator/patch/EmPatchModuleHtal.cpp \
	emulator/patch/PatchModuleNetlib.cpp \
	emulator/patch/clie/PatchModuleClieStubAll.cpp \
	emulator/patch/EmPatchMgr.cpp \
	emulator/suspend/SuspendManager.cpp \
	emulator/suspend/SuspendContext.cpp \
	emulator/suspend/SuspendContextClipboardPaste.cpp \
	emulator/suspend/SuspendContextClipboardCopy.cpp \
	emulator/suspend/SuspendContextNetworkConnect.cpp \
	emulator/suspend/SuspendContextNetworkRpc.cpp \
	emulator/suspend/SuspendContextSerialSync.cpp \
	emulator/EmSystemState.cpp \
	emulator/Miscellaneous.cpp \
	emulator/Marshall.cpp \
	emulator/Platform.cpp \
	emulator/EmDevice.cpp \
	emulator/EmLowMem.cpp \
	emulator/EmPalmOS.cpp \
	emulator/EmSubroutine.cpp \
	emulator/Frame.cpp \
	emulator/EmPoint.cpp \
	emulator/EmThreadSafeQueue.cpp \
	emulator/EmTransportSerial.cpp \
	emulator/EmTransportSerialNull.cpp \
	emulator/EmTransportSerialBuffer.cpp \
	emulator/MetaMemory.cpp \
	emulator/Byteswapping.cpp \
	emulator/EmSession.cpp \
	emulator/EmPalmStructs.cpp \
	emulator/EmROMReader.cpp \
	emulator/EmCommon.cpp \
	emulator/StringData.cpp \
	emulator/EmFileImport.cpp \
	emulator/ROMStubs.cpp \
	emulator/SessionImage.cpp \
	emulator/DbBackup.cpp \
	emulator/DbExporter.cpp \
	emulator/CallbackManager.cpp \
	emulator/DbInstaller.cpp \
	emulator/Feature.cpp \
	emulator/ScreenDimensions.cpp \
	emulator/NetworkProxy.cpp \
	emulator/ExternalStorage.cpp \
	emulator/MemoryRegion.cpp \
	emulator/StackDump.cpp \
	emulator/Debugger.cpp

SOURCE_C_NATIVE = $(SOURCE_C)
SOURCE_CXX_NATIVE = \
	$(SOURCE_CXX) \
	native/main.cpp \
	native/MainLoop.cpp \
	native/Silkscreen.cpp \
	native/util.cpp \
	native/EventHandler.cpp \
	native/Commands.cpp \
	native/proxyClientWs.cpp \
	native/proxyClientNative.cpp \
	native/ProxyHandler.cpp \
	native/GdbStub.cpp \
	native/ElfParser.cpp \
	native/network-backend/NetworkSession.cpp \
	native/network-backend/networkBackend.cpp \
	native/network-backend/codes.cpp \
	native/network-backend/sockopt.cpp \
	native/DebugSupport.cpp \
	emulator/assert_native.cpp \
	emulator/stacktrace.cpp

SOURCE_C_EMCC = $(SOURCE_C)
SOURCE_CXX_EMCC = \
	$(SOURCE_CXX) \
	emulator/assert_emscripten.cpp \
	web/main.cpp \
	web/Cloudpilot.cpp \
	web/RomInfo.cpp \
	web/SkinLoader.cpp

SOURCE_C_TEST = $(SOURCE_C)
SOURCE_CXX_TEST = \
	$(SOURCE_CXX) \
	emulator/assert_native.cpp \
	emulator/stacktrace.cpp \
	test/Fifo.cpp \
	test/Miscellaneous.cpp

OBJECTS_EXTRA_NATIVE = ../common/libcommon.a
OBJECTS_EXTRA_TEST = ../common/libcommon.a
OBJECTS_EXTRA_EMCC = ../common/libcommon-wasm.a ../skins/libskin-wasm.a

BINARY_NATIVE = cloudpilot-emu
BINARY_EMCC = cloudpilot_web.wasm
BINARY_TEST = test/test

include ../Makefile.common

bin: $(BINARY_NATIVE)

emscripten: $(BINARY_EMCC)

test: $(BINARY_TEST)
	$(BINARY_TEST)

.PHONY: bin emscripten clean test