# Step 0: VMware Station Pro

## 获取 VMware Workstation Pro

24年3月11日，[VMware 宣布个人版 Workstation Pro 免费](https://blogs.vmware.com/workstation/2024/05/vmware-workstation-pro-now-available-free-for-personal-use.html)，~~那天赐的羊毛肯定不能放着不捞~~，所以我打算使用 Workstation Pro 构建虚拟机。

比较讨厌的是，官网的账号注册比较讨厌，~~第一次见到用户名是姓+名的注册方式~~，稍微摸索了一下，登录进入后发现并没有直接跳转到下载页面~~感觉给 VMware 菜完了~~，找到 free downloads products 之后摸进了[下载地址](https://support.broadcom.com/group/ecx/productdownloads?subfamily=VMware%20Workstation%20Pro&freeDownloads=true)，还要求填一下具体的住址，感觉非常不牛。

![](1.png)

## 安装 VMware Station Pro

安装过程没什么好提的，一路 yes yes yes 即可。修改了下安装目录至 D:\，记得把添加到系统 PATH 勾选上。

![](2.png)
![](3.png)
![](4.png)
![](5.png)

# Step 1: OpenEuler DevStation

## 获取 OpenEuler DevStation 镜像

OpenEuler 发行版的官方版本有不少，我比较想部署的是正式版、LTS版和 DevStation 版，了解了一下，发现 DevStation 可以选择内置一些有用的工具链，所以选择该版本。同时需要注意我们是虚拟机安装，所以需要找到虚拟机镜像。于是找到了[官方文档](https://docs.openeuler.org/zh/docs/24.09/docs/Releasenotes/%E7%B3%BB%E7%BB%9F%E5%AE%89%E8%A3%85.html)给的链接是 [22.09 版本](https://repo.openeuler.org/openEuler-22.09/virtual_machine_img/)~~已经 404 了~~，还得自己改 url，改完发现这里的[虚拟机镜像](https://repo.openeuler.org/openEuler-24.09/virtual_machine_img/)中并没有想要的 DevStation 虚拟机镜像~~什么玩意啊~~，于是重新到 [DevStation 文档](https://docs.openeuler.org/zh/docs/24.09/docs/DevStation/%E5%AE%89%E8%A3%85%E5%87%86%E5%A4%87.html)中找到了 DevStation 虚拟机镜像下载。

## 创建虚拟机

官方[在这里](https://docs.openeuler.org/zh/docs/24.09/docs/DevStation/%E5%AE%89%E8%A3%85%E6%8C%87%E5%AF%BC.html)提供了一部分教程，~~而显然我没有兴趣看~~。

一波三折后开始虚拟机安装过程，iso文件选上刚刚的下载的镜像，~~默认检测出来是 Fedora 呵呵~~稍微多划分了一点CPU(8核)，内存(12G)和存储(20G)。启动虚拟机提示为了安全禁用了侧通道缓解，于是先退出调整了下设置。

![](6.png)
![](7.png)
![](8.png)
![](9.png)
![](10.png)
![](11.png)
![](20.png)
![](12.png)

## 安装 OpenEuler DevStation

启动虚拟机，进入 live，并没有什么好说的。live 自带的 DE 是 GNOME，新的 44 挺好看的，~~感觉可以改变一下对 GNOME 中庸且无趣的 UI 的印象了~~然而 OpenEuler 的安装程序却莫名卡顿，每一次输入后需要进入任务管理界面再重新选择安装程序 GUI 加载一遍，体验十分不友好~~以至于一开始我以为是我的电脑问题~~。原本磁盘就空的，直接选择自动分区懒得划分区了~~以前装 Deepin 还要自己划 var swap 分区~~，12G 内存感觉也没划分 swap 分区的必要了，一路 yes 到底。然后重启。

![](13.png)
![](14.png)
![](15.png)
![](16.png)
![](17.png)
![](18.png)
![](19.png)
![](21.png)
![](22.png)
![](23.png)
![](24.png)
![](25.png)
![](26.png)

# Step 3: 查看系统

启动后进入 GRUB，选择后进入系统，发现 OpenEuler DevStation 并没有内置 DE，输入用户密码，熟悉的密码不回显。并不熟悉 rpm 系包管理器，也没高兴换源了，查了一下[rpm 包管理器方法](https://blog.csdn.net/liaowenxiong/article/details/118934857)，在用户目录建立了 list.txt，将已安装包信息通过管道导入。

```
touch ~/list.txt
sudo dnf list installed > ~/list.txt
more ~/list.txt
```

简单查看了下，果然内置了不少开发工具链。后续也没有心情继续使用该系统，遂结束。

![](27.png)
![](28.png)
![](29.png)
![](30.png)
![](31.png)