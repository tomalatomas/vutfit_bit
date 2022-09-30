library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_arith.all;
use IEEE.std_logic_unsigned.all;

-- rozhrani Vigenerovy sifry
entity vigenere is
   port(
         CLK : in std_logic;
         RST : in std_logic;
         DATA : in std_logic_vector(7 downto 0);
         KEY : in std_logic_vector(7 downto 0);

         CODE : out std_logic_vector(7 downto 0)
    );
end vigenere;

-- V souboru fpga/sim/tb.vhd naleznete testbench, do ktereho si doplnte
-- znaky vaseho loginu (velkymi pismeny) a znaky klice dle vaseho prijmeni.

architecture behavioral of vigenere is

    -- Sem doplnte definice vnitrnich signalu, prip. typu, pro vase reseni,
    -- jejich nazvy doplnte tez pod nadpis Vigenere Inner Signals v souboru
    -- fpga/sim/isim.tcl. Nezasahujte do souboru, ktere nejsou explicitne
    -- v zadani urceny k modifikaci.
	--------------------------------------
					 --SIGNALS--
	--------------------------------------
	 
	 signal shift: std_logic_vector(7 downto 0);
	 signal shiftPlus: std_logic_vector(7 downto 0);
	 signal shiftMinus: std_logic_vector(7 downto 0);	
	 signal presState : std_logic := '1';
	 signal nextState : std_logic := '0';
	 --1 pricitani , 0 odecitani
	 signal mealyOutput: std_logic_vector(1 downto 0);
	 --01 pricita , 10 odecita


begin
    -- Sem doplnte popis obvodu. Doporuceni: pouzivejte zakladni obvodove prvky
    -- (multiplexory, registry, dekodery,...), jejich funkce popisujte pomoci
    -- procesu VHDL a propojeni techto prvku, tj. komunikaci mezi procesy,
    -- realizujte pomoci vnitrnich signalu deklarovanych vyse.

    -- DODRZUJTE ZASADY PSANI SYNTETIZOVATELNEHO VHDL KODU OBVODOVYCH PRVKU,
    -- JEZ JSOU PROBIRANY ZEJMENA NA UVODNICH CVICENI INP A SHRNUTY NA WEBU:
    -- http://merlin.fit.vutbr.cz/FITkit/docs/navody/synth_templates.html.


	--------------------------------------
					--PROCESSES--
	--------------------------------------
		------------shiftCalc-----------------
	shiftCalc: process (KEY, DATA) is
		--variable ASCIIBEGIN: integer := 64;
	begin
		shift <= KEY - 64;
	end process;
	
	------------shiftCalcMinus------------
	shiftCalcMinus: process (shift, DATA) is
		variable dataModif: std_logic_vector(7 downto 0);
	begin
		dataModif := DATA-shift;
		if (65 > dataModif ) then --65 beginning of alphabet in ascii table
			dataModif := dataModif+26; -- +26 go to end of alphabet in ascii table
		end if;
		shiftMinus <= dataModif;
	end process;
	
	------------shiftCalcPlus-------------
	shiftCalcPlus: process (shift, DATA) is
		variable dataModif: std_logic_vector(7 downto 0);
	begin
		dataModif := DATA+shift;
		if (dataModif > 90) then --90=End of alphabet in ascii table
			dataModif := dataModif-26;-- -26=Back to beginning of alphabet in ascii table
		end if;
		shiftPlus <= dataModif;
	end process;
	------------presentStateRegister--------------
	presStateReg: process (CLK, RST) is
	begin
		if RST = '1' then 
			presState <= '1';
		elsif (CLK'event) and (CLK='1') then 
			presState <= nextState;
		end if;
	end process;
	
	------------nextStateRegister----------
	nextStateReg: process (presState, DATA, RST) is
	begin
	   -- default values
		nextState <= presState;
		case presState is 
			when '1' =>
				nextState <= '0';
				mealyOutput <= "01";
			when '0' => 
				nextState <= '1';
				mealyOutput <= "10";
			when others => null;
		end case;	
		if(RST = '1') or (DATA > 47 and DATA < 58) then -- If reset or a number
			mealyOutput <= "11";
		end if;		
	end process;
	
	------------multiplexor--------------
	multiplrexor: process(mealyOutput,shiftPlus,shiftMinus) is 
	begin
		case mealyOutput is
			when "01" => CODE <= shiftPlus;
			when "10" => CODE <= shiftMinus;
			when others => CODE <= "00100011"; --HASHTAG
		end case;
	end process;
end behavioral;
