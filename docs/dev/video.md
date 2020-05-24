# 关于视频的碎碎念

## 视频中断 - int 10h

EGA/VGA的模式列表可以在[这里](https://www.stanislavs.org/helppc/int_10-0.html)和[这里](http://www.columbia.edu/~em36/wpdos/videomodes.txt)确认。默认情况下，经过BIOS自检后视频模式应为03h（80x25、16色、文字）。所以我们不需要设置模式就可以输出彩色文字。

输出文字时，我们要使用AH=13h的参数，其他参数可以在[这里](https://www.stanislavs.org/helppc/int_10-13.html)确认。其中的视频页号(page number)在前文的表中有提及，我们的例子只使用了0号页。颜色的对应关系在[这里](https://wiki.osdev.org/Text_mode#Colours)。

看完之后应该能读懂DispStr的操作了。

## 保护模式+文字模式

[这里](https://wiki.osdev.org/Printing_to_Screen)其实讲得很详细了，简单来说，BIOS映射了一块内存区域当作显存，只要往里面写数据就可以显示了。每个字符占**两个**字节，第一个是ASCII码，第二个是颜色属性。所以尽管我们的屏幕是80x25，代码中一行是160字节而不是80字节。

## 保护模式+图形模式

如果需要实现图形界面，VGA最高能提供320x200、256色的图形界面（13h号模式，也有分辨率高但色彩数少的模式），但是这个太不现代了。[画法](https://wiki.osdev.org/Drawing_In_Protected_Mode)和文字模式差不多，都是BIOS映射了一块内存区域。不过因为是画图，所以我们要用自己的字体了，然后又是一大堆麻烦事……

当然这个模式还是有些跟不上时代，而VESA推出的SVGA可以支持更高的分辨率，起码追上了XP时代。这个模式的设置比VGA要复杂一点，所以我就暂时把[模式介绍](https://wiki.osdev.org/VESA_Video_Modes)丢这里了。
