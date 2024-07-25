#include "Cache.hh"
#include "core/Common.hh"
#include <map>

#include "PatternList.hh"
#include "memory/GameAddress.hh"
#include "utils/ContainerUtils.hh"

void
MemorySignatureCache::ReadFromFile (const std::string &file)
{
    size_t signature;
    int    numAddresses;

    auto f = Rainbomizer::Common::GetRainbomizerDataFile (file, "rb");

    if (!f)
        return;

    fread (&signature, sizeof (signature), 1, f);

    if (signature != FileSignature)
        return;

    fread (&numAddresses, sizeof (numAddresses), 1, f);
    std::map<size_t, size_t> addresses;

    for (size_t i = 0; i < numAddresses; i++)
        {
            int address;
            int value;

            fread (&address, sizeof (address), 1, f);
            fread (&value, sizeof (value), 1, f);

            addresses[address] = value;
        }

    auto SetGameAddressValue = [&addresses]<std::size_t I> () {
        constexpr const auto &currentPattern = s_Patterns[I];
        if (auto *value = LookupMap (addresses, currentPattern.address))
            GameAddress<currentPattern.address>::SetResolvedAddress (*value);
    };

    [SetGameAddressValue]<std::size_t... I> (std::index_sequence<I...>) {
        (SetGameAddressValue.template operator()<I> (), ...);
    }(std::make_index_sequence<std::size (s_Patterns)>{});
}

void
MemorySignatureCache::WriteToFile (const std::string &filename)
{
    auto f = Rainbomizer::Common::GetRainbomizerDataFile (filename, "wb");

    if (!f)
        return;

    int currentAddress = 0;
    int patternSize = s_Patterns.size ();

    fwrite (&FileSignature, sizeof (FileSignature), 1, f);
    fwrite (&patternSize, sizeof (int), 1, f);

    auto WriteAddress = [&f, &currentAddress]<std::size_t I> () {

        constexpr const auto &currentPattern = s_Patterns[I];

        // Do not write addresses with no patterns
        if constexpr (!DoesPatternExist<currentPattern.address> ())
            return;

	currentAddress = currentPattern.address;
        fwrite (&currentAddress, sizeof (currentPattern.address), 1, f);
        fwrite (&GameAddress<currentPattern.address>::GetResolvedAddress (),
                sizeof (uintptr_t), 1, f);
    };

    [WriteAddress]<std::size_t... I> (std::index_sequence<I...>) {
        (WriteAddress.template operator()<I> (), ...);
    }(std::make_index_sequence<std::size (s_Patterns)>{});
}
