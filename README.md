# StayHomeOS

北京邮电大学2017级操作系统课程设计大作业

## 所需环境

建议使用您喜欢的 Linux 发行版。
WSL(Windows Subsystem for Linux) 和 macOS 可能需要手动完成某些步骤。

除此之外，还需要在系统中安装以下软件：

- [NASM](https://www.nasm.us/)
- GCC
- GNU Make
- [Mtools](https://www.gnu.org/software/mtools/manual/mtools.html)（注意不是MongoDB的脚本集合）

为了运行和调试，可能需要以下部分软件：

- [Bochs](http://bochs.sourceforge.net/)、[QEMU](https://www.qemu.org/)、[VirtualBox](https://www.virtualbox.org/) 等x86虚拟机软件
- [FreeDOS](http://www.freedos.org/) 或者其他的DOS环境
- 一个顺手的软盘映像编辑软件

## 生成

在终端执行`make`，会在 build 文件夹内生成`boot.img`，里面包含了自行编写的启动扇区(现在还不是操作系统)。

如果想生成用于调试的可执行文件，请执行`make debugimg`，执行后会在 build 文件夹内生成两个文件：`boot.com`（可执行文件本身）、`debug.img`（包含boot.com的软盘映像）。前者需要复制到DOS环境中，而后者可以直接被虚拟机加载读取或者写入软盘。
