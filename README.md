# Cache Timing Analysis

This repo contains some experiences in order to analyze the cache behaviour while running an
implementation of the AES that uses lookup tables.


The following programs:

    - server.c
    - study.c
    - search.c
    - corr.c

It is required to install use OpenSSL as a static library in the project. Here is a tutorial to do
so: http://razaina.fr/using-openssl-as-a-static-library-within-an-android-native-project/
    

Are from Bernstein's paper http://cr.yp.to/antiforgery/cachetiming-20050414.pdf
    
The following programs:

    - rijndael-alg\*.c 
    - rijndael-api\*.c

Are from ftp://ftp.aci.com.pl/pub/security/cryptography/algorithms/aes/aes-c-rijmen/    
