# WayMCA C++ 构建指南

## 概述

WayMCA 已从 QML 脚本效果重写为原生 C++ KWin 插件。这是因为 KWin 6 的 QML API 不支持全屏 shader 效果（详见 `IMPLEMENTATION_NOTE.md`）。

## 系统要求

### 必需的依赖

- **构建工具：**
  - CMake >= 3.16
  - C++ 编译器（支持 C++17）
  - make 或 ninja

- **Qt 6：**
  - Qt6Core
  - Qt6DBus
  - Qt6Gui
  - Qt6Widgets

- **KDE Frameworks 6：**
  - KF6Config
  - KF6ConfigWidgets
  - KF6CoreAddons
  - KF6GlobalAccel
  - KF6I18n
  - KF6WindowSystem

- **KWin：**
  - kwineffects
  - kwinglutils
  - KWinDBusInterface

- **其他：**
  - libepoxy

### 安装依赖（示例）

**Arch Linux / Manjaro:**
```bash
sudo pacman -S cmake extra-cmake-modules qt6-base kwin
```

**Ubuntu / Debian / KDE Neon:**
```bash
sudo apt install cmake extra-cmake-modules qtbase6-dev kwin-dev \
                 libkf6config-dev libkf6configwidgets-dev libkf6coreaddons-dev \
                 libkf6globalaccel-dev libkf6i18n-dev libkf6windowsystem-dev \
                 libepoxy-dev
```

**Fedora:**
```bash
sudo dnf install cmake extra-cmake-modules qt6-qtbase-devel kwin-devel \
                 kf6-kconfig-devel kf6-kconfigwidgets-devel kf6-kcoreaddons-devel \
                 kf6-kglobalaccel-devel kf6-ki18n-devel kf6-kwindowsystem-devel \
                 libepoxy-devel
```

**openSUSE:**
```bash
sudo zypper install cmake extra-cmake-modules qt6-base-devel kwin6-devel \
                    kf6-kconfig-devel kf6-kconfigwidgets-devel kf6-kcoreaddons-devel \
                    kf6-kglobalaccel-devel kf6-ki18n-devel kf6-kwindowsystem-devel \
                    libepoxy-devel
```

## 构建和安装

### 方法 1：使用提供的脚本（推荐）

```bash
# 1. 构建
./build.sh

# 2. 安装
./install-cpp.sh

# 3. 重启 KWin
# Wayland: 注销并重新登录
# X11: kwin_x11 --replace
```

### 方法 2：手动构建

```bash
# 创建构建目录
mkdir build
cd build

# 配置
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr

# 构建
make -j$(nproc)

# 安装
sudo make install

# 重启 KWin（Wayland 用户需要注销/登录）
```

## 启用效果

1. 打开系统设置
2. 导航到：**工作空间行为** → **桌面效果**
3. 搜索 "WayMCA"
4. 启用效果
5. 点击配置按钮调整参数（可选）

## 配置选项

- **Green Channel Blur Radius (0-20):** 绿色通道模糊半径，默认 3 像素
- **Blue Channel Blur Radius (0-20):** 蓝色通道模糊半径，默认 8 像素
- **Use Gaussian Blur:** 使用高斯模糊（更高质量）或盒式模糊（更快）
- **Debug: Full-Screen Blur:** 全屏模糊模式（用于测试效果是否工作）
- **Full-Screen Blur Radius (0-50):** 全屏模糊半径，默认 10 像素

## 卸载

```bash
./uninstall-cpp.sh
```

或手动删除：
```bash
sudo rm -f /usr/lib/*/qt6/plugins/kwin/effects/plugins/kwin_waymca.so
sudo rm -f /usr/lib/*/qt6/plugins/kwin/effects/configs/kwin_waymca_config.so
sudo rm -rf /usr/share/kwin/effects/waymca
sudo rm -f /usr/share/kwin/effects/waymca.json
```

## 故障排除

### 构建错误

**找不到 KWin 头文件：**
- 确保安装了 kwin-dev 或 kwin-devel 包
- 检查 CMake 输出，查看哪些依赖缺失

**CMake 版本错误：**
- 确保 CMake >= 3.16
- 运行 `cmake --version` 检查版本

### 运行时问题

**效果不出现在列表中：**
- 确认已重启 KWin
- 检查插件是否已安装：`ls /usr/lib/*/qt6/plugins/kwin/effects/plugins/kwin_waymca.so`
- 查看 KWin 日志：`journalctl --user -f | grep kwin`

**启用后无效果：**
- 启用"全屏模糊"调试模式，看是否出现明显模糊
- 检查 journalctl 日志中的 shader 错误
- 确认 OpenGL 合成已启用

**性能问题：**
- 降低模糊半径
- 切换到盒式模糊（禁用"使用高斯模糊"）
- 确保使用现代 GPU 驱动

## 项目结构

```
waymca/
├── CMakeLists.txt          # 主 CMake 文件
├── src/
│   ├── CMakeLists.txt      # 源文件 CMake
│   ├── waymca.h            # 效果头文件
│   ├── waymca.cpp          # 效果实现
│   ├── waymca_config.cpp   # 配置模块
│   ├── waymca_config.ui    # 配置界面
│   ├── waymca.kcfg         # KConfig 定义
│   └── waymcaconfig.kcfgc  # KConfig 编译器设置
├── data/
│   ├── CMakeLists.txt      # 数据文件 CMake
│   ├── waymca.json         # 插件元数据
│   └── shaders/
│       └── waymca.frag     # GLSL fragment shader
├── build.sh                # 构建脚本
├── install-cpp.sh          # 安装脚本
└── uninstall-cpp.sh        # 卸载脚本
```

## 开发说明

### 修改 Shader

编辑 `data/shaders/waymca.frag`，然后重新构建和安装。

### 添加配置选项

1. 在 `src/waymca.kcfg` 中定义配置项
2. 在 `src/waymca_config.ui` 中添加 UI 控件
3. 在 `src/waymca.h` 和 `src/waymca.cpp` 中使用配置值

### 调试

查看 KWin 日志：
```bash
journalctl --user -f | grep -i waymca
```

或启用详细日志：
```bash
QT_LOGGING_RULES="kwin_*.debug=true" kwin_wayland --replace
```

## 参考资料

- KWin 效果 API：https://develop.kde.org/docs/plasma/kwin/
- 范例效果：`/data/03.prjt/waymca/kwin/src/plugins/`
  - `blur/` - 模糊效果
  - `invert/` - 反色效果
  - `colorblindnesscorrection/` - 色盲矫正

## 许可证

GPL-3.0 - 详见 LICENSE 文件
