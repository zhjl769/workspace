使用说明：
1、打开hex.inf 把uartv2.out 修改为 您自己的out文件
2、双击运行hex.bat文件，会自动生成melp.bin文件
3、用CCS打开flash_burn工程，并在while(1)处设置断点
4、使用仿真器连接上目标板
5、加载flash_burn.out文件，并全速运行
6、程序在断点处停下后表示flash烧写完成

注意：自动生成的melp.bin文件名和位置不要改变，否则会出现无法打开文件