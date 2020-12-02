library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;  

entity fpga is
	port(CLOCK_50: in std_logic;				-- 50 MHz clock
		 SW: in std_logic_vector(9 downto 0);	-- Switches
		 KEY: in std_logic_vector(3 downto 0);	-- Keys
		 HEX0: out std_logic_vector(0 to 6);	-- Hex Displays
		 HEX1: out std_logic_vector(0 to 6);
		 HEX2: out std_logic_vector(0 to 6);
		 HEX3: out std_logic_vector(0 to 6);
		 HEX4: out std_logic_vector(0 to 6);
		 HEX5: out std_logic_vector(0 to 6));
end fpga;	

architecture test of fpga is
component MIPS
	port(CLK, RST: in std_logic;
		CS, WE: out std_logic;
		ADDR: out unsigned (31 downto 0);
		Mem_Bus_in:  in unsigned(31 downto 0);
		Mem_Bus_out: out unsigned(31 downto 0));
end component; 

component Memory
	port(CS, WE, CLK: in std_logic;
		ADDR: in unsigned(31 downto 0);
		Mem_Bus_in:  in unsigned(31 downto 0);
		Mem_Bus_out: out unsigned(31 downto 0));
end component;

component  pll IS 
	 port(outclk_0	:	OUT  STD_LOGIC;
		  refclk	:	IN  STD_LOGIC;
		  rst	:	IN  STD_LOGIC); 
end component;

function sevenSeg(h:unsigned(3 downto 0)) return std_logic_vector is
    variable ret:std_logic_vector(6 downto 0);
    begin
        case h is
            when X"0" => ret:="1111110";
            when x"1" => ret:="0110000";
            when x"2" => ret:="1101101";
            when x"3" => ret:="1111001";
            when x"4" => ret:="0110011";
            when x"5" => ret:="1011011";
            when x"6" => ret:="1011111";
            when x"7" => ret:="1110000";
            when x"8" => ret:="1111111";
            when x"9" => ret:="1111011";
            when x"a" => ret:="1110111";
            when x"b" => ret:="0011111";
            when x"c" => ret:="1001110";
            when x"d" => ret:="0111101";
            when x"e" => ret:="1001111";
            when x"f" => ret:="1000111";
		    when others => ret:="0000000";
        end case;
        ret := not ret;
        return ret;
    end function;

constant N: integer := 8;
constant W: integer := 26;
type Iarr is array(1 to W) of unsigned(31 downto 0);
constant Instr_List: Iarr := (
	x"30000000", -- andi $0, $0, 0 => $0 = 0
	x"20010006", -- addi $1, $0, 6 => $1 = 6
	x"34020012", -- ori $2, $0, 18 => $2 = 18
	x"00221820", -- add $3, $1, $2 => $3 = $1 + $2 = 24
	x"00412022", -- sub $4, $2, $1 => $4 = $2 – $1 = 12
	x"00222824", -- and $5, $1, $2 => $5 = $1 and $2 = 2
	x"00223025", -- or $6, $1, $2 => $6 = $1 or $2 = 22
	x"0022382A", -- slt $7, $1, $2 => $7 = 1 because $1<$2
	x"00024100", -- sll $8, $2, 4 => $8 = 18 * 16 = 288
	x"00014842", -- srl $9, $1, 1 => $9 = 6/2 = 3
	x"10220001", -- beq $1, $2, 1 => should not branch
	x"8C0A0004", -- lw $10, 4($0) => $10 = 5th instr = x"00412022" = 4268066
	x"14620001", -- bne $1, $2, 1 => must branch to PC+1+1
	x"30210000", -- andi $1, $1, 0 => $1 = 0 (skipped if bne worked correctly)
	x"08000010", -- j 16 => PC = 16
	x"30420000", -- andi $2, $2, 0 => $2 = 0 (skipped if j 16 worked correctly)
	x"00400008", -- jr $2 => PC = $2 = 18 = PC+1+1. $3 wrong if fails
	x"30630000", -- andi $3, $3, 0 => $3 = 0 (skipped if jr $2 worked correctly)
	x"AC030040", -- sw $3, 64($0) => Mem(64) = $3
	x"AC040041", -- sw $4, 65($0) => Mem(65) = $4
	x"AC050042", -- sw $5, 66($0) => Mem(66) = $5
	x"AC060043", -- sw $6, 67($0) => Mem(67) = $6
	x"AC070044", -- sw $7, 68($0) => Mem(68) = $7
	x"AC080045", -- sw $8, 69($0) => Mem(69) = $8
	x"AC090046", -- sw $9, 70($0) => Mem(70) = $9
	x"AC0A0047" -- sw $10, 71($0) => Mem(71) = $10
);
-- The last instructions perform a series of sw operations that store
-- registers 3–10 to memory. During the memory write stage, the testbench
-- will compare the value of these registers (by looking at the bus value)
-- with the expected output. No explicit check/assertion for branch
-- instructions, however if a branch does not execute as expected, an error
-- will be detected because the assertion for the instruction after the
-- branch instruction will be incorrect.  

