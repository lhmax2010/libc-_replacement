function ltrim(s) { sub(/^[[:space:]]+/, "", s); return s }
{
    line = $0
    directive = ltrim(line)
    if (directive ~ /^#[[:space:]]*(if|ifdef|ifndef)([[:space:]]|$)/) {
        depth++
        opened[depth] = FNR ":" directive
        current[depth] = FNR ":" directive
    } else if (directive ~ /^#[[:space:]]*(elif|else)([[:space:]]|$)/) {
        current[depth] = FNR ":" directive
    }

    if (index(line, needle) != 0) {
        printf("OCCURRENCE\t%s:%d\t%s\n", FILENAME, FNR, line)
        for (i = 1; i <= depth; i++) {
            printf("LEVEL_%d_OPEN\t%s\n", i, opened[i])
            if (current[i] != opened[i])
                printf("LEVEL_%d_CURRENT_BRANCH\t%s\n", i, current[i])
        }
        print "STACK_END"
    }

    if (directive ~ /^#[[:space:]]*endif([[:space:]]|$)/) {
        delete opened[depth]
        delete current[depth]
        depth--
    }
}
