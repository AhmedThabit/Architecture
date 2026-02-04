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
SOURCEFILES_QUOTED_IF_SPACED=../src/layers_docments.c ../src/config/default/peripheral/clk/plib_clk.c ../src/config/default/peripheral/evic/plib_evic.c ../src/config/default/peripheral/gpio/plib_gpio.c ../src/config/default/peripheral/spi/spi_master/plib_spi1_master.c ../src/config/default/peripheral/spi/spi_master/plib_spi3_master.c ../src/config/default/peripheral/tmr1/plib_tmr1.c ../src/config/default/peripheral/uart/plib_uart3.c ../src/config/default/peripheral/uart/plib_uart1.c ../src/config/default/peripheral/uart/plib_uart2.c ../src/config/default/stdio/xc32_monitor.c ../src/config/default/initialization.c ../src/config/default/interrupts.c ../src/config/default/exceptions.c ../src/store.c ../src/esp32_comm.c ../src/esp32_proto.c ../src/io_monitor.c ../src/alarm_mgr.c ../src/gsm_le910.c ../src/sms_service.c ../src/flash_w25q32.c ../src/storage.c ../src/phonebook_flash.c ../src/telit.c ../src/bsp.c ../src/esp32.c ../src/w5500if.c ../src/main.c protothreads.c ../src/handlers.c ../src/sd_card_thread.c ../src/tasks.c ../src/config/default/driver/sdspi/src/drv_sdspi.c ../src/config/default/driver/sdspi/src/drv_sdspi_file_system.c ../src/config/default/driver/sdspi/src/drv_sdspi_plib_interface.c ../src/config/default/system/dma/sys_dma.c ../src/config/default/system/fs/fat_fs/file_system/ff.c ../src/config/default/system/fs/fat_fs/file_system/ffunicode.c ../src/config/default/system/fs/fat_fs/hardware_access/diskio.c ../src/config/default/system/fs/src/sys_fs.c ../src/config/default/system/fs/src/sys_fs_fat_interface.c ../src/config/default/system/fs/src/sys_fs_media_manager.c ../src/config/default/system/int/src/sys_int.c ../src/config/default/system/time/src/sys_time.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1360937237/layers_docments.o ${OBJECTDIR}/_ext/60165520/plib_clk.o ${OBJECTDIR}/_ext/1865200349/plib_evic.o ${OBJECTDIR}/_ext/1865254177/plib_gpio.o ${OBJECTDIR}/_ext/298189674/plib_spi1_master.o ${OBJECTDIR}/_ext/298189674/plib_spi3_master.o ${OBJECTDIR}/_ext/1865638794/plib_tmr1.o ${OBJECTDIR}/_ext/1865657120/plib_uart3.o ${OBJECTDIR}/_ext/1865657120/plib_uart1.o ${OBJECTDIR}/_ext/1865657120/plib_uart2.o ${OBJECTDIR}/_ext/163028504/xc32_monitor.o ${OBJECTDIR}/_ext/1171490990/initialization.o ${OBJECTDIR}/_ext/1171490990/interrupts.o ${OBJECTDIR}/_ext/1171490990/exceptions.o ${OBJECTDIR}/_ext/1360937237/store.o ${OBJECTDIR}/_ext/1360937237/esp32_comm.o ${OBJECTDIR}/_ext/1360937237/esp32_proto.o ${OBJECTDIR}/_ext/1360937237/io_monitor.o ${OBJECTDIR}/_ext/1360937237/alarm_mgr.o ${OBJECTDIR}/_ext/1360937237/gsm_le910.o ${OBJECTDIR}/_ext/1360937237/sms_service.o ${OBJECTDIR}/_ext/1360937237/flash_w25q32.o ${OBJECTDIR}/_ext/1360937237/storage.o ${OBJECTDIR}/_ext/1360937237/phonebook_flash.o ${OBJECTDIR}/_ext/1360937237/telit.o ${OBJECTDIR}/_ext/1360937237/bsp.o ${OBJECTDIR}/_ext/1360937237/esp32.o ${OBJECTDIR}/_ext/1360937237/w5500if.o ${OBJECTDIR}/_ext/1360937237/main.o ${OBJECTDIR}/protothreads.o ${OBJECTDIR}/_ext/1360937237/handlers.o ${OBJECTDIR}/_ext/1360937237/sd_card_thread.o ${OBJECTDIR}/_ext/1360937237/tasks.o ${OBJECTDIR}/_ext/1100529066/drv_sdspi.o ${OBJECTDIR}/_ext/1100529066/drv_sdspi_file_system.o ${OBJECTDIR}/_ext/1100529066/drv_sdspi_plib_interface.o ${OBJECTDIR}/_ext/14461671/sys_dma.o ${OBJECTDIR}/_ext/411819097/ff.o ${OBJECTDIR}/_ext/411819097/ffunicode.o ${OBJECTDIR}/_ext/565198302/diskio.o ${OBJECTDIR}/_ext/1269487135/sys_fs.o ${OBJECTDIR}/_ext/1269487135/sys_fs_fat_interface.o ${OBJECTDIR}/_ext/1269487135/sys_fs_media_manager.o ${OBJECTDIR}/_ext/1881668453/sys_int.o ${OBJECTDIR}/_ext/101884895/sys_time.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1360937237/layers_docments.o.d ${OBJECTDIR}/_ext/60165520/plib_clk.o.d ${OBJECTDIR}/_ext/1865200349/plib_evic.o.d ${OBJECTDIR}/_ext/1865254177/plib_gpio.o.d ${OBJECTDIR}/_ext/298189674/plib_spi1_master.o.d ${OBJECTDIR}/_ext/298189674/plib_spi3_master.o.d ${OBJECTDIR}/_ext/1865638794/plib_tmr1.o.d ${OBJECTDIR}/_ext/1865657120/plib_uart3.o.d ${OBJECTDIR}/_ext/1865657120/plib_uart1.o.d ${OBJECTDIR}/_ext/1865657120/plib_uart2.o.d ${OBJECTDIR}/_ext/163028504/xc32_monitor.o.d ${OBJECTDIR}/_ext/1171490990/initialization.o.d ${OBJECTDIR}/_ext/1171490990/interrupts.o.d ${OBJECTDIR}/_ext/1171490990/exceptions.o.d ${OBJECTDIR}/_ext/1360937237/store.o.d ${OBJECTDIR}/_ext/1360937237/esp32_comm.o.d ${OBJECTDIR}/_ext/1360937237/esp32_proto.o.d ${OBJECTDIR}/_ext/1360937237/io_monitor.o.d ${OBJECTDIR}/_ext/1360937237/alarm_mgr.o.d ${OBJECTDIR}/_ext/1360937237/gsm_le910.o.d ${OBJECTDIR}/_ext/1360937237/sms_service.o.d ${OBJECTDIR}/_ext/1360937237/flash_w25q32.o.d ${OBJECTDIR}/_ext/1360937237/storage.o.d ${OBJECTDIR}/_ext/1360937237/phonebook_flash.o.d ${OBJECTDIR}/_ext/1360937237/telit.o.d ${OBJECTDIR}/_ext/1360937237/bsp.o.d ${OBJECTDIR}/_ext/1360937237/esp32.o.d ${OBJECTDIR}/_ext/1360937237/w5500if.o.d ${OBJECTDIR}/_ext/1360937237/main.o.d ${OBJECTDIR}/protothreads.o.d ${OBJECTDIR}/_ext/1360937237/handlers.o.d ${OBJECTDIR}/_ext/1360937237/sd_card_thread.o.d ${OBJECTDIR}/_ext/1360937237/tasks.o.d ${OBJECTDIR}/_ext/1100529066/drv_sdspi.o.d ${OBJECTDIR}/_ext/1100529066/drv_sdspi_file_system.o.d ${OBJECTDIR}/_ext/1100529066/drv_sdspi_plib_interface.o.d ${OBJECTDIR}/_ext/14461671/sys_dma.o.d ${OBJECTDIR}/_ext/411819097/ff.o.d ${OBJECTDIR}/_ext/411819097/ffunicode.o.d ${OBJECTDIR}/_ext/565198302/diskio.o.d ${OBJECTDIR}/_ext/1269487135/sys_fs.o.d ${OBJECTDIR}/_ext/1269487135/sys_fs_fat_interface.o.d ${OBJECTDIR}/_ext/1269487135/sys_fs_media_manager.o.d ${OBJECTDIR}/_ext/1881668453/sys_int.o.d ${OBJECTDIR}/_ext/101884895/sys_time.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1360937237/layers_docments.o ${OBJECTDIR}/_ext/60165520/plib_clk.o ${OBJECTDIR}/_ext/1865200349/plib_evic.o ${OBJECTDIR}/_ext/1865254177/plib_gpio.o ${OBJECTDIR}/_ext/298189674/plib_spi1_master.o ${OBJECTDIR}/_ext/298189674/plib_spi3_master.o ${OBJECTDIR}/_ext/1865638794/plib_tmr1.o ${OBJECTDIR}/_ext/1865657120/plib_uart3.o ${OBJECTDIR}/_ext/1865657120/plib_uart1.o ${OBJECTDIR}/_ext/1865657120/plib_uart2.o ${OBJECTDIR}/_ext/163028504/xc32_monitor.o ${OBJECTDIR}/_ext/1171490990/initialization.o ${OBJECTDIR}/_ext/1171490990/interrupts.o ${OBJECTDIR}/_ext/1171490990/exceptions.o ${OBJECTDIR}/_ext/1360937237/store.o ${OBJECTDIR}/_ext/1360937237/esp32_comm.o ${OBJECTDIR}/_ext/1360937237/esp32_proto.o ${OBJECTDIR}/_ext/1360937237/io_monitor.o ${OBJECTDIR}/_ext/1360937237/alarm_mgr.o ${OBJECTDIR}/_ext/1360937237/gsm_le910.o ${OBJECTDIR}/_ext/1360937237/sms_service.o ${OBJECTDIR}/_ext/1360937237/flash_w25q32.o ${OBJECTDIR}/_ext/1360937237/storage.o ${OBJECTDIR}/_ext/1360937237/phonebook_flash.o ${OBJECTDIR}/_ext/1360937237/telit.o ${OBJECTDIR}/_ext/1360937237/bsp.o ${OBJECTDIR}/_ext/1360937237/esp32.o ${OBJECTDIR}/_ext/1360937237/w5500if.o ${OBJECTDIR}/_ext/1360937237/main.o ${OBJECTDIR}/protothreads.o ${OBJECTDIR}/_ext/1360937237/handlers.o ${OBJECTDIR}/_ext/1360937237/sd_card_thread.o ${OBJECTDIR}/_ext/1360937237/tasks.o ${OBJECTDIR}/_ext/1100529066/drv_sdspi.o ${OBJECTDIR}/_ext/1100529066/drv_sdspi_file_system.o ${OBJECTDIR}/_ext/1100529066/drv_sdspi_plib_interface.o ${OBJECTDIR}/_ext/14461671/sys_dma.o ${OBJECTDIR}/_ext/411819097/ff.o ${OBJECTDIR}/_ext/411819097/ffunicode.o ${OBJECTDIR}/_ext/565198302/diskio.o ${OBJECTDIR}/_ext/1269487135/sys_fs.o ${OBJECTDIR}/_ext/1269487135/sys_fs_fat_interface.o ${OBJECTDIR}/_ext/1269487135/sys_fs_media_manager.o ${OBJECTDIR}/_ext/1881668453/sys_int.o ${OBJECTDIR}/_ext/101884895/sys_time.o

