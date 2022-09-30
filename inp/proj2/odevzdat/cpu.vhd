-- cpu.vhd: Simple 8-bit CPU (BrainF*ck interpreter)
-- Copyright (C) 2020 Brno University of Technology,
--                    Faculty of Information Technology
-- Author(s): DOPLNIT
--

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity cpu is
 port (
   CLK   : in std_logic;  -- hodinovy signal
   RESET : in std_logic;  -- asynchronni reset procesoru
   EN    : in std_logic;  -- povoleni cinnosti procesoru
 
   -- synchronni pamet ROM
   CODE_ADDR : out std_logic_vector(11 downto 0); -- adresa do pameti
   CODE_DATA : in std_logic_vector(7 downto 0);   -- CODE_DATA <- rom[CODE_ADDR] pokud CODE_EN='1'
   CODE_EN   : out std_logic;                     -- povoleni cinnosti
   
   -- synchronni pamet RAM
   DATA_ADDR  : out std_logic_vector(9 downto 0); -- adresa do pameti
   DATA_WDATA : out std_logic_vector(7 downto 0); -- ram[DATA_ADDR] <- DATA_WDATA pokud DATA_EN='1'
   DATA_RDATA : in std_logic_vector(7 downto 0);  -- DATA_RDATA <- ram[DATA_ADDR] pokud DATA_EN='1'
   DATA_WE    : out std_logic;                    -- cteni (0) / zapis (1)
   DATA_EN    : out std_logic;                    -- povoleni cinnosti 
   
   -- vstupni port
   IN_DATA   : in std_logic_vector(7 downto 0);   -- IN_DATA <- stav klavesnice pokud IN_VLD='1' a IN_REQ='1'
   IN_VLD    : in std_logic;                      -- data platna
   IN_REQ    : out std_logic;                     -- pozadavek na vstup data
   
   -- vystupni port
   OUT_DATA : out  std_logic_vector(7 downto 0);  -- zapisovana data
   OUT_BUSY : in std_logic;                       -- LCD je zaneprazdnen (1), nelze zapisovat
   OUT_WE   : out std_logic                       -- LCD <- OUT_DATA pokud OUT_WE='1' a OUT_BUSY='0'
 );
end cpu;


-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of cpu is

 -- zde dopiste potrebne deklarace signalu
 ------------------- PC -----------------------
	signal pc : std_logic_vector(11 downto 0); 		--Registr ProgramCounter
	signal pcInc : std_logic; 							--Signal inkrementace
   signal pcDec : std_logic; 							--Signal dekrementace
	signal pcLd : std_logic; 								--nacteni posledni adresy z RAS
 ------------------- RAS -----------------------
 	signal ras : std_logic_vector(191 downto 0); 	--Zasobnik navratovych PC pro [], 16x12 = 192 -> 191...0
	signal rasPush : std_logic; 							--Registr ProgramCounter
	signal rasPop : std_logic; 							--Registr ProgramCounter
 ------------------- CNT -----------------------
	signal cnt : std_logic_vector(7 downto 0); 	--Registr Count
	signal cntInc : std_logic; 							--Signal inkrementace
	signal cntDec : std_logic; 							--Signal dekrementace
	signal cntInsertOne : std_logic; 							--Signal dekrementace

 ------------------- PTR -----------------------
	signal ptr : std_logic_vector(9 downto 0);		--Registr ukazatel do pameti
	signal ptrInc : std_logic;							--Signal inkrementace
	signal ptrDec : std_logic;							--Signal dekrementace
	
	------------------- NULL -----------------------
	signal dataNull : std_logic;							--Null
 ------------------- Multiplexor -----------------------
 
	signal mulplexSel : std_logic_vector(1 downto 0) :=  (others => '0');
	signal mulplexOut : std_logic_vector(7 downto 0) :=  (others => '0');
	

 ------------------- FSM -----------------------
 type fsmState is (
	fsm_start,
	--CPU Stavy
	fsm_cpuFetch, fsm_cpuDecode, --fsm_cpuExecute, --Cviceni 4 slide 7
	--Stavy instrukci
	fsm_ptrInc, -- inkrementace hodnoty ukazatele, >
	fsm_ptrDec, -- dekrementace hodnoty ukazatele, <
	fsm_valInc, -- inkrementace hodnoty aktualni bunky +
	fsm_valInc2,
	fsm_valInc3,
	fsm_valDec, -- dekrementace hodnoty aktualni bunky -
	fsm_valDec2,
	fsm_valDec3,
	fsm_whileStart, --Zacatek whilu [
	fsm_whileStart2,
	fsm_whileStart3,
	fsm_whileStart4,
	fsm_whileEnd , -- konec whilu ]
	fsm_whileEnd2,
	fsm_write, -- vytiskni hodnotu aktualni bunky .
	fsm_write2,
	fsm_read, -- nacti hodnotu a uloz ji do aktualni bunky ,
	fsm_read2,
	fsm_null -- zastav vykonani programu null
 );
 signal presState : fsmState := fsm_start;
 signal nextState : fsmState;
