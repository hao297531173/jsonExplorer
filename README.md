# jsonExplorer<br>
## 使用方法<br>
  **引入头文件 [jsonExplorer.h](https://github.com/hao297531173/jsonExplorer/blob/master/jsonExplorer.h)**<br>
	对于数据结构和提供的接口里面有详细说明，在这里就不赘述了<br>
	测试用的json文件是[test.json](https://github.com/hao297531173/jsonExplorer/blob/master/test.json)<br>
	测试文件是[test.cpp](https://github.com/hao297531173/jsonExplorer/blob/master/test.cpp)<br>
	测试结果写在[selectOutput.txt](https://github.com/hao297531173/jsonExplorer/blob/master/selectOutput.txt)<br>
## 数据结构<br>
 **因为json文件对象是可以嵌套的，所以我考虑数据结构使用广义表，可以任意嵌套，并且可以是多类型的，实现上主要使用了cpp的stl中的vector和map**<br>
## 组成部分<br>
 **前端 词法分析+语法分析 [parser.cpp](https://github.com/hao297531173/jsonExplorer/blob/master/parser.cpp)**<br>
 **其头文件[parser.h](https://github.com/hao297531173/jsonExplorer/blob/master/parser.h)**<br>
 这个部分实现起来比较简单这里就不多说了
 **后端统一写成一个类了 [runner.cpp](https://github.com/hao297531173/jsonExplorer/blob/master/runner.cpp)**<br>
 **及其头文件[runner.h](https://github.com/hao297531173/jsonExplorer/blob/master/runner.h)**<br>
 **错误抛出统一写在[error.h](https://github.com/hao297531173/jsonExplorer/blob/master/error.h)**<br>
 ## 关于项目的一点说明<br>
 **version 1.0主要是实现了一些内核的功能，提供接口较少，目前只提供了查询数据，遍历数据和输出json文件的核心功能，具体的查询操作参见[test.cpp](https://github.com/hao297531173/jsonExplorer/blob/master/test.cpp)**<br>
 ## 我想说的一些话<br>
**这是我第一次一个人完成如此大的项目，难免会存在不足，比如对于代码的安排就很不合理，还有单词拼错（(*/ω＼*)），希望大佬们可以给予宝贵的意见或者建议，感谢大佬们的支持**<br>
 
