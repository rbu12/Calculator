
#include "Helpers.h"
#include "pch.h"

#include <string>
#include <stdexcept>

double CStringToDouble(const CString& str)
{
    //Convert CString to std::wstring
    std::wstring wideStr(str.GetString());

    try
    {
        // Convert std::wstring to double
        return std::stod(wideStr);
    }
    catch (const std::invalid_argument& e)
    {
        // Handle conversion error
        // For this example, return NaN (Not-a-Number)
        return std::numeric_limits<double>::quiet_NaN();
    }
    catch (const std::out_of_range& e)
    {
        // Handle range error
        // For this example, return NaN (Not-a-Number)
        return std::numeric_limits<double>::quiet_NaN();
    }
}