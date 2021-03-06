/*
 * tmc2130_interface_api.h
 *
 *  Created on: Jan 24, 2020
 *      Author: medprime
 */

#ifndef SRC_TMC2130_INTERFACE_API_H_
#define SRC_TMC2130_INTERFACE_API_H_

#include "tmc/helpers/API_Header.h"
#include "TMC2130_Register.h"
#include "TMC2130_Constants.h"
#include "TMC2130_Mask_Shift.h"
#include "stm32f4xx_hal.h"

// Helper macros
#define TMC2130_FIELD_READ(tdef, address, mask, shift) \
	FIELD_GET(TMC2130_Read_Register(tdef, address), mask, shift)
#define TMC2130_FIELD_UPDATE(tdef, address, mask, shift, value) \
	(TMC2130_Write_Register(tdef, address, FIELD_SET(TMC2130_Read_Register(tdef, address), mask, shift, value)))

// Typedefs
typedef struct
{
        int32_t shadowRegister[TMC_REGISTER_COUNT];
	uint8_t registerAccess[TMC2130_REGISTER_COUNT];

	GPIO_TypeDef *CS_Port;
	uint16_t CS_Pin;

	GPIO_TypeDef *Step_Port;
	uint16_t Step_Pin;

	GPIO_TypeDef *Dir_Port;
	uint16_t Dir_Pin;

	GPIO_TypeDef *Enable_Port;
	uint16_t Enable_Pin;

	GPIO_TypeDef *DIG0_Port;
	uint16_t DIG0_Pin;

	GPIO_TypeDef *Dig1_Port;
	uint16_t DIG1_Pin;
} TMC2130TypeDef;


// Default Register values
#define R10 0x00071703  // IHOLD_IRUN
#define R6C 0x000101D5  // CHOPCONF

static const int32_t tmc2130_defaultRegisterResetState[TMC2130_REGISTER_COUNT] =
{
//	0,   1,   2,   3,   4,   5,   6,   7,   8,   9,   A,   B,   C,   D,   E,   F
	0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, // 0x00 - 0x0F
	R10, 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, // 0x10 - 0x1F
	0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, // 0x20 - 0x2F
	0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, // 0x30 - 0x3F
	0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, // 0x40 - 0x4F
	0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, // 0x50 - 0x5F
	N_A, N_A, N_A, N_A, N_A, N_A, N_A, N_A, N_A, N_A, 0,   0,   R6C, 0,   0,   0, // 0x60 - 0x6F
	N_A, 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, // 0x70 - 0x7F
};

// Register access permissions:
//   0x00: none (reserved)
//   0x01: read
//   0x02: write
//   0x03: read/write
//   0x21: read, flag register (read to clear)
//   0x42: write, has hardware presets on reset
static const uint8_t tmc2130_defaultRegisterAccess[TMC2130_REGISTER_COUNT] =
{
      //  0     1     2     3     4     5     6     7     8     9     A     B     C     D     E     F
	0x03, 0x21, ____, ____, 0x01, ____, ____, ____, ____, ____, ____, ____, ____, ____, ____, ____, // 0x00 - 0x0F
	0x02, 0x02, 0x01, 0x02, 0x02, 0x02, ____, ____, ____, ____, ____, ____, ____, ____, ____, ____, // 0x10 - 0x1F
	____, ____, ____, ____, ____, ____, ____, ____, ____, ____, ____, ____, ____, 0x03, ____, ____, // 0x20 - 0x2F
	____, ____, ____, 0x02, ____, ____, ____, ____, ____, ____, ____, ____, ____, ____, ____, ____, // 0x30 - 0x3F
	____, ____, ____, ____, ____, ____, ____, ____, ____, ____, ____, ____, ____, ____, ____, ____, // 0x40 - 0x4F
	____, ____, ____, ____, ____, ____, ____, ____, ____, ____, ____, ____, ____, ____, ____, ____, // 0x50 - 0x5F
	0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x01, 0x01, 0x03, 0x02, 0x02, 0x01, // 0x60 - 0x6F
	0x42, 0x01, 0x02, 0x01, ____, ____, ____, ____, ____, ____, ____, ____, ____, ____, ____, ____  // 0x70 - 0x7F
};

