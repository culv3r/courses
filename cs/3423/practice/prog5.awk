/Acct-Input-Packets/ {input += $3}
/Acct-Output-Packets/ {output += $3}
END{
    printf "Total Input Bandwidth Usage = %d packets\n", input;
    printf "Total Output Bandwidth Usage = %d packets\n", output;
}
