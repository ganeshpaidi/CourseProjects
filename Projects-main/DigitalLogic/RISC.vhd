library std;
use std.standard.all;

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use ieee.std_logic_unsigned.all;

entity RISC is

	port (clk,rst : in  std_logic);
	 
end entity;

architecture RISC_ARC of RISC is
	
	component Reg_File is 
		port (address1, address2,	address3: in std_logic_vector(2 downto 0);
		data_in: in std_logic_vector(15 downto 0); 
		clk, wr: in std_logic;
		data_out1, data_out2: out std_logic_vector(15  downto 0));
	end component;

	component mem is 
		port (address1,address2,data_in: in std_logic_vector(15 downto 0); 
		clk,wr: in std_logic;
		data_out: out std_logic_vector(15 downto 0));
	end component;

	component ALU is 
		port (a,b :in std_logic_vector(15 downto 0) ;
		op : in std_logic;
		c : out std_logic_vector(15 downto 0);
		z ,cout: out std_logic);
	end component;

	component Extend6 is
		port (a: in std_logic_vector(5 downto 0);
		c: out std_logic_vector(15 downto 0));
	end component; 

	component Extend9 is
		port (a: in std_logic_vector(8 downto 0);
		op: in std_logic;
		c: out std_logic_vector(15 downto 0));
	end component;

	type st is (s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11, s12, s13, s14, s15, s16, s17, s18, s19, s20, s21, s22, s23, slast);
	signal state: st :=s0;
	signal ra1, ra2, ra3 : std_logic_vector(2 downto 0);
	signal rd1, rd2, din1, a_1, b_1, c_1, addr1, addr2, din2, dout, co_1, co_2 : std_logic_vector(15 downto 0);
	signal wr1, cout_1, z_1, op_1, wr2, op_2 : std_logic ;
	signal zero, carry : std_logic := '0';
	signal addr, ir, t1, t2, t3, t4 : std_logic_vector(15 downto 0) := "0000000000000000";
	signal opcode : std_logic_vector(3 downto 0) :="1111";
	signal imm_9 : std_logic_vector(8 downto 0);
	signal imm_6 : std_logic_vector(5 downto 0);

begin

	COMPONENT_1 : Reg_File 
	port map (address1 => ra1, address2 => ra2, address3 => ra3, data_in => din1, clk => clk, wr => wr1, data_out1 => rd1, data_out2 => rd2);

	COMPONENT_2 : ALU 
	port map (a => a_1, b => b_1,op => op_1, c => c_1, z => z_1, cout => cout_1);
	
	COMPONENT_3 : mem 
	port map ( address1 => addr1, address2 => addr2, data_in => din2, data_out => dout, wr => wr2, clk => clk);
	
	COMPONENT_4 : Extend6 
	port map ( a => imm_6, c => co_1 );
	
	COMPONENT_5 : Extend9 
	port map ( a => imm_9, op => op_2, c => co_2 );

	
