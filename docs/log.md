# LOG

---

* [2020-5-17 INFO]
>已成功在MAC上测试运行

---

* [2020-5-23 WARNING]

>出现了内存不能超过980M的bug

---

* [2020-5-23 INFO]
>具体原因分析如下
>```asm
>1c2c:05c5
>lb 0x301000
>c
>u /10
>```
>发现除了复制的PagingDemoProc，后面还有数据，怀疑之前写越界了。
>```asm
>watch w 0x301000
>c
>; 第一次断点在复制
>u / 10
>```
>在PSwitch里面，`ecx = 0x4000`，还剩这么多次初始化
`0x3294c`计算出页数
插入`push ecx; call DispInt`，显示页表个数为`0x100`(256)个PDE
最大内存不应该让页表写入`ProcPagingDemo`的位置，也就是`(00301000h-211000h)=F0000h`的大小，每个PTE占4字节，可以表示4096个字节的空间，所以最大内存为`F0000h/4*4096=3C000000h`(960MB)
bug已修复，目前内存可以达到`2G`

---

2020-7-1 INFO

>要将cs寄存器的内容分别装在第2 3 4 7个字节中:
>```asm
>xor     eax, eax                            ;清空eax寄存器
>mov     ax, cs                              ;去除cs寄存器的zhi5
>shl     eax, 4                              ;寻址是(段基址*15+偏移量) 因此先右移4位除以16
>add     eax, LABEL_SEG_CODE32               ;描述符对应的代码段
>mov     word[LABEL_DESC_CODE32 + 2], ax     ;装入第2 3个字节
>shr     eax, 16                             ;高16位移入低16位
>mov     byte[LABEL_DESC_CODE32 + 4], al     ;装入第4个字节
>mov     byte[LABEL_DESC_CODE32 + 7], ah     ;装入第7个字节
>```