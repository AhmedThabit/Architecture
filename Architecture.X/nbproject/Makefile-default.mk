#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/Architecture.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/Architecture.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../src/main.c protothreads.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/driver/sdspi/src/drv_sdspi.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/driver/sdspi/src/drv_sdspi_file_system.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/driver/sdspi/src/drv_sdspi_plib_interface.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/exceptions.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/initialization.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/interrupts.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/peripheral/clk/plib_clk.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/peripheral/coretimer/plib_coretimer.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/peripheral/evic/plib_evic.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/peripheral/gpio/plib_gpio.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/peripheral/spi/spi_master/plib_spi1_master.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/peripheral/spi/spi_master/plib_spi3_master.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/peripheral/tmr1/plib_tmr1.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/peripheral/tmr2/plib_tmr2.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/peripheral/uart/plib_uart1.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/peripheral/uart/plib_uart2.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/peripheral/uart/plib_uart3.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/stdio/xc32_monitor.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/system/dma/sys_dma.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/system/fs/fat_fs/file_system/ff.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/system/fs/fat_fs/file_system/ffunicode.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/system/fs/fat_fs/hardware_access/diskio.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/system/fs/src/sys_fs.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/system/fs/src/sys_fs_fat_interface.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/system/fs/src/sys_fs_media_manager.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/system/int/src/sys_int.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/system/time/src/sys_time.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/tasks.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/common/pt_yield_flag.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/hal/bsp/bsp.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/hal/sd_spi/sd_spi_lowlevel.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/hal/spi_guard/spi_bus_guard.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/drivers/esp32/esp32.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/drivers/esp32/esp32_comm.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/drivers/ethernet/w5500if.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/drivers/flash/flash_w25q32.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/drivers/modem/modem_at.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/drivers/modem/modem_uart.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/drivers/sd/sd_raw.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/drivers/telit/telit.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/services/alarm/alarm_mgr.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/services/esp32_proto/esp32_proto.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/services/gsm/gsm_le910.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/services/gsm/gsm_service.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/services/sd_service/sd_service.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/services/sms/sms_service.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/services/storage/phonebook_flash.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/services/storage/storage.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/services/storage/store.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/app/app.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/app/handlers.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/app/layers_docments.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/test/sd_card.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/test/sd_test.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/hal/io_monitor/io_monitor.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1360937237/main.o ${OBJECTDIR}/protothreads.o ${OBJECTDIR}/_ext/1060863540/drv_sdspi.o ${OBJECTDIR}/_ext/1060863540/drv_sdspi_file_system.o ${OBJECTDIR}/_ext/1060863540/drv_sdspi_plib_interface.o ${OBJECTDIR}/_ext/181187660/exceptions.o ${OBJECTDIR}/_ext/181187660/initialization.o ${OBJECTDIR}/_ext/181187660/interrupts.o ${OBJECTDIR}/_ext/1170764338/plib_clk.o ${OBJECTDIR}/_ext/134162454/plib_coretimer.o ${OBJECTDIR}/_ext/1934025339/plib_evic.o ${OBJECTDIR}/_ext/1934079167/plib_gpio.o ${OBJECTDIR}/_ext/525845896/plib_spi1_master.o ${OBJECTDIR}/_ext/525845896/plib_spi3_master.o ${OBJECTDIR}/_ext/1934463784/plib_tmr1.o ${OBJECTDIR}/_ext/1934463785/plib_tmr2.o ${OBJECTDIR}/_ext/1934482110/plib_uart1.o ${OBJECTDIR}/_ext/1934482110/plib_uart2.o ${OBJECTDIR}/_ext/1934482110/plib_uart3.o ${OBJECTDIR}/_ext/1366434042/xc32_monitor.o ${OBJECTDIR}/_ext/720722619/sys_dma.o ${OBJECTDIR}/_ext/764968315/ff.o ${OBJECTDIR}/_ext/764968315/ffunicode.o ${OBJECTDIR}/_ext/1812136516/diskio.o ${OBJECTDIR}/_ext/567453951/sys_fs.o ${OBJECTDIR}/_ext/567453951/sys_fs_fat_interface.o ${OBJECTDIR}/_ext/567453951/sys_fs_media_manager.o ${OBJECTDIR}/_ext/1302700025/sys_int.o ${OBJECTDIR}/_ext/33059905/sys_time.o ${OBJECTDIR}/_ext/181187660/tasks.o ${OBJECTDIR}/_ext/1123615171/pt_yield_flag.o ${OBJECTDIR}/_ext/211292139/bsp.o ${OBJECTDIR}/_ext/1345492590/sd_spi_lowlevel.o ${OBJECTDIR}/_ext/503574270/spi_bus_guard.o ${OBJECTDIR}/_ext/1534143173/esp32.o ${OBJECTDIR}/_ext/1534143173/esp32_comm.o ${OBJECTDIR}/_ext/1618932211/w5500if.o ${OBJECTDIR}/_ext/1534845780/flash_w25q32.o ${OBJECTDIR}/_ext/1541402254/modem_at.o ${OBJECTDIR}/_ext/1541402254/modem_uart.o ${OBJECTDIR}/_ext/341013869/sd_raw.o ${OBJECTDIR}/_ext/1547576810/telit.o ${OBJECTDIR}/_ext/1709141704/alarm_mgr.o ${OBJECTDIR}/_ext/346465585/esp32_proto.o ${OBJECTDIR}/_ext/963589544/gsm_le910.o ${OBJECTDIR}/_ext/963589544/gsm_service.o ${OBJECTDIR}/_ext/1898101824/sd_service.o ${OBJECTDIR}/_ext/963600896/sms_service.o ${OBJECTDIR}/_ext/1524448162/phonebook_flash.o ${OBJECTDIR}/_ext/1524448162/storage.o ${OBJECTDIR}/_ext/1524448162/store.o ${OBJECTDIR}/_ext/1734184631/app.o ${OBJECTDIR}/_ext/1734184631/handlers.o ${OBJECTDIR}/_ext/1734184631/layers_docments.o ${OBJECTDIR}/_ext/2075406954/sd_card.o ${OBJECTDIR}/_ext/2075406954/sd_test.o ${OBJECTDIR}/_ext/1718247445/io_monitor.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1360937237/main.o.d ${OBJECTDIR}/protothreads.o.d ${OBJECTDIR}/_ext/1060863540/drv_sdspi.o.d ${OBJECTDIR}/_ext/1060863540/drv_sdspi_file_system.o.d ${OBJECTDIR}/_ext/1060863540/drv_sdspi_plib_interface.o.d ${OBJECTDIR}/_ext/181187660/exceptions.o.d ${OBJECTDIR}/_ext/181187660/initialization.o.d ${OBJECTDIR}/_ext/181187660/interrupts.o.d ${OBJECTDIR}/_ext/1170764338/plib_clk.o.d ${OBJECTDIR}/_ext/134162454/plib_coretimer.o.d ${OBJECTDIR}/_ext/1934025339/plib_evic.o.d ${OBJECTDIR}/_ext/1934079167/plib_gpio.o.d ${OBJECTDIR}/_ext/525845896/plib_spi1_master.o.d ${OBJECTDIR}/_ext/525845896/plib_spi3_master.o.d ${OBJECTDIR}/_ext/1934463784/plib_tmr1.o.d ${OBJECTDIR}/_ext/1934463785/plib_tmr2.o.d ${OBJECTDIR}/_ext/1934482110/plib_uart1.o.d ${OBJECTDIR}/_ext/1934482110/plib_uart2.o.d ${OBJECTDIR}/_ext/1934482110/plib_uart3.o.d ${OBJECTDIR}/_ext/1366434042/xc32_monitor.o.d ${OBJECTDIR}/_ext/720722619/sys_dma.o.d ${OBJECTDIR}/_ext/764968315/ff.o.d ${OBJECTDIR}/_ext/764968315/ffunicode.o.d ${OBJECTDIR}/_ext/1812136516/diskio.o.d ${OBJECTDIR}/_ext/567453951/sys_fs.o.d ${OBJECTDIR}/_ext/567453951/sys_fs_fat_interface.o.d ${OBJECTDIR}/_ext/567453951/sys_fs_media_manager.o.d ${OBJECTDIR}/_ext/1302700025/sys_int.o.d ${OBJECTDIR}/_ext/33059905/sys_time.o.d ${OBJECTDIR}/_ext/181187660/tasks.o.d ${OBJECTDIR}/_ext/1123615171/pt_yield_flag.o.d ${OBJECTDIR}/_ext/211292139/bsp.o.d ${OBJECTDIR}/_ext/1345492590/sd_spi_lowlevel.o.d ${OBJECTDIR}/_ext/503574270/spi_bus_guard.o.d ${OBJECTDIR}/_ext/1534143173/esp32.o.d ${OBJECTDIR}/_ext/1534143173/esp32_comm.o.d ${OBJECTDIR}/_ext/1618932211/w5500if.o.d ${OBJECTDIR}/_ext/1534845780/flash_w25q32.o.d ${OBJECTDIR}/_ext/1541402254/modem_at.o.d ${OBJECTDIR}/_ext/1541402254/modem_uart.o.d ${OBJECTDIR}/_ext/341013869/sd_raw.o.d ${OBJECTDIR}/_ext/1547576810/telit.o.d ${OBJECTDIR}/_ext/1709141704/alarm_mgr.o.d ${OBJECTDIR}/_ext/346465585/esp32_proto.o.d ${OBJECTDIR}/_ext/963589544/gsm_le910.o.d ${OBJECTDIR}/_ext/963589544/gsm_service.o.d ${OBJECTDIR}/_ext/1898101824/sd_service.o.d ${OBJECTDIR}/_ext/963600896/sms_service.o.d ${OBJECTDIR}/_ext/1524448162/phonebook_flash.o.d ${OBJECTDIR}/_ext/1524448162/storage.o.d ${OBJECTDIR}/_ext/1524448162/store.o.d ${OBJECTDIR}/_ext/1734184631/app.o.d ${OBJECTDIR}/_ext/1734184631/handlers.o.d ${OBJECTDIR}/_ext/1734184631/layers_docments.o.d ${OBJECTDIR}/_ext/2075406954/sd_card.o.d ${OBJECTDIR}/_ext/2075406954/sd_test.o.d ${OBJECTDIR}/_ext/1718247445/io_monitor.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1360937237/main.o ${OBJECTDIR}/protothreads.o ${OBJECTDIR}/_ext/1060863540/drv_sdspi.o ${OBJECTDIR}/_ext/1060863540/drv_sdspi_file_system.o ${OBJECTDIR}/_ext/1060863540/drv_sdspi_plib_interface.o ${OBJECTDIR}/_ext/181187660/exceptions.o ${OBJECTDIR}/_ext/181187660/initialization.o ${OBJECTDIR}/_ext/181187660/interrupts.o ${OBJECTDIR}/_ext/1170764338/plib_clk.o ${OBJECTDIR}/_ext/134162454/plib_coretimer.o ${OBJECTDIR}/_ext/1934025339/plib_evic.o ${OBJECTDIR}/_ext/1934079167/plib_gpio.o ${OBJECTDIR}/_ext/525845896/plib_spi1_master.o ${OBJECTDIR}/_ext/525845896/plib_spi3_master.o ${OBJECTDIR}/_ext/1934463784/plib_tmr1.o ${OBJECTDIR}/_ext/1934463785/plib_tmr2.o ${OBJECTDIR}/_ext/1934482110/plib_uart1.o ${OBJECTDIR}/_ext/1934482110/plib_uart2.o ${OBJECTDIR}/_ext/1934482110/plib_uart3.o ${OBJECTDIR}/_ext/1366434042/xc32_monitor.o ${OBJECTDIR}/_ext/720722619/sys_dma.o ${OBJECTDIR}/_ext/764968315/ff.o ${OBJECTDIR}/_ext/764968315/ffunicode.o ${OBJECTDIR}/_ext/1812136516/diskio.o ${OBJECTDIR}/_ext/567453951/sys_fs.o ${OBJECTDIR}/_ext/567453951/sys_fs_fat_interface.o ${OBJECTDIR}/_ext/567453951/sys_fs_media_manager.o ${OBJECTDIR}/_ext/1302700025/sys_int.o ${OBJECTDIR}/_ext/33059905/sys_time.o ${OBJECTDIR}/_ext/181187660/tasks.o ${OBJECTDIR}/_ext/1123615171/pt_yield_flag.o ${OBJECTDIR}/_ext/211292139/bsp.o ${OBJECTDIR}/_ext/1345492590/sd_spi_lowlevel.o ${OBJECTDIR}/_ext/503574270/spi_bus_guard.o ${OBJECTDIR}/_ext/1534143173/esp32.o ${OBJECTDIR}/_ext/1534143173/esp32_comm.o ${OBJECTDIR}/_ext/1618932211/w5500if.o ${OBJECTDIR}/_ext/1534845780/flash_w25q32.o ${OBJECTDIR}/_ext/1541402254/modem_at.o ${OBJECTDIR}/_ext/1541402254/modem_uart.o ${OBJECTDIR}/_ext/341013869/sd_raw.o ${OBJECTDIR}/_ext/1547576810/telit.o ${OBJECTDIR}/_ext/1709141704/alarm_mgr.o ${OBJECTDIR}/_ext/346465585/esp32_proto.o ${OBJECTDIR}/_ext/963589544/gsm_le910.o ${OBJECTDIR}/_ext/963589544/gsm_service.o ${OBJECTDIR}/_ext/1898101824/sd_service.o ${OBJECTDIR}/_ext/963600896/sms_service.o ${OBJECTDIR}/_ext/1524448162/phonebook_flash.o ${OBJECTDIR}/_ext/1524448162/storage.o ${OBJECTDIR}/_ext/1524448162/store.o ${OBJECTDIR}/_ext/1734184631/app.o ${OBJECTDIR}/_ext/1734184631/handlers.o ${OBJECTDIR}/_ext/1734184631/layers_docments.o ${OBJECTDIR}/_ext/2075406954/sd_card.o ${OBJECTDIR}/_ext/2075406954/sd_test.o ${OBJECTDIR}/_ext/1718247445/io_monitor.o

