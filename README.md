# intel AES加密指令集

**使用方法:**

1. 编译所有汇编代码(.s文件)

	```
	gcc -maes -msse4 -c *.s
	```

2. 编译链接所有代码

	```
	gcc -maes -msse4 -c ecb.o cbc.o ctr.o key_expansion.o aes.c aes_DEMO.c -o DEMO
	```

