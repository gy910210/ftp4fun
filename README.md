# ftp4fun
Implementation of a simple FTP client and server
## How to use?
+ 服务器端，在Linux命令行界面输入./ftps命令，即启动FTP服务。
+ 客户端，在Linux命令行界面输入./fptc duan@202.120.38.146命令，即进入FTP客户端模式。其中，202.120.38.146是FTP服务器的IP地址，duan是FTP的用户名。
+ 此FTP客户端是一个交互式命令模式，可以使用help命令查看所有可以使用的命令和相应的功能解释。
+ 输入browse命令，可以查看服务器端的文件列表。
+ 输入upload命令，将客户端的文件上传到服务器。
+ 输入download命令，将服务器端的文件下载到客户端。
+ 输入delete命令，删除服务器端的文件。
+ 输入new命令，在服务器端创建新的文件。
+ 输入quit命令，退出客户端。
+ 服务器端采用多线程，支持多用户（多客户端）同时访问FTP服务器。
