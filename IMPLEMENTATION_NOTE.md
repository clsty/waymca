# WayMCA 实现问题说明

## 核心问题

经过深入研究 KWin 6 源代码，我发现了一个**根本性的架构问题**：

### KWin 6 的 QML 效果 API 不支持全屏 Shader 效果

**当前实现问题：**
- `X-Plasma-API: declarativescript` 只支持 QML 脚本效果
- KWin 6 的 QML API 只提供了 `SceneEffect` 类型，用于创建 UI overlay（如窗口选择器、特效面板等）
- **没有提供用于全屏 shader 处理的 QML 类型**（`FullScreenEffect` 不存在于 QML API 中）

**错误信息解释：**
```
FullScreenEffect is not a type
```
这是因为 `FullScreenEffect` 是 C++ 类，从未在 QML 中注册。

查看 `/data/03.prjt/waymca/kwin/src/scripting/scripting.cpp` 中注册的 QML 类型：
- `SceneEffect` - 用于 UI overlay
- `WindowThumbnail` - 窗口缩略图
- `DesktopBackground` - 桌面背景
- `ScreenEdgeHandler` - 屏幕边缘触发
- 等等...

**但是没有**用于全屏 shader 渲染的类型！

## 技术分析

### QML Script 效果的限制

KWin 6 的 declarativescript 效果架构：
1. 主要用于创建**叠加 UI 界面**
2. 使用 QtQuick 组件（Rectangle、Text、MouseArea 等）
3. **不提供直接访问渲染管线的能力**

### 全屏 Shader 效果需要什么

要实现像 WayMCA 这样的全屏 shader 效果，需要：
1. 捕获整个合成器的输出
2. 应用 OpenGL shader 到每个像素
3. 将修改后的帧缓冲输出到屏幕

这需要 C++ OffscreenEffect 类，它提供：
- `drawWindow()` - 拦截窗口渲染
- `redirect()` - 将渲染重定向到离屏纹理
- `setShader()` - 应用自定义 OpenGL shader
- 直接访问 OpenGL 上下文

## 解决方案

### 方案 A：用 C++ 重写插件（推荐）

创建一个真正的 KWin C++ 插件：

**所需文件结构：**
```
waymca/
├── CMakeLists.txt
├── waymca.h
├── waymca.cpp
├── waymca.json
├── shaders/
│   └── waymca.frag
└── kcm/
    ├── CMakeLists.txt
    ├── waymca_config.h
    ├── waymca_config.cpp
    └── waymca_config.ui
```

**关键代码示例（waymca.h）：**
```cpp
#include <kwineffects.h>
#include <kwinoffscreeneffect.h>

class WaymcaEffect : public KWin::OffscreenEffect
{
    Q_OBJECT
public:
    WaymcaEffect();
    ~WaymcaEffect() override;
    
    void reconfigure(ReconfigureFlags flags) override;
    void prePaintScreen(ScreenPrePaintData &data, std::chrono::milliseconds presentTime) override;
    void drawWindow(const RenderTarget &renderTarget, const RenderViewport &viewport, 
                    EffectWindow *w, int mask, const QRegion &region, WindowPaintData &data) override;
    
    static bool supported();

private:
    void loadShader();
    
    std::unique_ptr<GLShader> m_shader;
    int m_greenBlurRadius;
    int m_blueBlurRadius;
    bool m_useGaussianBlur;
};
```

**优点：**
- ✅ 完全访问渲染管线
- ✅ 高性能
- ✅ KWin 原生支持
- ✅ 可以在所有 KWin 6 安装上工作

**缺点：**
- ❌ 需要 C++ 知识
- ❌ 需要编译环境
- ❌ 开发周期较长

### 方案 B：使用替代技术

**选项 1：Compiz（如果可用）**
- Compiz 对脚本效果支持更好
- 但 Wayland 支持有限

**选项 2：自定义合成器**
- 使用 wlroots 创建自定义 Wayland 合成器
- 工作量巨大

**选项 3：后处理方法**
- 使用屏幕截图 + 图像处理
- 延迟高，不适合实时使用

## 当前状态

### 已修复的问题
✅ 配置文件 spinbox 警告（`kcfg_*_spinbox` 名称冲突）
✅ GLSL shader 语法（compatib版本 140）
✅ 研究clear了 KWin 6 API 限制

### 仍然存在的根本问题
❌ **QML API 不支持全屏 shader 效果** - 这是架构限制，无法通过修改现有代码解决

## 建议

### 对于项目作者

**短期（如果想快速演示概念）：**
1. 创建一个 Python/Shell 脚本，使用 `grim`（Wayland 截图工具）+ ImageMagick 进行后处理
2. 这只是概念验证，不是实用解决方案

**长期（正确的方法）：**
1. 将项目重写为 C++ KWin 插件
2. 参考 KWin 源码中的 blur、invert 等效果
3. 代码框架：
   ```bash
   # 基于现有的模糊效果
   cp -r /data/03.prjt/waymca/kwin/src/plugins/blur waymca-cpp
   # 修改代码以实现色差模糊
   ```

### 对于用户

如果您只想使用此效果而不开发，您需要：
1. 等待 C++ 版本发布
2. 或者尝试其他降低对比度/蓝光的方法：
   - KWin 的夜间色彩效果
   - 显示器硬件设置
   - 使用眼镜/隐形眼镜的光学滤镜

## 参考资料

### KWin C++ 效果示例
- **Blur 效果**：`/data/03.prjt/waymca/kwin/src/plugins/blur/`
- **Invert 效果**：`/data/03.prjt/waymca/kwin/src/plugins/invert/`
- **ColorBlindness 效果**：`/data/03.prjt/waymca/kwin/src/plugins/colorblindnesscorrection/`

### 文档
- KWin 效果教程：https://develop.kde.org/docs/plasma/kwin/
- OffscreenEffect API：`/data/03.prjt/waymca/kwin/src/effect/offscreeneffect.h`
- 示例效果：`/data/03.prjt/waymca/kwin/examples/`

## 结论

**目前的 QML 实现无法工作**，这是 KWin 6 架构的根本限制。要实现 WayMCA 的目标，**必须用 C++ 重写为原生 KWin 插件**。

我已经修复了配置文件的警告，但这对主要问题没有帮助。抱歉带来坏消息，但这是经过深入研究 KWin 源代码后的准确结论。
