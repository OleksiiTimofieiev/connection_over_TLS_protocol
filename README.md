# connection_over_TLS_protocol
le sécurité via mbedtls

Samsung test:
Knowledge base: https://tls.mbed.org/kb-search
General: https://tls.mbed.org/high-level-design
Sample applications: https://tls.mbed.org/kb/development/sample_applications
API: https://tls.mbed.org/api/index.html
API tree: https://tls.mbed.org/api/md_8h.html#ac4619cce171e558d059baae35da50a2a
Select with mbedTLS: https://tls.mbed.org/discussions/generic/select-with-mbedtls
Idea on the select implementation: https://forums.mbed.com/t/mbedtls-ssl-read-write-concurrently-from-multiple-ssl-connections/3772
Non-blocking idea: https://tls.mbed.org/discussions/generic/non-blocking-mbedtls_net_connect, https://tls.mbed.org/kb/development/restartable-ecc
Epoll: https://tls.mbed.org/discussions/generic/async-i-o-with-mbed-tls-epoll-kqueue
RSA: https://tls.mbed.org/kb/how-to/encrypt-and-decrypt-with-rsa
Generation of key pair for RSA: https://github.com/ARMmbed/mbedtls/blob/development/programs/pkey/rsa_genkey.c
Avoid padding: https://www.geeksforgeeks.org/how-to-avoid-structure-padding-in-c/
What to do if the data is not coorect -> UDP issues ?
libev docs://http://www.wangafu.net/~nickm/libevent-book/Ref4_event.html
Thread pool:
https://github.com/ARMmbed/mbedtls/blob/development/programs/ssl/ssl_pthread_server.c
https://randu.org/tutorials/threads/
https://people.clarkson.edu/~jmatthew/cs644.archive/cs644.fa2001/proj/locksmith/code/ExampleTest/threadpool.c
https://github.com/mbrossard/threadpool/blob/master/src/threadpool.c
https://gist.github.com/bodokaiser/5689562
https://www.youtube.com/watch?v=eWTGtp3HXiw
​
