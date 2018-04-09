# usage:
# tclsh clone_mp3_data.tcl file1.mp3 file2.mp3

set infile [lindex $argv 0]
set outfile [lindex $argv 1]

set fd [open $infile r]
fconfigure $fd -translation binary
set fd2 [open $outfile w]
fconfigure $fd2 -translation binary

set count [file size $infile]
while {$count} {
    set byte [read $fd 1]
    binary scan $byte H2 var1
    puts -nonewline $fd2 [binary format H2 $var1]
    incr count -1
}
close $fd
close $fd2

exit 0 
