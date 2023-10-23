add_rules("mode.debug", "mode.release")
add_requires("sfml","imgui","imgui-sfml")

set_allowedarchs("windows|x64")
set_warnings("allextra")

set_rundir("bin") -- Le dossier courant lors de l'exécution des binaires (depuis VS) - c'est depuis ce dossier que les chemins commencent
set_targetdir("bin/$(plat)_$(arch)_$(mode)") -- Le dossier de sortie des binaires, les $(X) sont remplacés par les valeurs existantes (plat = windows, arch = x64 et mode = debug)

set_languages("c++17")

-- Désactivation de quelques warnings pas utiles dans notre cas avec VS
if is_plat("windows") then
    set_runtimes("MD")
    add_cxflags("/wd4251") -- Disable warning: class needs to have dll-interface to be used by clients of class blah blah blah
    add_cxflags("/wd4275") -- Disable warning: DLL-interface class 'class_1' used as base for DLL-interface blah
end

target("CoraEngineLibrarie")
    set_kind("shared") 
    add_defines("CASSOULET_API")
    add_headerfiles("include/CoraEngineLibrarie/*.h", "include/CoraEngineLibrarie/*.hpp", "include/CoraEngineLibrarie/*.inl")
    add_includedirs("include", { public = true })
    add_files("src/CoraEngineLibrarie/*.cpp")
    add_packages("sfml","imgui","imgui-sfml", {public = true})

target("CoraEngineGame")
    set_kind("binary")
    add_deps("CoraEngineLibrarie")
    add_headerfiles("include/CoraEngineGame/*.h", "include/CoraEngineGame/*.hpp")
    add_files("src/CoraEngineGame/**.cpp")
