#include "FileFormats/2da.hpp"
#include "Utility/Assert.hpp"

namespace {

int TwoDAExample(char* path);

int TwoDAExample(char* path)
{
    using namespace FileFormats::TwoDA;

    Raw::TwoDA raw2da;
    bool loaded = Raw::TwoDA::ReadFromFile(path, &raw2da);

    std::printf("2DA Lines: %zu\n", raw2da.m_Lines.size());

    if (!loaded)
    {
        std::printf("Failed to load the 2DA file.\n");
        return 1;
    }

    Friendly::TwoDA twoDA(std::move(raw2da));

    std::vector<std::string> columnNames;

    // Convert column names from map to a flat vector.
    for (auto& kvp : twoDA.GetColumnNames())
    {
        if (columnNames.size() < kvp.second + 1)
        {
            columnNames.resize(kvp.second + 1);
        }
        columnNames[kvp.second] = kvp.first;
    }

    std::printf("\n");

    for (std::string const& column : columnNames)
    {
        std::printf("%-16s ", column.c_str());
    }

    std::printf("\n\n");

    for (Friendly::TwoDARow const& row : twoDA)
    {
        for (std::size_t i = 0; i < row.Size(); ++i)
        {
            Friendly::TwoDAEntry const& entry = row[i];

            // Copy here so we can modify it later.
            std::string str = entry.m_IsEmpty ? "[MISSING_DATA]" : entry.m_Data;

            if (!entry.m_IsEmpty && str.find(' ') != std::string::npos)
            {
                // We have whitespace, so we should surround the entry with quotes when pretty printing it.
                str = "\"" + str + "\"";
            }

            std::printf("%-16s ", str.c_str());
        }

        std::printf("\n");
    }

    // Then save the 2da back out next to the original.
    char pathBuffer[1024];
    std::sprintf(pathBuffer, "%s.1", path);
    bool written = twoDA.WriteToFile(pathBuffer);
    ASSERT(written);

    return 0;
}

}

int main(int argc, char** argv)
{
    ASSERT(argc == 2);
    return TwoDAExample(argv[1]);
}
