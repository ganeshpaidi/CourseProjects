library std;
use std.standard.all;



library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use ieee.std_logic_unsigned.all;



entity Reg_File is 

	port (	address1, address2,	address3: in std_logic_vector(2 downto 0);
			data_in: in std_logic_vector(15 downto 0); 
			clk, wr: in std_logic;
			data_out1, data_out2: out std_logic_vector(15  downto 0));
			
end entity;

architecture RF_Arc of Reg_File is
 
	type arra is array(7 downto 0) of std_logic_vector(15 downto 0);   
	signal reg: arra:= ( 0 => x"0014",
								1 => x"0046",
								others => x"0010");

begin

	data_out1 <= reg(to_integer(unsigned(address1)));
	data_out2 <= reg(to_integer(unsigned(address2)));

		process (data_in,address3,clk,wr)

		begin
		if(wr = '1') then
			if(rising_edge(clk)) then
				reg(to_integer(unsigned(address3))) <= data_in;
			end if;
		end if;
		
		end process;

end RF_Arc;
