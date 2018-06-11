#### Description
http1.1 + epoll


## 性能测试

短连接
>ab -n 100000 -c 100  http://127.0.0.1:8888/
```
Server Software:        http1.1
Server Hostname:        127.0.0.1
Server Port:            8888

Document Path:          /
Document Length:        23715 bytes

Concurrency Level:      100
Time taken for tests:   3.426 seconds
Complete requests:      100000
Failed requests:        0
Total transferred:      2382300000 bytes
HTML transferred:       2371500000 bytes
Requests per second:    29184.85 [#/sec] (mean)
Time per request:       3.426 [ms] (mean)
Time per request:       0.034 [ms] (mean, across all concurrent requests)
Transfer rate:          678975.33 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0    1   0.3      1       5
Processing:     0    2   0.6      2      12
Waiting:        0    1   0.6      1      10
Total:          0    3   0.6      3      13

Percentage of the requests served within a certain time (ms)
  50%      3
  66%      3
  75%      3
  80%      4
  90%      4
  95%      4
  98%      5
  99%      6
 100%     13 (longest request)
```
 
长连接
>ab -n 100000 -c 100 -k http://127.0.0.1:8888/
```
Server Software:        http1.1
Server Hostname:        127.0.0.1
Server Port:            8888

Document Path:          /
Document Length:        23715 bytes

Concurrency Level:      100
Time taken for tests:   1.107 seconds
Complete requests:      100000
Failed requests:        0
Keep-Alive requests:    100000
Total transferred:      2382300000 bytes
HTML transferred:       2371500000 bytes
Requests per second:    90298.02 [#/sec] (mean)
Time per request:       1.107 [ms] (mean)
Time per request:       0.011 [ms] (mean, across all concurrent requests)
Transfer rate:          2100751.68 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0    0   0.1      0       4
Processing:     0    1   0.3      1       5
Waiting:        0    0   0.3      0       4
Total:          0    1   0.3      1       5

Percentage of the requests served within a certain time (ms)
  50%      1
  66%      1
  75%      1
  80%      1
  90%      1
  95%      2
  98%      2
  99%      3
 100%      5 (longest request)
 ```