process(clk, state, addr, ir, t1, t2, t3, t4, zero ,carry ,opcode )	

	variable next_state : st;
	variable t1_next, t2_next, t3_next, t4_next, ir_next, addr_next : std_logic_vector(15 downto 0);
	variable opcode_next : std_logic_vector(3 downto 0);
	variable zero_next,carry_next : std_logic;
	
	begin
	
	next_state :=state;
	t1_next := t1;
	t2_next := t2;
	t3_next := t3;
	t4_next := t4;
	ir_next := ir;
	addr_next := addr;
	opcode_next := opcode;
	zero_next := zero;
	carry_next := carry;
	
	case state is
		when s0 =>
			wr1 <= '0';
			wr2 <= '0';
			addr1 <= addr;
			ir_next := dout;
			opcode_next := ir_next(15 downto 12);
			t1_next := "0000000000000000";
			t2_next := "0000000000000000";
			t3_next := "0000000000000000";
			t4_next := "0000000000000000";
			case opcode_next is
				when "0000" =>
					next_state :=s1;
				when "0001" =>
					next_state :=s4;
				when "0010" =>
					next_state :=s1;
				when "0011" =>
					next_state :=s15;
				when "0100" =>
					next_state :=s4;
				when "0101" =>
					next_state :=s4;
				when "0110" =>
					next_state :=s18;
				when "0111" =>
					next_state :=s18;
				when "1100" =>
					next_state :=s1;
				when "1000" =>
					next_state :=s17;
				when "1001" =>
					next_state :=s1;
				when others => null;
					next_state :=slast;
			end case;
			
		when s1 =>
			ra1 <= ir_next(11 downto 9);
			ra2 <= ir_next(8 downto 6);
			t1_next := rd1;
			t2_next := rd2;
			if ( opcode_next = "0000" and ( ( ir_next(1 downto 0) = "00" ) or ( ir_next(1 downto 0) = "10" and carry_next = '1' ) or ( ir_next(1 downto 0) = "01" and zero_next = '1' ) ) ) then
				next_state := s2 ;
			elsif ( opcode_next = "0010" and ( ( ir_next(1 downto 0) = "00" ) or ( ir_next(1 downto 0) = "10" and carry_next = '1' ) or ( ir_next(1 downto 0) = "01" and zero_next = '1' ) ) ) then 
				next_state := s23;
			elsif ( opcode_next = "1100" ) then
				next_state := s6;
			elsif ( opcode_next = "1001" ) then
				next_state := s13;
			else
				next_state := slast;
			end if;
			
		when s2 =>
			wr1 <= '0';
			wr2 <= '0';
			op_1 <= '0';
			a_1 <= t1_next;
			b_1 <= t2_next;
			t3_next := c_1;
			zero_next := z_1;
			carry_next := cout_1;
			next_state := s3;
			
		when s23 =>
			wr1 <= '0';
			wr2 <= '0';
			op_1 <= '1';
			a_1 <= t1_next;
			b_1 <= t2_next;
			t3_next := c_1;
			zero_next := z_1;
			next_state := s3;
			
		when s3 =>
			wr1 <= '1';
			wr2 <= '0';
			ra3 <= ir_next(5 downto 3);
			din1 <= t3_next;
			next_state := slast;
					
		when s4 =>
			wr1 <= '0';
			wr2 <= '0';
			imm_6 <= ir_next(5 downto 0);
			t4_next := co_1;
			if( ( opcode_next = "0100" ) or ( opcode_next = "0101" ) ) then
				next_state := s5;
			elsif ( opcode_next = "1100" ) then
				next_state := s8;
			else
				next_state := s7;
			end if;

		when s6 =>
			wr1 <= '0';
			wr2 <= '0';
			if ( t1_next=t2_next ) then
				next_state := s4;
			else 
				next_state := slast;
			end if;
			
		when s8 =>
			wr1 <= '0';
			wr2 <= '0';
			op_1 <= '0';
			a_1 <= t4_next;
			b_1 <= addr;
			addr_next := c_1;
			if ( opcode_next = "1000" ) then
				next_state := s14;
			else
				next_state := s0;
			end if;
			
		when s7 =>
			wr1 <= '0';
			wr2 <= '0';
			op_1 <= '0';
			ra1 <= ir_next(11 downto 9);
			t1_next := rd1;
			a_1 <= t1_next;
			b_1 <= t4_next;
			t3_next := c_1;
			zero_next := z_1;
			carry_next := cout_1;
			next_state := s9;
			
		when s9 =>
			wr1 <= '1';
			wr2 <= '0';
			ra3 <= ir_next(8 downto 6);
			din1 <= t3_next;
			next_state := slast;
			
		when s5 =>
			wr1 <= '0';
			wr2 <= '0';
			op_1 <= '0';			
			ra1 <= ir_next(8 downto 6);
			t1_next := rd1;
			a_1 <= t1_next;
			b_1 <= t4_next;
			t3_next := c_1;
			if ( opcode_next = "0100" ) then
				next_state := s10;
			elsif ( opcode_next = "0101" ) then
				next_state := s11;
			end if;
			
		when s10 =>
			wr1 <= '0';
			wr2 <= '0';
			addr1 <= t3_next;
			t2_next := dout;
			next_state := s12;

		when s12 =>
			wr1 <= '1';
			wr2 <= '0';
			ra3 <= ir_next(11 downto 9);
			din1 <= t2_next;
			zero_next := not ( t2_next(0) or t2_next(1) or t2_next(2) or t2_next(3) or t2_next(4) or t2_next(5) or t2_next(6) or t2_next(7) or t2_next(8) or t2_next(9) or t2_next(10) or t2_next(11) or t2_next(12) or t2_next(13) or t2_next(14) or t2_next(15) );
			next_state := slast;

		when s11 =>
			wr1 <= '0';
			wr2 <= '1';
			ra2 <= ir_next(11 downto 9);
			t2_next := rd2;
			addr2 <= t3_next;
			din2 <= t2_next;
			next_state := slast;

		when s13 =>
			wr1 <= '0';
			wr2 <= '0';
			addr_next := t2_next;
			next_state := s14;
			
		when s14 =>
			wr1 <= '1';
			wr2 <= '0';
			ra3 <= ir_next(11 downto 9);
			din1 <= addr;
			next_state := s0;
			
		when s15 => 
			wr1 <= '0';
			wr2 <= '0';
			op_2 <= '1';
			imm_9 <= ir_next(8 downto 0);
			t1_next := co_2;
			next_state := s16;
			
		when s16 => 
			wr1 <= '1';
			wr2 <= '0';
			ra3 <= ir_next(11 downto 9);
			din1 <= t1_next;
			next_state := slast;

		when s17 =>
			wr1 <= '0';
			wr2 <= '0';
			op_2 <= '0';
			imm_9 <= ir_next(8 downto 0);
			t4_next := co_2;
			next_state := s8;
			
		when s18 =>
			wr1 <= '0';
			wr2 <= '0';
			ra1 <= ir_next(11 downto 9);
			t1_next := rd1;
			t2_next := "0000000000000000";
			t3_next := "0000000000000001";
			if ( opcode_next = "0110" ) then
				next_state := s19;
			elsif ( opcode_next = "0111" ) then
				next_state := s20;
			end if;
			
		when s19 =>
			wr1 <= '1';
			wr2 <= '0';
			addr1 <= t1_next;
			t4_next := dout;
			ra3 <= t2_next(2 downto 0);
			din1 <= t4_next;
			next_state := s21;
			
		when s20 =>	
			wr1 <= '0';
			wr2 <= '1';
			ra1 <= t2_next(2 downto 0);
			t4_next := rd1;
			addr2 <= t1_next;
			din2 <= t4_next;
			next_state := s21;
			
		when s21 =>
			wr1 <= '0';
			wr2 <= '0';
			op_1 <= '0';
			a_1 <= t2_next;
			b_1 <= t3_next;
			t2_next := c_1;
			next_state := s22;
		
		when s22 =>
			wr1 <= '0';
			wr2 <= '0';
			op_1 <= '0';
			a_1 <= t1_next;
			b_1 <= t3_next;
			t1_next := c_1;
			if ( unsigned(t2_next)<8 ) then
				if ( opcode_next = "0110" ) then
					next_state := s19;
				else
					next_state := s20;
				end if;
			else
				next_state := slast;
			end if;
			
		when slast =>
			wr1 <= '0';
			wr2 <= '0';
			op_1 <= '0';
			a_1 <= "0000000000000001";
			b_1 <= addr_next;
			addr_next := c_1;
			next_state :=s0;
			
		when others => null;
	end case;

	if(rising_edge(clk)) then 
		if( rst='1' ) then
			t1 <= "0000000000000000";
			t2 <= "0000000000000000";
			t3 <= "0000000000000000";
			t4 <= "0000000000000000";
			zero <= '0';
			carry <= '0';
			opcode <= "1111";
			addr <= "0000000000000000";
			ir <= "0000000000000000";
			state <= s0;
		elsif(rst = '0') then
			t1 <= t1_next;
			t2 <= t2_next;
			t3 <= t3_next;
			t4 <= t4_next;
			zero <= zero_next;
			carry <= carry_next;
			opcode <= opcode_next;
			addr <= addr_next;
			ir <= ir_next;
			state <= next_state;
		end if;
	end if;

end process;
end RISC_ARC;