type output_arr is array(1 to N) of integer;
constant expected: output_arr := (24, 12, 2, 22, 1, 288, 3, 4268066);
signal CS, WE, CLK, CLK_MUX, ast: std_logic := '0';
signal Mem_Bus_in, Mem_Bus_in_Mux, Mem_Bus_out, Mem_Bus_out_TB, Address, AddressTB, Address_Mux: unsigned(31 downto 0);
signal RST, init, WE_Mux, CS_Mux, WE_TB, CS_TB: std_logic;
signal instrCount : integer range 1 to 26 := 1;
signal comparCount : integer range 1 to 8 := 1;
signal State, nState: integer range 0 to 6 := 0;

begin
	CPU: MIPS port map (CLK_Mux, RST, CS, WE, Address, Mem_Bus_out, Mem_Bus_out_TB);
	MEM: Memory port map (CS_Mux, WE_Mux, CLK_Mux, Address_Mux, Mem_Bus_in_Mux, Mem_Bus_out);
	PLL_CLK: pll port map(rst => rst, refclk => CLOCK_50, outclk_0 => CLK);
	--CLK <= not CLK after 10 ns;
	Address_Mux <= AddressTB when init = '1' else Address;
	WE_Mux <= WE_TB when init = '1' else WE;
	CS_Mux <= CS_TB when init = '1' else CS;
	Mem_Bus_in_Mux <= Mem_Bus_in when init = '1' else Mem_Bus_out_TB;	-- Mem_Bus MUX
	CLK_MUX <= not KEY(0) when SW(0)='0' else CLK;	-- CLK MUX
	--CLK <= not CLK_MUX after 10ns;	-- CLK
	RST <= not KEY(1);	-- RST on KEY1
	
	process(CLK_Mux, WE)
	begin
		if CLK_Mux = '1' and CLK_Mux'event then
		case state is
			when 0 =>	-- reset state
			--rst <= '1';
				if RST = '1' then
					nState <= 1;
				end if;
				
			when 1 =>
				init <= '1';
				CS_TB <= '1'; WE_TB <= '1';
				nState <= 2;
				
			when 2 =>	-- program load state
				AddressTB <= to_unsigned(instrCount - 1, 32);
				Mem_Bus_in <= Instr_List(instrCount);
				
				if instrCount = W then
					nState <= 3;
				else
					instrCount <= instrCount + 1;
				end if;
				
			when 3 =>
				CS_TB <= '0'; WE_TB <= '0';
				init <= '0';
				nState <= 4;
				
			when 4 =>
				--rst <= '0';
				if RST = '0' then
					nState <= 5;
				end if;
				
			when 5 =>	-- mips run state
				--wait until WE = '1' and WE'event; -- When a store word is executed
				--wait until CLK_Mux = '0' and CLK_Mux'event;
				if WE = '1' and comparCount < N then
					comparCount <= comparCount + 1;
					ast <= '1';
				end if;
				if comparCount = N then
					report "Testing Finished.";
					nState <= 6;
				else
					ast <= '0';
				end if;
				
			when 6 =>	-- infinite loop state
				nState <= 6; 
		end case;
		end if;
	end process;
	
	process(CLK_Mux)
	begin
		if CLK_Mux = '0' and ast = '1' then
			assert(to_integer(Mem_Bus_out_TB) = expected(comparCount))
			report "Output mismatch:" severity error;
		end if;
	end process;
			
	process(CLK_Mux)
	begin
		if CLK_Mux = '1' and CLK_Mux'event then -- Go to next state
			state <= nState;
		end if;
		
		if SW(9) = '0' then -- Display lower
			if SW(2 downto 1) = "00" then -- Display Address
				HEX5 <=	"0001000"; 		-- A
				HEX4 <= sevenSeg(x"0"); -- 0
				HEX3 <= sevenSeg(Address(15 downto 12));
				HEX2 <= sevenSeg(Address(11 downto 8));
				HEX1 <= sevenSeg(Address(7 downto 4));
				HEX0 <= sevenSeg(Address(3 downto 0));
			elsif SW(2 downto 1) = "01" then -- Display data into MIPS
				HEX5 <=	"1111000"; 		-- |-
				HEX4 <= sevenSeg(x"0"); -- 0
				HEX3 <= sevenSeg(Mem_Bus_out(15 downto 12));
				HEX2 <= sevenSeg(Mem_Bus_out(11 downto 8));
				HEX1 <= sevenSeg(Mem_Bus_out(7 downto 4));
				HEX0 <= sevenSeg(Mem_Bus_out(3 downto 0));
			elsif SW(2 downto 1) = "10" then -- Display data out of MIPS
				HEX5 <=	"1001110"; 		-- -|
				HEX4 <= sevenSeg(x"0"); -- 0
				HEX3 <= sevenSeg(Mem_Bus_out_TB(15 downto 12));
				HEX2 <= sevenSeg(Mem_Bus_out_TB(11 downto 8));
				HEX1 <= sevenSeg(Mem_Bus_out_TB(7 downto 4));
				HEX0 <= sevenSeg(Mem_Bus_out_TB(3 downto 0));
			else
				HEX5 <=	"1111111"; -- Blanks for unintended latch
				HEX4 <= "1111111";
				HEX3 <= "1111111";
				HEX2 <= "1111111";
				HEX1 <= "1111111";
				HEX0 <= "1111111";
			end if;
				
		elsif SW(9) = '1' then -- Display Upper
			if SW(2 downto 1) = "00" then -- Display Address
				HEX5 <=	"0001000"; 		-- A
				HEX4 <= sevenSeg(x"1"); -- 1
				HEX3 <= sevenSeg(Address(31 downto 28));
				HEX2 <= sevenSeg(Address(27 downto 24));
				HEX1 <= sevenSeg(Address(23 downto 20));
				HEX0 <= sevenSeg(Address(19 downto 16));
			elsif SW(2 downto 1) = "01" then -- Display data into MIPS
				HEX5 <=	"1111000"; 		-- A
				HEX4 <= sevenSeg(x"1"); -- 1
				HEX3 <= sevenSeg(Mem_Bus_out(31 downto 28));
				HEX2 <= sevenSeg(Mem_Bus_out(27 downto 24));
				HEX1 <= sevenSeg(Mem_Bus_out(23 downto 20));
				HEX0 <= sevenSeg(Mem_Bus_out(19 downto 16));
			elsif SW(2 downto 1) = "10" then -- Display data out of MIPS
				HEX5 <=	"1001110"; 		-- A
				HEX4 <= sevenSeg(x"1"); -- 1
				HEX3 <= sevenSeg(Mem_Bus_out_TB(31 downto 28));
				HEX2 <= sevenSeg(Mem_Bus_out_TB(27 downto 24));
				HEX1 <= sevenSeg(Mem_Bus_out_TB(23 downto 20));
				HEX0 <= sevenSeg(Mem_Bus_out_TB(19 downto 16));
			else
				HEX5 <=	"1111111"; -- Blanks for unintended latch
				HEX4 <= "1111111";
				HEX3 <= "1111111";
				HEX2 <= "1111111";
				HEX1 <= "1111111";
				HEX0 <= "1111111";
			end if;
		else
				HEX5 <=	"1111111"; -- Blanks for unintended latch
				HEX4 <= "1111111";
				HEX3 <= "1111111";
				HEX2 <= "1111111";
				HEX1 <= "1111111";
				HEX0 <= "1111111";
		end if;	
	end process;
end test;