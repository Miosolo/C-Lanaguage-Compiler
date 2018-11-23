# 如何对本项目进行测试？
## 项目文件结构
- Lexer.exe: 预编译的词法分析器（源文件在Lexer文件夹中，为VS 2017工程，exe使用VS 2017编译）
- transTable.csv: 词法分析器需要调用的分析表，须与Lexer.exe处在同一目录
- RDAnalysis.py: 语法分析器程序主体，含有一个类Parser，为本项目各个流程的载体
- RDAnalysis_test.py: pytest测试用例
- RDADemo.py: 一个简单的脚本，直接运行可以测试source2.txt的解析情况
- TestFile: 测试文件所在的目录，包含本项目中所使用的测试用例
  - error.txt: 语法分析产出的错误日志
  - details.txt: 语法分析产出的详细记录
  - *.c, *.txt: 测试用文件
  - *.csv: 词法分析器输出的符号表，可以使用Excel打开，作为语法分析器的输入

## 项目依赖
- Python: 3.7+
- Pytest(如需运行RDAnalysis_test.py需要)
  
## 使用方式
运行词法分析器，需要在命令行里传入两个变量：输入文件路径（.c，必选）；输出文件路径（.csv，可选）。运行RDAnalysis.py，需要另写一个小脚本指定符号表的位置并传入Parser类的实例；也可以直接运行RDADemo.py。