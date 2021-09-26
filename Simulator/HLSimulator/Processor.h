#pragma once

#include <set>

#include "Processor_state.h"

class IO_handler
{
public:
	virtual void out(uint8_t device, uint8_t data) = 0;
	virtual uint8_t in(uint8_t device) = 0;
};

enum class Run_state
{
	running,
	at_breakpoint,
	halted
};

class Processor
{
public:

	Processor(uint8_t program[256]) : state{ program } {}

	Run_state run();
	Run_state step();
	void set_breakpoint(uint8_t address);
	void clear_breakpoint(uint8_t address);

	Processor_state get_state() const { return state; }

private:

	using Opcode_processor = Run_state (Processor::*)(uint8_t);

	static Opcode_processor opcode_fns[256];

	Run_state process_unimplemented(uint8_t op_code);

	// Group 0a
	Run_state process_nop(uint8_t op_code);
	Run_state process_mov_reg_reg(uint8_t op_code);
	Run_state process_ret(uint8_t op_code);
	Run_state process_stc(uint8_t op_code);
	Run_state process_clc(uint8_t op_code);
	
	// Group 0b
	Run_state process_ld_imm(uint8_t op_code);
	Run_state process_ld_imm_ind(uint8_t op_code);
	Run_state process_st_imm_ind(uint8_t op_code);

	// Group 1
	Run_state process_mov_reg_acc(uint8_t op_code);
	Run_state process_mov_acc_reg(uint8_t op_code);
	Run_state process_pop_reg(uint8_t op_code);
	Run_state process_push_reg(uint8_t op_code);
	Run_state process_mov_reg_sp(uint8_t op_code);
	Run_state process_mov_sp_reg(uint8_t op_code);
	Run_state process_ld_reg_sp_off(uint8_t op_code);
	Run_state process_st_sp_off_reg(uint8_t op_code);

	// Group 2
	// TODO: Make sure flags are set the same as the 74181
	Run_state process_add_acc_reg(uint8_t op_code);
	Run_state process_sub_acc_reg(uint8_t op_code);
	Run_state process_adc_acc_reg(uint8_t op_code);
	Run_state process_sbb_acc_reg(uint8_t op_code);
	Run_state process_xor_acc_reg(uint8_t op_code);
	Run_state process_and_acc_reg(uint8_t op_code);
	Run_state process_or_acc_reg(uint8_t op_code);
	Run_state process_cmp_acc_reg(uint8_t op_code);

	IO_handler& io_handler;
	Processor_state state;
	std::set<uint8_t> breakpoints;
};
