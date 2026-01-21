/*
    SPDX-FileCopyrightText: 2024 WayMCA Contributors
    SPDX-License-Identifier: GPL-3.0-or-later
*/

#include "waymca_config.h"

// KConfigSkeleton
#include "waymcaconfig.h"

#include <KPluginFactory>
#include "kwineffects_interface.h"

namespace KWin
{

K_PLUGIN_CLASS(WaymcaEffectConfig)

WaymcaEffectConfig::WaymcaEffectConfig(QObject *parent, const KPluginMetaData &data)
    : KCModule(parent, data)
{
    ui.setupUi(widget());
    WaymcaConfig::instance("kwinrc");
    addConfig(WaymcaConfig::self(), widget());
}

WaymcaEffectConfig::~WaymcaEffectConfig()
{
}

void WaymcaEffectConfig::save()
{
    KCModule::save();

    OrgKdeKwinEffectsInterface interface(QStringLiteral("org.kde.KWin"),
                                         QStringLiteral("/Effects"),
                                         QDBusConnection::sessionBus());
    interface.reconfigureEffect(QStringLiteral("waymca"));
}

} // namespace KWin

#include "waymca_config.moc"

#include "moc_waymca_config.cpp"