# Source Files
SOURCEFILES=../src/layers_docments.c ../src/config/default/peripheral/clk/plib_clk.c ../src/config/default/peripheral/evic/plib_evic.c ../src/config/default/peripheral/gpio/plib_gpio.c ../src/config/default/peripheral/spi/spi_master/plib_spi1_master.c ../src/config/default/peripheral/spi/spi_master/plib_spi3_master.c ../src/config/default/peripheral/tmr1/plib_tmr1.c ../src/config/default/peripheral/uart/plib_uart3.c ../src/config/default/peripheral/uart/plib_uart1.c ../src/config/default/peripheral/uart/plib_uart2.c ../src/config/default/stdio/xc32_monitor.c ../src/config/default/initialization.c ../src/config/default/interrupts.c ../src/config/default/exceptions.c ../src/store.c ../src/esp32_comm.c ../src/esp32_proto.c ../src/io_monitor.c ../src/alarm_mgr.c ../src/gsm_le910.c ../src/sms_service.c ../src/flash_w25q32.c ../src/storage.c ../src/phonebook_flash.c ../src/telit.c ../src/bsp.c ../src/esp32.c ../src/w5500if.c ../src/main.c protothreads.c ../src/handlers.c ../src/sd_card_thread.c ../src/tasks.c ../src/config/default/driver/sdspi/src/drv_sdspi.c ../src/config/default/driver/sdspi/src/drv_sdspi_file_system.c ../src/config/default/driver/sdspi/src/drv_sdspi_plib_interface.c ../src/config/default/system/dma/sys_dma.c ../src/config/default/system/fs/fat_fs/file_system/ff.c ../src/config/default/system/fs/fat_fs/file_system/ffunicode.c ../src/config/default/system/fs/fat_fs/hardware_access/diskio.c ../src/config/default/system/fs/src/sys_fs.c ../src/config/default/system/fs/src/sys_fs_fat_interface.c ../src/config/default/system/fs/src/sys_fs_media_manager.c ../src/config/default/system/int/src/sys_int.c ../src/config/default/system/time/src/sys_time.c



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
${OBJECTDIR}/_ext/1360937237/layers_docments.o: ../src/layers_docments.c  .generated_files/flags/default/f92be70ada240b557f4f6b24fe7b04b367ed3481 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/layers_docments.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/layers_docments.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/layers_docments.o.d" -o ${OBJECTDIR}/_ext/1360937237/layers_docments.o ../src/layers_docments.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/60165520/plib_clk.o: ../src/config/default/peripheral/clk/plib_clk.c  .generated_files/flags/default/83a0c487055c33eed1645aa7efb92112cfee0136 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/60165520" 
	@${RM} ${OBJECTDIR}/_ext/60165520/plib_clk.o.d 
	@${RM} ${OBJECTDIR}/_ext/60165520/plib_clk.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/60165520/plib_clk.o.d" -o ${OBJECTDIR}/_ext/60165520/plib_clk.o ../src/config/default/peripheral/clk/plib_clk.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1865200349/plib_evic.o: ../src/config/default/peripheral/evic/plib_evic.c  .generated_files/flags/default/732e5418c25ce8238d72fedbea3ee3ad28d66848 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1865200349" 
	@${RM} ${OBJECTDIR}/_ext/1865200349/plib_evic.o.d 
	@${RM} ${OBJECTDIR}/_ext/1865200349/plib_evic.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/1865200349/plib_evic.o.d" -o ${OBJECTDIR}/_ext/1865200349/plib_evic.o ../src/config/default/peripheral/evic/plib_evic.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1865254177/plib_gpio.o: ../src/config/default/peripheral/gpio/plib_gpio.c  .generated_files/flags/default/bc38c37259a1ab2df9f60aeca151c58df7920778 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1865254177" 
	@${RM} ${OBJECTDIR}/_ext/1865254177/plib_gpio.o.d 
	@${RM} ${OBJECTDIR}/_ext/1865254177/plib_gpio.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/1865254177/plib_gpio.o.d" -o ${OBJECTDIR}/_ext/1865254177/plib_gpio.o ../src/config/default/peripheral/gpio/plib_gpio.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/298189674/plib_spi1_master.o: ../src/config/default/peripheral/spi/spi_master/plib_spi1_master.c  .generated_files/flags/default/3b84035b2e0c42fccee7ecc4ece100792e255313 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/298189674" 
	@${RM} ${OBJECTDIR}/_ext/298189674/plib_spi1_master.o.d 
	@${RM} ${OBJECTDIR}/_ext/298189674/plib_spi1_master.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/298189674/plib_spi1_master.o.d" -o ${OBJECTDIR}/_ext/298189674/plib_spi1_master.o ../src/config/default/peripheral/spi/spi_master/plib_spi1_master.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/298189674/plib_spi3_master.o: ../src/config/default/peripheral/spi/spi_master/plib_spi3_master.c  .generated_files/flags/default/6d0fe905a60720f014f9480ba0efe066981b699c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/298189674" 
	@${RM} ${OBJECTDIR}/_ext/298189674/plib_spi3_master.o.d 
	@${RM} ${OBJECTDIR}/_ext/298189674/plib_spi3_master.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/298189674/plib_spi3_master.o.d" -o ${OBJECTDIR}/_ext/298189674/plib_spi3_master.o ../src/config/default/peripheral/spi/spi_master/plib_spi3_master.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1865638794/plib_tmr1.o: ../src/config/default/peripheral/tmr1/plib_tmr1.c  .generated_files/flags/default/72c24115f2417109e37f960216621f752deabf02 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1865638794" 
	@${RM} ${OBJECTDIR}/_ext/1865638794/plib_tmr1.o.d 
	@${RM} ${OBJECTDIR}/_ext/1865638794/plib_tmr1.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/1865638794/plib_tmr1.o.d" -o ${OBJECTDIR}/_ext/1865638794/plib_tmr1.o ../src/config/default/peripheral/tmr1/plib_tmr1.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1865657120/plib_uart3.o: ../src/config/default/peripheral/uart/plib_uart3.c  .generated_files/flags/default/35638a4e300bd845f1d744ee03fd04aa0de1fa59 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1865657120" 
	@${RM} ${OBJECTDIR}/_ext/1865657120/plib_uart3.o.d 
	@${RM} ${OBJECTDIR}/_ext/1865657120/plib_uart3.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/1865657120/plib_uart3.o.d" -o ${OBJECTDIR}/_ext/1865657120/plib_uart3.o ../src/config/default/peripheral/uart/plib_uart3.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1865657120/plib_uart1.o: ../src/config/default/peripheral/uart/plib_uart1.c  .generated_files/flags/default/2803f208fb4a3ffc9eb4953b30dce95ea50defd4 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1865657120" 
	@${RM} ${OBJECTDIR}/_ext/1865657120/plib_uart1.o.d 
	@${RM} ${OBJECTDIR}/_ext/1865657120/plib_uart1.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/1865657120/plib_uart1.o.d" -o ${OBJECTDIR}/_ext/1865657120/plib_uart1.o ../src/config/default/peripheral/uart/plib_uart1.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1865657120/plib_uart2.o: ../src/config/default/peripheral/uart/plib_uart2.c  .generated_files/flags/default/2a4491baf7261deef738ffa64e407ab668b9efb5 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1865657120" 
	@${RM} ${OBJECTDIR}/_ext/1865657120/plib_uart2.o.d 
	@${RM} ${OBJECTDIR}/_ext/1865657120/plib_uart2.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/1865657120/plib_uart2.o.d" -o ${OBJECTDIR}/_ext/1865657120/plib_uart2.o ../src/config/default/peripheral/uart/plib_uart2.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/163028504/xc32_monitor.o: ../src/config/default/stdio/xc32_monitor.c  .generated_files/flags/default/33ae95aa30157feacadcfcda7584e94ba649cdf8 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/163028504" 
	@${RM} ${OBJECTDIR}/_ext/163028504/xc32_monitor.o.d 
	@${RM} ${OBJECTDIR}/_ext/163028504/xc32_monitor.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/163028504/xc32_monitor.o.d" -o ${OBJECTDIR}/_ext/163028504/xc32_monitor.o ../src/config/default/stdio/xc32_monitor.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1171490990/initialization.o: ../src/config/default/initialization.c  .generated_files/flags/default/bbce5c3b45dcb3eda69b0aa98f442ca631973791 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1171490990" 
	@${RM} ${OBJECTDIR}/_ext/1171490990/initialization.o.d 
	@${RM} ${OBJECTDIR}/_ext/1171490990/initialization.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/1171490990/initialization.o.d" -o ${OBJECTDIR}/_ext/1171490990/initialization.o ../src/config/default/initialization.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1171490990/interrupts.o: ../src/config/default/interrupts.c  .generated_files/flags/default/395744f5d0452ede9e636c46b1f61eed89a0cbab .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1171490990" 
	@${RM} ${OBJECTDIR}/_ext/1171490990/interrupts.o.d 
	@${RM} ${OBJECTDIR}/_ext/1171490990/interrupts.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/1171490990/interrupts.o.d" -o ${OBJECTDIR}/_ext/1171490990/interrupts.o ../src/config/default/interrupts.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1171490990/exceptions.o: ../src/config/default/exceptions.c  .generated_files/flags/default/f9cb78b9f4f1c170762408118e4c4cc5583cfb3c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1171490990" 
	@${RM} ${OBJECTDIR}/_ext/1171490990/exceptions.o.d 
	@${RM} ${OBJECTDIR}/_ext/1171490990/exceptions.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/1171490990/exceptions.o.d" -o ${OBJECTDIR}/_ext/1171490990/exceptions.o ../src/config/default/exceptions.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/store.o: ../src/store.c  .generated_files/flags/default/f0c5d0dd1b42ced4146bf09d01e2281dfa79bcea .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/store.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/store.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/store.o.d" -o ${OBJECTDIR}/_ext/1360937237/store.o ../src/store.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/esp32_comm.o: ../src/esp32_comm.c  .generated_files/flags/default/77a9ea42272d87bbdc38701ffea733091021993e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/esp32_comm.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/esp32_comm.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/esp32_comm.o.d" -o ${OBJECTDIR}/_ext/1360937237/esp32_comm.o ../src/esp32_comm.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/esp32_proto.o: ../src/esp32_proto.c  .generated_files/flags/default/eed8f29ab7594d1aa805a02728ed6c55d003fdda .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/esp32_proto.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/esp32_proto.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/esp32_proto.o.d" -o ${OBJECTDIR}/_ext/1360937237/esp32_proto.o ../src/esp32_proto.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/io_monitor.o: ../src/io_monitor.c  .generated_files/flags/default/4293313ec86d6237185a51bb8e6ef44b684b1ae6 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/io_monitor.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/io_monitor.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/io_monitor.o.d" -o ${OBJECTDIR}/_ext/1360937237/io_monitor.o ../src/io_monitor.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/alarm_mgr.o: ../src/alarm_mgr.c  .generated_files/flags/default/332fc0e0d7b0a4ffb29e9a71c7a8a51a81b0035 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/alarm_mgr.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/alarm_mgr.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/alarm_mgr.o.d" -o ${OBJECTDIR}/_ext/1360937237/alarm_mgr.o ../src/alarm_mgr.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/gsm_le910.o: ../src/gsm_le910.c  .generated_files/flags/default/f9145712c3805f49c6ce2b2242b65499714d48 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/gsm_le910.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/gsm_le910.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/gsm_le910.o.d" -o ${OBJECTDIR}/_ext/1360937237/gsm_le910.o ../src/gsm_le910.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/sms_service.o: ../src/sms_service.c  .generated_files/flags/default/6718e0bb49b5705f3af8f61312da8317bf147b2f .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/sms_service.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/sms_service.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/sms_service.o.d" -o ${OBJECTDIR}/_ext/1360937237/sms_service.o ../src/sms_service.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/flash_w25q32.o: ../src/flash_w25q32.c  .generated_files/flags/default/9229d0a3b31e9d8b0c46c715e103d9d391d3176a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/flash_w25q32.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/flash_w25q32.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/flash_w25q32.o.d" -o ${OBJECTDIR}/_ext/1360937237/flash_w25q32.o ../src/flash_w25q32.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/storage.o: ../src/storage.c  .generated_files/flags/default/c8b1d6e704275b9a3d1e29d59d4efadb681806ae .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/storage.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/storage.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/storage.o.d" -o ${OBJECTDIR}/_ext/1360937237/storage.o ../src/storage.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/phonebook_flash.o: ../src/phonebook_flash.c  .generated_files/flags/default/4236c4cd9be9eb3bc6046096962aedc73d339cf9 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/phonebook_flash.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/phonebook_flash.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/phonebook_flash.o.d" -o ${OBJECTDIR}/_ext/1360937237/phonebook_flash.o ../src/phonebook_flash.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/telit.o: ../src/telit.c  .generated_files/flags/default/f49fc583d7dc03dac248a72046f115b457b1af44 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/telit.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/telit.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/telit.o.d" -o ${OBJECTDIR}/_ext/1360937237/telit.o ../src/telit.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/bsp.o: ../src/bsp.c  .generated_files/flags/default/341d01898cfaf121819d66bea2b1fc50d6d3477d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/bsp.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/bsp.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/bsp.o.d" -o ${OBJECTDIR}/_ext/1360937237/bsp.o ../src/bsp.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/esp32.o: ../src/esp32.c  .generated_files/flags/default/2ce2e58917dab856022e95c468ef66d1c3a7ce1a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/esp32.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/esp32.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/esp32.o.d" -o ${OBJECTDIR}/_ext/1360937237/esp32.o ../src/esp32.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/w5500if.o: ../src/w5500if.c  .generated_files/flags/default/d59b34c828821d4354e5a50b678c2d0bfaa577b3 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/w5500if.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/w5500if.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/w5500if.o.d" -o ${OBJECTDIR}/_ext/1360937237/w5500if.o ../src/w5500if.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/main.o: ../src/main.c  .generated_files/flags/default/e61090b668d30b811d58c359faa14d26cf329c50 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/main.o.d" -o ${OBJECTDIR}/_ext/1360937237/main.o ../src/main.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/protothreads.o: protothreads.c  .generated_files/flags/default/7f3e7c6987c8f7fca5ba8390ffbed7f4b5bdefc5 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/protothreads.o.d 
	@${RM} ${OBJECTDIR}/protothreads.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/protothreads.o.d" -o ${OBJECTDIR}/protothreads.o protothreads.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/handlers.o: ../src/handlers.c  .generated_files/flags/default/dfde51410f0bebc41e4b4c17042d8510a69a03b9 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/handlers.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/handlers.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/handlers.o.d" -o ${OBJECTDIR}/_ext/1360937237/handlers.o ../src/handlers.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/sd_card_thread.o: ../src/sd_card_thread.c  .generated_files/flags/default/97de0f94fa678e0c665166f64150082666ff222c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/sd_card_thread.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/sd_card_thread.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/sd_card_thread.o.d" -o ${OBJECTDIR}/_ext/1360937237/sd_card_thread.o ../src/sd_card_thread.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/tasks.o: ../src/tasks.c  .generated_files/flags/default/ca0c2e5414c3678874cbfd66fc5506af6380cf9a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/tasks.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/tasks.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/tasks.o.d" -o ${OBJECTDIR}/_ext/1360937237/tasks.o ../src/tasks.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1100529066/drv_sdspi.o: ../src/config/default/driver/sdspi/src/drv_sdspi.c  .generated_files/flags/default/ff9ddd69f3ba0f638f699d793aaa9d6a37617d20 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1100529066" 
	@${RM} ${OBJECTDIR}/_ext/1100529066/drv_sdspi.o.d 
	@${RM} ${OBJECTDIR}/_ext/1100529066/drv_sdspi.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/1100529066/drv_sdspi.o.d" -o ${OBJECTDIR}/_ext/1100529066/drv_sdspi.o ../src/config/default/driver/sdspi/src/drv_sdspi.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1100529066/drv_sdspi_file_system.o: ../src/config/default/driver/sdspi/src/drv_sdspi_file_system.c  .generated_files/flags/default/4167792af6f3010c4cdb05877cf501363a84327c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1100529066" 
	@${RM} ${OBJECTDIR}/_ext/1100529066/drv_sdspi_file_system.o.d 
	@${RM} ${OBJECTDIR}/_ext/1100529066/drv_sdspi_file_system.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/1100529066/drv_sdspi_file_system.o.d" -o ${OBJECTDIR}/_ext/1100529066/drv_sdspi_file_system.o ../src/config/default/driver/sdspi/src/drv_sdspi_file_system.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1100529066/drv_sdspi_plib_interface.o: ../src/config/default/driver/sdspi/src/drv_sdspi_plib_interface.c  .generated_files/flags/default/13d9005bb67c0ec13e17cbdecc479ed137e4f77d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1100529066" 
	@${RM} ${OBJECTDIR}/_ext/1100529066/drv_sdspi_plib_interface.o.d 
	@${RM} ${OBJECTDIR}/_ext/1100529066/drv_sdspi_plib_interface.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/1100529066/drv_sdspi_plib_interface.o.d" -o ${OBJECTDIR}/_ext/1100529066/drv_sdspi_plib_interface.o ../src/config/default/driver/sdspi/src/drv_sdspi_plib_interface.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/14461671/sys_dma.o: ../src/config/default/system/dma/sys_dma.c  .generated_files/flags/default/12d5c504c3fe51b4420866151e6445764a08dd1 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/14461671" 
	@${RM} ${OBJECTDIR}/_ext/14461671/sys_dma.o.d 
	@${RM} ${OBJECTDIR}/_ext/14461671/sys_dma.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/14461671/sys_dma.o.d" -o ${OBJECTDIR}/_ext/14461671/sys_dma.o ../src/config/default/system/dma/sys_dma.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/411819097/ff.o: ../src/config/default/system/fs/fat_fs/file_system/ff.c  .generated_files/flags/default/a0f7df7cd45572302df121158ffbbb1686db5948 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/411819097" 
	@${RM} ${OBJECTDIR}/_ext/411819097/ff.o.d 
	@${RM} ${OBJECTDIR}/_ext/411819097/ff.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/411819097/ff.o.d" -o ${OBJECTDIR}/_ext/411819097/ff.o ../src/config/default/system/fs/fat_fs/file_system/ff.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/411819097/ffunicode.o: ../src/config/default/system/fs/fat_fs/file_system/ffunicode.c  .generated_files/flags/default/a0f0a3efd6383fe17b2292192deec1bc5223860a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/411819097" 
	@${RM} ${OBJECTDIR}/_ext/411819097/ffunicode.o.d 
	@${RM} ${OBJECTDIR}/_ext/411819097/ffunicode.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/411819097/ffunicode.o.d" -o ${OBJECTDIR}/_ext/411819097/ffunicode.o ../src/config/default/system/fs/fat_fs/file_system/ffunicode.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/565198302/diskio.o: ../src/config/default/system/fs/fat_fs/hardware_access/diskio.c  .generated_files/flags/default/f1f3884f24775e313936e1257a0c72ed7d438802 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/565198302" 
	@${RM} ${OBJECTDIR}/_ext/565198302/diskio.o.d 
	@${RM} ${OBJECTDIR}/_ext/565198302/diskio.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/565198302/diskio.o.d" -o ${OBJECTDIR}/_ext/565198302/diskio.o ../src/config/default/system/fs/fat_fs/hardware_access/diskio.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1269487135/sys_fs.o: ../src/config/default/system/fs/src/sys_fs.c  .generated_files/flags/default/85ca0b42772a3cf27c9c754d9d1482363b49630e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1269487135" 
	@${RM} ${OBJECTDIR}/_ext/1269487135/sys_fs.o.d 
	@${RM} ${OBJECTDIR}/_ext/1269487135/sys_fs.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/1269487135/sys_fs.o.d" -o ${OBJECTDIR}/_ext/1269487135/sys_fs.o ../src/config/default/system/fs/src/sys_fs.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1269487135/sys_fs_fat_interface.o: ../src/config/default/system/fs/src/sys_fs_fat_interface.c  .generated_files/flags/default/c45d13f7a1121911bd2d54045c8683c578d61582 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1269487135" 
	@${RM} ${OBJECTDIR}/_ext/1269487135/sys_fs_fat_interface.o.d 
	@${RM} ${OBJECTDIR}/_ext/1269487135/sys_fs_fat_interface.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/1269487135/sys_fs_fat_interface.o.d" -o ${OBJECTDIR}/_ext/1269487135/sys_fs_fat_interface.o ../src/config/default/system/fs/src/sys_fs_fat_interface.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1269487135/sys_fs_media_manager.o: ../src/config/default/system/fs/src/sys_fs_media_manager.c  .generated_files/flags/default/3ca9f88738f8930cbfc75461faf6325e85da4122 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1269487135" 
	@${RM} ${OBJECTDIR}/_ext/1269487135/sys_fs_media_manager.o.d 
	@${RM} ${OBJECTDIR}/_ext/1269487135/sys_fs_media_manager.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/1269487135/sys_fs_media_manager.o.d" -o ${OBJECTDIR}/_ext/1269487135/sys_fs_media_manager.o ../src/config/default/system/fs/src/sys_fs_media_manager.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1881668453/sys_int.o: ../src/config/default/system/int/src/sys_int.c  .generated_files/flags/default/bbfd5b6dcbdec6d8771465c0db7cf313b5fd7d42 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1881668453" 
	@${RM} ${OBJECTDIR}/_ext/1881668453/sys_int.o.d 
	@${RM} ${OBJECTDIR}/_ext/1881668453/sys_int.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/1881668453/sys_int.o.d" -o ${OBJECTDIR}/_ext/1881668453/sys_int.o ../src/config/default/system/int/src/sys_int.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/101884895/sys_time.o: ../src/config/default/system/time/src/sys_time.c  .generated_files/flags/default/346cb97c463455fe355512e5a200fafcdbd29772 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/101884895" 
	@${RM} ${OBJECTDIR}/_ext/101884895/sys_time.o.d 
	@${RM} ${OBJECTDIR}/_ext/101884895/sys_time.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/101884895/sys_time.o.d" -o ${OBJECTDIR}/_ext/101884895/sys_time.o ../src/config/default/system/time/src/sys_time.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
