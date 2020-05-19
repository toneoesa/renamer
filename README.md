# renamer
用于批量重命名大量文件的 Linux 命令行工具

**这是一个练习项目，用作课程的作业**

## 用法
### 帮助信息
```
usage: renamer --dir=string [options] ...
options:
  -h, --help         print this message
  -d, --dir          directory containing the file to be renamed (string)
      --hidden       Don't ignore hidden files
  -s, --namestr      a string describe filename, *,$ represent the original names and the number added (string [=])
      --begin        the begin of number added (int [=1])
      --step         the step of number added (int [=1])
      --digit        digit of number added(supplement 0 when necessary) (int [=1])
  -e, --extension    change file extension (string [=])
  -l, --lower        change the [basename(1), extension(2), both(3)] to lower case (int [=3])
  -u, --upper        change the [basename(1), extension(2), both(3)] to upper case (int [=3])
  -r, --replace      string need to be replaced (string [=])
      --to           the string will be replaced to this(empty string - delete) (string [=])
  -i, --insert       string to insert (string [=])
      --into         insert position (int [=0])
  -a, --append       append string to the original names (string [=])
  -D, --delete       which position start to delete (int [=0])
      --dnum         number of character to be deleted (int [=1])
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
### 替换文件名中的字符（串）

例如： `renamer -d ./test -r "image" --to "pic"`

>将 `./test` 目录下所有文件（不包括隐藏文件和文件夹）中出现的 `image` 替换为 `pic`

### 插入、追加字符（串）

例如： `renamer -d ./test -i "pic_" --into 0`

>将 `./test` 目录下所有文件（不包括隐藏文件和文件夹）的文件名前插入 `pic_`

例如： `renamer -d ./test -a "_pic"`

>将 `./test` 目录下所有文件（不包括隐藏文件和文件夹）的文件名后（并非扩展名后）追加 `_pic`

### 指定位置删除指定个数字符

例如： `reanmer -d ./test -D 0 --dnum 2`

>将 `./test` 目录下所有文件（不包括隐藏文件和文件夹）的文件名从 `开头` 开始删除 `2` 个字符

**注意**： 当删除的过多以至于文件名为空时，会出现以下错误提示

    (5) Some filenames are cleared!

### 预览结果

例如： `renamer -d ./test --hidden -s "No.#_*" --digit 3 --lower 2 -p --maxitem 10`

>将 `./test` 目录下所有文件（**包括**隐藏文件，不包括文件夹）进行以下操作
>
>1. 按照 `No.{数字编号}_{原文件名}` 的格式命名
>2. 将扩展名改为小写
>
>后的**结果**，输出前 `10` 个在屏幕上。

### 注意

+ 各参数的顺序可以**任意**放置，但程序严格按照[帮助信息](#帮助信息)中参数的顺序执行，和键入的参数顺序**无关**.

## 错误信息

### Open directory failed!

打开目录失败，可能的原因

+ 目录不存在
+ 没有足够的权限
+ 其他

### Inappropriate character in original filename!

原因：打开的目录下的文件名包含 `*`, `$` 字符

尽管 Linux 文件名中可以使用这些字符，但普遍不推荐使用。

本程序不处理这类文件，请手动去除这些字符后再执行本程序。

### Inappropriate character in new filename!

原因：使用的文件名包含 `/` 字符

请不要使用 `/` 字符。

### No file to rename!

原因：目录下没有找到要命名的文件

### The range of number is too large!

原因：编号的范围太大

请尝试缩小范围

### Some filenames are cleared!

原因：某些文件名被删除过多，变成空字符串

请合理命名

### Rename failed!

重命名失败，可能的原因

+ 文件被占用
+ 没有足够的权限
+ 其他

## 作者

工程概论第20小组

+ 刘
+ 俞
+ 张

# 感谢 cmdline 项目

本项目使用了开源的命令行参数解析库

[cmdline: A Command Line Parser](https://github.com/tanakh/cmdline)
