add_rules("mode.debug", "mode.release")

if is_plat("windows") then
    set_runtimes("MT")
end

package("quazip")
    add_deps("cmake")
    -- add_packages("minizip")
    add_deps("minizip")
    set_sourcedir(path.join(os.scriptdir(), "3rdparty/quazip"))
    set_policy("package.install_always", true)
    on_install(function(package)
        local configs = {}
        table.insert(configs, "-DCMAKE_BUILD_TYPE=" .. (package:debug() and "Debug" or "Release"))
        table.insert(configs, "-DBUILD_SHARED_LIBS=" .. (package:config("shared") and "ON" or "OFF"))
        import("package.tools.cmake").install(package, configs)
    end)
    add_includedirs("include/QuaZip-Qt5-1.0/quazip")
package_end()

add_requires("minizip")
add_requires("quazip")

target("zeitgeist")
    set_kind("binary")
    add_rules("qt.widgetapp")
    add_files("src/*.cpp")
    add_files("src/*.h")
    add_packages("quazip")
    add_packages("minizip")
