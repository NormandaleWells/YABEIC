//
// Simulator for 8-bit computer
//
//	This will eventually be moved to an Arduino project,
//	and parts of the hardware will gradually be moved
//	onto breadboards.
//

//	Quick summary:
//
//	Any reference to logic chips are 74HCT family,
//	unles otherwise noted.
//
//	Registers
//		A		accumulator - fed "live" into ALU
//				'574 and '245 (for easier wiring)
//		B		other ALU input
//				'574 and '245 (for easier wiring)
//		R0-R3	general purpose registers
//				R2 and R3 may be used for indirect addresses
//				output to DB or ACREG bus (see below)
//				input from AC or DB via address bus
//				probably '377s
//		PC		program counter
//				'274 (for clear signal)
//		SP		stack pointer - predecrement, postincrement
//				'274 (for clear signal)
//		MAR		memory address register
//				probably '377
//		IR		instruction register
//				probably '377
//		FLAGS	flags register ZCON
//				zero, carry, overflow, negative

//	Computation
//		ALU		arithmetic and logic unit
//				inputs are A and B registers, output gated via '245
//				output to data bus
//		AC		address calculator
//				2 multiplexed inputs
//					input 1: PC, SP, R2, R3
//					input 2: 0, 1, -1, immediate offset
//				output to MAR, DB, address registers

// There is one main data bus (DB), and several mini-buses:
//		ACREG - feeds the address calculator (AC)
//		ACOFF - feeds the address calculator
//		AB    - address bus - either AC output, or DB

// Control signals
// A '_' suffix indicates an active-low signal.

enum class ControlSignals
{
	// Inputs to the ACREG minibus which feeds the AC.
	// A '139 is used to select one of these possible inputs
	// (via a '245 transceiver), with one of them always
	// being fed live to the AC.
	// NOTE: These are active high, since the '139 has
	// inverted outputs.
	ACREG_MASK = 0x000003,	// mask
	PCOUT      = 0x000000,
	SPOUT      = 0x000001,
	R2OUTAC    = 0x000002,
	R3OUTAC    = 0x000003,

	// Inputs to the ACOFF minibus which feeds the AC.
	// A '139 is used to select one of these possible inputs
	// (via a set of '245 transceivers), with one of them
	// always fed live to the AC.
	// NOTE: These are active high, since the '139 has
	// inverted outputs.
	ACOFF_MASK = 0x00000C,	// mask
	AC0        = 0x000000,	// add 0 to address reg
	AC1        = 0x000004,	// add 1 to address reg
	ACDBOFF    = 0x000008,	// add off from DB to address reg
	ACN1       = 0x00000C,	// add -1 to address reg

	// The address bus may optionally be read into one of
	// the 4 address registers, via yet another '139.  In
	// this case, we need to have another control line to
	// enable the '139, since we don't always want to be
	// writing to a register.
	// NOTE: This is the only want to write to these
	// registers.  To update an address register from
	// the data bus, 
	ARIN_MASK = 0x000030,	// mask
	PCIN      = 0x000000,
	SPIN      = 0x000010,
	R2IN      = 0x000020,
	R3IN      = 0x000030,
	ARENABLE  = 0x000040,	// enables register selection from AB

	// The address bus may be selected from either the DB
	// or the AC output, via a pair of '157 selectors.
	AB_MASK = 0x000080,
	ACTOAB  = 0x000000,
	DBTOAB  = 0x000080,

	// outputs to DB - multiplexed through two '138s
	// DBSELECT selects which '138 is used
	// All use '245, but signals are active high, since
	// '138 uses inverted outputs.
	DBOUT_MASK = 0x000F00,
	NONEOUT    = 0x000000,
	AOUT       = 0x000100,
	BOUT       = 0x000200,
	RAMOUT     = 0x000300,
	R0OUT      = 0x000400,
	R1OUT      = 0x000400,
	R2OUT      = 0x000600,
	R3OUT      = 0x000700,
	ALUOUT     = 0x000800,
	ACTODB     = 0x000900,
	// can support 6 more (perhaps PCOUT and SPOUT?)
	DBSELECT = 0x000800,	// Selects which '138

	// inputs from DB - multiplexed through '138
	// All register chips use active low select, but
	// these are all active high because the '138 uses
	// inverted outputs.
	// NOTE: To read from DB into R2 or R3, use
	//		DBTOAB | RxIN | ARENABLE
	DBIN_MASK = 0x007000,
	NONEIN    = 0x000000,
	AIN       = 0x001000,
	BIN       = 0x002000,
	RAMIN     = 0x003000,
	R0IN      = 0x004000,
	R1IN      = 0x005000,
	IRIN      = 0x006000,

	// Memory Address Register in - fed from AC output
	// Note that this cannot be multiplexed with other
	// signals - it may enabled at the same time as others.
	MARIN_ = 0x008000,

	// ALU control signals
	ALUF0 = 0x010000,
	ALUF1 = 0x020000,
	ALUF2 = 0x040000,
	ALUF3 = 0x080000,
	ALUM  = 0x100000,		// logic/arithmetic select
	ALUC_ = 0x200000,		// carry in

	// Reset the sequencer
	SEQRST = 0x400000,
};