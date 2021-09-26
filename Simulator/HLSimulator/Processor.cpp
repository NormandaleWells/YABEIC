
#include "Processor.h"

#include <functional>
#include <sstream>
#include <stdexcept>

Processor::Opcode_processor Processor::opcode_fns[256] =
{
	// Group 0a
	// This group is mainly register-register moves, but
	// there are some special cases where the source and
	// destination are the same register.
	Processor::process_nop,				// 0b000'0'00'00 NOP
	Processor::process_mov_reg_reg,		// 0b000'0'00'01 MOV R0,R1
	Processor::process_mov_reg_reg,		// 0b000'0'00'10 MOV R0,R2
	Processor::process_mov_reg_reg,		// 0b000'0'00'11 MOV R0,R3
	Processor::process_mov_reg_reg,		// 0b000'0'01'00 MOV R1,R0
	Processor::process_ret,				// 0b000'0'01'01 RET
	Processor::process_mov_reg_reg,		// 0b000'0'01'10 MOV R1,R2
	Processor::process_mov_reg_reg,		// 0b000'0'01'11 MOV R1,R3
	Processor::process_mov_reg_reg,		// 0b000'0'10'00 MOV R2,R0
	Processor::process_mov_reg_reg,		// 0b000'0'10'01 MOV R2,R1
	Processor::process_stc,				// 0b000'0'10'10 STC
	Processor::process_mov_reg_reg,		// 0b000'0'10'11 MOV R2,R3
	Processor::process_mov_reg_reg,		// 0b000'0'11'00 MOV R3,R0
	Processor::process_mov_reg_reg,		// 0b000'0'11'01 MOV R3,R1
	Processor::process_mov_reg_reg,		// 0b000'0'11'10 MOV R3,R2
	Processor::process_clc,				// 0b000'0'11'11 CLC

	// Group 0b
	// This group consists of single-register operations
	// with an immediate value (which may be an address).
	Processor::process_ld_imm,			// 0b000'1'00'00 LD R0,imm
	Processor::process_ld_imm,			// 0b000'1'00'01 LD R1,imm
	Processor::process_ld_imm,			// 0b000'1'00'10 LD R2,imm
	Processor::process_ld_imm,			// 0b000'1'00'11 LD R3,imm
	Processor::process_unimplemented,	// 0b000'1'01'00
	Processor::process_unimplemented,	// 0b000'1'01'01
	Processor::process_unimplemented,	// 0b000'1'01'10
	Processor::process_unimplemented,	// 0b000'1'01'11
	Processor::process_ld_imm_ind,		// 0b000'1'10'00 LD R0,(imm)
	Processor::process_ld_imm_ind,		// 0b000'1'10'01 LD R1,(imm)
	Processor::process_ld_imm_ind,		// 0b000'1'10'10 LD R2,(imm)
	Processor::process_ld_imm_ind,		// 0b000'1'10'11 LD R3,(imm)
	Processor::process_st_imm_ind,		// 0b000'1'11'00 ST (imm),R0
	Processor::process_st_imm_ind,		// 0b000'1'11'00 ST (imm),R1
	Processor::process_st_imm_ind,		// 0b000'1'11'00 ST (imm),R2
	Processor::process_st_imm_ind,		// 0b000'1'11'00 ST (imm),R3

	// Group 1
	// This group contains other register ops
	Processor::process_mov_reg_acc,		// 0b001'000'00 MOV R0,A
	Processor::process_mov_reg_acc,		// 0b001'000'01 MOV R1,A
	Processor::process_mov_reg_acc,		// 0b001'000'10 MOV R2,A
	Processor::process_mov_reg_acc,		// 0b001'000'11 MOV R3,A
	Processor::process_mov_acc_reg,		// 0b001'001'00 MOV A,R0
	Processor::process_mov_acc_reg,		// 0b001'001'01 MOV A,R1
	Processor::process_mov_acc_reg,		// 0b001'001'10 MOV A,R2
	Processor::process_mov_acc_reg,		// 0b001'001'11 MOV A,R3
	Processor::process_pop_reg,			// 0b001'010'00 POP R0
	Processor::process_pop_reg,			// 0b001'010'01 POP R1
	Processor::process_pop_reg,			// 0b001'010'10 POP R2
	Processor::process_pop_reg,			// 0b001'010'11 POP R3
	Processor::process_push_reg,		// 0b001'011'00 PUSH R0
	Processor::process_push_reg,		// 0b001'011'01 PUSH R1
	Processor::process_push_reg,		// 0b001'011'10 PUSH R2
	Processor::process_push_reg,		// 0b001'011'11 PUSH R3
	Processor::process_mov_reg_sp,		// 0b001'100'00 MOV R0,SP
	Processor::process_mov_reg_sp,		// 0b001'100'01 MOV R1,SP
	Processor::process_mov_reg_sp,		// 0b001'100'10 MOV R2,SP
	Processor::process_mov_reg_sp,		// 0b001'100'11 MOV R3,SP
	Processor::process_mov_sp_reg,		// 0b001'101'00 MOV SP,R0
	Processor::process_mov_sp_reg,		// 0b001'101'01 MOV SP,R1
	Processor::process_mov_sp_reg,		// 0b001'101'10 MOV SP,R2
	Processor::process_mov_sp_reg,		// 0b001'101'11 MOV SP,R3
	Processor::process_ld_reg_sp_off,	// 0b001'110'00 LD R0,(SP+imm)
	Processor::process_ld_reg_sp_off,	// 0b001'110'01 LD R1,(SP+imm)
	Processor::process_ld_reg_sp_off,	// 0b001'110'10 LD R2,(SP+imm)
	Processor::process_ld_reg_sp_off,	// 0b001'110'11 LD R3,(SP+imm)
	Processor::process_st_sp_off_reg,	// 0b001'111'00 ST (SP+imm),R0
	Processor::process_st_sp_off_reg,	// 0b001'111'01 ST (SP+imm),R1
	Processor::process_st_sp_off_reg,	// 0b001'111'10 ST (SP+imm),R2
	Processor::process_st_sp_off_reg,	// 0b001'111'11 ST (SP+imm),R3

	// Group 2
	// This group consists of ALU operations
	Processor::process_add_acc_reg,		// 0b010'000'00 ADD A,R0
	Processor::process_add_acc_reg,		// 0b010'000'01 ADD A,R1
	Processor::process_add_acc_reg,		// 0b010'000'02 ADD A,R2
	Processor::process_add_acc_reg,		// 0b010'000'03 ADD A,R3
	Processor::process_sub_acc_reg,		// 0b010'001'00 SUB A,R0
	Processor::process_sub_acc_reg,		// 0b010'001'01 SUB A,R1
	Processor::process_sub_acc_reg,		// 0b010'001'02 SUB A,R2
	Processor::process_sub_acc_reg,		// 0b010'001'03 SUB A,R3
	Processor::process_adc_acc_reg,		// 0b010'010'00 ADC A,R0
	Processor::process_adc_acc_reg,		// 0b010'010'01 ADC A,R1
	Processor::process_adc_acc_reg,		// 0b010'010'02 ADC A,R2
	Processor::process_adc_acc_reg,		// 0b010'010'03 ADC A,R3
	Processor::process_sbb_acc_reg,		// 0b010'011'00 SBB A,R0
	Processor::process_sbb_acc_reg,		// 0b010'011'01 SBB A,R1
	Processor::process_sbb_acc_reg,		// 0b010'011'02 SBB A,R2
	Processor::process_sbb_acc_reg,		// 0b010'011'03 SBB A,R3
	Processor::process_xor_acc_reg,		// 0b010'100'00 XOR A,R0
	Processor::process_xor_acc_reg,		// 0b010'100'01 XOR A,R1
	Processor::process_xor_acc_reg,		// 0b010'100'02 XOR A,R2
	Processor::process_xor_acc_reg,		// 0b010'100'03 XOR A,R3
	Processor::process_and_acc_reg,		// 0b010'101'00 AND A,R0
	Processor::process_and_acc_reg,		// 0b010'101'01 AND A,R1
	Processor::process_and_acc_reg,		// 0b010'101'02 AND A,R2
	Processor::process_and_acc_reg,		// 0b010'101'03 AND A,R3
	Processor::process_or_acc_reg,		// 0b010'110'00 OR A,R0
	Processor::process_or_acc_reg,		// 0b010'110'01 OR A,R1
	Processor::process_or_acc_reg,		// 0b010'110'02 OR A,R2
	Processor::process_or_acc_reg,		// 0b010'110'03 OR A,R3
	Processor::process_cmp_acc_reg,		// 0b010'111'00 CMP A,R0
	Processor::process_cmp_acc_reg,		// 0b010'111'01 CMP A,R1
	Processor::process_cmp_acc_reg,		// 0b010'111'02 CMP A,R2
	Processor::process_cmp_acc_reg,		// 0b010'111'03 CMP A,R3


	// Group 3
	// Group 4
	// Group 5
	// Group 6
	// Group 7

	Processor::process_unimplemented,
};

