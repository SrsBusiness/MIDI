#******************************************************************************
#
# Makefile - Rules for building the interrupts example.
#
# Copyright (c) 2012-2013 Texas Instruments Incorporated.  All rights reserved.
# Software License Agreement
# 
# Texas Instruments (TI) is supplying this software for use solely and
# exclusively on TI's microcontroller products. The software is owned by
# TI and/or its suppliers, and is protected under applicable copyright
# laws. You may not combine this software with "viral" open-source
# software in order to form a larger program.
# 
# THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
# NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
# NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
# CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
# DAMAGES, FOR ANY REASON WHATSOEVER.
# 
# This is part of revision 1.0 of the EK-TM4C123GXL Firmware Package.
#
#******************************************************************************

#
# Defines the part type that this project uses.
#
PART=TM4C123GH6PM

#
# The base directory for TivaWare.
#
ROOT=.

#
# Include the common make definitions.
#
include ${ROOT}/makedefs

#
# Where to find source files that do not live in this directory.
#
VPATH=utils

#
# Where to find header files that do not live in the source directory.
#
IPATH=.

#
# The default rule, which causes the interrupts example to be built.
#
all: ${COMPILER}
all: ${COMPILER}/MIDI.axf

#
# The rule to clean out all the build products.
#
clean:
	@rm -rf ${COMPILER} ${wildcard *~}

#
# The rule to create the target directory.
#
${COMPILER}:
	@mkdir -p ${COMPILER}

#
# Rules for building the interrupts example.
#
${COMPILER}/MIDI.axf: ${COMPILER}/startup_${COMPILER}.o
${COMPILER}/MIDI.axf: ${COMPILER}/uartstdio.o
${COMPILER}/MIDI.axf: ${COMPILER}/PLL.o
${COMPILER}/MIDI.axf: ${COMPILER}/MIDI.o
${COMPILER}/MIDI.axf: ${COMPILER}/test.o

${COMPILER}/MIDI.axf: ${ROOT}/driverlib/${COMPILER}/libdriver.a
${COMPILER}/MIDI.axf: MIDI.ld
SCATTERgcc_MIDI=MIDI.ld
ENTRY_MIDI=ResetISR
CFLAGSgcc=-DTARGET_IS_BLIZZARD_RB1

#
# Include the automatically generated dependency files.
#
ifneq (${MAKECMDGOALS},clean)
-include ${wildcard ${COMPILER}/*.d} __dummy__
endif
