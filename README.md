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

在终端执行`make`，会在 build 文件夹内生成`boot.img`，里面包含了自行编写的启动扇区(目前生成的文件已经超过512字节，这个功能失效，等文件系统实现后修复)。

如果想生成用于调试的可执行文件，请执行`make debugimg`，执行后会在 build 文件夹内生成两个文件：`boot.com`（可执行文件本身）、`debug.img`（包含boot.com的软盘映像）。前者需要复制到DOS环境中，而后者可以直接被虚拟机加载读取或者写入软盘。

## 调试

如果在FreeDOS上运行，请在启动的时候选择不加载驱动(Emergency Mode)或者加载少量驱动(Safe Mode)。
虚拟机需要至少8M的RAM。
