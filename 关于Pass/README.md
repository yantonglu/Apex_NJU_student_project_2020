## 1. Hello文件直接覆盖原本Hello文件夹。建议删掉原本的build/lib/LLVMHello.so然后重新make。pass部分测试了快排等常见程序，没有什么问题。
## 2. Makefile为Makefile.1,做出一些改动
（1）删掉了与instrument.o的连接，因为会与llvm-link后的文件发生重定义的冲突。
（2） src.3.bc为src.2.bc与instrument.bc 的link生成
（3）src.4.bc为src.3.bc经过mystuff两个pass处理后的结果。同时生成两个.ll文件便于查看。
## 3. instrument.c注释掉了目前不用的代码。直接放到APEX/llvm文件夹下。然后执行makefile即可。
## 4. mystuff1标准输出只有报错信息，包括 id error, id repeat , instruction error, type error，mystuff2 标准输出打印执行流程 ， 报错包括：id error, id repeat以及llvm error
