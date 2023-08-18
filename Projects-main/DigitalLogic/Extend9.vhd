library ieee;
use ieee.std_logic_1164.all;

entity Extend9 is
	port (a: in std_logic_vector(8 downto 0);
	oper: in std_logic;
	c: out std_logic_vector(15 downto 0));
end entity ;

architecture Extender9  of Extend9 is

begin

	process(a,oper)

	begin
		if(oper='1') then
			c(15 downto 7) <= a(8 downto 0);
			c(6) <= '0';
			c(5) <= '0'; 
			c(4) <= '0';
			c(3) <= '0'; 
			c(2) <= '0';
			c(1) <= '0';
			c(0) <= '0';
		else 
			c(8 downto 0) <= a(8 downto 0);		
			c(15) <= a(8);
			c(14) <= a(8); 
			c(13) <= a(8);
			c(12) <= a(8); 
			c(11) <= a(8);
			c(10) <= a(8);
			c(9) <= a(8);
 
		end if;
	end process;

end Extender9 ;