
# H 01001000 -> 72
# e 01100101 -> 101
# l 01101100 -> 108
# l 01101100 -> 108
# o 01101111 -> 111
# space 11111110 -> 254
# W 01010111 -> 87
# o 01101111 -> 111
# r 01110010 -> 114
# l 01101100 -> 108
# d 01100100 -> 100
# ! 00100001 -> 33

%define data_H 72 
%define data_e 101 
%define data_l 108
%define data_o 111
%define data_SPACE 254
%define data_W 87
%define data_r 114
%define data_d 100
%define data_EXCLAMATION 33

%macro
send_command $value
	# set rs low
	# set byte to data
	# pulse enable low. It starts at low, goes high for >450ns, goes low

	out 0 0 
	out 1 $value
	out 0 2
	out 0 0
%endm

%macro
send_data $value
	# set rs high
	# set byte to data
	# pulse enable low. It starts at low, goes high for >450ns, goes low

	out 0 1 
	out 1 $value
	out 0 3
	out 0 1
%endm

%macro
disable_lcd 
	out 0 0
%endm

start:
	# disable_lcd

	# sets mode to bit and 16x2
	send_command 60
	send_command 60
	send_command 60
	send_command 60
	
	# display off
	send_command 8
	# display on
	send_command 14

	# entry mode set
	send_command 6

	# write hello world
	send_data data_H
	send_data data_e
	send_data data_l
	send_data data_l
	send_data data_o
	send_data data_SPACE
	send_data data_W
	send_data data_o
	send_data data_r
	send_data data_l
	send_data data_d
	send_data data_EXCLAMATION

end:
	b end
