BEGIN {
    FS = "\t"
    print "# " title
    print ""
}

{
    for (i = 1; i <= NF; ++i) {
        gsub(/\\/, "\\\\", $i)
        gsub(/\|/, "\\|", $i)
    }
    printf "|"
    for (i = 1; i <= NF; ++i)
        printf " %s |", $i
    printf "\n"
    if (NR == 1) {
        printf "|"
        for (i = 1; i <= NF; ++i)
            printf " --- |"
        printf "\n"
    }
}
