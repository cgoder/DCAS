# DCAS
多CAS动态切换系统。支持CDCAS/DVNCAS 等。
---
note：
1. 在用MAKEFILE时， 需要将MAKEFILE中SRC += 与 INCDIR += 修改为需要的文件名与目录；
2. 编译时只需要执行：make compile 就可以了， 此编译只负责编译成.o文件的
3. 要生成库文件时， 首先要确定CA库的目录与MAKEFILE中所写的目录是否相同；然后， 如要生成静态库执行： make static_lib 就可以生成静态库了。要生成动态库时， 执行：make dynamic_lib就可以生成动态库了。
4. make libs命令可直接一次性生成动态和静态库。
5. 生成的库文件在根目录/lib下。
