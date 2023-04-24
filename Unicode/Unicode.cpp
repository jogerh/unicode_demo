// Unicode.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <cassert>
#include <iostream>
#include <Windows.h>
#include <filesystem>
#include <fstream>

constexpr auto CentralEuropeanCodePage = 1250;
constexpr auto GreekCodePage = 1253;
constexpr auto HebrewCodePage = 1255;
constexpr auto ArabicCodePage = 1256;
constexpr auto CyrillicCodePage = 1251;

#pragma execution_character_set("utf-8")

std::string ToSingleByteString(const std::wstring& str, int codePage)
{

    int bytes = WideCharToMultiByte(codePage, 0, str.data(), static_cast<int>(str.size()), nullptr, 0, nullptr, nullptr);

    if (bytes == 0)
        throw std::system_error(static_cast<int>(GetLastError()), std::system_category());

    std::string output(bytes, ' ');

    bytes = WideCharToMultiByte(codePage, 0, str.data(), static_cast<int>(str.size()), output.data(), static_cast<int>(output.size()), nullptr, nullptr);

    if (bytes == 0)
        throw std::system_error(static_cast<int>(GetLastError()), std::system_category());

    return { output.begin(), output.begin() + bytes };
}

std::wstring ToMultiByteString(const std::string& str, int codePage)
{
    int bytes = MultiByteToWideChar(codePage, 0, str.data(), static_cast<int>(str.size()), nullptr, 0);

    if (bytes == 0)
        throw std::system_error(static_cast<int>(GetLastError()), std::system_category());

    std::wstring output(bytes, ' ');

    bytes = MultiByteToWideChar(codePage, 0, str.data(), static_cast<int>(str.size()), output.data(), static_cast<int>(output.size()));

    if (bytes == 0)
        throw std::system_error(static_cast<int>(GetLastError()), std::system_category());

    return { output.begin(), output.begin() + bytes };
}

namespace ScConvert {
    std::string ToANSI(const std::wstring& input)
    {
        return ToSingleByteString(input, CP_ACP);
    }
}


void FunWithAnsi()
{
    const std::wstring input = L"Hélène Strauß Jäger";

    // ScConvert on western computer
    const std::string ansi = ScConvert::ToANSI(input);

    // ScConvert on Central European computer
    const std::string centralEuropean = ToSingleByteString(input, CentralEuropeanCodePage);

    // ScConvert on Greek computer
    const std::string greek = ToSingleByteString(input, GreekCodePage);

    // ScConvert on Hebrew computer
    const std::string hebrew = ToSingleByteString(input, HebrewCodePage);

    // ScConvert on Arabic computer
    const std::string arabic = ToSingleByteString(input, ArabicCodePage);

    // ScConvert on Cyrillic computer
    const std::string cyrillic = ToSingleByteString(input, CyrillicCodePage);

    // Utf-8
    const std::string utf8 = ToSingleByteString(input, CP_UTF8);

    // Utf-16
    const std::wstring utf16 = ToMultiByteString(utf8, CP_UTF8);

    // SetConsoleOutputCP(CP_UTF8);
    std::cout << utf8.c_str() << std::endl;
}



void DoesFopenSupportUtf8()
{
    const std::string filenameUtf8 = ToSingleByteString(L"通用电气威曼超声.txt", CP_UTF8);
   
    // setlocale(LC_ALL, ".UTF-8"); // Or better, set it system wide in control panel

    FILE* file = nullptr;

    const errno_t status = fopen_s(&file, filenameUtf8.c_str(), "r");

    if (status == 0) {
        std::cout << "fopen_s supports utf-8 path" << std::endl;
        fclose(file);
    }
    else
        std::cout << "fopen_s does not support utf-8 path" << std::endl;
}




void DoesFopenSupportUtf16()
{
    const std::wstring filenameUtf16 = L"🏃‍♂️.txt";

    FILE* file = nullptr;

    const errno_t status = _wfopen_s(&file, filenameUtf16.c_str(), L"r");

    if (status == 0) {
        std::cout << "_wfopen_s supports utf-16 path" << std::endl;
        fclose(file);
    }
    else
        std::cout << "_wfopen_s does not support utf-16 path" << std::endl;
}




void DoesCreateFileASupportUtf8()
{
    const std::string filenameUtf8 = ToSingleByteString(L"通用电气威曼超声.txt", CP_UTF8);

    const HANDLE f = CreateFileA(filenameUtf8.c_str(), GENERIC_READ, 0, {}, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

    if (f == INVALID_HANDLE_VALUE) {
        std::cout << "CreateFileA does not support utf-8 path" << std::endl;
    } else {
        std::cout << "CreateFileA supports utf-8 path" << std::endl;
        CloseHandle(f);
    }
}




void DoesCreateFileWSupportUtf16()
{
    const std::wstring filenameUtf16 = L"🏃‍♂️.txt";

    const HANDLE f = CreateFileW(filenameUtf16.c_str(), GENERIC_READ, 0, {}, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

    if (f == INVALID_HANDLE_VALUE) {
        std::cout << "CreateFileW does not support utf-16 path" << std::endl;
    }
    else {
        std::cout << "CreateFileW supports utf-16 path" << std::endl;
        CloseHandle(f);
    }
}




int main()
{
    FunWithAnsi();
    DoesFopenSupportUtf8();
    DoesFopenSupportUtf16();
    DoesCreateFileASupportUtf8();
    DoesCreateFileWSupportUtf16();
    return 0;
}
