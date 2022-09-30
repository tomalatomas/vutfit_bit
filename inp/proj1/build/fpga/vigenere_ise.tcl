#============================================================================
# Run: 
#    xtclsh vigenere_ise.tcl  - creates XILINX ISE project file
#    ise vigenere_project.ise - opens the project
#============================================================================
source "../../../../base/xilinxise.tcl"

project_new "vigenere_project"
project_set_props
puts "Adding source files"
xfile add "../../fpga/vigenere.vhd"
puts "Adding simulation files"
xfile add "../../fpga/sim/tb.vhd" -view Simulation
puts "Libraries"
project_set_isimscript "vigenere_xsim.tcl"
project_set_top "vigenere"
project_close
