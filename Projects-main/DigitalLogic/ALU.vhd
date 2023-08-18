library ieee;
use ieee.std_logic_1164.all;

entity ALU is 

port (a,b :in std_logic_vector(15 downto 0) ;
		opcode : in std_logic;
		c : out std_logic_vector(15 downto 0);
		z ,cout: out std_logic);
end ALU;

architecture RISC_ALU of ALU is 

	component mux_complex is 
		port (a1,b1 :in std_logic_vector(15 downto 0) ;
		sel :in std_logic ;
		c1 : out std_logic_vector(15 downto 0));
	end component;

	component Adder_16Bit is 
		port (a1,b1 :in std_logic_vector(15 downto 0) ;
		c1 : out std_logic_vector(15 downto 0);
		cout1: out std_logic);
	end component;

	component Nand_16Bit is 
		port (a1,b1 :in std_logic_vector(15 downto 0) ;
		c1 : out std_logic_vector(15 downto 0) );
	end component;
	
	signal P1,P2,P3 : std_logic_vector(15 downto 0) ;
	signal Q :std_logic;
	
begin

	Adder : Adder_16Bit
	port map (a,b,P1,Q);

	NAND : Nand_16Bit 
	port map (a,b,P2);

	MUX : mux_complex 
	port map (P1,P2,opcode,P3);

	c <= P3;	
	cout <= Q and not opcode;
	z <= not ( P3(0) or P3(1) or P3(2) or P3(3) or P3(4) or P3(5) or P3(6) or P3(7) or P3(8) or P3(9) or P3(10) or P3(11) or P3(12) or P3(13) or P3(14) or P3(15) );

end RISC_ALU;