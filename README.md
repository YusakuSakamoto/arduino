# ARDUINO

##FEATURES
1.c++
2.Serial Communication
3.checksum

##PROGRAM
1.ping sensor analysis
Parallax社の超音波レンジファインダー「PING)))センサを用いた距離計測プログラムです。  
[参考:Arduino 距離センサ/超音波センサ](https://getpocket.com/a/read/1071923344)  
超音波は、その名にもあるとおり音波の一種であるので、その波の伝播速度は音波と同じ式で求められます。
```
[ 公式 ] c ＝ 331.5 ＋ 0.6 t (m/sec)
```
ここで t = セルシウス度(℃)とする。この式によると、この超音波センサは温度の影響を著しく受けることがわかる。しかし、今回使用するトマトロボット競技会において、せいぜい1mほどの短い距離であるので、音波の途中経過中の温度変化については無視し、**測定の前に温度を測定し、プログラムに組み込む必要がある**。

[音速の式の導出](http://w3e.kanazawa-it.ac.jp/e-scimath/contents/t24/textbook_t24_c02.pdf)
