#pragma once

#include <opencv2/core/cvstd.hpp>

// global variables
// and all the initial constatns to avoid magic numbers

namespace globals
{
    // default filename for logging
    const CString STRING_DEFAULT_LOG_FILENAME("log.txt");

    // mathematical operation to be performed
    enum MATHEMATICAL_OPERATION
    {
        NONE,
        PLUS,
        MINUS,
        MULTIPLY,
        DIVIDE
    };

    // possible initial values for controls
    const int COMBOBOX_PRECISION_INT = 0;
    const int COMBOBOX_PRECISION_DOUBLE = 1;

    const bool CHECKBOX_LOG_FALSE = false;
    const bool CHECKBOX_LOG_TRUE = true;

    const int RADIO_GROUP_LOG_TIME_DISABLE = 0;
    const int RADIO_GROUP_LOG_TIME_ENABLE = 1;

    // precision strings constants for comparison
    const CString STRING_PRECISION_INT("int");
    const CString STRING_PRECISION_DOUBLE("double");

    // openCV stages
    const cv::String STRING_WINDOW_NAME("Calculator");
    const cv::String STRING_IMAGE_NAME_JOB("job.jpg");
    const cv::String STRING_IMAGE_NAME_CALCULATOR("Calculator.jpg");

}