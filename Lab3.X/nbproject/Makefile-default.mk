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
MKDIR=mkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/Lab3.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/Lab3.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
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
SOURCEFILES_QUOTED_IF_SPACED=auxillary_files/crc16.c auxillary_files/lab3_server.c auxillary_files/lab3_troll.c auxillary_files/pc_crc16.c auxillary_files/serialdebug.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/auxillary_files/crc16.o ${OBJECTDIR}/auxillary_files/lab3_server.o ${OBJECTDIR}/auxillary_files/lab3_troll.o ${OBJECTDIR}/auxillary_files/pc_crc16.o ${OBJECTDIR}/auxillary_files/serialdebug.o
POSSIBLE_DEPFILES=${OBJECTDIR}/auxillary_files/crc16.o.d ${OBJECTDIR}/auxillary_files/lab3_server.o.d ${OBJECTDIR}/auxillary_files/lab3_troll.o.d ${OBJECTDIR}/auxillary_files/pc_crc16.o.d ${OBJECTDIR}/auxillary_files/serialdebug.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/auxillary_files/crc16.o ${OBJECTDIR}/auxillary_files/lab3_server.o ${OBJECTDIR}/auxillary_files/lab3_troll.o ${OBJECTDIR}/auxillary_files/pc_crc16.o ${OBJECTDIR}/auxillary_files/serialdebug.o

# Source Files
SOURCEFILES=auxillary_files/crc16.c auxillary_files/lab3_server.c auxillary_files/lab3_troll.c auxillary_files/pc_crc16.c auxillary_files/serialdebug.c



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
	${MAKE}  -f nbproject/Makefile-default.mk ${DISTDIR}/Lab3.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=33FJ256MC710
MP_LINKER_FILE_OPTION=,--script=p33FJ256MC710.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/auxillary_files/crc16.o: auxillary_files/crc16.c  .generated_files/flags/default/27ea7b14bab86e8df8e76388f4d09d47094d87d0 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/auxillary_files" 
	@${RM} ${OBJECTDIR}/auxillary_files/crc16.o.d 
	@${RM} ${OBJECTDIR}/auxillary_files/crc16.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  auxillary_files/crc16.c  -o ${OBJECTDIR}/auxillary_files/crc16.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/auxillary_files/crc16.o.d"      -g -D__DEBUG     -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/auxillary_files/lab3_server.o: auxillary_files/lab3_server.c  .generated_files/flags/default/8224d3280463cf7e94eb07117750a4bb6702116e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/auxillary_files" 
	@${RM} ${OBJECTDIR}/auxillary_files/lab3_server.o.d 
	@${RM} ${OBJECTDIR}/auxillary_files/lab3_server.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  auxillary_files/lab3_server.c  -o ${OBJECTDIR}/auxillary_files/lab3_server.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/auxillary_files/lab3_server.o.d"      -g -D__DEBUG     -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/auxillary_files/lab3_troll.o: auxillary_files/lab3_troll.c  .generated_files/flags/default/dc09f7d9dc5721098d1d4c3dee0e865938c220b3 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/auxillary_files" 
	@${RM} ${OBJECTDIR}/auxillary_files/lab3_troll.o.d 
	@${RM} ${OBJECTDIR}/auxillary_files/lab3_troll.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  auxillary_files/lab3_troll.c  -o ${OBJECTDIR}/auxillary_files/lab3_troll.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/auxillary_files/lab3_troll.o.d"      -g -D__DEBUG     -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/auxillary_files/pc_crc16.o: auxillary_files/pc_crc16.c  .generated_files/flags/default/29297e399b70bb7b066e94a92aeac6d27357d75d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/auxillary_files" 
	@${RM} ${OBJECTDIR}/auxillary_files/pc_crc16.o.d 
	@${RM} ${OBJECTDIR}/auxillary_files/pc_crc16.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  auxillary_files/pc_crc16.c  -o ${OBJECTDIR}/auxillary_files/pc_crc16.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/auxillary_files/pc_crc16.o.d"      -g -D__DEBUG     -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/auxillary_files/serialdebug.o: auxillary_files/serialdebug.c  .generated_files/flags/default/4b1fadabcd8a50032f58e122acc13a1cd949c6dd .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/auxillary_files" 
	@${RM} ${OBJECTDIR}/auxillary_files/serialdebug.o.d 
	@${RM} ${OBJECTDIR}/auxillary_files/serialdebug.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  auxillary_files/serialdebug.c  -o ${OBJECTDIR}/auxillary_files/serialdebug.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/auxillary_files/serialdebug.o.d"      -g -D__DEBUG     -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
