## 当前状态
此插件是半成品。

在启用此 Kwin effect 之后，
各个窗口都会出现模糊效果。

但是 **它们都会被卡死** ，并且卡的过程中模糊效果还在不断加深。

不过，其背后的进程实际上是没有卡死的，只是看起来定格了，
所以你仍然可以尝试用鼠标取消勾选 WayMCA 条目
（点一次就行了，它看上去没有变化，实际上只要你操作正确，就已经取消勾选了），
再点“应用”来禁用此插件。

**测试此插件时，请做好随时重启（以及切换 tty 把 `~/.local/share/kwin/effects/kwin6_effect_waymca` 删掉）的准备。**

## 安装
运行以下命令：

```bash
git clone --depth=1 -b testing https://github.com/clsty/waymca ~/.local/share/kwin/effects/kwin6_effect_waymca
```

## 启用
在 Plasma 设置中找到桌面特效 - 无障碍功能 - WayMCA，勾选并启用。
