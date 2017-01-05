import qbs 1.0

TiledPlugin {
    cpp.defines: ["JSON_LIBRARY"]

    files: [
        "rpd_global.h",
        "rpdplugin.cpp",
        "rpdplugin.h",
        "plugin.json",
        "qjsonparser/json.cpp",
        "qjsonparser/json.h",
    ]
}
