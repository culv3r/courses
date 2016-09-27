BEGIN{one=0;two=0;three=0;four=0;five=0;}
/Ascend-Data-Rate/ { if ($3 <= 14400)
                        one++;
                    if ($3 >= 14401 && $3 <= 19200)
                        two++;
                    if ($3 >= 19201 && $3 <= 28800)
                        three++;
                    if ($3 >= 28801 && $3 <= 33600)
                        four++;
                    if ($3 > 33600)
                        five++;
                }
                END{print "0-14400", one;
                    print "14401-19200", two;
                    print "19201-28800", three;
                    print "28801-33600", four;
                    print "above 33600", five;}
