目前主要的是两个分支，**develop**分支和**release**分支

- **develop**用来开发
- **release**用来发布



**注意**

- 团队开发，修改本地代码前先pull回远程代码，确保本地和远程代码一致
- 队员只修改develop分支下的代码
- release经由团队讨论确认产生
- 开发时避免无意义代码修改，如删除空行空格调整位置等

2016-11-22 09:27:23

- by linyuan
- 1.取消对CAN连接函数中密码获取判断的屏蔽
- 2.在initialDialog函数中对密码的初始化分debug模式和release模式，debug模式下全写0，release模式下只写0x。


2016-11-21 08:24:39

- by xianfeng
- CAN连接、启动、发送部分已完善，在CAN连接成功后启动ReceiveThread 

2016-11-20 13:18:11

- by linyuan
- 当前完成了主要的开发任务，等待拿回设备进行调试
- 修改.gitignore文件，取消对.lib文件的忽略，因为要用到CANCONTROL.LIB文件的引用


## About ##
![主界面截图](https://github.com/ChaiAndKe/SREC/raw/master/shootscreen.jpg)

**This Application** is a developed by vs2010 with c++(MFC).



MD file Edit tips:


- **Bold** (`Ctrl+B`) and *Italic* (`Ctrl+I`)
- Quotes (`Ctrl+Q`)
- Code blocks (`Ctrl+K`)
- Headings 1, 2, 3 (`Ctrl+1`, `Ctrl+2`, `Ctrl+3`)
- Lists (`Ctrl+U` and `Ctrl+Shift+O`)