# Source Files
SOURCEFILES=../src/main.c protothreads.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/driver/sdspi/src/drv_sdspi.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/driver/sdspi/src/drv_sdspi_file_system.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/driver/sdspi/src/drv_sdspi_plib_interface.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/exceptions.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/initialization.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/interrupts.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/peripheral/clk/plib_clk.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/peripheral/coretimer/plib_coretimer.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/peripheral/evic/plib_evic.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/peripheral/gpio/plib_gpio.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/peripheral/spi/spi_master/plib_spi1_master.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/peripheral/spi/spi_master/plib_spi3_master.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/peripheral/tmr1/plib_tmr1.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/peripheral/tmr2/plib_tmr2.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/peripheral/uart/plib_uart1.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/peripheral/uart/plib_uart2.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/peripheral/uart/plib_uart3.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/stdio/xc32_monitor.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/system/dma/sys_dma.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/system/fs/fat_fs/file_system/ff.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/system/fs/fat_fs/file_system/ffunicode.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/system/fs/fat_fs/hardware_access/diskio.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/system/fs/src/sys_fs.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/system/fs/src/sys_fs_fat_interface.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/system/fs/src/sys_fs_media_manager.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/system/int/src/sys_int.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/system/time/src/sys_time.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/tasks.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/common/pt_yield_flag.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/hal/bsp/bsp.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/hal/sd_spi/sd_spi_lowlevel.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/hal/spi_guard/spi_bus_guard.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/drivers/esp32/esp32.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/drivers/esp32/esp32_comm.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/drivers/ethernet/w5500if.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/drivers/flash/flash_w25q32.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/drivers/modem/modem_at.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/drivers/modem/modem_uart.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/drivers/sd/sd_raw.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/drivers/telit/telit.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/services/alarm/alarm_mgr.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/services/esp32_proto/esp32_proto.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/services/gsm/gsm_le910.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/services/gsm/gsm_service.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/services/sd_service/sd_service.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/services/sms/sms_service.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/services/storage/phonebook_flash.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/services/storage/storage.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/services/storage/store.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/app/app.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/app/handlers.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/app/layers_docments.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/test/sd_card.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/test/sd_test.c C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/hal/io_monitor/io_monitor.c



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk ${DISTDIR}/Architecture.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=32MM0256GPM064
MP_LINKER_FILE_OPTION=,--script="..\src\config\default\p32MM0256GPM064.ld"
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1360937237/main.o: ../src/main.c  .generated_files/flags/default/abd2a09bbf01d831ad131a76d0d1e36628e4fbce .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/main.o.d" -o ${OBJECTDIR}/_ext/1360937237/main.o ../src/main.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/protothreads.o: protothreads.c  .generated_files/flags/default/7db35ee478d7b381057f8c8221a10be072812c03 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/protothreads.o.d 
	@${RM} ${OBJECTDIR}/protothreads.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/protothreads.o.d" -o ${OBJECTDIR}/protothreads.o protothreads.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1060863540/drv_sdspi.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/driver/sdspi/src/drv_sdspi.c  .generated_files/flags/default/2bbcaecf5a9f02eaec855c43789c5d51ca8d78d4 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1060863540" 
	@${RM} ${OBJECTDIR}/_ext/1060863540/drv_sdspi.o.d 
	@${RM} ${OBJECTDIR}/_ext/1060863540/drv_sdspi.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/1060863540/drv_sdspi.o.d" -o ${OBJECTDIR}/_ext/1060863540/drv_sdspi.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/driver/sdspi/src/drv_sdspi.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1060863540/drv_sdspi_file_system.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/driver/sdspi/src/drv_sdspi_file_system.c  .generated_files/flags/default/5d20f3c1f76ea8061f71f73fc93057acd4ca9d6d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1060863540" 
	@${RM} ${OBJECTDIR}/_ext/1060863540/drv_sdspi_file_system.o.d 
	@${RM} ${OBJECTDIR}/_ext/1060863540/drv_sdspi_file_system.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/1060863540/drv_sdspi_file_system.o.d" -o ${OBJECTDIR}/_ext/1060863540/drv_sdspi_file_system.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/driver/sdspi/src/drv_sdspi_file_system.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1060863540/drv_sdspi_plib_interface.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/driver/sdspi/src/drv_sdspi_plib_interface.c  .generated_files/flags/default/436fd31e7d6f07b49900c2fce09500cd3e6512cc .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1060863540" 
	@${RM} ${OBJECTDIR}/_ext/1060863540/drv_sdspi_plib_interface.o.d 
	@${RM} ${OBJECTDIR}/_ext/1060863540/drv_sdspi_plib_interface.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/1060863540/drv_sdspi_plib_interface.o.d" -o ${OBJECTDIR}/_ext/1060863540/drv_sdspi_plib_interface.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/driver/sdspi/src/drv_sdspi_plib_interface.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/181187660/exceptions.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/exceptions.c  .generated_files/flags/default/f90c87434863da9d64a8a61c09746be9d927b325 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/181187660" 
	@${RM} ${OBJECTDIR}/_ext/181187660/exceptions.o.d 
	@${RM} ${OBJECTDIR}/_ext/181187660/exceptions.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/181187660/exceptions.o.d" -o ${OBJECTDIR}/_ext/181187660/exceptions.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/exceptions.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/181187660/initialization.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/initialization.c  .generated_files/flags/default/66097011e666030a04c71b4c130f5664cbdb89a0 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/181187660" 
	@${RM} ${OBJECTDIR}/_ext/181187660/initialization.o.d 
	@${RM} ${OBJECTDIR}/_ext/181187660/initialization.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/181187660/initialization.o.d" -o ${OBJECTDIR}/_ext/181187660/initialization.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/initialization.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/181187660/interrupts.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/interrupts.c  .generated_files/flags/default/631e114f0c2d4f0990a71bb972083f3989466ff8 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/181187660" 
	@${RM} ${OBJECTDIR}/_ext/181187660/interrupts.o.d 
	@${RM} ${OBJECTDIR}/_ext/181187660/interrupts.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/181187660/interrupts.o.d" -o ${OBJECTDIR}/_ext/181187660/interrupts.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/interrupts.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1170764338/plib_clk.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/peripheral/clk/plib_clk.c  .generated_files/flags/default/151857d9234675cfb652a89d087525fdab9c5631 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1170764338" 
	@${RM} ${OBJECTDIR}/_ext/1170764338/plib_clk.o.d 
	@${RM} ${OBJECTDIR}/_ext/1170764338/plib_clk.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/1170764338/plib_clk.o.d" -o ${OBJECTDIR}/_ext/1170764338/plib_clk.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/peripheral/clk/plib_clk.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/134162454/plib_coretimer.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/peripheral/coretimer/plib_coretimer.c  .generated_files/flags/default/9667d70d51654cb2c92a415a95ec6a3fce438d12 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/134162454" 
	@${RM} ${OBJECTDIR}/_ext/134162454/plib_coretimer.o.d 
	@${RM} ${OBJECTDIR}/_ext/134162454/plib_coretimer.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/134162454/plib_coretimer.o.d" -o ${OBJECTDIR}/_ext/134162454/plib_coretimer.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/peripheral/coretimer/plib_coretimer.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1934025339/plib_evic.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/peripheral/evic/plib_evic.c  .generated_files/flags/default/ee2068bfdee5d0222a56f529157515dec9f8d9c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1934025339" 
	@${RM} ${OBJECTDIR}/_ext/1934025339/plib_evic.o.d 
	@${RM} ${OBJECTDIR}/_ext/1934025339/plib_evic.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/1934025339/plib_evic.o.d" -o ${OBJECTDIR}/_ext/1934025339/plib_evic.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/peripheral/evic/plib_evic.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1934079167/plib_gpio.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/peripheral/gpio/plib_gpio.c  .generated_files/flags/default/c2d0ed851575cadb3f187135388cec7c1c1e0245 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1934079167" 
	@${RM} ${OBJECTDIR}/_ext/1934079167/plib_gpio.o.d 
	@${RM} ${OBJECTDIR}/_ext/1934079167/plib_gpio.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/1934079167/plib_gpio.o.d" -o ${OBJECTDIR}/_ext/1934079167/plib_gpio.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/peripheral/gpio/plib_gpio.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/525845896/plib_spi1_master.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/peripheral/spi/spi_master/plib_spi1_master.c  .generated_files/flags/default/834e9355216fda96a94c01b2dab2424c415ea058 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/525845896" 
	@${RM} ${OBJECTDIR}/_ext/525845896/plib_spi1_master.o.d 
	@${RM} ${OBJECTDIR}/_ext/525845896/plib_spi1_master.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/525845896/plib_spi1_master.o.d" -o ${OBJECTDIR}/_ext/525845896/plib_spi1_master.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/peripheral/spi/spi_master/plib_spi1_master.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/525845896/plib_spi3_master.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/peripheral/spi/spi_master/plib_spi3_master.c  .generated_files/flags/default/d5328870689e9985fa5c5c792446d8d4c667825a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/525845896" 
	@${RM} ${OBJECTDIR}/_ext/525845896/plib_spi3_master.o.d 
	@${RM} ${OBJECTDIR}/_ext/525845896/plib_spi3_master.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/525845896/plib_spi3_master.o.d" -o ${OBJECTDIR}/_ext/525845896/plib_spi3_master.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/peripheral/spi/spi_master/plib_spi3_master.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1934463784/plib_tmr1.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/peripheral/tmr1/plib_tmr1.c  .generated_files/flags/default/3b4891ae87292b2b3fddb140b9337e748c61793a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1934463784" 
	@${RM} ${OBJECTDIR}/_ext/1934463784/plib_tmr1.o.d 
	@${RM} ${OBJECTDIR}/_ext/1934463784/plib_tmr1.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/1934463784/plib_tmr1.o.d" -o ${OBJECTDIR}/_ext/1934463784/plib_tmr1.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/peripheral/tmr1/plib_tmr1.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1934463785/plib_tmr2.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/peripheral/tmr2/plib_tmr2.c  .generated_files/flags/default/ce11b422b367f0b049a7e9aee6d447eb185f9dcc .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1934463785" 
	@${RM} ${OBJECTDIR}/_ext/1934463785/plib_tmr2.o.d 
	@${RM} ${OBJECTDIR}/_ext/1934463785/plib_tmr2.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/1934463785/plib_tmr2.o.d" -o ${OBJECTDIR}/_ext/1934463785/plib_tmr2.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/peripheral/tmr2/plib_tmr2.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1934482110/plib_uart1.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/peripheral/uart/plib_uart1.c  .generated_files/flags/default/d18a35802b9f5f19b7943a9caf1db9942978f50c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1934482110" 
	@${RM} ${OBJECTDIR}/_ext/1934482110/plib_uart1.o.d 
	@${RM} ${OBJECTDIR}/_ext/1934482110/plib_uart1.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/1934482110/plib_uart1.o.d" -o ${OBJECTDIR}/_ext/1934482110/plib_uart1.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/peripheral/uart/plib_uart1.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1934482110/plib_uart2.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/peripheral/uart/plib_uart2.c  .generated_files/flags/default/e1ef4b072973949752b4f5962a22b49a8158f3ec .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1934482110" 
	@${RM} ${OBJECTDIR}/_ext/1934482110/plib_uart2.o.d 
	@${RM} ${OBJECTDIR}/_ext/1934482110/plib_uart2.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/1934482110/plib_uart2.o.d" -o ${OBJECTDIR}/_ext/1934482110/plib_uart2.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/peripheral/uart/plib_uart2.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1934482110/plib_uart3.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/peripheral/uart/plib_uart3.c  .generated_files/flags/default/9100d912bc9cba1b723163858226740cbc5ab8db .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1934482110" 
	@${RM} ${OBJECTDIR}/_ext/1934482110/plib_uart3.o.d 
	@${RM} ${OBJECTDIR}/_ext/1934482110/plib_uart3.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/1934482110/plib_uart3.o.d" -o ${OBJECTDIR}/_ext/1934482110/plib_uart3.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/peripheral/uart/plib_uart3.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1366434042/xc32_monitor.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/stdio/xc32_monitor.c  .generated_files/flags/default/2a02cb617ef235b300ba63dcdd4b7a7868ced445 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1366434042" 
	@${RM} ${OBJECTDIR}/_ext/1366434042/xc32_monitor.o.d 
	@${RM} ${OBJECTDIR}/_ext/1366434042/xc32_monitor.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/1366434042/xc32_monitor.o.d" -o ${OBJECTDIR}/_ext/1366434042/xc32_monitor.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/stdio/xc32_monitor.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/720722619/sys_dma.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/system/dma/sys_dma.c  .generated_files/flags/default/7715faf92e43e32743bc6c7513f62d7be3b0b721 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/720722619" 
	@${RM} ${OBJECTDIR}/_ext/720722619/sys_dma.o.d 
	@${RM} ${OBJECTDIR}/_ext/720722619/sys_dma.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/720722619/sys_dma.o.d" -o ${OBJECTDIR}/_ext/720722619/sys_dma.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/system/dma/sys_dma.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/764968315/ff.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/system/fs/fat_fs/file_system/ff.c  .generated_files/flags/default/a4268cfd551cd4501d620dd21d483f0c46f4f120 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/764968315" 
	@${RM} ${OBJECTDIR}/_ext/764968315/ff.o.d 
	@${RM} ${OBJECTDIR}/_ext/764968315/ff.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/764968315/ff.o.d" -o ${OBJECTDIR}/_ext/764968315/ff.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/system/fs/fat_fs/file_system/ff.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/764968315/ffunicode.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/system/fs/fat_fs/file_system/ffunicode.c  .generated_files/flags/default/50299a5c6fa01caceba725900e5b06a17e39ac52 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/764968315" 
	@${RM} ${OBJECTDIR}/_ext/764968315/ffunicode.o.d 
	@${RM} ${OBJECTDIR}/_ext/764968315/ffunicode.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/764968315/ffunicode.o.d" -o ${OBJECTDIR}/_ext/764968315/ffunicode.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/system/fs/fat_fs/file_system/ffunicode.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1812136516/diskio.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/system/fs/fat_fs/hardware_access/diskio.c  .generated_files/flags/default/cce78c836d741f6f52bc5e94242b59ab4197589b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1812136516" 
	@${RM} ${OBJECTDIR}/_ext/1812136516/diskio.o.d 
	@${RM} ${OBJECTDIR}/_ext/1812136516/diskio.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/1812136516/diskio.o.d" -o ${OBJECTDIR}/_ext/1812136516/diskio.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/system/fs/fat_fs/hardware_access/diskio.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/567453951/sys_fs.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/system/fs/src/sys_fs.c  .generated_files/flags/default/baa067f5254fd5b916da287ab332c7db4e5a8c3b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/567453951" 
	@${RM} ${OBJECTDIR}/_ext/567453951/sys_fs.o.d 
	@${RM} ${OBJECTDIR}/_ext/567453951/sys_fs.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/567453951/sys_fs.o.d" -o ${OBJECTDIR}/_ext/567453951/sys_fs.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/system/fs/src/sys_fs.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/567453951/sys_fs_fat_interface.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/system/fs/src/sys_fs_fat_interface.c  .generated_files/flags/default/f958370fa507d44806616c1af36d4e3b81ce7813 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/567453951" 
	@${RM} ${OBJECTDIR}/_ext/567453951/sys_fs_fat_interface.o.d 
	@${RM} ${OBJECTDIR}/_ext/567453951/sys_fs_fat_interface.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/567453951/sys_fs_fat_interface.o.d" -o ${OBJECTDIR}/_ext/567453951/sys_fs_fat_interface.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/system/fs/src/sys_fs_fat_interface.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/567453951/sys_fs_media_manager.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/system/fs/src/sys_fs_media_manager.c  .generated_files/flags/default/3a8a3c57fcf531c3437002aaf78d4aa3087848d2 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/567453951" 
	@${RM} ${OBJECTDIR}/_ext/567453951/sys_fs_media_manager.o.d 
	@${RM} ${OBJECTDIR}/_ext/567453951/sys_fs_media_manager.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/567453951/sys_fs_media_manager.o.d" -o ${OBJECTDIR}/_ext/567453951/sys_fs_media_manager.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/system/fs/src/sys_fs_media_manager.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1302700025/sys_int.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/system/int/src/sys_int.c  .generated_files/flags/default/921c13edd498e61d2ac6cafc9569f41c8cd31ff7 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1302700025" 
	@${RM} ${OBJECTDIR}/_ext/1302700025/sys_int.o.d 
	@${RM} ${OBJECTDIR}/_ext/1302700025/sys_int.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/1302700025/sys_int.o.d" -o ${OBJECTDIR}/_ext/1302700025/sys_int.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/system/int/src/sys_int.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/33059905/sys_time.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/system/time/src/sys_time.c  .generated_files/flags/default/5814ae3cce1d45b09a8be9470685ad5bc5623061 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/33059905" 
	@${RM} ${OBJECTDIR}/_ext/33059905/sys_time.o.d 
	@${RM} ${OBJECTDIR}/_ext/33059905/sys_time.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/33059905/sys_time.o.d" -o ${OBJECTDIR}/_ext/33059905/sys_time.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/system/time/src/sys_time.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/181187660/tasks.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/tasks.c  .generated_files/flags/default/eb69ab2ccb0a1a61515d1775bc206974d3763d29 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/181187660" 
	@${RM} ${OBJECTDIR}/_ext/181187660/tasks.o.d 
	@${RM} ${OBJECTDIR}/_ext/181187660/tasks.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/181187660/tasks.o.d" -o ${OBJECTDIR}/_ext/181187660/tasks.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/tasks.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1123615171/pt_yield_flag.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/common/pt_yield_flag.c  .generated_files/flags/default/2cd2c8983dcfca6ae0221b0cb95b33dbfefaee3f .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1123615171" 
	@${RM} ${OBJECTDIR}/_ext/1123615171/pt_yield_flag.o.d 
	@${RM} ${OBJECTDIR}/_ext/1123615171/pt_yield_flag.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/1123615171/pt_yield_flag.o.d" -o ${OBJECTDIR}/_ext/1123615171/pt_yield_flag.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/common/pt_yield_flag.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/211292139/bsp.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/hal/bsp/bsp.c  .generated_files/flags/default/b6e1465b34119a5018ccd3a14f27e717dde77dd0 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/211292139" 
	@${RM} ${OBJECTDIR}/_ext/211292139/bsp.o.d 
	@${RM} ${OBJECTDIR}/_ext/211292139/bsp.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/211292139/bsp.o.d" -o ${OBJECTDIR}/_ext/211292139/bsp.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/hal/bsp/bsp.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1345492590/sd_spi_lowlevel.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/hal/sd_spi/sd_spi_lowlevel.c  .generated_files/flags/default/5c92bee80d855f955b33a8c667d66775af3adbb4 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1345492590" 
	@${RM} ${OBJECTDIR}/_ext/1345492590/sd_spi_lowlevel.o.d 
	@${RM} ${OBJECTDIR}/_ext/1345492590/sd_spi_lowlevel.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/1345492590/sd_spi_lowlevel.o.d" -o ${OBJECTDIR}/_ext/1345492590/sd_spi_lowlevel.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/hal/sd_spi/sd_spi_lowlevel.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/503574270/spi_bus_guard.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/hal/spi_guard/spi_bus_guard.c  .generated_files/flags/default/12eaee1dfdbb77856f81c4a830d71ccfd6dc4a8a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/503574270" 
	@${RM} ${OBJECTDIR}/_ext/503574270/spi_bus_guard.o.d 
	@${RM} ${OBJECTDIR}/_ext/503574270/spi_bus_guard.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/503574270/spi_bus_guard.o.d" -o ${OBJECTDIR}/_ext/503574270/spi_bus_guard.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/hal/spi_guard/spi_bus_guard.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1534143173/esp32.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/drivers/esp32/esp32.c  .generated_files/flags/default/10686887e163a7dae7367e173f47be1f7285a6ad .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1534143173" 
	@${RM} ${OBJECTDIR}/_ext/1534143173/esp32.o.d 
	@${RM} ${OBJECTDIR}/_ext/1534143173/esp32.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/1534143173/esp32.o.d" -o ${OBJECTDIR}/_ext/1534143173/esp32.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/drivers/esp32/esp32.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1534143173/esp32_comm.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/drivers/esp32/esp32_comm.c  .generated_files/flags/default/dee79321dda695dfac3e0034ac446745849d6cce .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1534143173" 
	@${RM} ${OBJECTDIR}/_ext/1534143173/esp32_comm.o.d 
	@${RM} ${OBJECTDIR}/_ext/1534143173/esp32_comm.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/1534143173/esp32_comm.o.d" -o ${OBJECTDIR}/_ext/1534143173/esp32_comm.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/drivers/esp32/esp32_comm.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1618932211/w5500if.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/drivers/ethernet/w5500if.c  .generated_files/flags/default/75adf15c3b7ac965ec705941d15b6b526e06c3d2 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1618932211" 
	@${RM} ${OBJECTDIR}/_ext/1618932211/w5500if.o.d 
	@${RM} ${OBJECTDIR}/_ext/1618932211/w5500if.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/1618932211/w5500if.o.d" -o ${OBJECTDIR}/_ext/1618932211/w5500if.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/drivers/ethernet/w5500if.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1534845780/flash_w25q32.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/drivers/flash/flash_w25q32.c  .generated_files/flags/default/fa5330a28bddeb628a9e756ac520e3faeb6a0b3d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1534845780" 
	@${RM} ${OBJECTDIR}/_ext/1534845780/flash_w25q32.o.d 
	@${RM} ${OBJECTDIR}/_ext/1534845780/flash_w25q32.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/1534845780/flash_w25q32.o.d" -o ${OBJECTDIR}/_ext/1534845780/flash_w25q32.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/drivers/flash/flash_w25q32.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1541402254/modem_at.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/drivers/modem/modem_at.c  .generated_files/flags/default/c0bf5aae821ece200964b4bc3718aaadda1b0781 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1541402254" 
	@${RM} ${OBJECTDIR}/_ext/1541402254/modem_at.o.d 
	@${RM} ${OBJECTDIR}/_ext/1541402254/modem_at.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/1541402254/modem_at.o.d" -o ${OBJECTDIR}/_ext/1541402254/modem_at.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/drivers/modem/modem_at.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1541402254/modem_uart.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/drivers/modem/modem_uart.c  .generated_files/flags/default/36365df4b1b77bc8b89f258f116f9ec4468ca907 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1541402254" 
	@${RM} ${OBJECTDIR}/_ext/1541402254/modem_uart.o.d 
	@${RM} ${OBJECTDIR}/_ext/1541402254/modem_uart.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/1541402254/modem_uart.o.d" -o ${OBJECTDIR}/_ext/1541402254/modem_uart.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/drivers/modem/modem_uart.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/341013869/sd_raw.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/drivers/sd/sd_raw.c  .generated_files/flags/default/9661d036294751e1a67dd001ae4b66fc7b0f05e4 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/341013869" 
	@${RM} ${OBJECTDIR}/_ext/341013869/sd_raw.o.d 
	@${RM} ${OBJECTDIR}/_ext/341013869/sd_raw.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/341013869/sd_raw.o.d" -o ${OBJECTDIR}/_ext/341013869/sd_raw.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/drivers/sd/sd_raw.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1547576810/telit.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/drivers/telit/telit.c  .generated_files/flags/default/47d274c4250dba8e94a59892611ccce07c5b7147 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1547576810" 
	@${RM} ${OBJECTDIR}/_ext/1547576810/telit.o.d 
	@${RM} ${OBJECTDIR}/_ext/1547576810/telit.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/1547576810/telit.o.d" -o ${OBJECTDIR}/_ext/1547576810/telit.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/drivers/telit/telit.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1709141704/alarm_mgr.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/services/alarm/alarm_mgr.c  .generated_files/flags/default/d228f421a0e9668002cb1021d594dec4e19ad79c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1709141704" 
	@${RM} ${OBJECTDIR}/_ext/1709141704/alarm_mgr.o.d 
	@${RM} ${OBJECTDIR}/_ext/1709141704/alarm_mgr.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/1709141704/alarm_mgr.o.d" -o ${OBJECTDIR}/_ext/1709141704/alarm_mgr.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/services/alarm/alarm_mgr.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/346465585/esp32_proto.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/services/esp32_proto/esp32_proto.c  .generated_files/flags/default/86a05f9cd11184080a37cb07b42d09e823d62f8 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/346465585" 
	@${RM} ${OBJECTDIR}/_ext/346465585/esp32_proto.o.d 
	@${RM} ${OBJECTDIR}/_ext/346465585/esp32_proto.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/346465585/esp32_proto.o.d" -o ${OBJECTDIR}/_ext/346465585/esp32_proto.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/services/esp32_proto/esp32_proto.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/963589544/gsm_le910.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/services/gsm/gsm_le910.c  .generated_files/flags/default/c16e7909ef83066aa11b27289eb767c6869c6af1 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/963589544" 
	@${RM} ${OBJECTDIR}/_ext/963589544/gsm_le910.o.d 
	@${RM} ${OBJECTDIR}/_ext/963589544/gsm_le910.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/963589544/gsm_le910.o.d" -o ${OBJECTDIR}/_ext/963589544/gsm_le910.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/services/gsm/gsm_le910.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/963589544/gsm_service.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/services/gsm/gsm_service.c  .generated_files/flags/default/5370de367760be1f130f223663b904b3989853b7 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/963589544" 
	@${RM} ${OBJECTDIR}/_ext/963589544/gsm_service.o.d 
	@${RM} ${OBJECTDIR}/_ext/963589544/gsm_service.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/963589544/gsm_service.o.d" -o ${OBJECTDIR}/_ext/963589544/gsm_service.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/services/gsm/gsm_service.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1898101824/sd_service.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/services/sd_service/sd_service.c  .generated_files/flags/default/4e9eb35a35f4cfbf9648421864b2e2d774e7ec1e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1898101824" 
	@${RM} ${OBJECTDIR}/_ext/1898101824/sd_service.o.d 
	@${RM} ${OBJECTDIR}/_ext/1898101824/sd_service.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/1898101824/sd_service.o.d" -o ${OBJECTDIR}/_ext/1898101824/sd_service.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/services/sd_service/sd_service.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/963600896/sms_service.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/services/sms/sms_service.c  .generated_files/flags/default/a289648ab81ccae6870aa017d94a199f90e04ce4 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/963600896" 
	@${RM} ${OBJECTDIR}/_ext/963600896/sms_service.o.d 
	@${RM} ${OBJECTDIR}/_ext/963600896/sms_service.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/963600896/sms_service.o.d" -o ${OBJECTDIR}/_ext/963600896/sms_service.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/services/sms/sms_service.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1524448162/phonebook_flash.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/services/storage/phonebook_flash.c  .generated_files/flags/default/80f7e379fad2f532601b0199057ca21adcbab561 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1524448162" 
	@${RM} ${OBJECTDIR}/_ext/1524448162/phonebook_flash.o.d 
	@${RM} ${OBJECTDIR}/_ext/1524448162/phonebook_flash.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/1524448162/phonebook_flash.o.d" -o ${OBJECTDIR}/_ext/1524448162/phonebook_flash.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/services/storage/phonebook_flash.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1524448162/storage.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/services/storage/storage.c  .generated_files/flags/default/68c3edc8a07051c178d3aa5388d3b734a08c7de9 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1524448162" 
	@${RM} ${OBJECTDIR}/_ext/1524448162/storage.o.d 
	@${RM} ${OBJECTDIR}/_ext/1524448162/storage.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/1524448162/storage.o.d" -o ${OBJECTDIR}/_ext/1524448162/storage.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/services/storage/storage.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1524448162/store.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/services/storage/store.c  .generated_files/flags/default/f8c0aea95da593cf19b01d81c2ab45cb58cf5ba7 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1524448162" 
	@${RM} ${OBJECTDIR}/_ext/1524448162/store.o.d 
	@${RM} ${OBJECTDIR}/_ext/1524448162/store.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/1524448162/store.o.d" -o ${OBJECTDIR}/_ext/1524448162/store.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/services/storage/store.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1734184631/app.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/app/app.c  .generated_files/flags/default/69f3f8c4b03176c1f913fc590fd8c8b10c446ce0 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1734184631" 
	@${RM} ${OBJECTDIR}/_ext/1734184631/app.o.d 
	@${RM} ${OBJECTDIR}/_ext/1734184631/app.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/1734184631/app.o.d" -o ${OBJECTDIR}/_ext/1734184631/app.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/app/app.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1734184631/handlers.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/app/handlers.c  .generated_files/flags/default/5941ea89c019feb9f108bd2224070cdfce07f6dc .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1734184631" 
	@${RM} ${OBJECTDIR}/_ext/1734184631/handlers.o.d 
	@${RM} ${OBJECTDIR}/_ext/1734184631/handlers.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/1734184631/handlers.o.d" -o ${OBJECTDIR}/_ext/1734184631/handlers.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/app/handlers.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1734184631/layers_docments.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/app/layers_docments.c  .generated_files/flags/default/5151366b6ce9634239b70e10b03ea80ed793c392 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1734184631" 
	@${RM} ${OBJECTDIR}/_ext/1734184631/layers_docments.o.d 
	@${RM} ${OBJECTDIR}/_ext/1734184631/layers_docments.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/1734184631/layers_docments.o.d" -o ${OBJECTDIR}/_ext/1734184631/layers_docments.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/app/layers_docments.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/2075406954/sd_card.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/test/sd_card.c  .generated_files/flags/default/fdb4071e8dae5db56900a737ba9cb5a47495defc .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/2075406954" 
	@${RM} ${OBJECTDIR}/_ext/2075406954/sd_card.o.d 
	@${RM} ${OBJECTDIR}/_ext/2075406954/sd_card.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/2075406954/sd_card.o.d" -o ${OBJECTDIR}/_ext/2075406954/sd_card.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/test/sd_card.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/2075406954/sd_test.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/test/sd_test.c  .generated_files/flags/default/f98821ee7351747a6faf8215b44aa2bf3ba5df32 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/2075406954" 
	@${RM} ${OBJECTDIR}/_ext/2075406954/sd_test.o.d 
	@${RM} ${OBJECTDIR}/_ext/2075406954/sd_test.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/2075406954/sd_test.o.d" -o ${OBJECTDIR}/_ext/2075406954/sd_test.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/test/sd_test.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1718247445/io_monitor.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/hal/io_monitor/io_monitor.c  .generated_files/flags/default/9e4b767ded42b45d21b281f9631dae0042b6edb6 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1718247445" 
	@${RM} ${OBJECTDIR}/_ext/1718247445/io_monitor.o.d 
	@${RM} ${OBJECTDIR}/_ext/1718247445/io_monitor.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/1718247445/io_monitor.o.d" -o ${OBJECTDIR}/_ext/1718247445/io_monitor.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/hal/io_monitor/io_monitor.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
