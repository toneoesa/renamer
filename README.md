# renamer
用于批量重命名大量文件的 Linux 命令行工具

**这是一个练习项目，用作课程的作业**

目前只完成了部分功能，还有功能在添加中

## 用法
### 帮助信息
```
usage: renamer --dir=string [options] ...
options:
  -h, --help         print this message
  -d, --dir          directory containing the file to be renamed (string)
      --hidden       Don't ignore hidden files
  -s, --namestr      a string describe filename, *,# represent the original names and the number added (string [=])
      --begin        the begin of number added (int [=1])
      --step         the step of number added (int [=1])
      --digit        digit of number added(supplement 0 when necessary) (int [=1])
  -e, --extension    change file extension (string [=])
      --lower        change the [basename(1), extension(2), both(3)] to lower case (int [=3])
      --upper        change the [basename(1), extension(2), both(3)] to upper case (int [=3])
  -p, --preview      preview the results(Don't execute now)
      --maxitem      max number of showed items, 0 - unlimit (int [=20])
```
### 按照格式字符串改名

例如： `renamer -d ./test -s "No.#_*" --begin 1 --step 1 --digit 3`

>将 `./test` 目录下所有文件（不包括隐藏文件和文件夹）按照 `No.{数字编号}_{原文件名}` 的格式命名。
>
>其中数字编号是从 `1` 开始步长为 `1` 的一列 `3` 位数（超过3位不变，不足3位补0）。

### 改变文件扩展名

例如： `renamer -d ./test -e "txt"`

>将 `./test` 目录下所有文件（不包括隐藏文件和文件夹）的扩展名都改成（添加为） `txt`

### 改变文件名大小写

例如： `renamer -d ./test --lower 2 --upper 1`

>将 `./test` 目录下所有文件（不包括隐藏文件和文件夹）的扩展名改为小写、基本名改为大写。

### 预览功能

例如： `renamer -d ./test --hidden -s "No.#_*" --digit 3 --lower 2 -p --maxitem 10`

>将 `./test` 目录下所有文件（**包括**隐藏文件，不包括文件夹）进行以下操作
>
>1. 按照 `No.{数字编号}_{原文件名}` 的格式命名
>2. 将扩展名改为小写
>
>后的**结果**，输出前 `10` 个在屏幕上。

### 注意

+ 各参数的顺序可以**任意**放置，但程序严格按照[帮助信息](#帮助信息)中的顺序执行，和参数顺序**无关**.

## 作者

工程概论第20小组

+ 刘
+ 余
+ 张

（姓名不透露）
