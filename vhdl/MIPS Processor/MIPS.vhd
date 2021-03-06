library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;	

entity MIPS is
	port(CLK, RST: in std_logic;
		CS, WE: out std_logic;
		ADDR: out unsigned (31 downto 0);
		Mem_Bus_in:  in unsigned(31 downto 0);
		Mem_Bus_out: out unsigned(31 downto 0));
end MIPS; 

architecture structure of MIPS is
component REG is
	port(CLK: in std_logic;
		RegW: in std_logic;
		DR, SR1, SR2: in unsigned(4 downto 0);
		Reg_In: in unsigned(31 downto 0);
		ReadReg1, ReadReg2: out unsigned(31 downto 0));
end component;	 

type Operation is (and1, or1, add, sub, mul, mulu, div, divu, mfhi, mflo, slt, shr, shl, jr);
signal ALU_Result_64 : unsigned(63 downto 0) := x"0000000000000000"; -- 64 bit register
signal ALU_Result_64_Save : unsigned(63 downto 0) := x"0000000000000000";
signal enable64 : std_logic;
signal Op, OpSave: Operation := and1;
type Instr_Format is (R, I, J); -- (Arithmetic, Addr_Imm, Jump)
signal Format: Instr_Format := R;
signal Instr, Imm_Ext: unsigned (31 downto 0);
signal PC, nPC, ReadReg1, ReadReg2, Reg_In: unsigned(31 downto 0);
signal ALU_InA, ALU_InB, ALU_Result: unsigned(31 downto 0);
signal ALU_Result_Save: unsigned(31 downto 0);
signal ALUorMEM, RegW, FetchDorI, Writing, REGorIMM: std_logic := '0';
signal REGorIMM_Save, ALUorMEM_Save: std_logic := '0';
signal DR: unsigned(4 downto 0);
signal State, nState: integer range 0 to 4 := 0;
constant addi: unsigned(5 downto 0) := "001000"; -- 8
constant andi: unsigned(5 downto 0) := "001100"; -- 12
constant ori: unsigned(5 downto 0) := "001101"; -- 13
constant lw: unsigned(5 downto 0) := "100011"; -- 35
constant sw: unsigned(5 downto 0) := "101011"; -- 43
constant beq: unsigned(5 downto 0) := "000100"; -- 4
constant bne: unsigned(5 downto 0) := "000101"; -- 5
constant jump: unsigned(5 downto 0) := "000010"; -- 2
alias opcode: unsigned(5 downto 0) is Instr(31 downto 26);
alias SR1: unsigned(4 downto 0) is Instr(25 downto 21);
alias SR2: unsigned(4 downto 0) is Instr(20 downto 16);
alias F_Code: unsigned(5 downto 0) is Instr(5 downto 0);
alias NumShift: unsigned(4 downto 0) is Instr(10 downto 6);
alias ImmField: unsigned (15 downto 0) is Instr(15 downto 0);

