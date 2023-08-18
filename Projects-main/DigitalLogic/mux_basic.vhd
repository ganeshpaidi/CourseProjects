library ieee;
use ieee.std_logic_1164.all;
 
entity mux_basic is 

port (x,y,sel1 :in std_logic ;
		d: out std_logic);
		
end mux_basic;


architecture arc of mux_basic is

begin

	signal p,q,r: std_logic;
	
	component or_gate
		port (a,b: in std_logic; c: out std_logic);
	end component;
	
	component not_gate
		port (a: in std_logic; b: out std_logic);
	end component;
	
	component and_gate
		port (a,b: in std_logic; c: out std_logic);
	end component;
	
begin
	gate_1: not_gate
		port map(a=>sel1,b=>p);
	gate_2: and_gate
		port map(a=>x,b=>p,c=>q);
	gate_3: and_gate
		port map(a=>y,b=>sel1,c=>r);
	gate_4: or_gate
		port map(a=>q,b=>r,c=>d);
	 
end arc;

library ieee;
use ieee.std_logic_1164.all;

entity not_gate is
	port(a: in std_logic; b: out std_logic);
end entity;

architecture not_arc of not_gate is
begin
	b <= not a;
end architecture;

library ieee;
use ieee.std_logic_1164.all;

entity or_gate is
	port(a,b: in std_logic; c: out std_logic);
end entity;

architecture or_arc of or_gate is
begin
	c <= a or b;
end architecture;

library ieee;
use ieee.std_logic_1164.all;

entity and_gate is
	port(a,b: in std_logic; c: out std_logic);
end entity;

architecture and_arc of and_gate is
begin
	c <= a and b;
end architecture;