library std;
use std.standard.all;

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use ieee.std_logic_unsigned.all;

entity mem is 
	port ( address1,address2,data_in: in std_logic_vector(15 downto 0);
	clk,wr: in std_logic;
	data_out: out std_logic_vector(15 downto 0));
end mem;

architecture MEM_ARC of mem is 

	type memo is array(65535 downto 0) of std_logic_vector(15 downto 0);

	signal m: memo:=(
		0 => x"4054",
		1 => x"1010",
		2 => x"2042",
		3 => x"3210",
		4 => x"54c3",
		5 => x"63be",
		6 => x"73fb", 
		7 => x"8038",
		8 => x"933a", 
		9 => x"c079", 
		10 => x"1f86",
		11 => x"4f9f",
		12 => x"c9c2",
		13 => x"abcd",
		14 => x"8e02",
		15 => x"1234", 
		16 => x"7caa", 
		17 => x"91c0",
		128 => x"ffff",
		129 => x"0002", 
		130 => x"0000", 
		131 => x"0000", 
		132 => x"0001", 
		133 => x"0000",
		others => x"ffff");

begin

	data_out <= m(to_integer(unsigned(address1)));
	
	process (data_in,address2,clk,wr)
	
		begin
		
		if(wr = '1') then
			if(rising_edge(clk)) then
				m(to_integer(unsigned(address2))) <= data_in;
			end if;
		end if;
		
	end process;

end MEM_ARC;