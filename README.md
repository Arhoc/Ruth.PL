# Ruth.PL
This is a perl obfuscator written in C

# Compile Instructions
This program depends on OpenSSL, so your system needs to have it installed, go ahead with your favourite package manager!

Then run:
```
gcc Ruth.c -o Ruth -lcrypto // Or compiler of your choice
```

# Usage
./Ruth <Perl_Source> <Iterations> <br/>
IE: `./Ruth Secret_Malware.pl 30`