else
${OBJECTDIR}/auxillary_files/crc16.o: auxillary_files/crc16.c  .generated_files/flags/default/943f069bda4c2b5db9addcf54c87f5089400eeb4 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/auxillary_files" 
	@${RM} ${OBJECTDIR}/auxillary_files/crc16.o.d 
	@${RM} ${OBJECTDIR}/auxillary_files/crc16.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  auxillary_files/crc16.c  -o ${OBJECTDIR}/auxillary_files/crc16.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/auxillary_files/crc16.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/auxillary_files/lab3_server.o: auxillary_files/lab3_server.c  .generated_files/flags/default/45891ceb32394b8624fa1ec7023ae90fa5d40c57 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/auxillary_files" 
	@${RM} ${OBJECTDIR}/auxillary_files/lab3_server.o.d 
	@${RM} ${OBJECTDIR}/auxillary_files/lab3_server.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  auxillary_files/lab3_server.c  -o ${OBJECTDIR}/auxillary_files/lab3_server.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/auxillary_files/lab3_server.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/auxillary_files/lab3_troll.o: auxillary_files/lab3_troll.c  .generated_files/flags/default/7db499fa094c10a27ee6095d154673d1948649db .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/auxillary_files" 
	@${RM} ${OBJECTDIR}/auxillary_files/lab3_troll.o.d 
	@${RM} ${OBJECTDIR}/auxillary_files/lab3_troll.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  auxillary_files/lab3_troll.c  -o ${OBJECTDIR}/auxillary_files/lab3_troll.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/auxillary_files/lab3_troll.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/auxillary_files/pc_crc16.o: auxillary_files/pc_crc16.c  .generated_files/flags/default/ffd1ffc2080f8bb30283050354184efe653f8bf2 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/auxillary_files" 
	@${RM} ${OBJECTDIR}/auxillary_files/pc_crc16.o.d 
	@${RM} ${OBJECTDIR}/auxillary_files/pc_crc16.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  auxillary_files/pc_crc16.c  -o ${OBJECTDIR}/auxillary_files/pc_crc16.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/auxillary_files/pc_crc16.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/auxillary_files/serialdebug.o: auxillary_files/serialdebug.c  .generated_files/flags/default/c9c9fc06653111f19f02b3c7f5dd8d3410b1dd67 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/auxillary_files" 
	@${RM} ${OBJECTDIR}/auxillary_files/serialdebug.o.d 
	@${RM} ${OBJECTDIR}/auxillary_files/serialdebug.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  auxillary_files/serialdebug.c  -o ${OBJECTDIR}/auxillary_files/serialdebug.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/auxillary_files/serialdebug.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemblePreproc
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/Lab3.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/Lab3.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG=__DEBUG   -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)      -Wl,,,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D__DEBUG=__DEBUG,,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml$(MP_EXTRA_LD_POST)  -mdfp="${DFP_DIR}/xc16" 
	
else
${DISTDIR}/Lab3.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/Lab3.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -Wl,,,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml$(MP_EXTRA_LD_POST)  -mdfp="${DFP_DIR}/xc16" 
	${MP_CC_DIR}/xc16-bin2hex ${DISTDIR}/Lab3.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf   -mdfp="${DFP_DIR}/xc16" 
	
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
