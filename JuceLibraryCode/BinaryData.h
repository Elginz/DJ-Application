/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   bassDrum_mp3;
    const int            bassDrum_mp3Size = 64226;

    extern const char*   bassOpen_mp3;
    const int            bassOpen_mp3Size = 16896;

    extern const char*   cymbal_mp3;
    const int            cymbal_mp3Size = 48907;

    extern const char*   drumSticks_mp3;
    const int            drumSticks_mp3Size = 30661;

    extern const char*   floorDrum_mp3;
    const int            floorDrum_mp3Size = 94992;

    extern const char*   guitar_mp3;
    const int            guitar_mp3Size = 37920;

    extern const char*   hiHat_mp3;
    const int            hiHat_mp3Size = 70942;

    extern const char*   slapBass_mp3;
    const int            slapBass_mp3Size = 61022;

    extern const char*   snare_mp3;
    const int            snare_mp3Size = 31622;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 9;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Points to the start of a list of resource filenames.
    extern const char* originalFilenames[];

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes);

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding original, non-mangled filename (or a null pointer if the name isn't found).
    const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
}
