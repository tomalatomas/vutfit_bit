proc isim_script {} {

   add_divider "Signals of the Vigenere Interface"
   add_wave_label "" "CLK" /testbench/clk
   add_wave_label "" "RST" /testbench/rst
   add_wave_label "-radix ascii" "DATA" /testbench/tb_data
   add_wave_label "-radix ascii" "KEY" /testbench/tb_key
   add_wave_label "-radix ascii" "CODE" /testbench/tb_code

   add_divider "Vigenere Inner Signals"
   #add_wave_label "" "state" /testbench/uut/state
   # sem doplnte vase vnitrni signaly. chcete-li v diagramu zobrazit desitkove
	#add_wave_label "-radix ascii" "shiftNumber" /testbench/uut/shiftNumber
   # cislo, vlozte do prvnich uvozovek: -radix dec
	add_wave_label "-radix unsigned" "shift" /testbench/uut/shift
	add_wave_label "-radix ascii" "shiftPlus" /testbench/uut/shiftPlus
	add_wave_label "-radix ascii" "shiftMinus" /testbench/uut/shiftMinus
	add_wave_label "" "prevState" /testbench/uut/presState
	add_wave_label "" "nextState" /testbench/uut/nextState
	add_wave_label "" "fsmOutput" /testbench/uut/mealyOutput

   run 8 ns
}
