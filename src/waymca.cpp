/*
    SPDX-FileCopyrightText: 2024 WayMCA Contributors
    SPDX-License-Identifier: GPL-3.0-or-later
*/

#include "waymca.h"

#include <core/rendertarget.h>
#include <core/renderviewport.h>
#include <effect/effecthandler.h>
#include <opengl/glshader.h>
#include <opengl/glshadermanager.h>
#include <opengl/gltexture.h>
#include <opengl/glframebuffer.h>
#include <opengl/glvertexbuffer.h>
#include <opengl/glutils.h>

#include <QAction>
#include <QFile>
#include <QStandardPaths>
#include <QTextStream>

#include <KConfig>
#include <KConfigGroup>
#include <KGlobalAccel>

namespace KWin
{

WaymcaEffect::WaymcaEffect()
    : Effect()
{
    reconfigure(ReconfigureAll);
    
    // Register global shortcut
    QAction *toggleAction = new QAction(this);
    toggleAction->setObjectName(QStringLiteral("ToggleWaymca"));
    toggleAction->setText(QStringLiteral("Toggle WayMCA Effect"));
    KGlobalAccel::self()->setDefaultShortcut(toggleAction, QList<QKeySequence>());
    KGlobalAccel::self()->setShortcut(toggleAction, QList<QKeySequence>());
    connect(toggleAction, &QAction::triggered, this, &WaymcaEffect::toggleEffect);
}

WaymcaEffect::~WaymcaEffect() = default;

bool WaymcaEffect::supported()
{
    return effects->isOpenGLCompositing();
}

bool WaymcaEffect::enabledByDefault()
{
    return false;
}

void WaymcaEffect::reconfigure(ReconfigureFlags flags)
{
    Q_UNUSED(flags)

    KConfig config(QStringLiteral("kwinrc"));
    KConfigGroup conf(&config, QStringLiteral("Effect-waymca"));
    
    m_greenBlurRadius = conf.readEntry("GreenBlurRadius", 3);
    m_blueBlurRadius = conf.readEntry("BlueBlurRadius", 8);
    m_useGaussianBlur = conf.readEntry("UseGaussianBlur", true);
    m_fullScreenBlurMode = conf.readEntry("FullScreenBlur", false);
    m_fullScreenBlurRadius = conf.readEntry("FullScreenBlurRadius", 10);

    loadShader();
    
    // Request repaint to apply new settings
    effects->addRepaintFull();
}

void WaymcaEffect::loadShader()
{
    m_valid = false;
    m_shader.reset();

    if (!supported()) {
        return;
    }

    // Load shader from file
    const QString fragmentPath = QStandardPaths::locate(
        QStandardPaths::GenericDataLocation,
        QStringLiteral("kwin/effects/waymca/shaders/waymca.frag"));

    if (fragmentPath.isEmpty()) {
        qWarning() << "WayMCA: Could not find shader file";
        return;
    }

    QFile file(fragmentPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "WayMCA: Could not open shader file:" << fragmentPath;
        return;
    }

    QTextStream stream(&file);
    const QString fragmentSource = stream.readAll();
    file.close();

    m_shader = ShaderManager::instance()->generateCustomShader(ShaderTrait::MapTexture, QByteArray(), fragmentSource.toUtf8());

    if (!m_shader || !m_shader->isValid()) {
        qWarning() << "WayMCA: Failed to load shader";
        m_shader.reset();
        return;
    }

    m_valid = true;
    qDebug() << "WayMCA: Shader loaded successfully";
}

void WaymcaEffect::updateShaderUniforms()
{
    if (!m_valid || !m_shader) {
        return;
    }

    ShaderBinder binder(m_shader.get());
    
    m_shader->setUniform("greenBlurRadius", static_cast<float>(m_greenBlurRadius));
    m_shader->setUniform("blueBlurRadius", static_cast<float>(m_blueBlurRadius));
    m_shader->setUniform("useGaussianBlur", m_useGaussianBlur ? 1.0f : 0.0f);
    m_shader->setUniform("fullScreenBlur", m_fullScreenBlurMode ? 1.0f : 0.0f);
    m_shader->setUniform("fullScreenBlurRadius", static_cast<float>(m_fullScreenBlurRadius));
}

bool WaymcaEffect::ensureResources()
{
    if (!m_valid) {
        return false;
    }
    
    return true;
}

void WaymcaEffect::prePaintScreen(ScreenPrePaintData &data, std::chrono::milliseconds presentTime)
{
    // Set flag to capture the screen
    if (m_valid) {
        m_captureInProgress = true;
    }
    
    effects->prePaintScreen(data, presentTime);
}

void WaymcaEffect::paintScreen(const RenderTarget &renderTarget, const RenderViewport &viewport, int mask, const QRegion &region, Output *screen)
{
    if (!m_valid || !m_shader) {
        // Effect not ready, just render normally
        effects->paintScreen(renderTarget, viewport, mask, region, screen);
        m_captureInProgress = false;
        return;
    }

    // Render everything normally first, but to an offscreen texture
    const QRect screenRect = viewport.renderRect().toRect();
    const QSize screenSize = screenRect.size();
    
    // Create or update the FBO and texture if needed
    if (!m_fbo || !m_texture || m_texture->size() != screenSize) {
        m_texture = std::make_unique<GLTexture>(GL_RGBA8, screenSize);
        m_texture->setFilter(GL_LINEAR);
        m_texture->setWrapMode(GL_CLAMP_TO_EDGE);
        m_fbo = std::make_unique<GLFramebuffer>(m_texture.get());
        
        if (!m_fbo->valid()) {
            qWarning() << "WayMCA: Failed to create FBO";
            m_valid = false;
            effects->paintScreen(renderTarget, viewport, mask, region, screen);
            m_captureInProgress = false;
            return;
        }
    }
    
    // Render the screen to our offscreen texture
    GLFramebuffer::pushFramebuffer(m_fbo.get());
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    
    effects->paintScreen(renderTarget, viewport, mask, region, screen);
    
    GLFramebuffer::popFramebuffer();
    
    // Now render the offscreen texture to the actual screen with our shader applied
    ShaderBinder shaderBinder(m_shader.get());
    
    // Update shader uniforms
    updateShaderUniforms();
    
    // Set up the transformation matrix
    QMatrix4x4 mvp = viewport.projectionMatrix();
    mvp.translate(screenRect.x(), screenRect.y());
    m_shader->setUniform(GLShader::Mat4Uniform::ModelViewProjectionMatrix, mvp);
    m_shader->setUniform("sampler", 0);
    
    // Bind our captured texture
    m_texture->bind();
    
    // Render a full-screen quad
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    
    // Create vertex data for a full-screen quad
    const QRectF target = QRectF(0, 0, screenSize.width(), screenSize.height());
    const QRectF source = QRectF(0, 0, 1, 1);  // Normalized texture coordinates
    
    GLVertexBuffer *vbo = GLVertexBuffer::streamingBuffer();
    vbo->reset();
    vbo->setAttribLayout(std::span(GLVertexBuffer::GLVertex2DLayout), sizeof(GLVertex2D));
    
    GLVertex2D *verts = (GLVertex2D*)vbo->map(6 * sizeof(GLVertex2D));
    if (verts) {
        // First triangle
        verts[0].position = QVector2D(target.left(), target.top());
        verts[0].texcoord = QVector2D(source.left(), source.top());
        verts[1].position = QVector2D(target.right(), target.top());
        verts[1].texcoord = QVector2D(source.right(), source.top());
        verts[2].position = QVector2D(target.left(), target.bottom());
        verts[2].texcoord = QVector2D(source.left(), source.bottom());
        
        // Second triangle
        verts[3].position = QVector2D(target.right(), target.top());
        verts[3].texcoord = QVector2D(source.right(), source.top());
        verts[4].position = QVector2D(target.right(), target.bottom());
        verts[4].texcoord = QVector2D(source.right(), source.bottom());
        verts[5].position = QVector2D(target.left(), target.bottom());
        verts[5].texcoord = QVector2D(source.left(), source.bottom());
        
        vbo->unmap();
        vbo->render(GL_TRIANGLES);
    }
    
    m_texture->unbind();
    glDisable(GL_BLEND);
    
    m_captureInProgress = false;
}

void WaymcaEffect::postPaintScreen()
{
    m_captureInProgress = false;
    effects->postPaintScreen();
}

bool WaymcaEffect::isActive() const
{
    return m_valid && effects->isOpenGLCompositing();
}

int WaymcaEffect::requestedEffectChainPosition() const
{
    return 99; // Very late in the chain, after composition
}

void WaymcaEffect::toggleEffect()
{
    // Toggle the effect on/off
    if (m_valid) {
        m_valid = false;
    } else {
        loadShader();
    }
    effects->addRepaintFull();
}

} // namespace KWin
