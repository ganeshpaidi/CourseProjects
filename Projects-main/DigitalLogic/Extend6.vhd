library ieee;
use ieee.std_logic_1164.all;

entity Extend6 is
	port (a: in std_logic_vector(5 downto 0);
	c: out std_logic_vector(15 downto 0));
end entity ;

architecture Extender6  of Extend6 is

begin
	
	c(5 downto 0) <= a(5 downto 0); 	
	c(15) <= a(5);
	c(14) <= a(5); 
	c(13) <= a(5);
	c(12) <= a(5); 
	c(11) <= a(5);
	c(10) <= a(5);
	c(9) <= a(5);
	c(8) <= a(5);
	c(7) <= a(5);
	c(6) <= a(5);

end Extender6 ;