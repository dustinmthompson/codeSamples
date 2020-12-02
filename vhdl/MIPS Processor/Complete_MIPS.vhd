library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all; 

entity Complete_MIPS is
	port(CLK, RST: in std_logic;
		A_Out, D_Out: out unsigned(31 downto 0));
end Complete_MIPS;	

architecture model of Complete_MIPS is
component MIPS is
	port(CLK, RST: in std_logic;
		CS, WE: out std_logic;
		ADDR: out unsigned(31 downto 0);
		Mem_Bus_in:  in unsigned(31 downto 0);
		Mem_Bus_out: out unsigned(31 downto 0));
end component;	  

component Memory is
	port(CS, WE, Clk: in std_logic;
		ADDR: in unsigned(31 downto 0);
		Mem_Bus_in:  in unsigned(31 downto 0);
		Mem_Bus_out: out unsigned(31 downto 0));
end component; 

signal CS, WE: std_logic;
signal ADDR, Mem_Bus_in, Mem_Bus_out: unsigned(31 downto 0);  

begin
	CPU: MIPS port map (CLK, RST, CS, WE, ADDR, Mem_Bus_in, Mem_Bus_out);
	MEM: Memory port map (CS, WE, CLK, ADDR, Mem_Bus_in, Mem_Bus_out);
	A_Out <= ADDR;
	D_Out <= Mem_Bus_out;
end model;