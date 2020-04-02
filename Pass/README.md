# 这是使Apex运行的对llvm中间代码进行优化的Pass部分
## Hello.exports由于是空文件，所以无法上传，请大家自行创建一个空文件然后另存为Hello.exports，或者在QQ群里可以找到
## 使用方法：为了便于大家操作，我们直接用这个Pass覆盖了Hello，所以大家只需要将这个文件夹内所有内容以及Hello.exports复制到llvm（源码）的lib/Transform/Hello目录下，然后在build目录下make，
## 我们已经修改了Makefile.1，用这些Pass优化llvm的中间代码。
