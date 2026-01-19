/*
    SPDX-FileCopyrightText: 2024 WayMCA Contributors
    SPDX-License-Identifier: GPL-3.0-or-later
*/

#include <KPluginFactory>
#include <KCModule>
#include <KSharedConfig>
#include <KConfigGroup>

#include <QSpinBox>
#include <QCheckBox>
#include <QSlider>

#include "ui_waymca_config.h"

namespace KWin
{

class WaymcaEffectConfig : public KCModule
{
    Q_OBJECT

public:
    explicit WaymcaEffectConfig(QObject *parent, const KPluginMetaData &data);
    ~WaymcaEffectConfig() override;

    void save() override;
    void load() override;
    void defaults() override;

private Q_SLOTS:
    void onValueChanged();

private:
    Ui::WayMCAConfig ui;
    KSharedConfig::Ptr m_config;
};

WaymcaEffectConfig::WaymcaEffectConfig(QObject *parent, const KPluginMetaData &data)
    : KCModule(parent, data)
{
    ui.setupUi(widget());
    m_config = KSharedConfig::openConfig(QStringLiteral("kwinrc"));
    
    // Connect signals to track changes
    connect(ui.kcfg_GreenBlurRadius, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &WaymcaEffectConfig::onValueChanged);
    connect(ui.kcfg_BlueBlurRadius, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &WaymcaEffectConfig::onValueChanged);
    connect(ui.kcfg_UseGaussianBlur, &QCheckBox::stateChanged,
            this, &WaymcaEffectConfig::onValueChanged);
    connect(ui.kcfg_FullScreenBlur, &QCheckBox::stateChanged,
            this, &WaymcaEffectConfig::onValueChanged);
    connect(ui.kcfg_FullScreenBlurRadius, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &WaymcaEffectConfig::onValueChanged);
    
    load();
}

WaymcaEffectConfig::~WaymcaEffectConfig()
{
}

void WaymcaEffectConfig::onValueChanged()
{
    setNeedsSave(true);
}

void WaymcaEffectConfig::save()
{
    KConfigGroup conf = m_config->group(QStringLiteral("Effect-waymca"));
    conf.writeEntry("GreenBlurRadius", ui.kcfg_GreenBlurRadius->value());
    conf.writeEntry("BlueBlurRadius", ui.kcfg_BlueBlurRadius->value());
    conf.writeEntry("UseGaussianBlur", ui.kcfg_UseGaussianBlur->isChecked());
    conf.writeEntry("FullScreenBlur", ui.kcfg_FullScreenBlur->isChecked());
    conf.writeEntry("FullScreenBlurRadius", ui.kcfg_FullScreenBlurRadius->value());
    conf.sync();
    
    KCModule::save();
    setNeedsSave(false);
}

void WaymcaEffectConfig::load()
{
    KConfigGroup conf = m_config->group(QStringLiteral("Effect-waymca"));
    ui.kcfg_GreenBlurRadius->setValue(conf.readEntry("GreenBlurRadius", 3));
    ui.kcfg_BlueBlurRadius->setValue(conf.readEntry("BlueBlurRadius", 8));
    ui.kcfg_UseGaussianBlur->setChecked(conf.readEntry("UseGaussianBlur", true));
    ui.kcfg_FullScreenBlur->setChecked(conf.readEntry("FullScreenBlur", false));
    ui.kcfg_FullScreenBlurRadius->setValue(conf.readEntry("FullScreenBlurRadius", 10));
    
    KCModule::load();
    setNeedsSave(false);
}

void WaymcaEffectConfig::defaults()
{
    ui.kcfg_GreenBlurRadius->setValue(3);
    ui.kcfg_BlueBlurRadius->setValue(8);
    ui.kcfg_UseGaussianBlur->setChecked(true);
    ui.kcfg_FullScreenBlur->setChecked(false);
    ui.kcfg_FullScreenBlurRadius->setValue(10);
    
    setNeedsSave(true);
}

K_PLUGIN_CLASS(WaymcaEffectConfig)

} // namespace KWin

#include "waymca_config.moc"
