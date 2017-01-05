/*
 * JSON Tiled Plugin
 * Copyright 2011, Porfírio José Pereira Ribeiro <porfirioribeiro@gmail.com>
 * Copyright 2011, Thorbjørn Lindeijer <thorbjorn@lindeijer.nl>
 *
 * This file is part of Tiled.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "rpdplugin.h"

#include "maptovariantconverter.h"
#include "varianttomapconverter.h"
#include "savefile.h"

#include "qjsonparser/json.h"

#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTextStream>

namespace Rpd {

void RpdPlugin::initialize()
{
    addObject(new RpdMapFormat(RpdMapFormat::Rpd, this));
    addObject(new RpdTilesetFormat(this));
}


RpdMapFormat::RpdMapFormat(SubFormat subFormat, QObject *parent)
    : mSubFormat(subFormat)
{}

bool RpdMapFormat::write(const Tiled::Map *map, const QString &fileName)
{
    Tiled::SaveFile file(fileName);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        mError = tr("Could not open file for writing.");
        return false;
    }

    Tiled::MapToVariantConverter converter;
    QVariant variant = converter.toVariant(map, QFileInfo(fileName).dir());

    JsonWriter writer;
    writer.setAutoFormatting(true);

    if (!writer.stringify(variant)) {
        // This can only happen due to coding error
        mError = writer.errorString();
        return false;
    }

    QTextStream out(file.device());

    out << writer.result();

    out.flush();

    if (file.error() != QFileDevice::NoError) {
        mError = tr("Error while writing file:\n%1").arg(file.errorString());
        return false;
    }

    if (!file.commit()) {
        mError = file.errorString();
        return false;
    }

    return true;
}

QString RpdMapFormat::nameFilter() const
{
    return tr("Remixed Pixel Dungeon levels (*.json)");
}

QString RpdMapFormat::errorString() const
{
    return mError;
}


RpdTilesetFormat::RpdTilesetFormat(QObject *parent)
    : Tiled::TilesetFormat(parent)
{
}

Tiled::SharedTileset RpdTilesetFormat::read(const QString &fileName)
{
    return Tiled::SharedTileset();
}

bool RpdTilesetFormat::supportsFile(const QString &fileName) const
{
    return false;
}

bool RpdTilesetFormat::write(const Tiled::Tileset &tileset,
                              const QString &fileName)
{
    Tiled::SaveFile file(fileName);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        mError = tr("Could not open file for writing.");
        return false;
    }

    Tiled::MapToVariantConverter converter;
    QVariant variant = converter.toVariant(tileset, QFileInfo(fileName).dir());

    JsonWriter writer;
    writer.setAutoFormatting(true);

    if (!writer.stringify(variant)) {
        // This can only happen due to coding error
        mError = writer.errorString();
        return false;
    }

    QTextStream out(file.device());
    out << writer.result();
    out.flush();

    if (file.error() != QFileDevice::NoError) {
        mError = tr("Error while writing file:\n%1").arg(file.errorString());
        return false;
    }

    if (!file.commit()) {
        mError = file.errorString();
        return false;
    }

    return true;
}

QString RpdTilesetFormat::nameFilter() const
{
    return tr("Json tileset files (*.json)");
}

QString RpdTilesetFormat::errorString() const
{
    return mError;
}

} // namespace Rpd
