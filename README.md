# OSLab-project

To make the project, run: 
```
make install 
```

To specif whether you want to use threads or proceses, run: 
```
make type=MULTIPROCESS
```
or
```
make type=MULTITHREAD
```
(you could also run either of these without running `make install`). 

After building the project, start the web server by running `./server` in this directory. 

Within the source code (`simpleWebServer.c`), the variable `PORTNUM` specifies the port that's used by the server and `THREAD_POOL_SIZE` specifies the maximum number of threads allowed.  

