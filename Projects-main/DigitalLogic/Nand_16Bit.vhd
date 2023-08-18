library ieee;
use ieee.std_logic_1164.all;
 
entity Nand_16Bit is 
	port (a1,b1 :in std_logic_vector(15 downto 0) ;
	c1 : out std_logic_vector(15 downto 0) );
end Nand_16Bit;

architecture NAND16_ARC of Nand_16Bit is 

begin

	c1(0) <= a1(0) nand b1(0) ;
	c1(1) <= a1(1) nand b1(1) ;
	c1(2) <= a1(2) nand b1(2) ;
	c1(3) <= a1(3) nand b1(3) ;
	c1(4) <= a1(4) nand b1(4) ;
	c1(5) <= a1(5) nand b1(5) ;
	c1(6) <= a1(6) nand b1(6) ;
	c1(7) <= a1(7) nand b1(7) ;
	c1(8) <= a1(8) nand b1(8) ;
	c1(9) <= a1(9) nand b1(9) ;
	c1(10) <= a1(10) nand b1(10) ;
	c1(11) <= a1(11) nand b1(11) ;
	c1(12) <= a1(12) nand b1(12) ;
	c1(13) <= a1(13) nand b1(13) ;
	c1(14) <= a1(14) nand b1(14) ;
	c1(15) <= a1(15) nand b1(15) ;
	
end NAND16_ARC;