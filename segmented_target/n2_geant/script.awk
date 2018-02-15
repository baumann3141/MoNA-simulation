FILENAME == ARGV[1] { one[FNR]=$3 }
FILENAME == ARGV[2] { two[FNR]=$3 }

END {
    for (i=1; i<=length(one); i++) {
        print one[i], two[i]
    }
}
