# intel AES加密指令集

**AES指令支持检查**

编译check代码

```
gcc check.c check_cpu.c -o CHECK
```

运行CHECK

```
./CHECK
```


**使用方法:**

1. 编译所有汇编代码(.s文件)

	```
	gcc -maes -msse4 -c *.s
	```

2. 编译链接所有代码

	```
	gcc -maes -msse4 -c ecb.o cbc.o ctr.o key_expansion.o aes.c aes_DEMO.c -o DEMO
	```

本来编译好的.o或执行文件可能因权限问题不能直接运行，建议重新编译一遍
