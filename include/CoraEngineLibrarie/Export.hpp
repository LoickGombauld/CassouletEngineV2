#pragma once

#ifdef _WIN32

// Sur Windows, les symboles s'exportent avec l'attribut dllexport mais doivent s'importer avec dllimport
// On utilise donc un define lors de la compilation du moteur (qui ne sera pas présent à son utilisation) pour les distinguer

#ifdef CASSOULET_API
#define CASSOULET_DLL __declspec(dllexport)
#else // CASSOULET_DLL
#define CASSOULET_DLL __declspec(dllimport)
#endif

#else
// Sur les autres systèmes, rendre le symbole visible suffit à l'import comme à l'export

#define CASSOULET_DLL __attribute__((visibility("default")))

#endif