// Register constants (only required for 0x42 registers, since we do not have
// any way to find out the content but want to hold the actual value in the
// shadow register so an application (i.e. the TMCL IDE) can still display
// the values. This only works when the register content is constant.
static const TMCRegisterConstant tmc2130_RegisterConstants[] =
{		// Use ascending addresses!
		{ 0x60, 0xAAAAB554 }, // MSLUT[0]
		{ 0x61, 0x4A9554AA }, // MSLUT[1]
		{ 0x62, 0x24492929 }, // MSLUT[2]
		{ 0x63, 0x10104222 }, // MSLUT[3]
		{ 0x64, 0xFBFFFFFF }, // MSLUT[4]
		{ 0x65, 0xB5BB777D }, // MSLUT[5]
		{ 0x66, 0x49295556 }, // MSLUT[6]
		{ 0x67, 0x00404222 }, // MSLUT[7]
		{ 0x68, 0xFFFF8056 }, // MSLUTSEL
		{ 0x69, 0x00F70000 }, // MSLUTSTART
		{ 0x70, 0x00050480 }  // PWMCONF
};

int32_t TMC2130_Read_Register(TMC2130TypeDef *tmc2130, uint8_t address);
void TMC2130_Write_Register(TMC2130TypeDef *tmc2130, uint8_t address, int32_t value);
void TMC2130_Init(TMC2130TypeDef *tmc2130);
void TMC2130_Set_Max_Current(TMC2130TypeDef *motor_handle, int32_t value);
int32_t TMC2130_Get_Max_Current(TMC2130TypeDef *motor_handle);
void TMC2130_Set_Standby_Current(TMC2130TypeDef *motor_handle, int32_t value);
int32_t TMC2130_Get_Standby_Current(TMC2130TypeDef *motor_handle);
void TMC2130_Set_Power_Down(TMC2130TypeDef *motor_handle, int32_t value);
void TMC2130_Set_Speed_Threshold(TMC2130TypeDef *motor_handle, int32_t value);
int32_t TMC2130_Get_Speed_Threshold(TMC2130TypeDef *motor_handle);
void TMC2130_Set_MIN_Speed_dcStep(TMC2130TypeDef *motor_handle, int32_t value);
int32_t TMC2130_Get_MIN_Speed_dcStep(TMC2130TypeDef *motor_handle);
void TMC2130_Set_High_Speed_Full_Step_Mode(TMC2130TypeDef *motor_handle,
	int32_t value);
int32_t TMC2130_Get_High_Speed_Full_Step_Mode(TMC2130TypeDef *motor_handle);
void TMC2130_Set_High_Speed_Chopper_Mode(TMC2130TypeDef *motor_handle,
	int32_t value);
int32_t TMC2130_Get_High_Speed_Chopper_Mode(TMC2130TypeDef *motor_handle);
void TMC2130_Set_Internal_RSense(TMC2130TypeDef *motor_handle, int32_t value);
int32_t TMC2130_Get_Internal_RSense(TMC2130TypeDef *motor_handle);
void TMC2130_Set_Microstep(TMC2130TypeDef *motor_handle, int32_t value);
int32_t TMC2130_Get_Microstep(TMC2130TypeDef *motor_handle);
void TMC2130_Set_Chopper_Blank_Time(TMC2130TypeDef *motor_handle, int32_t value);
int32_t TMC2130_Get_Chopper_Blank_Time(TMC2130TypeDef *motor_handle);
void TMC2130_Set_Constant_Toff_Mode(TMC2130TypeDef *motor_handle, int32_t value);
int32_t TMC2130_Get_Constant_Toff_Mode(TMC2130TypeDef *motor_handle);
void TMC2130_Set_Fast_Decay_Comparator(TMC2130TypeDef *motor_handle,
	int32_t value);
int32_t TMC2130_Get_Fast_Decay_Comparator(TMC2130TypeDef *motor_handle);
void TMC2130_Set_Chopper_Hysteresis_Time(TMC2130TypeDef *motor_handle,
	int32_t value);
int32_t TMC2130_Get_Chopper_Hysteresis_Time(TMC2130TypeDef *motor_handle);
void TMC2130_Set_Chopper_Hysteresis_Offset(TMC2130TypeDef *motor_handle,
	int32_t value);
