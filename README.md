# Exchange Rate Optimizer
Used Floyd - Warshall Algorithm to find the optimum all pair minimum path for exchanges

# How to run
```zsh
$ g++ solution.cpp -o solution
$ ./solution
```
or
```zsh
$ g++ -std=c++11 solution.cpp -o solution
$ ./solution
```

# Note
> As the problem had put constraint on language, and unfortunately I don't have experience in any of them. I really want to learn ```Rust``` and ```Swift``` for a while, but haven't started coding with them, yet. I would beg you to consider my assignment as valid and not reject it due to language constraint.

> I have used ```C++``` (Cpp), and I hope you'll understand. I have prepared a well - documented version of code with proper variables and function names.

> Please report major and minor bugs, if found. And I welcome suggestion from your team on my approach to the problem and more.

# Example Run
```cpp
./solution

2017-11-01T09:42:23+00:00 KRAKEN BTC USD 1000.0 0.0009
2017-11-01T09:43:23+00:00 GDAX BTC USD 1001.0 0.0008
EXCHANGE_RATE_REQUEST KRAKEN BTC GDAX USD
BEST_RATES_BEGIN KRAKEN BTC GDAX USD 1001
KRAKEN, BTC
GDAX, BTC
GDAX, USD
BEST_RATES_END
```
