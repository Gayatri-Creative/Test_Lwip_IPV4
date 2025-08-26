################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
"../Libraries/Ethernet/Phy_Rtl8211f/IfxGeth_Phy_Rtl8211f.c" 

COMPILED_SRCS += \
"Libraries/Ethernet/Phy_Rtl8211f/IfxGeth_Phy_Rtl8211f.src" 

C_DEPS += \
"./Libraries/Ethernet/Phy_Rtl8211f/IfxGeth_Phy_Rtl8211f.d" 

OBJS += \
"Libraries/Ethernet/Phy_Rtl8211f/IfxGeth_Phy_Rtl8211f.o" 


# Each subdirectory must supply rules for building sources it contributes
"Libraries/Ethernet/Phy_Rtl8211f/IfxGeth_Phy_Rtl8211f.src":"../Libraries/Ethernet/Phy_Rtl8211f/IfxGeth_Phy_Rtl8211f.c" "Libraries/Ethernet/Phy_Rtl8211f/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc39xb "-fC:/Users/admin/AURIX-v1.10.6-workspace/Ethernet_1_KIT_TC397_TFT/TriCore Debug (TASKING)/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc39xb -Y0 -N0 -Z0 -o "$@" "$<"
"Libraries/Ethernet/Phy_Rtl8211f/IfxGeth_Phy_Rtl8211f.o":"Libraries/Ethernet/Phy_Rtl8211f/IfxGeth_Phy_Rtl8211f.src" "Libraries/Ethernet/Phy_Rtl8211f/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-Libraries-2f-Ethernet-2f-Phy_Rtl8211f

clean-Libraries-2f-Ethernet-2f-Phy_Rtl8211f:
	-$(RM) ./Libraries/Ethernet/Phy_Rtl8211f/IfxGeth_Phy_Rtl8211f.d ./Libraries/Ethernet/Phy_Rtl8211f/IfxGeth_Phy_Rtl8211f.o ./Libraries/Ethernet/Phy_Rtl8211f/IfxGeth_Phy_Rtl8211f.src

.PHONY: clean-Libraries-2f-Ethernet-2f-Phy_Rtl8211f