begin
	A1: Reg port map (CLK, RegW, DR, SR1, SR2, Reg_In, ReadReg1, ReadReg2);
	Imm_Ext <= x"FFFF" & Instr(15 downto 0) when Instr(15) = '1'
	else x"0000" & Instr(15 downto 0); -- Sign extend immediate field
	DR <= Instr(15 downto 11) when Format = R
	else Instr(20 downto 16); -- Destination Register MUX (MUX1)
	ALU_InA <= ReadReg1;
	ALU_InB <= Imm_Ext when REGorIMM_Save = '1' else ReadReg2; -- ALU MUX (MUX2)
	Reg_in <= Mem_Bus_in when ALUorMEM_Save = '1' else ALU_Result_Save; -- Data MUX
	Format <= R when Opcode = 0 else J when Opcode = 2 else I;
	Mem_Bus_out <= ReadReg2 when Writing = '1' else x"00000000";-- prevents inferred latch
	--else "ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ"; -- drive memory bus only during writes
	ADDR <= PC when FetchDorI = '1' else ALU_Result_Save; --ADDR Mux
		
	process(State, PC, Instr, Format, F_Code, opcode, Op, ALU_InA, ALU_InB, Imm_Ext)
	begin
		FetchDorI <= '0'; CS <= '0'; WE <= '0'; RegW <= '0'; Writing <= '0';
		ALU_Result <= "00000000000000000000000000000000";
		ALU_Result_64 <= x"0000000000000000"; enable64 <= '0';
		npc <= pc; Op <= jr; REGorIMM <= '0'; ALUorMEM <= '0';
		case state is
			when 0 => --fetch instruction
				nPC <= PC + 1; CS <= '1'; nState <= 1; -- increment by 1 since word address
				FetchDorI <= '1';
			when 1 =>
				nState <= 2; REGorIMM <= '0'; ALUorMEM <= '0';
				if Format = J then
					nPC <= "000000" & Instr(25 downto 0); nState <= 0; --jump, and finish
					-- offset not multiplied by 4 since mem is word address
				elsif Format = R then -- register instructions
					if F_code = "100000" then Op <= add; -- add
					elsif F_code = "100010" then Op <= sub; -- subtract
					elsif F_code = "011000" then Op <= mul; -- multiply
					elsif F_code = "011001" then Op <= mulu; -- multiply unsigned
					elsif F_code = "011010" then Op <= div; -- divide
					elsif F_code = "011011" then Op <= divu; -- divide unsigned
					elsif F_code = "010000" then Op <= mfhi; -- move from HI
					elsif F_code = "010010" then Op <= mflo; -- move from LO
					elsif F_code = "100100" then Op <= and1; -- and
					elsif F_code = "100101" then Op <= or1; -- or
					elsif F_code = "101010" then Op <= slt; -- set on less than
					elsif F_code = "000010" then Op <= shr; -- shift right
					elsif F_code = "000000" then Op <= shl; -- shift left
					elsif F_code = "001000" then Op <= jr; -- jump register
					end if;
				elsif Format = I then -- immediate instructions
					REGorIMM <= '1';
					if Opcode = lw or Opcode = sw or Opcode = addi then Op <= add;
					elsif Opcode = beq or Opcode = bne then Op <= sub; REGorIMM <= '0';
					elsif Opcode = andi then Op <= and1;
					elsif Opcode = ori then Op <= or1;
					end if;	
					
					if Opcode = lw then ALUorMEM <= '1'; end if;
				end if;
			when 2 =>
				nState <= 3;
				if OpSave = and1 then ALU_Result <= ALU_InA and ALU_InB;
				elsif OpSave = or1 then ALU_Result <= ALU_InA or ALU_InB;
				elsif OpSave = add then ALU_Result <= ALU_InA + ALU_InB;
				elsif OpSave = sub then ALU_Result <= ALU_InA - ALU_InB;
				elsif OpSave = mul then ALU_Result_64 <= ALU_InA * ALU_InB;  enable64 <= '1'; -- Multiply
				elsif OpSave = mulu then ALU_Result_64 <= ALU_InA * ALU_InB; enable64 <= '1'; -- Multiply Unsigned
				elsif OpSave = div then ALU_Result_64 <= (ALU_InA mod ALU_InB) & (ALU_InA / ALU_InB);  enable64 <= '1'; -- Divide
				elsif OpSave = divu then ALU_Result_64 <= (ALU_InA mod ALU_InB) & (ALU_InA / ALU_InB); enable64 <= '1'; -- Divide Unsigned
				elsif OpSave = mfhi then ALU_Result <= ALU_Result_64_Save(63 downto 32); -- Move from HI
				elsif OpSave = mflo then ALU_Result <= ALU_Result_64_Save(31 downto 0);  -- Move from LO
				elsif OpSave = shr then ALU_Result <= ALU_InB srl to_integer(numshift);
				elsif OpSave = shl then ALU_Result <= ALU_InB sll to_integer(numshift);
				elsif OpSave = slt then -- set on less than
					if ALU_InA < ALU_InB then ALU_Result <= X"00000001";
					else ALU_Result <= X"00000000";
					end if;
				end if;
				if ((ALU_InA = ALU_InB) and Opcode = beq) or
					((ALU_InA /= ALU_InB) and Opcode = bne) then
					nPC <= PC + Imm_Ext; nState <= 0;
				elsif opcode = bne or opcode = beq then nState <= 0;
				elsif OpSave = jr then nPC <= ALU_InA; nState <= 0;
				end if;
		when 3 =>
			nState <= 0;
			if Format = R or Opcode = addi or Opcode = andi or Opcode = ori then
				RegW <= '1';
			elsif Opcode = sw then CS <= '1'; WE <= '1'; Writing <= '1';
			elsif Opcode = lw then CS <= '1'; nState <= 4;
			end if;
		when 4 =>
			nState <= 0; CS <= '1';
			if Opcode = lw then RegW <= '1'; end if;
		end case;
	end process; 
	
	process(CLK)
	begin
		if CLK = '1' and CLK'event then
			if rst = '1' then
				State <= 0;
				PC <= x"00000000";
			else
				State <= nState;
				PC <= nPC;
			end if;
			if State = 0 then Instr <= Mem_Bus_in; end if;
			if State = 1 then
				OpSave <= Op;
				REGorIMM_Save <= REGorIMM;
				ALUorMEM_Save <= ALUorMEM;
			end if;
			if State = 2 then ALU_Result_Save <= ALU_Result;
				if enable64 = '1' then
					ALU_Result_64_Save <= ALU_Result_64;
				end if;
			end if;
		end if;
	end process;
end structure;