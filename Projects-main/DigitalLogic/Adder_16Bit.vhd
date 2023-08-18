library ieee;
use ieee.std_logic_1164.all;
 
entity Adder_16Bit is 
	port ( a1,b1 :in std_logic_vector(15 downto 0) ;
	c1 : out std_logic_vector(15 downto 0);
	cout1: out std_logic );
end Adder_16Bit;

architecture RippleAdder of Adder_16Bit is 

	component FullAdder is
		port (x,y,cin :in std_logic ;
		sum,cout: out std_logic );
	end component ;

	signal P : std_logic_vector(14 downto 0) ;
	
begin 
	
	FA0: FullAdder 
	port map (x => a1(0), y => b1(0), cin => '0', sum=> c1(0), cout=> P(0));
	
	FA1: FullAdder 
	port map (x => a1(1), y => b1(1), cin => P(0), sum=> c1(1), cout=> P(1));
	
	FA2: FullAdder 
	port map (x => a1(2), y => b1(2), cin => P(1), sum=> c1(2), cout=> P(2));
	
	FA3: FullAdder 
	port map (x => a1(3), y => b1(3), cin => P(2), sum=> c1(3), cout=> P(3));
	
	FA4: FullAdder 
	port map (x => a1(4), y => b1(4), cin => P(3), sum=> c1(4), cout=> P(4));	
	
	FA5: FullAdder 
	port map (x => a1(5), y => b1(5), cin => P(4), sum=> c1(5), cout=> P(5));
	
	FA6: FullAdder 
	port map (x => a1(6), y => b1(6), cin => P(5), sum=> c1(6), cout=> P(6));
	
	FA7: FullAdder 
	port map (x => a1(7), y => b1(7), cin => P(6), sum=> c1(7), cout=> P(7));
	
	FA8: FullAdder 
	port map (x => a1(8), y => b1(8), cin => P(7), sum=> c1(8), cout=> P(8));
	
	FA9: FullAdder 
	port map (x => a1(9), y => b1(9), cin => P(8), sum=> c1(9), cout=> P(9));
	
	FA10: FullAdder 
	port map (x => a1(10), y => b1(10), cin => P(9), sum=> c1(10), cout=> P(10));
	
	FA11: FullAdder 
	port map (x => a1(11), y => b1(11), cin => P(10), sum=> c1(11), cout=> P(11));
	
	FA12: FullAdder 
	port map (x => a1(12), y => b1(12), cin => P(11), sum=> c1(12), cout=> P(12));	
	
	FA13: FullAdder 
	port map (x => a1(13), y => b1(13), cin => P(12), sum=> c1(13), cout=> P(13));
	
	FA14: FullAdder 
	port map (x => a1(14), y => b1(14), cin => P(13), sum=> c1(14), cout=> P(14));
	
	FA15: FullAdder 
	port map (x => a1(15), y => b1(15), cin => P(14), sum=> c1(15), cout=> cout1);


end RippleAdder;



--1 bit FULL ADDER.

library ieee;
use ieee.std_logic_1164.all;
 
entity FullAdder is 
	port (x, y, cin : in std_logic ;
	sum, cout : out std_logic );
end FullAdder;


architecture FA of FullAdder is

begin

	sum <= x xor ( y xor cin ) ;
	cout <= ( x and y ) or ( x and cin ) or ( cin and y );
	
end FA;