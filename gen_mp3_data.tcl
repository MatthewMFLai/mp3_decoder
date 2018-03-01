# usage:
# tclsh gen_mp3_data.tcl Gout-1.mp3 mp3_sample.c 16
# 16 => each line contains 16 hex bytes

set infile [lindex $argv 0]
set outfile [lindex $argv 1]

set line1 "const static uint8_t mp3data\[\] = \{"
set line2 "\};"

set fd [open $infile r]
fconfigure $fd -translation binary
set fd2 [open $outfile w]

set count [file size $infile]
set line ""
set linetokenmax [lindex $argv 2]
set linetokencnt $linetokenmax
while {$count} {
    set byte [read $fd 1]
    binary scan $byte H2 var1
    append line "0x$var1,"
    incr linetokencnt -1
    if {$linetokencnt == 0} {
        puts $fd2 $line
	set line ""
	set linetokencnt $linetokenmax
    }
    incr count -1
}
close $fd

if {$line != ""} {
    puts $fd2 $line
}
puts $fd2 $line2
close $fd2

exit 0 
