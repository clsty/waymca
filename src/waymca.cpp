/*
 * WayMCA - Myopic Chromatic Aberration Effect for KWin
 * Copyright (C) 2024 WayMCA Contributors
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "waymca.h"
#include "waymca.h.moc"
#include "waymcaconfig.h"

#include <QFile>
#include <QStandardPaths>
#include <KConfigGroup>
#include <KPluginFactory>

namespace KWin
{

K_PLUGIN_CLASS(WayMCAEffect)

WayMCAEffect::WayMCAEffect()
    : Effect()
    , m_greenBlurRadius(3)
    , m_blueBlurRadius(8)
    , m_useGaussianBlur(true)
    , m_valid(false)
{
    loadConfig();
    loadShader();

    connect(effects, &EffectsHandler::windowAdded, this, &WayMCAEffect::slotWindowAdded);
    connect(effects, &EffectsHandler::windowDeleted, this, &WayMCAEffect::slotWindowDeleted);
}

WayMCAEffect::~WayMCAEffect()
{
}

bool WayMCAEffect::supported()
{
    return effects->isOpenGLCompositing();
}

bool WayMCAEffect::enabledByDefault()
{
    return false;
}

void WayMCAEffect::loadConfig()
{
    WayMCAConfig::self()->load();
    m_greenBlurRadius = WayMCAConfig::greenBlurRadius();
    m_blueBlurRadius = WayMCAConfig::blueBlurRadius();
    m_useGaussianBlur = WayMCAConfig::useGaussianBlur();
}

void WayMCAEffect::loadShader()
{
    m_valid = false;

    if (!supported()) {
        return;
    }

    // Find the shader file
    QString fragmentPath = QStandardPaths::locate(QStandardPaths::GenericDataLocation,
                                                   QStringLiteral("kwin/effects/waymca/waymca.frag"));
    
    if (fragmentPath.isEmpty()) {
        qWarning() << "WayMCA: Could not find fragment shader file";
        return;
    }

    QFile fragmentFile(fragmentPath);
    if (!fragmentFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "WayMCA: Could not open fragment shader file:" << fragmentPath;
        return;
    }

    QByteArray fragmentSource = fragmentFile.readAll();
    fragmentFile.close();

    // Create shader
    m_shader = std::make_unique<GLShader>(GLShader::ExplicitLinking);
    m_shader->setVertexShader(ShaderManager::instance()->generateShaderFromFile(
        ShaderTrait::MapTexture, QString(), QStringLiteral("")));
    m_shader->setFragmentShader(fragmentSource);

    if (!m_shader->link()) {
        qWarning() << "WayMCA: Failed to link shader";
        m_shader.reset();
        return;
    }

    m_valid = true;
}

void WayMCAEffect::reconfigure(ReconfigureFlags flags)
{
    Q_UNUSED(flags)
    loadConfig();
}

void WayMCAEffect::prePaintScreen(ScreenPrePaintData &data, std::chrono::milliseconds presentTime)
{
    if (m_valid && isActive()) {
        data.mask |= PAINT_SCREEN_WITH_TRANSFORMED_WINDOWS;
    }
    effects->prePaintScreen(data, presentTime);
}

void WayMCAEffect::paintScreen(const RenderTarget &renderTarget, const RenderViewport &viewport, 
                                int mask, const QRegion &region, Output *screen)
{
    if (!m_valid || !isActive()) {
        effects->paintScreen(renderTarget, viewport, mask, region, screen);
        return;
    }

    const QSize screenSize = effects->virtualScreenSize();
    
    // Create or recreate offscreen texture if needed
    if (!m_offscreenTexture || m_offscreenTexture->size() != screenSize) {
        m_offscreenTexture = std::make_unique<GLTexture>(GL_RGBA8, screenSize);
        m_offscreenTexture->setFilter(GL_LINEAR);
        m_offscreenTexture->setWrapMode(GL_CLAMP_TO_EDGE);
        
        m_offscreenTarget = std::make_unique<GLFramebuffer>(m_offscreenTexture.get());
        
        if (!m_offscreenTarget->valid()) {
            qWarning() << "WayMCA: Failed to create offscreen framebuffer";
            m_valid = false;
            effects->paintScreen(renderTarget, viewport, mask, region, screen);
            return;
        }
    }

    // Render scene to offscreen texture
    GLFramebuffer::pushFramebuffer(m_offscreenTarget.get());
    effects->paintScreen(renderTarget, viewport, mask, region, screen);
    GLFramebuffer::popFramebuffer();

    // Apply chromatic aberration shader
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    ShaderBinder binder(m_shader.get());
    
    // Set shader uniforms
    m_shader->setUniform(GLShader::Mat4Uniform::ModelViewProjectionMatrix, viewport.projectionMatrix());
    m_shader->setUniform("renderTextureSize", QVector2D(screenSize.width(), screenSize.height()));
    m_shader->setUniform("greenRadius", m_greenBlurRadius);
    m_shader->setUniform("blueRadius", m_blueBlurRadius);
    m_shader->setUniform("useGaussian", m_useGaussianBlur);
    m_shader->setUniform("sampler", 0);

    // Bind the offscreen texture
    m_offscreenTexture->bind();

    // Render a fullscreen quad
    QRectF screenRect(0, 0, screenSize.width(), screenSize.height());
    GLVertexBuffer *vbo = GLVertexBuffer::streamingBuffer();
    vbo->reset();
    
    const QRectF localRect = renderTarget.transform().map(screenRect, Qt::TransformationMode::FastTransformation).boundingRect();
    const QRectF deviceRect = viewport.transform().map(localRect, Qt::TransformationMode::FastTransformation).boundingRect();
    
    QVector<float> verts;
    QVector<float> texCoords;
    
    verts << deviceRect.left() << deviceRect.top();
    verts << deviceRect.left() << deviceRect.bottom();
    verts << deviceRect.right() << deviceRect.bottom();
    verts << deviceRect.right() << deviceRect.bottom();
    verts << deviceRect.right() << deviceRect.top();
    verts << deviceRect.left() << deviceRect.top();
    
    texCoords << 0.0f << 0.0f;
    texCoords << 0.0f << 1.0f;
    texCoords << 1.0f << 1.0f;
    texCoords << 1.0f << 1.0f;
    texCoords << 1.0f << 0.0f;
    texCoords << 0.0f << 0.0f;
    
    vbo->setVertices(verts);
    vbo->setTexCoords(2, texCoords);
    vbo->render(GL_TRIANGLES);

    m_offscreenTexture->unbind();
}

bool WayMCAEffect::isActive() const
{
    return m_valid && effects->isOpenGLCompositing();
}

void WayMCAEffect::slotWindowAdded(EffectWindow *w)
{
    Q_UNUSED(w)
    // Could be used for per-window tracking if needed
}

void WayMCAEffect::slotWindowDeleted(EffectWindow *w)
{
    Q_UNUSED(w)
    // Could be used for cleanup if needed
}

} // namespace KWin

#include "waymca.moc"
