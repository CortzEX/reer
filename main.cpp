#include <Windows.h>
#include <iostream>
#include <string>
#include <vector>

int main()
{
    const std::vector<std::string> dlls = {
        "ChatReaderLunar.dll",
        "slinky_library.dll",
        "slinkyhook.dll"
    };

    char exePath[MAX_PATH]{};

    if (!GetModuleFileNameA(nullptr, exePath, MAX_PATH))
    {
        std::cerr << "Failed to get executable path.\n";
        return 1;
    }

    std::string directory(exePath);

    size_t slash = directory.find_last_of("\\/");

    if (slash != std::string::npos)
        directory.resize(slash + 1);
    else
        directory.clear();

    std::vector<HMODULE> loadedModules;

    for (const auto& dllName : dlls)
    {
        std::string fullPath = directory + dllName;

        if (GetFileAttributesA(fullPath.c_str()) ==
            INVALID_FILE_ATTRIBUTES)
        {
            std::cerr << "DLL not found: "
                      << fullPath << '\n';
            continue;
        }

        HMODULE module = LoadLibraryA(fullPath.c_str());

        if (!module)
        {
            std::cerr << "Failed to load: "
                      << dllName
                      << " | Error: "
                      << GetLastError()
                      << '\n';
            continue;
        }

        std::cout << "Loaded: "
                  << dllName << '\n';

        loadedModules.push_back(module);
    }

    std::cout << "\nAll DLLs processed.\n";
    std::cin.get();

    // Unload them when the program exits.
    for (HMODULE module : loadedModules)
        FreeLibrary(module);

    return 0;
}
