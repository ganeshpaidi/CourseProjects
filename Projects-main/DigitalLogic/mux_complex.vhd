library ieee;
use ieee.std_logic_1164.all;
 
entity mux_complex is 
	port (a1,b1 :in std_logic_vector(15 downto 0) ;
	sel :in std_logic ;
	c1 : out std_logic_vector(15 downto 0) );
end mux_complex;

architecture MUX16_ARC of mux_complex is 

	component mux_basic is
		port (x,y,sel1 :in std_logic ;
		d: out std_logic);
	end component ;
	
begin
	
	MUX1: mux_basic 
	port map (x => a1(0), y => b1(0), sel1 => sel, d=> c1(0));
	
	MUX2: mux_basic 
	port map (x => a1(1), y => b1(1), sel1 => sel, d=> c1(1));
	
	MUX3: mux_basic 
	port map (x => a1(2), y => b1(2), sel1 => sel, d=> c1(2));
	
	MUX4: mux_basic 
	port map (x => a1(3), y => b1(3), sel1 => sel, d=> c1(3));
	
	MUX5: mux_basic 
	port map (x => a1(4), y => b1(4), sel1 => sel, d=> c1(4));
	
	MUX6: mux_basic 
	port map (x => a1(5), y => b1(5), sel1 => sel, d=> c1(5));
	
	MUX7: mux_basic 
	port map (x => a1(6), y => b1(6), sel1 => sel, d=> c1(6));
	
	MUX8: mux_basic 
	port map (x => a1(7), y => b1(7), sel1 => sel, d=> c1(7));
	
	MUX9: mux_basic 
	port map (x => a1(8), y => b1(8), sel1 => sel, d=> c1(8));
	
	MUX10: mux_basic 
	port map (x => a1(9), y => b1(9), sel1 => sel, d=> c1(9));
	
	MUX11: mux_basic 
	port map (x => a1(10), y => b1(10), sel1 => sel, d=> c1(10));
	
	MUX12: mux_basic 
	port map (x => a1(11), y => b1(11), sel1 => sel, d=> c1(11));
	
	MUX13: mux_basic 
	port map (x => a1(12), y => b1(12), sel1 => sel, d=> c1(12));
	
	MUX14: mux_basic 
	port map (x => a1(13), y => b1(13), sel1 => sel, d=> c1(13));
	
	MUX15: mux_basic 
	port map (x => a1(14), y => b1(14), sel1 => sel, d=> c1(14));
	
	MUX16: mux_basic 
	port map (x => a1(15), y => b1(15), sel1 => sel, d=> c1(15));
	

end MUX16_ARC;