int32_t TMC2130_Get_Chopper_Hysteresis_Offset(TMC2130TypeDef *motor_handle);
void TMC2130_Set_Chpper_Off_Time(TMC2130TypeDef *motor_handle, int32_t value);
int32_t TMC2130_Get_Chpper_Off_Time(TMC2130TypeDef *motor_handle);
void TMC2130_Set_SEIMIN(TMC2130TypeDef *motor_handle, int32_t value);
int32_t TMC2130_Get_SEIMIN(TMC2130TypeDef *motor_handle);
void TMC2130_Set_SEIDN(TMC2130TypeDef *motor_handle, int32_t value);
int32_t TMC2130_Get_SEIDN(TMC2130TypeDef *motor_handle);
void TMC2130_Set_SEHYS(TMC2130TypeDef *motor_handle, int32_t value);
int32_t TMC2130_Get_SEHYS(TMC2130TypeDef *motor_handle);
void TMC2130_Set_SEIU(TMC2130TypeDef *motor_handle, int32_t value);
int32_t TMC2130_Get_SEIU(TMC2130TypeDef *motor_handle);
void TMC2130_Set_SEHYS_Start(TMC2130TypeDef *motor_handle, int32_t value);
int32_t TMC2130_Get_SEHYS_Start(TMC2130TypeDef *motor_handle);
void TMC2130_Set_Stall_Flter(TMC2130TypeDef *motor_handle, int32_t value);
int32_t TMC2130_Get_Stall_Flter(TMC2130TypeDef *motor_handle);
void TMC2130_Set_Stall_Threshold(TMC2130TypeDef *motor_handle, int32_t value);
int32_t TMC2130_Get_Stall_Threshold(TMC2130TypeDef *motor_handle);
void TMC2130_Set_Vsense(TMC2130TypeDef *motor_handle, int32_t value);
int32_t TMC2130_Get_Vsense(TMC2130TypeDef *motor_handle);
int32_t TMC2130_Get_SE_Actual_Current(TMC2130TypeDef *motor_handle);
void TMC2130_Set_SE_Stallth_Speed(TMC2130TypeDef *motor_handle, int32_t value);
int32_t TMC2130_Get_SE_Stallth_Speed(TMC2130TypeDef *motor_handle);
void TMC2130_Set_Random_Toff_Mode(TMC2130TypeDef *motor_handle, int32_t value);
int32_t TMC2130_Get_Random_Toff_Mode(TMC2130TypeDef *motor_handle);
void TMC2130_Set_Chopper_Sync(TMC2130TypeDef *motor_handle, int32_t value);
int32_t TMC2130_Get_Chopper_Sync(TMC2130TypeDef *motor_handle);
void TMC2130_Set_PWM_Threshold(TMC2130TypeDef *motor_handle, int32_t value);
int32_t TMC2130_Get_PWM_Threshold(TMC2130TypeDef *motor_handle);
void TMC2130_Set_PWM_Gradient(TMC2130TypeDef *motor_handle, int32_t value);
int32_t TMC2130_Get_PWM_Gradient(TMC2130TypeDef *motor_handle);
void TMC2130_Set_PWM_Amplitude(TMC2130TypeDef *motor_handle, int32_t value);
int32_t TMC2130_Get_PWM_Amplitude(TMC2130TypeDef *motor_handle);
void TMC2130_Set_PWM_Frequency(TMC2130TypeDef *motor_handle, int32_t value);
int32_t TMC2130_Get_PWM_Frequency(TMC2130TypeDef *motor_handle);
void TMC2130_Set_PWM_Autoscale(TMC2130TypeDef *motor_handle, int32_t value);
int32_t TMC2130_Get_PWM_Autoscale(TMC2130TypeDef *motor_handle);
void TMC2130_Set_Freewheeling_Mode(TMC2130TypeDef *motor_handle, int32_t value);
int32_t TMC2130_Get_Freewheeling_Mode(TMC2130TypeDef *motor_handle);
int32_t TMC2130_Get_Load_Value(TMC2130TypeDef *motor_handle);

void TMC2130_Set_I_Scale_Analog(TMC2130TypeDef *motor_handle, int32_t value);

#endif /* SRC_TMC2130_INTERFACE_API_H_ */
