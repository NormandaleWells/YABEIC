#pragma once

#include <array>
#include <cstdint>

using Memory = std::array<uint8_t, 256>;

struct Processor_state
{
	Processor_state(Memory program)
		: p_mem{ program }, d_mem{ 0 }
	{
	}

	const uint8_t zero     = 0b0001;
	const uint8_t carry    = 0b0010;
	const uint8_t overflow = 0b0100;
	const uint8_t negative = 0b1000;

	void set_zero()       { flags |=  zero;     }
	void clear_zero()     { flags &= ~zero;     }
	void set_carry()      { flags |=  carry;    }
	void clear_carry()    { flags &= ~carry;    }
	void set_overflow()   { flags |=  overflow; }
	void clear_overflow() { flags &= ~overflow; }
	void set_negative()   { flags |=  negative; }
	void clear_negative() { flags &= ~negative; }

	void set_zero(uint_fast16_t result)
	{
		if ((result & 0xFF) == 0) set_zero(); else clear_zero();
	}
	void set_carry(uint_fast16_t result)
	{
		if ((result & 0x100) != 0) set_carry(); else clear_carry();
	}
	void set_overflow(bool c)
	{
		if (c) set_overflow(); else clear_zero();
	}
	void set_negative(uint_fast16_t result)
	{
		if ((result & 0x80) != 0) set_negative(); else clear_zero();
	}

	bool get_zero()     { return (flags & zero)     != 0; }
	bool get_carry()    { return (flags & carry)    != 0; }
	bool get_overflow() { return (flags & overflow) != 0; }
	bool get_negative() { return (flags & negative) != 0; }

	uint8_t a = 0;

	uint8_t gpr[4] = { 0, 0, 0, 0 };

	uint8_t sp = 0;
	uint8_t pc = 0;

	uint8_t flags = 0;

	Memory p_mem;
	Memory d_mem;
};
