0. 应修改目录结构与运行逻辑使我们的程序可以一次性处理一批测试样例；所以应增设学生提交程序目录stu和教师提供正确版本目录tea，及stu_symb目录和tea_symb目录；（也需要考虑将input扩展为input文件夹）；A，B文件夹保留为工作文件夹，原result文件夹改名叫C，result放结果。
1. 预处理：对于stu和tea中所有的src_no（no为序列号），分别放入工作文件夹，执行其对应Makefile，将symb_no.out放入对应的stu_symb目录或tea_symb目录。
2. 自学习：任取一个tea_symb目录中的symb.out，与stu_symb中的symb.out进行匹配，如果结果为correct，那么将学生的symb.out放入tea_symb目录。
3. 匹配：经过自学习环节，stu_symb中的全是错误的提交了，对于每个提交，用tea_symb里所有的symb.out去匹配，选取得分最高的，作为其评分，将评分编号并放入result。

使用方法：
1. 将test.sh放入apex目录。
2. 创建stu, tea, stu_symb, tea_symb, C目录，将result中的Makefile放入C目录，然后清空result文件夹。
3. 将正确与错误样例分别放入stu, tea文件夹，命名为src_no.c，no为序列号，从0开始。
4. 保险起见先对A, B和C目录make clean。
5. sudo chmod +x test.sh赋予运行权限，执行test.sh脚本。

会取最高的评分放入result作为最终评分。