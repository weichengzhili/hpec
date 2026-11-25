# timing —— 时序分析与收敛约束
# 对应文档: ../05-fpga/fpga-03-时序分析与收敛.md
# 职责: 时钟、输入输出延迟、时序例外约束（SDC）。

# ---- 主时钟约束 ----
create_clock -name clk_main -period 10.000 [get_ports clk]     # 100MHz
create_clock -name clk_serdes -period 5.000 [get_pins serdes/clk]  # 200MHz

# ---- 生成时钟（PLL/MMCM 输出） ----
# create_generated_clock -name clk_div -source [get_pins pll/clkin] \
#   -divide_by 2 [get_pins pll/clkout0]

# ---- 输入/输出延迟约束 ----
set_input_delay -clock clk_main -max 2.000 [get_ports din]
set_input_delay -clock clk_main -min 0.500 [get_ports din]
set_output_delay -clock clk_main -max 2.000 [get_ports dout]
set_output_delay -clock clk_main -min 0.500 [get_ports dout]

# ---- 跨时钟域（见 cdc 模块，伪路径约束） ----
set_false_path -from [get_clocks clk_main] -to [get_clocks clk_serdes]
set_false_path -from [get_clocks clk_serdes] -to [get_clocks clk_main]

# ---- 多周期路径例外（谨慎使用，见文档） ----
# set_multicycle_path 2 -from [get_pins ...] -to [get_pins ...]
