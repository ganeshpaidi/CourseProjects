library std;
use std.standard.all;


library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;	 
use ieee.std_logic_unsigned.all;


entity Register is
	port ( data_in: in std_logic_vector(15 downto 0); 
	clk, wr: in std_logic;
	data_out: out std_logic_vector(15 downto 0));
end entity;

architecture REG_ARC of Register is 

	signal R: std_logic_vector(15 downto 0) := (others => '0');

begin
	data_out <= R;

	process (data_in,clk,wr)
		begin
		if(wr = '1') then
			if(rising_edge(clk)) then
				R <= data_in;
			end if;
		end if;
		end process;

end REG_ARC;
