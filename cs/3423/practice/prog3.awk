/Acct-Session-Time/{ total += $3; }
END{
    print "Total User Session Time =", total;
} 
