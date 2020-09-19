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
- libc6-dev-i386

为了运行和调试，可能需要以下部分软件：

- [Bochs](http://bochs.sourceforge.net/)、[QEMU](https://www.qemu.org/)、[VirtualBox](https://www.virtualbox.org/) 等x86虚拟机软件
  - 如果需要使用配套的工具生成适用的硬盘映像，建议使用支持 RAW 格式的虚拟机，比如 Bochs
- [FreeDOS](http://www.freedos.org/) 或者其他的DOS环境
- 一个顺手的软盘映像编辑软件



## 生成

在终端执行 `make`，会在 build 文件夹内生成 `boot.img`，里面包含了自行编写的启动扇区、加载器(`loader.bin`)和系统内核(`kernel.bin`)。

如果想编译配套的小工具，请确保已经在根目录解压附带的 `80m.img.gz` 硬盘镜像压缩包，并手动进入 `command` 文件夹，执行 `make install` 命令，可以自动将打包好的 `cmd.tar` 写入到硬盘镜像中去。

## 调试

由于目前代码写死只从A盘读取，而DOS启动盘通常是A盘，故调试时需要将 `boot.img`、`loader.bin`、`kernel.bin` 三个文件复制到A盘。

如果在FreeDOS上运行，请在启动的时候选择不加载驱动(Emergency Mode)或者加载少量驱动(Safe Mode)。

~~虚拟机需要至少8M的RAM。~~ 此版本尚未测试
