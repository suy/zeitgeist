add_rules("mode.debug", "mode.release")

if is_plat("windows") then
    set_runtimes("MT")
end

option('qt6')
    set_default(false)
    set_showmenu(true)
option_end()

local qt = get_config('qt')
local qt6 = get_config('qt6')

package("quazip")
    add_deps("cmake")
    add_deps("minizip")
    set_sourcedir(path.join(os.scriptdir(), "3rdparty/quazip"))
    set_policy("package.install_always", true)
    on_install(function(package)
        local configs = {}
        if qt and qt ~= "" then
            table.insert(configs, "-DCMAKE_PREFIX_PATH=" .. qt)
        end
        if qt6 then
            table.insert(configs, "-DQUAZIP_QT_MAJOR_VERSION=6")
        end
        table.insert(configs, "-DCMAKE_BUILD_TYPE=" .. (package:debug() and "Debug" or "Release"))
        table.insert(configs, "-DBUILD_SHARED_LIBS=" .. (package:config("shared") and "ON" or "OFF"))
        import("package.tools.cmake").install(package, configs)
    end)
    add_includedirs(string.format("include/QuaZip-Qt%s-1.0/quazip", qt6 and "6" or "5"))
package_end()

add_requires("minizip")
add_requires("quazip")

target("zeitgeist")
    set_kind("binary")
    if qt6 then
        add_frameworks("QtCore5Compat")
    end
    add_rules("qt.widgetapp")
    add_files("src/*.cpp")
    add_files("src/*.h")
    add_packages("quazip")
    add_packages("minizip")