begin
 -- zde dopiste vlastni VHDL kod
 -- pri tvorbe kodu reflektujte rady ze cviceni INP, zejmena mejte na pameti, ze 
 --   - nelze z vice procesu ovladat stejny signal,
 --   - je vhodne mit jeden proces pro popis jedne hardwarove komponenty, protoze pak
 --   - u synchronnich komponent obsahuje sensitivity list pouze CLK a RESET a 
 --   - u kombinacnich komponent obsahuje sensitivity list vsechny ctene signaly.
 
 -- Program counter PC --
	pcProc: process (RESET,CLK,pcInc,pcDec,pcLd)
	begin
		if (RESET='1') then
				pc <= (others=>'0');
		elsif (CLK'event) and (CLK='1') then
			if (pcInc='1') then
				pc <= pc+1;
			elsif (pcDec='1') then
				pc <= pc-1;
			elsif (pcLd='1') then
				pc <= ras(191 downto 180); --Posledni adresa
			end if;
		end if;
	end process;
	CODE_ADDR <= pc;
	
	-- PTR --
	ptrProc: process (RESET, CLK, ptrInc,ptrDec)
	begin
		if (RESET='1') then
				ptr <= (others=>'0'); 
		elsif (CLK'event) and (CLK='1') then
			if (ptrInc='1') then
				ptr <= ptr+1;
			elsif (ptrDec='1') then
				ptr <= ptr-1;
				-- Doplnit
			end if;
		end if;
	end process;
	DATA_ADDR <= ptr;
	  -- COUNTER --
	cntProc: process (RESET, CLK, cnt,cntInc,cntDec)
	begin
		if (RESET = '1') then
			cnt <= (others => '0');
		elsif (CLK'event) and (CLK = '1') then 
			if (cntInsertOne = '1') then
				cnt <= "00000001";
			elsif (cntInc = '1') then
				cnt <= cnt + 1;
			elsif (cntDec = '1') then
				cnt <= cnt - 1; 
			end if;
		end if;
	end process ;
  -- MULTIPLEXOR --
	mulplex: process (RESET, CLK, mulplexSel)
	begin
		if (RESET='1') then
				mulplexOut <= (others=>'0');
		elsif (CLK'event) and (CLK='1') then
			case mulplexSel is 
				when "00" => 
					mulplexOut <=  IN_DATA;
				when "01" => 
					mulplexOut <= DATA_RDATA+1;
				when "10" => 
					mulplexOut <= DATA_RDATA-1;
				when others =>
					mulplexOut <= (others=>'0');
			end case;
		end if;
	end process;
	DATA_WDATA <= mulplexOut;
	
--RAS registr
	rasReg: process(RESET, CLK,rasPush,rasPop)
	begin
		if (RESET = '1') then
			ras <= (others => '0');
		elsif (CLK'event) and (CLK = '1') then
			if (rasPush = '1') then
				ras<= pc & ras(191 downto 12); -- Naplni se cislem pc a zbytek posunutou predchozi hodnotou
			elsif (rasPop = '1') then
				ras <= ras(179 downto 0) & "000000000000"; -- Posune se registr a naplni se nulama    
			end if;
		end if;
	end process;
-- State registr --
	presStateReg: process (CLK, RESET, EN) is
	begin
		if RESET = '1' then 
			presState <= fsm_start;
		elsif (CLK'event) and (CLK='1') then 
			if EN = '1' then
			presState <= nextState;
			end if;
		end if;
	end process;
--FMS
	fsm: process (CODE_DATA, DATA_RDATA, IN_VLD, OUT_BUSY, presState,cnt) is
	begin
--Init hodnot
		CODE_EN <= '0';
		DATA_EN <= '0';
		DATA_WE <= '0';
		IN_REQ <= '0';
		OUT_WE <= '0';
		mulplexSel <= "00";
		pcInc <= '0';
		pcDec <= '0';
		pcLd <= '0';
		rasPush <= '0';
		rasPop <= '0';
		ptrInc <= '0';
		ptrDec <= '0';
		cntInc <= '0';
		cntDec <= '0';	
		cntInsertOne <= '0'; --Vlozi jednicku, obejiti upravy signalu ze dvou procesu

		case presState is
--Start
			when fsm_start =>
				nextState <= fsm_cpuFetch;
--Nacteni instrukce
			when fsm_cpuFetch =>
				nextState <= fsm_cpuDecode;
				CODE_EN <= '1';
--instrukce
			when fsm_cpuDecode =>
				case CODE_DATA is
					when X"3E" =>
						nextState <= fsm_ptrInc; -- dekrementace hodnoty ukazatele, <
					when X"3C" =>
						nextState  <= fsm_ptrDec; -- dekrementace hodnoty ukazatele, <
					when X"2B" =>
						nextState  <= fsm_valInc; -- inkrementace hodnoty aktualni bunky +
					when X"2D" =>
						nextState  <= fsm_valDec; -- dekrementace hodnoty aktualni bunky -
					when X"5B" =>
						nextState  <= fsm_whileStart; --Zacatek whilu [
					when X"5D" =>
						nextState  <= fsm_whileEnd; -- konec whilu ]
					when X"2E" =>
						nextState  <= fsm_write; -- vytiskni hodnotu aktualni bunky .
					when X"2C" =>
						nextState  <= fsm_read; -- nacti hodnotu a uloz ji do aktualni bunky
					when X"00" =>
						nextState  <= fsm_null; -- zastav vykonani programu null
					when others =>
						pcInc <= '1'; -- Instrukce, kterou nezname, preskocime a vracime se zpet na rozpoznani instrukce
						nextState <= fsm_cpuFetch;
				end case;
 -- PTR				
			when fsm_ptrInc =>
			--PTR ‹ PTR + 1, PC ‹ PC +
				nextState <= fsm_cpuFetch;
				ptrInc <= '1';
				pcInc <= '1';	
			when fsm_ptrDec =>
			--PTR ‹ PTR - 1, PC ‹ PC + 1
				nextState <= fsm_cpuFetch;
				ptrDec <= '1';
				pcInc <= '1';
 -- VAL 				
			when fsm_valInc =>
				nextState <= fsm_valInc2;
				DATA_EN <= '1';
				DATA_WE <= '0';
			when fsm_valInc2  =>
				--DATA RDATA ‹ ram[PTR]; ram[PTR] ‹ DATA RDATA + 1;
				mulplexSel <= "01";
				nextState <= fsm_valInc3;
			when fsm_valInc3 =>
				-- PC ‹ PC + 1
				pcInc <= '1';
				nextState <= fsm_cpuFetch;
				DATA_EN <= '1';
				DATA_WE <= '1';

				
			when fsm_valDec =>
				nextState <= fsm_valDec2;
				DATA_EN <= '1';
				DATA_WE <= '0';
			when fsm_valDec2 =>
			--DATA RDATA ‹ ram[PTR]; ram[PTR] ‹ DATA RDATA - 1;
				mulplexSel <= "10";
				nextState <= fsm_valDec3;
			when fsm_valDec3 =>
				-- PC ‹ PC + 1
				pcInc <= '1';
				nextState <= fsm_cpuFetch;
				DATA_EN <= '1';
				DATA_WE <= '1';
 -- WRITE
				
			when fsm_write =>
				nextState <= fsm_write2;
				DATA_EN <= '1';
				DATA_WE <= '0';
			when fsm_write2 =>
				if (OUT_BUSY = '1') then
					--while (OUT BUSY) {}
					DATA_EN <= '1';
					DATA_WE <= '0';
					nextState <= fsm_write2;
				else
				--OUT DATA ‹ ram[PTR], PC ‹ PC + 1
					OUT_DATA <= DATA_RDATA;
					OUT_WE <= '1';
					pcInc <= '1';
					nextState <= fsm_cpuFetch;
				end if;
 -- READ 

			when fsm_read =>
			--IN REQ ‹ 1
				nextState <= fsm_read2;
				mulplexSel <= "00";
				IN_REQ <= '1';
			when fsm_read2 =>
				IN_REQ <= '1';
				if (IN_VLD /= '1') then
					--while (!IN VLD) {}
					nextState <= fsm_read2;
					mulplexSel <= "00";
					IN_REQ <= '1';
				else 
					--ram[PTR] ‹ IN DATA, PC ‹ PC + 1
					nextState <= fsm_cpuFetch;
					DATA_EN <= '1';
					DATA_WE <= '1';
					pcInc <= '1';
				end if;
				
 -- WHILE
			when fsm_whileStart =>
			--PC ‹ PC + 1
 				nextState <= fsm_whileStart2;
				pcInc <= '1';
				DATA_EN <= '1';
				DATA_WE <= '0';
			when fsm_whileStart2 =>
				--if (ram[PTR] == 0)			
				if DATA_RDATA = "000000000000" then 
				--CNT ‹ 1
					nextState <= fsm_whileStart3;
					cntInsertOne <=  '1';
				else 
					--RAS.push(PC)
					rasPush <= '1';
					nextState <= fsm_cpuFetch;
				end if;

			when fsm_whileStart3 =>
				--while (CNT != 0)
				if (cnt = "00000000") then
					nextState <= fsm_cpuFetch;
				else
					-- c ‹ rom[PC]
					CODE_EN <= '1';
					nextState<= fsm_whileStart4;
				end if;
				
			when fsm_whileStart4 =>
				-- if (c == '[') CNT ‹ CNT + 1 elsif (c == ']') CNT ‹ CNT - 1
				if (CODE_DATA = X"5D") then 
					cntDec <= '1';
				elsif (CODE_DATA = X"5B") then
					 cntInc <= '1';
				end if;
				pcInc <= '1';
				nextState <= fsm_whileStart3; -- navrat na zacatek while
				
			when fsm_whileEnd =>
 				nextState <= fsm_whileEnd2;
				DATA_EN <= '1';
				DATA_WE <= '0';
			when fsm_whileEnd2 =>
				--if (ram[PTR] == 0)			
				if DATA_RDATA = "000000000000" then 
					--PC ‹ PC + 1
					pcInc <= '1';
					--RAS.pop()
					rasPop <= '1';
					nextState <= fsm_cpuFetch;
				else 
					--PC ‹ RAS.top()
					pcLd <= '1';
					nextState <= fsm_cpuFetch;
				end if;		
			when fsm_null =>
				--PC ‹ PC
					nextState <= fsm_null;
		end case;
	end process;
end behavioral;
 