else
${OBJECTDIR}/_ext/1360937237/layers_docments.o: ../src/layers_docments.c  .generated_files/flags/default/2cdeaf8f05cabdafb80940a3b99077242f4e7e25 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/layers_docments.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/layers_docments.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/layers_docments.o.d" -o ${OBJECTDIR}/_ext/1360937237/layers_docments.o ../src/layers_docments.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/60165520/plib_clk.o: ../src/config/default/peripheral/clk/plib_clk.c  .generated_files/flags/default/338d6d97531962d5421da5b8c896536cb9e1f764 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/60165520" 
	@${RM} ${OBJECTDIR}/_ext/60165520/plib_clk.o.d 
	@${RM} ${OBJECTDIR}/_ext/60165520/plib_clk.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/60165520/plib_clk.o.d" -o ${OBJECTDIR}/_ext/60165520/plib_clk.o ../src/config/default/peripheral/clk/plib_clk.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1865200349/plib_evic.o: ../src/config/default/peripheral/evic/plib_evic.c  .generated_files/flags/default/108dde86a4cc7aba2e81d51d7dc33c5c952482d7 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1865200349" 
	@${RM} ${OBJECTDIR}/_ext/1865200349/plib_evic.o.d 
	@${RM} ${OBJECTDIR}/_ext/1865200349/plib_evic.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/1865200349/plib_evic.o.d" -o ${OBJECTDIR}/_ext/1865200349/plib_evic.o ../src/config/default/peripheral/evic/plib_evic.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1865254177/plib_gpio.o: ../src/config/default/peripheral/gpio/plib_gpio.c  .generated_files/flags/default/50842ffeefb4f7d3a32307768f3e35c6916bd024 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1865254177" 
	@${RM} ${OBJECTDIR}/_ext/1865254177/plib_gpio.o.d 
	@${RM} ${OBJECTDIR}/_ext/1865254177/plib_gpio.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/1865254177/plib_gpio.o.d" -o ${OBJECTDIR}/_ext/1865254177/plib_gpio.o ../src/config/default/peripheral/gpio/plib_gpio.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/298189674/plib_spi1_master.o: ../src/config/default/peripheral/spi/spi_master/plib_spi1_master.c  .generated_files/flags/default/817635f81534c08e9d967f8e8a1e01971f3a35a4 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/298189674" 
	@${RM} ${OBJECTDIR}/_ext/298189674/plib_spi1_master.o.d 
	@${RM} ${OBJECTDIR}/_ext/298189674/plib_spi1_master.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/298189674/plib_spi1_master.o.d" -o ${OBJECTDIR}/_ext/298189674/plib_spi1_master.o ../src/config/default/peripheral/spi/spi_master/plib_spi1_master.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/298189674/plib_spi3_master.o: ../src/config/default/peripheral/spi/spi_master/plib_spi3_master.c  .generated_files/flags/default/446e971db00127b1b4b38520baf2b2681390f504 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/298189674" 
	@${RM} ${OBJECTDIR}/_ext/298189674/plib_spi3_master.o.d 
	@${RM} ${OBJECTDIR}/_ext/298189674/plib_spi3_master.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/298189674/plib_spi3_master.o.d" -o ${OBJECTDIR}/_ext/298189674/plib_spi3_master.o ../src/config/default/peripheral/spi/spi_master/plib_spi3_master.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1865638794/plib_tmr1.o: ../src/config/default/peripheral/tmr1/plib_tmr1.c  .generated_files/flags/default/92d42ba9151884883f362c8ce3f2380fbc33a75a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1865638794" 
	@${RM} ${OBJECTDIR}/_ext/1865638794/plib_tmr1.o.d 
	@${RM} ${OBJECTDIR}/_ext/1865638794/plib_tmr1.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/1865638794/plib_tmr1.o.d" -o ${OBJECTDIR}/_ext/1865638794/plib_tmr1.o ../src/config/default/peripheral/tmr1/plib_tmr1.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1865657120/plib_uart3.o: ../src/config/default/peripheral/uart/plib_uart3.c  .generated_files/flags/default/21a3dbf2f53c6b09e09d9c693c5c46312125ee9b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1865657120" 
	@${RM} ${OBJECTDIR}/_ext/1865657120/plib_uart3.o.d 
	@${RM} ${OBJECTDIR}/_ext/1865657120/plib_uart3.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/1865657120/plib_uart3.o.d" -o ${OBJECTDIR}/_ext/1865657120/plib_uart3.o ../src/config/default/peripheral/uart/plib_uart3.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1865657120/plib_uart1.o: ../src/config/default/peripheral/uart/plib_uart1.c  .generated_files/flags/default/d63875c03c155c4076367267936175e914314949 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1865657120" 
	@${RM} ${OBJECTDIR}/_ext/1865657120/plib_uart1.o.d 
	@${RM} ${OBJECTDIR}/_ext/1865657120/plib_uart1.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/1865657120/plib_uart1.o.d" -o ${OBJECTDIR}/_ext/1865657120/plib_uart1.o ../src/config/default/peripheral/uart/plib_uart1.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1865657120/plib_uart2.o: ../src/config/default/peripheral/uart/plib_uart2.c  .generated_files/flags/default/40accf344011ce6642cd0fa815bcf9548bec5b70 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1865657120" 
	@${RM} ${OBJECTDIR}/_ext/1865657120/plib_uart2.o.d 
	@${RM} ${OBJECTDIR}/_ext/1865657120/plib_uart2.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/1865657120/plib_uart2.o.d" -o ${OBJECTDIR}/_ext/1865657120/plib_uart2.o ../src/config/default/peripheral/uart/plib_uart2.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/163028504/xc32_monitor.o: ../src/config/default/stdio/xc32_monitor.c  .generated_files/flags/default/b37e6f619642fbd59533a7755787f049c6dc7e5d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/163028504" 
	@${RM} ${OBJECTDIR}/_ext/163028504/xc32_monitor.o.d 
	@${RM} ${OBJECTDIR}/_ext/163028504/xc32_monitor.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/163028504/xc32_monitor.o.d" -o ${OBJECTDIR}/_ext/163028504/xc32_monitor.o ../src/config/default/stdio/xc32_monitor.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1171490990/initialization.o: ../src/config/default/initialization.c  .generated_files/flags/default/3fb2401c76d9510c6e961eae4cd8fcb9bc1e9f2c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1171490990" 
	@${RM} ${OBJECTDIR}/_ext/1171490990/initialization.o.d 
	@${RM} ${OBJECTDIR}/_ext/1171490990/initialization.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/1171490990/initialization.o.d" -o ${OBJECTDIR}/_ext/1171490990/initialization.o ../src/config/default/initialization.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1171490990/interrupts.o: ../src/config/default/interrupts.c  .generated_files/flags/default/43b33968ba2ca7ffb156022ed894e862f5232b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1171490990" 
	@${RM} ${OBJECTDIR}/_ext/1171490990/interrupts.o.d 
	@${RM} ${OBJECTDIR}/_ext/1171490990/interrupts.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/1171490990/interrupts.o.d" -o ${OBJECTDIR}/_ext/1171490990/interrupts.o ../src/config/default/interrupts.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1171490990/exceptions.o: ../src/config/default/exceptions.c  .generated_files/flags/default/588fcf7db0fe9c73423e85107213dd9f020286a1 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1171490990" 
	@${RM} ${OBJECTDIR}/_ext/1171490990/exceptions.o.d 
	@${RM} ${OBJECTDIR}/_ext/1171490990/exceptions.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/1171490990/exceptions.o.d" -o ${OBJECTDIR}/_ext/1171490990/exceptions.o ../src/config/default/exceptions.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/store.o: ../src/store.c  .generated_files/flags/default/85053744dc620224cf47e0c6ae1e4ed3e138cfff .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/store.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/store.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/store.o.d" -o ${OBJECTDIR}/_ext/1360937237/store.o ../src/store.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/esp32_comm.o: ../src/esp32_comm.c  .generated_files/flags/default/2b5b59a9a49aff0efcf45f63513840f332532a4e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/esp32_comm.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/esp32_comm.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/esp32_comm.o.d" -o ${OBJECTDIR}/_ext/1360937237/esp32_comm.o ../src/esp32_comm.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/esp32_proto.o: ../src/esp32_proto.c  .generated_files/flags/default/6cd9b0808db9dec8bfd01e68260c1e6973285cea .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/esp32_proto.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/esp32_proto.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/esp32_proto.o.d" -o ${OBJECTDIR}/_ext/1360937237/esp32_proto.o ../src/esp32_proto.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/io_monitor.o: ../src/io_monitor.c  .generated_files/flags/default/6c6a3d160aa3479126e0f25d3bb7ad106af092bf .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/io_monitor.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/io_monitor.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/io_monitor.o.d" -o ${OBJECTDIR}/_ext/1360937237/io_monitor.o ../src/io_monitor.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/alarm_mgr.o: ../src/alarm_mgr.c  .generated_files/flags/default/8d9b1a1eb7fe30968719d32aaaf2e6b4d58d44c5 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/alarm_mgr.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/alarm_mgr.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/alarm_mgr.o.d" -o ${OBJECTDIR}/_ext/1360937237/alarm_mgr.o ../src/alarm_mgr.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/gsm_le910.o: ../src/gsm_le910.c  .generated_files/flags/default/5c4131091f87479fe1b7b2d0bec6f4cc1f0948c5 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/gsm_le910.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/gsm_le910.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/gsm_le910.o.d" -o ${OBJECTDIR}/_ext/1360937237/gsm_le910.o ../src/gsm_le910.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/sms_service.o: ../src/sms_service.c  .generated_files/flags/default/101d1865f89461eb63d1de1986fea397c9d7dc1a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/sms_service.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/sms_service.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/sms_service.o.d" -o ${OBJECTDIR}/_ext/1360937237/sms_service.o ../src/sms_service.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/flash_w25q32.o: ../src/flash_w25q32.c  .generated_files/flags/default/7866c230a58d75bface08bdb39e0384af9551919 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/flash_w25q32.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/flash_w25q32.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/flash_w25q32.o.d" -o ${OBJECTDIR}/_ext/1360937237/flash_w25q32.o ../src/flash_w25q32.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/storage.o: ../src/storage.c  .generated_files/flags/default/c4465adfca873f59457098652b3f74c0e7260fe0 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/storage.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/storage.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/storage.o.d" -o ${OBJECTDIR}/_ext/1360937237/storage.o ../src/storage.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/phonebook_flash.o: ../src/phonebook_flash.c  .generated_files/flags/default/256cdf036c728d40a919a3f0659d65f233e5d64 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/phonebook_flash.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/phonebook_flash.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/phonebook_flash.o.d" -o ${OBJECTDIR}/_ext/1360937237/phonebook_flash.o ../src/phonebook_flash.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/telit.o: ../src/telit.c  .generated_files/flags/default/3d6c08f793b977986f5e1bb8038840e425df5631 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/telit.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/telit.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/telit.o.d" -o ${OBJECTDIR}/_ext/1360937237/telit.o ../src/telit.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/bsp.o: ../src/bsp.c  .generated_files/flags/default/e906c1b0c3ad338d9fc70e20da85bfe8e140a3f1 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/bsp.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/bsp.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/bsp.o.d" -o ${OBJECTDIR}/_ext/1360937237/bsp.o ../src/bsp.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/esp32.o: ../src/esp32.c  .generated_files/flags/default/f242d24e0bf6179a24feeee1c45e0021455d51c3 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/esp32.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/esp32.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/esp32.o.d" -o ${OBJECTDIR}/_ext/1360937237/esp32.o ../src/esp32.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/w5500if.o: ../src/w5500if.c  .generated_files/flags/default/631c2e419b562f984e05b5ad12f8a5634b6b899e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/w5500if.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/w5500if.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/w5500if.o.d" -o ${OBJECTDIR}/_ext/1360937237/w5500if.o ../src/w5500if.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/main.o: ../src/main.c  .generated_files/flags/default/a3854bd592aa58f47019e4234a39d98afcd7d20 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/main.o.d" -o ${OBJECTDIR}/_ext/1360937237/main.o ../src/main.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/protothreads.o: protothreads.c  .generated_files/flags/default/f8b9b485783bb85c419163d7465a087f3f55c187 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/protothreads.o.d 
	@${RM} ${OBJECTDIR}/protothreads.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/protothreads.o.d" -o ${OBJECTDIR}/protothreads.o protothreads.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/handlers.o: ../src/handlers.c  .generated_files/flags/default/7c6832563dec98b2d9d312b8e78e3579c620138 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/handlers.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/handlers.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/handlers.o.d" -o ${OBJECTDIR}/_ext/1360937237/handlers.o ../src/handlers.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/sd_card_thread.o: ../src/sd_card_thread.c  .generated_files/flags/default/df6d43c22be3f0044f1098d6ee0e59761643a403 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/sd_card_thread.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/sd_card_thread.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/sd_card_thread.o.d" -o ${OBJECTDIR}/_ext/1360937237/sd_card_thread.o ../src/sd_card_thread.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/tasks.o: ../src/tasks.c  .generated_files/flags/default/9c493d61aafae3e771ed9592c0ff6bb92fdb0f76 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/tasks.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/tasks.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/tasks.o.d" -o ${OBJECTDIR}/_ext/1360937237/tasks.o ../src/tasks.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1100529066/drv_sdspi.o: ../src/config/default/driver/sdspi/src/drv_sdspi.c  .generated_files/flags/default/a7ecb9b22f6b42537b2a67bf60bd8b7be427707c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1100529066" 
	@${RM} ${OBJECTDIR}/_ext/1100529066/drv_sdspi.o.d 
	@${RM} ${OBJECTDIR}/_ext/1100529066/drv_sdspi.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/1100529066/drv_sdspi.o.d" -o ${OBJECTDIR}/_ext/1100529066/drv_sdspi.o ../src/config/default/driver/sdspi/src/drv_sdspi.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1100529066/drv_sdspi_file_system.o: ../src/config/default/driver/sdspi/src/drv_sdspi_file_system.c  .generated_files/flags/default/f00b628f7883157432476f8b2b1c94909a393259 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1100529066" 
	@${RM} ${OBJECTDIR}/_ext/1100529066/drv_sdspi_file_system.o.d 
	@${RM} ${OBJECTDIR}/_ext/1100529066/drv_sdspi_file_system.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/1100529066/drv_sdspi_file_system.o.d" -o ${OBJECTDIR}/_ext/1100529066/drv_sdspi_file_system.o ../src/config/default/driver/sdspi/src/drv_sdspi_file_system.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1100529066/drv_sdspi_plib_interface.o: ../src/config/default/driver/sdspi/src/drv_sdspi_plib_interface.c  .generated_files/flags/default/9e8df75d976ba3c56b591be76ffe3fc1a77f23b6 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1100529066" 
	@${RM} ${OBJECTDIR}/_ext/1100529066/drv_sdspi_plib_interface.o.d 
	@${RM} ${OBJECTDIR}/_ext/1100529066/drv_sdspi_plib_interface.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/1100529066/drv_sdspi_plib_interface.o.d" -o ${OBJECTDIR}/_ext/1100529066/drv_sdspi_plib_interface.o ../src/config/default/driver/sdspi/src/drv_sdspi_plib_interface.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/14461671/sys_dma.o: ../src/config/default/system/dma/sys_dma.c  .generated_files/flags/default/56c291cacb5b9a658344d10e28fa421ea926b666 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/14461671" 
	@${RM} ${OBJECTDIR}/_ext/14461671/sys_dma.o.d 
	@${RM} ${OBJECTDIR}/_ext/14461671/sys_dma.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/14461671/sys_dma.o.d" -o ${OBJECTDIR}/_ext/14461671/sys_dma.o ../src/config/default/system/dma/sys_dma.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/411819097/ff.o: ../src/config/default/system/fs/fat_fs/file_system/ff.c  .generated_files/flags/default/97543f3bdd369829cc6194e4ead7dff466985dc7 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/411819097" 
	@${RM} ${OBJECTDIR}/_ext/411819097/ff.o.d 
	@${RM} ${OBJECTDIR}/_ext/411819097/ff.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/411819097/ff.o.d" -o ${OBJECTDIR}/_ext/411819097/ff.o ../src/config/default/system/fs/fat_fs/file_system/ff.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/411819097/ffunicode.o: ../src/config/default/system/fs/fat_fs/file_system/ffunicode.c  .generated_files/flags/default/7330bec9811f7ed8c08e998bf7381a86f63fc928 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/411819097" 
	@${RM} ${OBJECTDIR}/_ext/411819097/ffunicode.o.d 
	@${RM} ${OBJECTDIR}/_ext/411819097/ffunicode.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/411819097/ffunicode.o.d" -o ${OBJECTDIR}/_ext/411819097/ffunicode.o ../src/config/default/system/fs/fat_fs/file_system/ffunicode.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/565198302/diskio.o: ../src/config/default/system/fs/fat_fs/hardware_access/diskio.c  .generated_files/flags/default/ee2f64f20b2f86e2cede5f045cc0c341035809d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/565198302" 
	@${RM} ${OBJECTDIR}/_ext/565198302/diskio.o.d 
	@${RM} ${OBJECTDIR}/_ext/565198302/diskio.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/565198302/diskio.o.d" -o ${OBJECTDIR}/_ext/565198302/diskio.o ../src/config/default/system/fs/fat_fs/hardware_access/diskio.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1269487135/sys_fs.o: ../src/config/default/system/fs/src/sys_fs.c  .generated_files/flags/default/77c2e363f3da0020c628fc4f9ae9fdf87a02c966 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1269487135" 
	@${RM} ${OBJECTDIR}/_ext/1269487135/sys_fs.o.d 
	@${RM} ${OBJECTDIR}/_ext/1269487135/sys_fs.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/1269487135/sys_fs.o.d" -o ${OBJECTDIR}/_ext/1269487135/sys_fs.o ../src/config/default/system/fs/src/sys_fs.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1269487135/sys_fs_fat_interface.o: ../src/config/default/system/fs/src/sys_fs_fat_interface.c  .generated_files/flags/default/9d4e5aab2f4d2b29d85dd24b2b40313c03ecb03b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1269487135" 
	@${RM} ${OBJECTDIR}/_ext/1269487135/sys_fs_fat_interface.o.d 
	@${RM} ${OBJECTDIR}/_ext/1269487135/sys_fs_fat_interface.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/1269487135/sys_fs_fat_interface.o.d" -o ${OBJECTDIR}/_ext/1269487135/sys_fs_fat_interface.o ../src/config/default/system/fs/src/sys_fs_fat_interface.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1269487135/sys_fs_media_manager.o: ../src/config/default/system/fs/src/sys_fs_media_manager.c  .generated_files/flags/default/9a304840ad8e75f14583b7861e697b77aec063af .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1269487135" 
	@${RM} ${OBJECTDIR}/_ext/1269487135/sys_fs_media_manager.o.d 
	@${RM} ${OBJECTDIR}/_ext/1269487135/sys_fs_media_manager.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/1269487135/sys_fs_media_manager.o.d" -o ${OBJECTDIR}/_ext/1269487135/sys_fs_media_manager.o ../src/config/default/system/fs/src/sys_fs_media_manager.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1881668453/sys_int.o: ../src/config/default/system/int/src/sys_int.c  .generated_files/flags/default/18f377ccff6d6d5fa5d67d36e938e65ea4723b20 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1881668453" 
	@${RM} ${OBJECTDIR}/_ext/1881668453/sys_int.o.d 
	@${RM} ${OBJECTDIR}/_ext/1881668453/sys_int.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/1881668453/sys_int.o.d" -o ${OBJECTDIR}/_ext/1881668453/sys_int.o ../src/config/default/system/int/src/sys_int.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/101884895/sys_time.o: ../src/config/default/system/time/src/sys_time.c  .generated_files/flags/default/7f445e03eb19b0a99468a97aa5ee4e64db00ef98 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/101884895" 
	@${RM} ${OBJECTDIR}/_ext/101884895/sys_time.o.d 
	@${RM} ${OBJECTDIR}/_ext/101884895/sys_time.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -MP -MMD -MF "${OBJECTDIR}/_ext/101884895/sys_time.o.d" -o ${OBJECTDIR}/_ext/101884895/sys_time.o ../src/config/default/system/time/src/sys_time.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -w -Wno-error=unused-variable -mdfp="${DFP_DIR}"  
	
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
	${MP_CC} $(MP_EXTRA_LD_PRE) -g   -mprocessor=$(MP_PROCESSOR_OPTION)  -o ${DISTDIR}/Architecture.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)      -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D=__DEBUG_D,--defsym=_min_heap_size=512,--gc-sections,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,${DISTDIR}/memoryfile.xml -mdfp="${DFP_DIR}"
	
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
