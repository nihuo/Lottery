# Lottery
为2014 Toastmaster 秋季峰会的抽奖环节做的一个小程序。

Requirement: Visual Studio 2013

程序打开的时候需要选择候选人csv文件。csv文件有如下要求

> * 以逗号（,）分隔
> * UTF-8编码。一般excel导出时都是ansi编码，可以用Notepad++， 转换为UTF8编码。 
> * csv第一行会被当成标题行忽略
> * csv每行数据必须依次为 Ticket No.,English Name,中文全名,Mobile Phone,Email,Club,Division,Area,Ticket type。 
> * 需要检查导出的csv文件末尾，如果有无效数据需要删除， 不得有空行