else
${OBJECTDIR}/_ext/1360937237/main.o: ../src/main.c  .generated_files/flags/default/bb337a5bca0645d4fbd5406b612a6b4aece7cc69 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/main.o.d" -o ${OBJECTDIR}/_ext/1360937237/main.o ../src/main.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/protothreads.o: protothreads.c  .generated_files/flags/default/4a1be809e014fcc997b9f45df422d0b1390a0a51 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/protothreads.o.d 
	@${RM} ${OBJECTDIR}/protothreads.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/protothreads.o.d" -o ${OBJECTDIR}/protothreads.o protothreads.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1060863540/drv_sdspi.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/driver/sdspi/src/drv_sdspi.c  .generated_files/flags/default/11d30ba13f15cca9731f9dc8191f6a5be9b93cbf .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1060863540" 
	@${RM} ${OBJECTDIR}/_ext/1060863540/drv_sdspi.o.d 
	@${RM} ${OBJECTDIR}/_ext/1060863540/drv_sdspi.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/1060863540/drv_sdspi.o.d" -o ${OBJECTDIR}/_ext/1060863540/drv_sdspi.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/driver/sdspi/src/drv_sdspi.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1060863540/drv_sdspi_file_system.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/driver/sdspi/src/drv_sdspi_file_system.c  .generated_files/flags/default/2534fa55f497cfce2f98c278d4c3ab71a8af6a89 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1060863540" 
	@${RM} ${OBJECTDIR}/_ext/1060863540/drv_sdspi_file_system.o.d 
	@${RM} ${OBJECTDIR}/_ext/1060863540/drv_sdspi_file_system.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/1060863540/drv_sdspi_file_system.o.d" -o ${OBJECTDIR}/_ext/1060863540/drv_sdspi_file_system.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/driver/sdspi/src/drv_sdspi_file_system.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1060863540/drv_sdspi_plib_interface.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/driver/sdspi/src/drv_sdspi_plib_interface.c  .generated_files/flags/default/bbc3aa635b1efcf9bb505217e6a8bc38eba907a6 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1060863540" 
	@${RM} ${OBJECTDIR}/_ext/1060863540/drv_sdspi_plib_interface.o.d 
	@${RM} ${OBJECTDIR}/_ext/1060863540/drv_sdspi_plib_interface.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/1060863540/drv_sdspi_plib_interface.o.d" -o ${OBJECTDIR}/_ext/1060863540/drv_sdspi_plib_interface.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/driver/sdspi/src/drv_sdspi_plib_interface.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/181187660/exceptions.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/exceptions.c  .generated_files/flags/default/727cbc04b92138151caf74a61c0e6e77feef736a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/181187660" 
	@${RM} ${OBJECTDIR}/_ext/181187660/exceptions.o.d 
	@${RM} ${OBJECTDIR}/_ext/181187660/exceptions.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/181187660/exceptions.o.d" -o ${OBJECTDIR}/_ext/181187660/exceptions.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/exceptions.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/181187660/initialization.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/initialization.c  .generated_files/flags/default/fb1ec9cc42e8b715ea87beba3129f90e1ac07f6f .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/181187660" 
	@${RM} ${OBJECTDIR}/_ext/181187660/initialization.o.d 
	@${RM} ${OBJECTDIR}/_ext/181187660/initialization.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/181187660/initialization.o.d" -o ${OBJECTDIR}/_ext/181187660/initialization.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/initialization.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/181187660/interrupts.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/interrupts.c  .generated_files/flags/default/3e35ac28e09848ff4c89ffeba826cedf71574a08 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/181187660" 
	@${RM} ${OBJECTDIR}/_ext/181187660/interrupts.o.d 
	@${RM} ${OBJECTDIR}/_ext/181187660/interrupts.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/181187660/interrupts.o.d" -o ${OBJECTDIR}/_ext/181187660/interrupts.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/interrupts.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1170764338/plib_clk.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/peripheral/clk/plib_clk.c  .generated_files/flags/default/ff23710e34dadcdc4043c5481f143530c31205ef .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1170764338" 
	@${RM} ${OBJECTDIR}/_ext/1170764338/plib_clk.o.d 
	@${RM} ${OBJECTDIR}/_ext/1170764338/plib_clk.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/1170764338/plib_clk.o.d" -o ${OBJECTDIR}/_ext/1170764338/plib_clk.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/peripheral/clk/plib_clk.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/134162454/plib_coretimer.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/peripheral/coretimer/plib_coretimer.c  .generated_files/flags/default/628dd5f0b52cd7ef9a6e8894b28d11573a8592f0 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/134162454" 
	@${RM} ${OBJECTDIR}/_ext/134162454/plib_coretimer.o.d 
	@${RM} ${OBJECTDIR}/_ext/134162454/plib_coretimer.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/134162454/plib_coretimer.o.d" -o ${OBJECTDIR}/_ext/134162454/plib_coretimer.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/peripheral/coretimer/plib_coretimer.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1934025339/plib_evic.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/peripheral/evic/plib_evic.c  .generated_files/flags/default/164db193a33da029a5d7a0b6ccb2b778b405c38 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1934025339" 
	@${RM} ${OBJECTDIR}/_ext/1934025339/plib_evic.o.d 
	@${RM} ${OBJECTDIR}/_ext/1934025339/plib_evic.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/1934025339/plib_evic.o.d" -o ${OBJECTDIR}/_ext/1934025339/plib_evic.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/peripheral/evic/plib_evic.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1934079167/plib_gpio.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/peripheral/gpio/plib_gpio.c  .generated_files/flags/default/fc797c2b352ab50b0bebf4c8db405b78aacc72f3 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1934079167" 
	@${RM} ${OBJECTDIR}/_ext/1934079167/plib_gpio.o.d 
	@${RM} ${OBJECTDIR}/_ext/1934079167/plib_gpio.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/1934079167/plib_gpio.o.d" -o ${OBJECTDIR}/_ext/1934079167/plib_gpio.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/peripheral/gpio/plib_gpio.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/525845896/plib_spi1_master.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/peripheral/spi/spi_master/plib_spi1_master.c  .generated_files/flags/default/abce67870f4843db53f25f3998c47d9a55db161a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/525845896" 
	@${RM} ${OBJECTDIR}/_ext/525845896/plib_spi1_master.o.d 
	@${RM} ${OBJECTDIR}/_ext/525845896/plib_spi1_master.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/525845896/plib_spi1_master.o.d" -o ${OBJECTDIR}/_ext/525845896/plib_spi1_master.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/peripheral/spi/spi_master/plib_spi1_master.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/525845896/plib_spi3_master.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/peripheral/spi/spi_master/plib_spi3_master.c  .generated_files/flags/default/196ac5f65683b3af7b3085f5dd09aa3a3ba0ef53 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/525845896" 
	@${RM} ${OBJECTDIR}/_ext/525845896/plib_spi3_master.o.d 
	@${RM} ${OBJECTDIR}/_ext/525845896/plib_spi3_master.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/525845896/plib_spi3_master.o.d" -o ${OBJECTDIR}/_ext/525845896/plib_spi3_master.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/peripheral/spi/spi_master/plib_spi3_master.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1934463784/plib_tmr1.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/peripheral/tmr1/plib_tmr1.c  .generated_files/flags/default/2e586aa352d4252e23b4757391e9275da5ca5d6 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1934463784" 
	@${RM} ${OBJECTDIR}/_ext/1934463784/plib_tmr1.o.d 
	@${RM} ${OBJECTDIR}/_ext/1934463784/plib_tmr1.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/1934463784/plib_tmr1.o.d" -o ${OBJECTDIR}/_ext/1934463784/plib_tmr1.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/peripheral/tmr1/plib_tmr1.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1934463785/plib_tmr2.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/peripheral/tmr2/plib_tmr2.c  .generated_files/flags/default/119be5fb11b0ed18448f99f5b7bccbc69e23c902 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1934463785" 
	@${RM} ${OBJECTDIR}/_ext/1934463785/plib_tmr2.o.d 
	@${RM} ${OBJECTDIR}/_ext/1934463785/plib_tmr2.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/1934463785/plib_tmr2.o.d" -o ${OBJECTDIR}/_ext/1934463785/plib_tmr2.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/peripheral/tmr2/plib_tmr2.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1934482110/plib_uart1.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/peripheral/uart/plib_uart1.c  .generated_files/flags/default/aa3ff51dd980fcaf1372b9da49edd018388267eb .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1934482110" 
	@${RM} ${OBJECTDIR}/_ext/1934482110/plib_uart1.o.d 
	@${RM} ${OBJECTDIR}/_ext/1934482110/plib_uart1.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/1934482110/plib_uart1.o.d" -o ${OBJECTDIR}/_ext/1934482110/plib_uart1.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/peripheral/uart/plib_uart1.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1934482110/plib_uart2.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/peripheral/uart/plib_uart2.c  .generated_files/flags/default/a035e4007eb1fe4cdeb0b112e4c1333260ed39c0 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1934482110" 
	@${RM} ${OBJECTDIR}/_ext/1934482110/plib_uart2.o.d 
	@${RM} ${OBJECTDIR}/_ext/1934482110/plib_uart2.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/1934482110/plib_uart2.o.d" -o ${OBJECTDIR}/_ext/1934482110/plib_uart2.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/peripheral/uart/plib_uart2.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1934482110/plib_uart3.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/peripheral/uart/plib_uart3.c  .generated_files/flags/default/b03f109dc40530e5f5a16ebc3b5234446b02cc72 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1934482110" 
	@${RM} ${OBJECTDIR}/_ext/1934482110/plib_uart3.o.d 
	@${RM} ${OBJECTDIR}/_ext/1934482110/plib_uart3.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/1934482110/plib_uart3.o.d" -o ${OBJECTDIR}/_ext/1934482110/plib_uart3.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/peripheral/uart/plib_uart3.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1366434042/xc32_monitor.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/stdio/xc32_monitor.c  .generated_files/flags/default/452c7c2577644764829788a0073d40fe36f43054 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1366434042" 
	@${RM} ${OBJECTDIR}/_ext/1366434042/xc32_monitor.o.d 
	@${RM} ${OBJECTDIR}/_ext/1366434042/xc32_monitor.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/1366434042/xc32_monitor.o.d" -o ${OBJECTDIR}/_ext/1366434042/xc32_monitor.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/stdio/xc32_monitor.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/720722619/sys_dma.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/system/dma/sys_dma.c  .generated_files/flags/default/a4f5431a8088e75def60dc860b800101ace4f540 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/720722619" 
	@${RM} ${OBJECTDIR}/_ext/720722619/sys_dma.o.d 
	@${RM} ${OBJECTDIR}/_ext/720722619/sys_dma.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/720722619/sys_dma.o.d" -o ${OBJECTDIR}/_ext/720722619/sys_dma.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/system/dma/sys_dma.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/764968315/ff.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/system/fs/fat_fs/file_system/ff.c  .generated_files/flags/default/af861d1eb1cc8b284e0a52364ee43eddde472055 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/764968315" 
	@${RM} ${OBJECTDIR}/_ext/764968315/ff.o.d 
	@${RM} ${OBJECTDIR}/_ext/764968315/ff.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/764968315/ff.o.d" -o ${OBJECTDIR}/_ext/764968315/ff.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/system/fs/fat_fs/file_system/ff.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/764968315/ffunicode.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/system/fs/fat_fs/file_system/ffunicode.c  .generated_files/flags/default/56050b376f0b5cb2228ebb0b3ccc48cc87d019ea .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/764968315" 
	@${RM} ${OBJECTDIR}/_ext/764968315/ffunicode.o.d 
	@${RM} ${OBJECTDIR}/_ext/764968315/ffunicode.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/764968315/ffunicode.o.d" -o ${OBJECTDIR}/_ext/764968315/ffunicode.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/system/fs/fat_fs/file_system/ffunicode.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1812136516/diskio.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/system/fs/fat_fs/hardware_access/diskio.c  .generated_files/flags/default/7d154b7ff6d0955a3e638fe51481deaa6373c00a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1812136516" 
	@${RM} ${OBJECTDIR}/_ext/1812136516/diskio.o.d 
	@${RM} ${OBJECTDIR}/_ext/1812136516/diskio.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/1812136516/diskio.o.d" -o ${OBJECTDIR}/_ext/1812136516/diskio.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/system/fs/fat_fs/hardware_access/diskio.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/567453951/sys_fs.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/system/fs/src/sys_fs.c  .generated_files/flags/default/fe64f1b173b1db40d42fc5ee320546c1d0e7c994 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/567453951" 
	@${RM} ${OBJECTDIR}/_ext/567453951/sys_fs.o.d 
	@${RM} ${OBJECTDIR}/_ext/567453951/sys_fs.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/567453951/sys_fs.o.d" -o ${OBJECTDIR}/_ext/567453951/sys_fs.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/system/fs/src/sys_fs.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/567453951/sys_fs_fat_interface.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/system/fs/src/sys_fs_fat_interface.c  .generated_files/flags/default/45a88804fea664fde43c84e44917937474c03391 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/567453951" 
	@${RM} ${OBJECTDIR}/_ext/567453951/sys_fs_fat_interface.o.d 
	@${RM} ${OBJECTDIR}/_ext/567453951/sys_fs_fat_interface.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/567453951/sys_fs_fat_interface.o.d" -o ${OBJECTDIR}/_ext/567453951/sys_fs_fat_interface.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/system/fs/src/sys_fs_fat_interface.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/567453951/sys_fs_media_manager.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/system/fs/src/sys_fs_media_manager.c  .generated_files/flags/default/f55c2bf579c9fdf0629c42ece8e6d35724072c62 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/567453951" 
	@${RM} ${OBJECTDIR}/_ext/567453951/sys_fs_media_manager.o.d 
	@${RM} ${OBJECTDIR}/_ext/567453951/sys_fs_media_manager.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/567453951/sys_fs_media_manager.o.d" -o ${OBJECTDIR}/_ext/567453951/sys_fs_media_manager.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/system/fs/src/sys_fs_media_manager.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1302700025/sys_int.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/system/int/src/sys_int.c  .generated_files/flags/default/9f4d1d0e01360b0a6a1f95a16810bf100699c3a7 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1302700025" 
	@${RM} ${OBJECTDIR}/_ext/1302700025/sys_int.o.d 
	@${RM} ${OBJECTDIR}/_ext/1302700025/sys_int.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/1302700025/sys_int.o.d" -o ${OBJECTDIR}/_ext/1302700025/sys_int.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/system/int/src/sys_int.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/33059905/sys_time.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/system/time/src/sys_time.c  .generated_files/flags/default/12c05fab95c211e942f310b8f5581c18ff19fb4 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/33059905" 
	@${RM} ${OBJECTDIR}/_ext/33059905/sys_time.o.d 
	@${RM} ${OBJECTDIR}/_ext/33059905/sys_time.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/33059905/sys_time.o.d" -o ${OBJECTDIR}/_ext/33059905/sys_time.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/system/time/src/sys_time.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/181187660/tasks.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/tasks.c  .generated_files/flags/default/de0568c0c456e167e5e252d8512946bbfde4de09 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/181187660" 
	@${RM} ${OBJECTDIR}/_ext/181187660/tasks.o.d 
	@${RM} ${OBJECTDIR}/_ext/181187660/tasks.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/181187660/tasks.o.d" -o ${OBJECTDIR}/_ext/181187660/tasks.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/config/default/tasks.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1123615171/pt_yield_flag.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/common/pt_yield_flag.c  .generated_files/flags/default/c6064f4f0065e7bbd59e731e35cdafbc90592215 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1123615171" 
	@${RM} ${OBJECTDIR}/_ext/1123615171/pt_yield_flag.o.d 
	@${RM} ${OBJECTDIR}/_ext/1123615171/pt_yield_flag.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/1123615171/pt_yield_flag.o.d" -o ${OBJECTDIR}/_ext/1123615171/pt_yield_flag.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/common/pt_yield_flag.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/211292139/bsp.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/hal/bsp/bsp.c  .generated_files/flags/default/f8bc4a7f5067c64da5b0c2fedc19a1ac349fb229 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/211292139" 
	@${RM} ${OBJECTDIR}/_ext/211292139/bsp.o.d 
	@${RM} ${OBJECTDIR}/_ext/211292139/bsp.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/211292139/bsp.o.d" -o ${OBJECTDIR}/_ext/211292139/bsp.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/hal/bsp/bsp.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1345492590/sd_spi_lowlevel.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/hal/sd_spi/sd_spi_lowlevel.c  .generated_files/flags/default/59b2c27d2128843a312482971ff0868c83dd1af9 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1345492590" 
	@${RM} ${OBJECTDIR}/_ext/1345492590/sd_spi_lowlevel.o.d 
	@${RM} ${OBJECTDIR}/_ext/1345492590/sd_spi_lowlevel.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/1345492590/sd_spi_lowlevel.o.d" -o ${OBJECTDIR}/_ext/1345492590/sd_spi_lowlevel.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/hal/sd_spi/sd_spi_lowlevel.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/503574270/spi_bus_guard.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/hal/spi_guard/spi_bus_guard.c  .generated_files/flags/default/3552c4c28aeed2bbbad023b8cc5a9fa6ac3aeb33 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/503574270" 
	@${RM} ${OBJECTDIR}/_ext/503574270/spi_bus_guard.o.d 
	@${RM} ${OBJECTDIR}/_ext/503574270/spi_bus_guard.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/503574270/spi_bus_guard.o.d" -o ${OBJECTDIR}/_ext/503574270/spi_bus_guard.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/hal/spi_guard/spi_bus_guard.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1534143173/esp32.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/drivers/esp32/esp32.c  .generated_files/flags/default/ee5baf950f60adfeb39e6e93da88aadea102763c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1534143173" 
	@${RM} ${OBJECTDIR}/_ext/1534143173/esp32.o.d 
	@${RM} ${OBJECTDIR}/_ext/1534143173/esp32.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/1534143173/esp32.o.d" -o ${OBJECTDIR}/_ext/1534143173/esp32.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/drivers/esp32/esp32.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1534143173/esp32_comm.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/drivers/esp32/esp32_comm.c  .generated_files/flags/default/63594cb487d9cc3eeb48f08191872811c5be71fa .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1534143173" 
	@${RM} ${OBJECTDIR}/_ext/1534143173/esp32_comm.o.d 
	@${RM} ${OBJECTDIR}/_ext/1534143173/esp32_comm.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/1534143173/esp32_comm.o.d" -o ${OBJECTDIR}/_ext/1534143173/esp32_comm.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/drivers/esp32/esp32_comm.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1618932211/w5500if.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/drivers/ethernet/w5500if.c  .generated_files/flags/default/42a8ee96818b94c5d2a237a046171cada0782619 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1618932211" 
	@${RM} ${OBJECTDIR}/_ext/1618932211/w5500if.o.d 
	@${RM} ${OBJECTDIR}/_ext/1618932211/w5500if.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/1618932211/w5500if.o.d" -o ${OBJECTDIR}/_ext/1618932211/w5500if.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/drivers/ethernet/w5500if.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1534845780/flash_w25q32.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/drivers/flash/flash_w25q32.c  .generated_files/flags/default/75f3f104b32d938afb241012eb85ffa0145380f9 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1534845780" 
	@${RM} ${OBJECTDIR}/_ext/1534845780/flash_w25q32.o.d 
	@${RM} ${OBJECTDIR}/_ext/1534845780/flash_w25q32.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/1534845780/flash_w25q32.o.d" -o ${OBJECTDIR}/_ext/1534845780/flash_w25q32.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/drivers/flash/flash_w25q32.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1541402254/modem_at.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/drivers/modem/modem_at.c  .generated_files/flags/default/86c4a12abd9b41e27a38cc4d9205d7be565ffa1d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1541402254" 
	@${RM} ${OBJECTDIR}/_ext/1541402254/modem_at.o.d 
	@${RM} ${OBJECTDIR}/_ext/1541402254/modem_at.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/1541402254/modem_at.o.d" -o ${OBJECTDIR}/_ext/1541402254/modem_at.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/drivers/modem/modem_at.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1541402254/modem_uart.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/drivers/modem/modem_uart.c  .generated_files/flags/default/3b26f19da5c95269036f3a2c4522e720e2364132 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1541402254" 
	@${RM} ${OBJECTDIR}/_ext/1541402254/modem_uart.o.d 
	@${RM} ${OBJECTDIR}/_ext/1541402254/modem_uart.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/1541402254/modem_uart.o.d" -o ${OBJECTDIR}/_ext/1541402254/modem_uart.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/drivers/modem/modem_uart.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/341013869/sd_raw.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/drivers/sd/sd_raw.c  .generated_files/flags/default/7ae87db8dd4ee6af1c93f9adbcc8ade0324754b9 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/341013869" 
	@${RM} ${OBJECTDIR}/_ext/341013869/sd_raw.o.d 
	@${RM} ${OBJECTDIR}/_ext/341013869/sd_raw.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/341013869/sd_raw.o.d" -o ${OBJECTDIR}/_ext/341013869/sd_raw.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/drivers/sd/sd_raw.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1547576810/telit.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/drivers/telit/telit.c  .generated_files/flags/default/e138729b6cda5ebae023c41499c7104d895900e7 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1547576810" 
	@${RM} ${OBJECTDIR}/_ext/1547576810/telit.o.d 
	@${RM} ${OBJECTDIR}/_ext/1547576810/telit.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/1547576810/telit.o.d" -o ${OBJECTDIR}/_ext/1547576810/telit.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/drivers/telit/telit.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1709141704/alarm_mgr.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/services/alarm/alarm_mgr.c  .generated_files/flags/default/ac6c66e3651b83c1a4bfb9755e3da3f2ae8ea390 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1709141704" 
	@${RM} ${OBJECTDIR}/_ext/1709141704/alarm_mgr.o.d 
	@${RM} ${OBJECTDIR}/_ext/1709141704/alarm_mgr.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/1709141704/alarm_mgr.o.d" -o ${OBJECTDIR}/_ext/1709141704/alarm_mgr.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/services/alarm/alarm_mgr.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/346465585/esp32_proto.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/services/esp32_proto/esp32_proto.c  .generated_files/flags/default/2e494f4244c00e306b0f4a4970610314f112d977 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/346465585" 
	@${RM} ${OBJECTDIR}/_ext/346465585/esp32_proto.o.d 
	@${RM} ${OBJECTDIR}/_ext/346465585/esp32_proto.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/346465585/esp32_proto.o.d" -o ${OBJECTDIR}/_ext/346465585/esp32_proto.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/services/esp32_proto/esp32_proto.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/963589544/gsm_le910.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/services/gsm/gsm_le910.c  .generated_files/flags/default/fd1c05931bf1f417970bff35951731c4c47e0458 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/963589544" 
	@${RM} ${OBJECTDIR}/_ext/963589544/gsm_le910.o.d 
	@${RM} ${OBJECTDIR}/_ext/963589544/gsm_le910.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/963589544/gsm_le910.o.d" -o ${OBJECTDIR}/_ext/963589544/gsm_le910.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/services/gsm/gsm_le910.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/963589544/gsm_service.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/services/gsm/gsm_service.c  .generated_files/flags/default/b9d4d42875d38318818d541ddb5947b8c7e25370 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/963589544" 
	@${RM} ${OBJECTDIR}/_ext/963589544/gsm_service.o.d 
	@${RM} ${OBJECTDIR}/_ext/963589544/gsm_service.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/963589544/gsm_service.o.d" -o ${OBJECTDIR}/_ext/963589544/gsm_service.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/services/gsm/gsm_service.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1898101824/sd_service.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/services/sd_service/sd_service.c  .generated_files/flags/default/a0110f6ef122ddb1e1bfe1afa9f5f47becf73a54 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1898101824" 
	@${RM} ${OBJECTDIR}/_ext/1898101824/sd_service.o.d 
	@${RM} ${OBJECTDIR}/_ext/1898101824/sd_service.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/1898101824/sd_service.o.d" -o ${OBJECTDIR}/_ext/1898101824/sd_service.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/services/sd_service/sd_service.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/963600896/sms_service.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/services/sms/sms_service.c  .generated_files/flags/default/53c6154a4ceea53094bb98f4bd4fc58f699b2bb7 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/963600896" 
	@${RM} ${OBJECTDIR}/_ext/963600896/sms_service.o.d 
	@${RM} ${OBJECTDIR}/_ext/963600896/sms_service.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/963600896/sms_service.o.d" -o ${OBJECTDIR}/_ext/963600896/sms_service.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/services/sms/sms_service.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1524448162/phonebook_flash.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/services/storage/phonebook_flash.c  .generated_files/flags/default/aeaf2b6910e6df7b58a5e0e0cfe2c2f46c5d2c9e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1524448162" 
	@${RM} ${OBJECTDIR}/_ext/1524448162/phonebook_flash.o.d 
	@${RM} ${OBJECTDIR}/_ext/1524448162/phonebook_flash.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/1524448162/phonebook_flash.o.d" -o ${OBJECTDIR}/_ext/1524448162/phonebook_flash.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/services/storage/phonebook_flash.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1524448162/storage.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/services/storage/storage.c  .generated_files/flags/default/9bd899f52d9655bdebaf591ed25dba09e13f6639 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1524448162" 
	@${RM} ${OBJECTDIR}/_ext/1524448162/storage.o.d 
	@${RM} ${OBJECTDIR}/_ext/1524448162/storage.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/1524448162/storage.o.d" -o ${OBJECTDIR}/_ext/1524448162/storage.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/services/storage/storage.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1524448162/store.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/services/storage/store.c  .generated_files/flags/default/ad6643ea13dd9722ae3d58c7b7de360e7c47b392 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1524448162" 
	@${RM} ${OBJECTDIR}/_ext/1524448162/store.o.d 
	@${RM} ${OBJECTDIR}/_ext/1524448162/store.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/1524448162/store.o.d" -o ${OBJECTDIR}/_ext/1524448162/store.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/services/storage/store.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1734184631/app.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/app/app.c  .generated_files/flags/default/67479ace2d2823115a873507b7cca77512cbf67e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1734184631" 
	@${RM} ${OBJECTDIR}/_ext/1734184631/app.o.d 
	@${RM} ${OBJECTDIR}/_ext/1734184631/app.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/1734184631/app.o.d" -o ${OBJECTDIR}/_ext/1734184631/app.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/app/app.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1734184631/handlers.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/app/handlers.c  .generated_files/flags/default/d68402bfc30dc6f67ddc507097fe635bd6171567 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1734184631" 
	@${RM} ${OBJECTDIR}/_ext/1734184631/handlers.o.d 
	@${RM} ${OBJECTDIR}/_ext/1734184631/handlers.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/1734184631/handlers.o.d" -o ${OBJECTDIR}/_ext/1734184631/handlers.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/app/handlers.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1734184631/layers_docments.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/app/layers_docments.c  .generated_files/flags/default/8a97b7e1c09542a855f4377ffae0a947ecf18aec .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1734184631" 
	@${RM} ${OBJECTDIR}/_ext/1734184631/layers_docments.o.d 
	@${RM} ${OBJECTDIR}/_ext/1734184631/layers_docments.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/1734184631/layers_docments.o.d" -o ${OBJECTDIR}/_ext/1734184631/layers_docments.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/app/layers_docments.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/2075406954/sd_card.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/test/sd_card.c  .generated_files/flags/default/ee88fbf0b00d79c77f776b31778e3c7dfb7cc9f2 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/2075406954" 
	@${RM} ${OBJECTDIR}/_ext/2075406954/sd_card.o.d 
	@${RM} ${OBJECTDIR}/_ext/2075406954/sd_card.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/2075406954/sd_card.o.d" -o ${OBJECTDIR}/_ext/2075406954/sd_card.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/test/sd_card.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/2075406954/sd_test.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/test/sd_test.c  .generated_files/flags/default/5c4ce45d2e29b06bf4e39b846126fc330092ccfd .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/2075406954" 
	@${RM} ${OBJECTDIR}/_ext/2075406954/sd_test.o.d 
	@${RM} ${OBJECTDIR}/_ext/2075406954/sd_test.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/2075406954/sd_test.o.d" -o ${OBJECTDIR}/_ext/2075406954/sd_test.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/test/sd_test.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1718247445/io_monitor.o: C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/hal/io_monitor/io_monitor.c  .generated_files/flags/default/e91855b4527806fb3de9e3a3ff9d323262638fdc .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1718247445" 
	@${RM} ${OBJECTDIR}/_ext/1718247445/io_monitor.o.d 
	@${RM} ${OBJECTDIR}/_ext/1718247445/io_monitor.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -MP -MMD -MF "${OBJECTDIR}/_ext/1718247445/io_monitor.o.d" -o ${OBJECTDIR}/_ext/1718247445/io_monitor.o C:/Users/ahmed.thabit/Desktop/Minesh/ArchitectureFolder/src/hal/io_monitor/io_monitor.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/Architecture.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    ../src/config/default/p32MM0256GPM064.ld
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -g   -mprocessor=$(MP_PROCESSOR_OPTION)  -o ${DISTDIR}/Architecture.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)   -mreserve=data@0x0:0x1FC -mreserve=boot@0x1FC00490:0x1FC016FF -mreserve=boot@0x1FC00490:0x1FC00BEF  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D=__DEBUG_D,--defsym=_min_heap_size=512,--gc-sections,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,${DISTDIR}/memoryfile.xml -mdfp="${DFP_DIR}"
	
else
${DISTDIR}/Architecture.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   ../src/config/default/p32MM0256GPM064.ld
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o ${DISTDIR}/Architecture.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=_min_heap_size=512,--gc-sections,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,${DISTDIR}/memoryfile.xml -mdfp="${DFP_DIR}"
	${MP_CC_DIR}\\xc32-bin2hex ${DISTDIR}/Architecture.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${OBJECTDIR}
	${RM} -r ${DISTDIR}

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(wildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
