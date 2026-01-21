/*
    SPDX-FileCopyrightText: 2024 WayMCA Contributors
    SPDX-License-Identifier: GPL-3.0-or-later
*/

#pragma once

#include "ui_waymca_config.h"
#include <KCModule>

namespace KWin
{

class WaymcaEffectConfig : public KCModule
{
    Q_OBJECT

public:
    explicit WaymcaEffectConfig(QObject *parent, const KPluginMetaData &data);
    ~WaymcaEffectConfig() override;

    void save() override;

private:
    ::Ui::WayMCAConfig ui;
};

} // namespace KWin
