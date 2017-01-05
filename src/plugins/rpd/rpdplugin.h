#ifndef RPDPLUGIN_H
#define RPDPLUGIN_H

#include "rpd_global.h"

#include "mapformat.h"
#include "plugin.h"
#include "tilesetformat.h"

#include <QObject>

namespace Tiled {
class Map;
}

namespace Rpd {

class JSONSHARED_EXPORT RpdPlugin : public Tiled::Plugin
{
    Q_OBJECT
    Q_INTERFACES(Tiled::Plugin)
    Q_PLUGIN_METADATA(IID "org.mapeditor.Plugin" FILE "plugin.json")

public:
    void initialize() override;
};


class RPDSHARED_EXPORT RpdMapFormat : public Tiled::MapFormat
{
    Q_OBJECT
    Q_INTERFACES(Tiled::MapFormat)

public:
    enum SubFormat {
        Json,
        JavaScript,
    };

    RpdMapFormat(SubFormat subFormat, QObject *parent = nullptr);

    Tiled::Map *read(const QString &fileName) override;
    bool supportsFile(const QString &fileName) const override;

    bool write(const Tiled::Map *map, const QString &fileName) override;

    QString nameFilter() const override;
    QString errorString() const override;

protected:
    QString mError;
    SubFormat mSubFormat;
};


class RPDSHARED_EXPORT RpdTilesetFormat : public Tiled::TilesetFormat
{
    Q_OBJECT
    Q_INTERFACES(Tiled::TilesetFormat)

public:
    RpdTilesetFormat(QObject *parent = nullptr);

    Tiled::SharedTileset read(const QString &fileName) override;
    bool supportsFile(const QString &fileName) const override;

    bool write(const Tiled::Tileset &tileset, const QString &fileName) override;

    QString nameFilter() const override;
    QString errorString() const override;

protected:
    QString mError;
};

} // namespace Rpd

#endif // RPDPLUGIN_H