Run_state Processor::run()
{
	while (true)
	{
		Run_state rs = step();
		if (rs != Run_state::running)
			return rs;
		if (breakpoints.contains(state.pc))
			return Run_state::at_breakpoint;
	}
}

Run_state Processor::step()
{
	uint8_t op_code = state.p_mem[state.pc];
	Opcode_processor proc = opcode_fns[op_code];
	return std::invoke(proc, this, op_code);
}

void Processor::set_breakpoint(uint8_t address)
{
	breakpoints.insert(address);
}

void Processor::clear_breakpoint(uint8_t address)
{
	breakpoints.erase(address);
}

Run_state Processor::process_unimplemented(uint8_t op_code)
{
	std::stringstream ss;
	uint8_t group = (op_code & 0b111'00000) >> 5;
	ss << "Unrecognized group " << group << "opcode: " << op_code;
	throw std::runtime_error(ss.str());
}

Run_state Processor::process_nop(uint8_t op_code)
{
	return Run_state::running;
}

Run_state Processor::process_mov_reg_reg(uint8_t op_code)
{
	uint8_t dest = (op_code & 0b0000'11'00) >> 2;
	uint8_t src = op_code & 0b0000'00'11;
	state.gpr[dest] = state.gpr[src];
	return Run_state::running;
}

Run_state Processor::process_ret(uint8_t op_code)
{
	state.pc = state.d_mem[state.sp++];
	return Run_state::running;
}

Run_state Processor::process_stc(uint8_t op_code)
{
	state.set_carry();
	return Run_state::running;
}

Run_state Processor::process_clc(uint8_t op_code)
{
	state.clear_carry();
	return Run_state::running;
}

Run_state Processor::process_ld_imm(uint8_t op_code)
{
	uint8_t reg = op_code & 0b000'0'00'11;
	uint8_t imm = state.p_mem[state.pc++];
	state.gpr[reg] = imm;
	return Run_state::running;
}

Run_state Processor::process_ld_imm_ind(uint8_t op_code)
{
	uint8_t reg = op_code & 0b000'0'00'11;
	uint8_t imm = state.p_mem[state.pc++];
	state.gpr[reg] = state.d_mem[imm];
	return Run_state::running;
}

Run_state Processor::process_st_imm_ind(uint8_t op_code)
{
	uint8_t reg = op_code & 0b000'0'00'11;
	uint8_t imm = state.p_mem[state.pc++];
	state.d_mem[imm] = state.gpr[reg];
	return Run_state::running;
}

Run_state Processor::process_mov_reg_acc(uint8_t op_code)
{
	uint8_t reg = op_code & 0b000'0'00'11;
	state.gpr[reg] = state.a;
	return Run_state::running;
}

Run_state Processor::process_mov_acc_reg(uint8_t op_code)
{
	uint8_t reg = op_code & 0b000'0'00'11;
	state.a = state.gpr[reg];
	return Run_state::running;
}

Run_state Processor::process_pop_reg(uint8_t op_code)
{
	uint8_t reg = op_code & 0b000'0'00'11;
	state.gpr[reg] = state.d_mem[state.sp++];
	return Run_state::running;
}

Run_state Processor::process_push_reg(uint8_t op_code)
{
	uint8_t reg = op_code & 0b000'0'00'11;
	state.d_mem[--state.sp] = state.gpr[reg];
	return Run_state::running;
}

Run_state Processor::process_mov_reg_sp(uint8_t op_code)
{
	uint8_t reg = op_code & 0b000'0'00'11;
	state.gpr[reg] = state.sp;
	return Run_state::running;
}

Run_state Processor::process_mov_sp_reg(uint8_t op_code)
{
	uint8_t reg = op_code & 0b000'0'00'11;
	state.sp = state.gpr[reg];
	return Run_state::running;
}

Run_state Processor::process_ld_reg_sp_off(uint8_t op_code)
{
	uint8_t reg = op_code & 0b000'0'00'11;
	uint8_t imm = state.p_mem[state.pc++];
	state.gpr[reg] = state.d_mem[state.sp + imm];
	return Run_state::running;
}

Run_state Processor::process_st_sp_off_reg(uint8_t op_code)
{
	uint8_t reg = op_code & 0b000'0'00'11;
	uint8_t imm = state.p_mem[state.pc++];
	state.d_mem[state.sp + imm] = state.gpr[reg];
	return Run_state::running;
}

static bool calc_overflow(uint_fast16_t a, uint_fast16_t b, uint_fast16_t res)
{
	return ((a & 0x80) ^ (res & 0x80)) & ((b & 0x80) ^ (res & 0x80)) != 0;
}

static uint8_t add_op(uint_fast16_t a, uint_fast16_t b, Processor_state& state, bool use_carry)
{
	uint_fast16_t res = a + b + (use_carry && state.get_carry()) ? 1 : 0;
	state.set_carry(res & 0x0100 != 0);
	state.set_zero(res & 0x00FF == 0);
	state.set_overflow(calc_overflow(a, b, res));
	state.set_negative(res & 0x80 != 0);
	return static_cast<uint8_t>(res);
}

static uint8_t sub_op(uint_fast16_t a, uint_fast16_t b, Processor_state& state, bool use_carry)
{
	uint_fast16_t res = a - b - (use_carry && state.get_carry()) ? 1 : 0;
	state.set_carry(res & 0x0100 != 0);
	state.set_zero(res & 0x00FF == 0);
	state.set_overflow(calc_overflow(a, b, res));
	state.set_negative(res & 0x80 != 0);
	return static_cast<uint8_t>(res);
}

Run_state Processor::process_add_acc_reg(uint8_t op_code)
{
	uint8_t reg = op_code & 0b000'000'11;
	state.a = add_op(state.a, state.gpr[reg], state, false);
	return Run_state::running;
}

Run_state Processor::process_sub_acc_reg(uint8_t op_code)
{
	uint8_t reg = op_code & 0b000'000'11;
	state.a = sub_op(state.a, state.gpr[reg], state, false);
	return Run_state::running;
}

Run_state Processor::process_adc_acc_reg(uint8_t op_code)
{
	uint8_t reg = op_code & 0b000'000'11;
	state.a = add_op(state.a, state.gpr[reg], state, true);
	return Run_state::running;
}

Run_state Processor::process_sbb_acc_reg(uint8_t op_code)
{
	uint8_t reg = op_code & 0b000'000'11;
	state.a = sub_op(state.a, state.gpr[reg], state, true);
	return Run_state::running;
}

static uint8_t xor_op(uint_fast16_t a, uint_fast16_t b, Processor_state& state)
{
	uint_fast16_t res = a ^ b;
	state.set_zero(res & 0x00FF == 0);
	state.set_negative(res & 0x0080 != 0);
	return static_cast<uint8_t>(res);
}

Run_state Processor::process_xor_acc_reg(uint8_t op_code)
{
	uint8_t reg = op_code & 0b000'000'11;
	state.a = xor_op(state.a, state.gpr[reg], state);
	return Run_state::running;
}

static uint8_t and_op(uint_fast16_t a, uint_fast16_t b, Processor_state& state)
{
	uint_fast16_t res = a & b;
	state.set_zero(res & 0x00FF == 0);
	state.set_negative(res & 0x0080 != 0);
	return static_cast<uint8_t>(res);
}

Run_state Processor::process_and_acc_reg(uint8_t op_code)
{
	uint8_t reg = op_code & 0b000'000'11;
	state.a = and_op(state.a, state.gpr[reg], state);
	return Run_state::running;
}

static uint8_t or_op(uint_fast16_t a, uint_fast16_t b, Processor_state& state)
{
	uint_fast16_t res = a | b;
	state.set_zero(res & 0x00FF == 0);
	state.set_negative(res & 0x0080 != 0);
	return static_cast<uint8_t>(res);
}

Run_state Processor::process_or_acc_reg(uint8_t op_code)
{
	uint8_t reg = op_code & 0b000'000'11;
	state.a = or_op(state.a, state.gpr[reg], state);
	return Run_state::running;
}

Run_state Processor::process_cmp_acc_reg(uint8_t op_code)
{
	uint8_t reg = op_code & 0b000'000'11;
	sub_op(state.a, state.gpr[reg], state, false);
	return Run_state::running;
}
