
// CalculatorDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "Calculator.h"
#include "CalculatorDlg.h"
#include "afxdialogex.h"

#include "LogDlg.h"
#include "globals.h"

#include <exception>
#include <cmath>

#include <BCGPDateTimeCtrl.h>
#include <comdef.h>

#include <opencv_all.h>



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


std::atomic<bool> CCalculatorDlg::m_flagCameraClose = false;



// CCalculatorDlg dialog

CCalculatorDlg::CCalculatorDlg(CWnd* pParent /*=nullptr*/)
	: CBCGPDialog(IDD_CALCULATOR_DIALOG, pParent)
    , m_check_log(globals::CHECKBOX_LOG_FALSE)
    , m_radio_group_log_time(globals::RADIO_GROUP_LOG_TIME_DISABLE)
    , v_editPosX(0)
    , v_cameraFrameNum(0)
{
    //// Stage 4.1 (show picture)
    //cv::Mat image = cv::imread(globals::STRING_IMAGE_NAME_JOB);
    ////cv::namedWindow(globals::STRING_IMAGE_NAME_JOB, cv::WINDOW_AUTOSIZE);
    //cv::imshow(globals::STRING_IMAGE_NAME_JOB, image);
    //cv::moveWindow(globals::STRING_IMAGE_NAME_JOB, 30, 100);
    //
    //cv::Mat imageBinary;
    //cv::Scalar lowerBound(100, 100, 100);
    //cv::Scalar upperBound(255, 255, 255);
    //cv::inRange(image, lowerBound, upperBound, imageBinary);
    //cv::imshow("imageBinary.jpg", imageBinary);
    //
    //// Apply binary threshold
    //cv::Mat imageThreshold;
    //cv::threshold(imageBinary, imageThreshold, 128, 255, cv::THRESH_BINARY_INV);
    //cv::imshow("imageThreshold.jpg", imageThreshold);
    //
    //// Invert the binary image
    //cv::Mat imageInverted;
    //cv::bitwise_not(imageBinary, imageInverted);
    //cv::imshow("imageInverted.jpg", imageInverted);
    //
    //// Apply medianBlur
    //cv::Mat imageMedianBlured;
    //cv::medianBlur(imageInverted, imageMedianBlured, 3);
    //cv::imshow("imageMedianBlured.jpg", imageMedianBlured);
    //
    //int kernelSize = 3;
    //cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(kernelSize, kernelSize));
    //
    //// Apply erosion
    //cv::Mat imageEroded;
    //cv::erode(imageMedianBlured, imageEroded, kernel);
    //cv::imshow("imageEroded.jpg", imageEroded);
    //
    //// Apply dilation
    //cv::Mat imageDilated;
    //cv::erode(imageEroded, imageDilated, kernel);
    //cv::imshow("imageDilated.jpg", imageDilated);
    //
    //// Run Camera
    //m_cameraThread = std::thread(CameraCapture(&cameraFrameNum));

    // RESIZING
    EnableLayout();
}

CCalculatorDlg::~CCalculatorDlg()
{

//    g_flagCameraClose = true;
//    
//    if(m_cameraThread.joinable())
//        m_cameraThread.join();
//
//    cv::destroyAllWindows();
}

void CCalculatorDlg::DoDataExchange(CDataExchange* pDX)
{
    CBCGPDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT_INPUT, m_edit_input);
    DDX_Text(pDX, IDC_EDIT_RESULT, m_edit_result);
    DDX_Control(pDX, IDC_COMBO_PRECISION, m_combobox_precision);
    DDX_Check(pDX, IDC_CHECK_LOG, m_check_log);
    DDX_Radio(pDX, IDC_RADIO_LOG_TIME_DISABLE, m_radio_group_log_time);
    DDX_Control(pDX, IDC_DATETIMEPICKER_LOG, m_dt_picker_log);
    DDX_Control(pDX, IDC_SLIDER_PRECISION, m_slider_precision);
    DDX_Control(pDX, IDC_MFCCOLORBUTTON_BACKGROUND, c_colorButtonBackground);
    DDX_Control(pDX, IDC_MFCCOLORBUTTON_TEXT, c_colorButtonText);
    DDX_Control(pDX, IDC_STATIC_IMAGEVIEWER_JOB, c_imageViewerJob);
    DDX_Text(pDX, IDC_EDIT_POS_X, v_editPosX);
    DDX_Text(pDX, IDC_EDIT_POS_Y, v_editPosY);
    DDX_Text(pDX, IDC_EDIT_CAMERA_FRAME, v_cameraFrameNum);
}

BEGIN_MESSAGE_MAP(CCalculatorDlg, CBCGPDialog)
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON_PLUS, &CCalculatorDlg::OnBnClickedButtonOperationPlus)
    ON_BN_CLICKED(IDC_BUTTON_MINUS, &CCalculatorDlg::OnBnClickedButtonOperationMinus)
    ON_BN_CLICKED(IDC_BUTTON_MULTIPLY, &CCalculatorDlg::OnBnClickedButtonOperationMultiply)
    ON_BN_CLICKED(IDC_BUTTON_DIVIDE, &CCalculatorDlg::OnBnClickedButtonOperationDivide)
    ON_BN_CLICKED(IDC_BUTTON_EQUALS, &CCalculatorDlg::OnBnClickedButtonEquals)
    ON_WM_DESTROY()
    ON_BN_CLICKED(IDC_BUTTON_LOG_FILE_CHANGE, &CCalculatorDlg::OnBnClickedButtonLogFileChange)
    ON_BN_CLICKED(IDC_RADIO_LOG_TIME_DISABLE, &CCalculatorDlg::OnBnClickedRadioLogTimeDisable)
    ON_BN_CLICKED(IDC_RADIO_LOG_TIME_ENABLE, &CCalculatorDlg::OnBnClickedRadioLogTimeEnable)
    ON_CBN_SELCHANGE(IDC_COMBO_PRECISION, &CCalculatorDlg::OnCbnSelchangeComboPrecision)
    ON_BN_CLICKED(IDC_BUTTON_SAVE_IMAGES, &CCalculatorDlg::OnBnClickedButtonSaveImages)
END_MESSAGE_MAP()


// CCalculatorDlg message handlers

BOOL CCalculatorDlg::OnInitDialog()
{
    CBCGPDialog::OnInitDialog();

    // Init m_combobox_precision
    m_combobox_precision.AddString(globals::STRING_PRECISION_INT);
    m_combobox_precision.AddString(globals::STRING_PRECISION_DOUBLE);
    m_combobox_precision.SetCurSel(globals::COMBOBOX_PRECISION_INT);

    //Init m_file_log, (log.txt is initial file, can be changed in LogDlg)
    if (!OpenLogFile(globals::STRING_DEFAULT_LOG_FILENAME))
    {
        // TODO: If a new file could not be opened for some reason,
        // maybe try again or exit program ?
        EndDialog(IDCANCEL);
        return FALSE;
    }

    // Init m_dt_picker_log
    m_dt_picker_log.EnableWindow(globals::RADIO_GROUP_LOG_TIME_DISABLE);

    // Init m_slider_precision
    m_slider_precision.SetRange(0, 10);
    m_slider_precision.EnableWindow(m_combobox_precision.GetCurSel());

    // Init ColorButtons
    c_colorButtonBackground.SetColor(RGB(0,0,0));
    c_colorButtonText.SetColor(RGB(0,0,255));

    // RESIZING
    CBCGPStaticLayout* pLayout = (CBCGPStaticLayout*)GetLayout();

    if (pLayout != NULL)
    {
        if (m_edit_input)
            pLayout->AddAnchor(m_edit_input.GetSafeHwnd(),         CBCGPStaticLayout::e_MoveTypeVert, CBCGPStaticLayout::e_SizeTypeBoth, CPoint(100, 50), CPoint(100, 50) );
        if (m_combobox_precision)
            pLayout->AddAnchor(m_combobox_precision.GetSafeHwnd(), CBCGPStaticLayout::e_MoveTypeHorz, CBCGPStaticLayout::e_SizeTypeNone);
        if (m_slider_precision)
            pLayout->AddAnchor(m_slider_precision.GetSafeHwnd(),   CBCGPStaticLayout::e_MoveTypeHorz, CBCGPStaticLayout::e_SizeTypeHorz, CPoint(100, 100), CPoint(0, 100));
        if (c_colorButtonBackground)
            pLayout->AddAnchor(c_colorButtonBackground.GetSafeHwnd(), CBCGPStaticLayout::e_MoveTypeBoth, CBCGPStaticLayout::e_SizeTypeNone);
        if (c_colorButtonText)
            pLayout->AddAnchor(c_colorButtonText.GetSafeHwnd(), CBCGPStaticLayout::e_MoveTypeBoth, CBCGPStaticLayout::e_SizeTypeNone);
        if (c_imageViewerJob)
            pLayout->AddAnchor(c_imageViewerJob.GetSafeHwnd(), CBCGPStaticLayout::e_MoveTypeBoth, CBCGPStaticLayout::e_SizeTypeNone);

        pLayout->AddAnchor(IDC_EDIT_RESULT,            CBCGPStaticLayout::e_MoveTypeNone, CBCGPStaticLayout::e_SizeTypeBoth, CPoint(100, 100), CPoint(100, 50));
        pLayout->AddAnchor(IDC_BUTTON_PLUS,            CBCGPStaticLayout::e_MoveTypeVert, CBCGPStaticLayout::e_SizeTypeNone);
        pLayout->AddAnchor(IDC_BUTTON_MINUS,           CBCGPStaticLayout::e_MoveTypeVert, CBCGPStaticLayout::e_SizeTypeNone);
        pLayout->AddAnchor(IDC_BUTTON_MULTIPLY,        CBCGPStaticLayout::e_MoveTypeVert, CBCGPStaticLayout::e_SizeTypeNone);
        pLayout->AddAnchor(IDC_BUTTON_DIVIDE,          CBCGPStaticLayout::e_MoveTypeVert, CBCGPStaticLayout::e_SizeTypeNone);
        pLayout->AddAnchor(IDC_BUTTON_EQUALS,          CBCGPStaticLayout::e_MoveTypeBoth, CBCGPStaticLayout::e_SizeTypeNone);
        pLayout->AddAnchor(IDC_BUTTON_LOG_FILE_CHANGE, CBCGPStaticLayout::e_MoveTypeBoth, CBCGPStaticLayout::e_SizeTypeNone);
        pLayout->AddAnchor(IDC_CHECK_LOG,              CBCGPStaticLayout::e_MoveTypeBoth, CBCGPStaticLayout::e_SizeTypeNone);
        pLayout->AddAnchor(IDC_STATIC_GROUP_TIME_LOG,  CBCGPStaticLayout::e_MoveTypeBoth, CBCGPStaticLayout::e_SizeTypeNone);
        pLayout->AddAnchor(IDC_DATETIMEPICKER_LOG,     CBCGPStaticLayout::e_MoveTypeBoth, CBCGPStaticLayout::e_SizeTypeNone);
        pLayout->AddAnchor(IDC_RADIO_LOG_TIME_DISABLE, CBCGPStaticLayout::e_MoveTypeBoth, CBCGPStaticLayout::e_SizeTypeNone);
        pLayout->AddAnchor(IDC_RADIO_LOG_TIME_ENABLE,  CBCGPStaticLayout::e_MoveTypeBoth, CBCGPStaticLayout::e_SizeTypeNone);
        pLayout->AddAnchor(IDC_STATIC_BACKGROUND,      CBCGPStaticLayout::e_MoveTypeBoth, CBCGPStaticLayout::e_SizeTypeNone);
        pLayout->AddAnchor(IDC_STATIC_TEXT,            CBCGPStaticLayout::e_MoveTypeBoth, CBCGPStaticLayout::e_SizeTypeNone);
    }

    // Stage 6.5 (show picture using CBCGPKImageViewer)
    cv::Mat imageForImageViewer = cv::imread("job.jpg");
    c_imageViewerJob.SetImage(imageForImageViewer);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// The system calls this function to obtain the cursor to display while the user drags
// the minimized window.
HCURSOR CCalculatorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CCalculatorDlg::OnBnClickedButtonOperationPlus()
{
    if (!GetInput(m_input1))
    {
        CString error_str("Invalid first number!");
        AfxMessageBox(error_str);
        return;
    }

    m_mathematical_operation = globals::MATHEMATICAL_OPERATION::PLUS;
    m_edit_input.SetWindowTextW(_T(""));
}

void CCalculatorDlg::OnBnClickedButtonOperationMinus()
{
    if (!GetInput(m_input1))
    {
        CString error_str("Invalid first number!");
        AfxMessageBox(error_str);
        return;
    }

    m_mathematical_operation = globals::MATHEMATICAL_OPERATION::MINUS;
    m_edit_input.SetWindowTextW(_T(""));
}


void CCalculatorDlg::OnBnClickedButtonOperationMultiply()
{
    if (!GetInput(m_input1))
    {
        CString error_str("Invalid first number!");
        AfxMessageBox(error_str);
        return;
    }

    m_mathematical_operation = globals::MATHEMATICAL_OPERATION::MULTIPLY;
    m_edit_input.SetWindowTextW(_T(""));
}


void CCalculatorDlg::OnBnClickedButtonOperationDivide()
{
    if (!GetInput(m_input1))
    {
        CString error_str("Invalid first number!");
        AfxMessageBox(error_str);
        return;
    }

    m_mathematical_operation = globals::MATHEMATICAL_OPERATION::DIVIDE;
    m_edit_input.SetWindowTextW(_T(""));
}

void CCalculatorDlg::OnBnClickedButtonEquals()
{
    UpdateData();

    if (m_mathematical_operation == globals::MATHEMATICAL_OPERATION::NONE)
    {
        CString error_str("Missing values to compute !");
        AfxMessageBox(error_str);
        return;
    }

    if (!GetInput(m_input2))
    {
        CString error_str("Enter second number !");
        AfxMessageBox(error_str);
        return;
    }


    // Read m_combobox_precision
    CString selected_item_text;
    int selected_item_index = m_combobox_precision.GetCurSel();
    if (selected_item_index != CB_ERR)
    {
        m_combobox_precision.GetLBText(selected_item_index, selected_item_text);
    }

    // Convert to int if needed (ignore floating point part)
    if (selected_item_text == globals::STRING_PRECISION_INT ||
        m_slider_precision.GetPos() == 0)
    {
        m_input1 = static_cast<int>(m_input1);
        m_input2 = static_cast<int>(m_input2);
    }

    // Handle zero division
    if (m_mathematical_operation == globals::MATHEMATICAL_OPERATION::DIVIDE &&
        m_input2 == 0)
    {
        CString error_str("Zero division !");
        AfxMessageBox(error_str);
        return;
    }

    double result = 0;

    switch (m_mathematical_operation)
    {
    case globals::MATHEMATICAL_OPERATION::PLUS:
        result = m_input1 + m_input2;
        break;
    case globals::MATHEMATICAL_OPERATION::MINUS:
        result = m_input1 - m_input2;
        break;
    case globals::MATHEMATICAL_OPERATION::MULTIPLY:
        result = m_input1 * m_input2;
        break;
    case globals::MATHEMATICAL_OPERATION::DIVIDE:
        result = m_input1 / m_input2;
        break;
    }

    if (selected_item_text == globals::STRING_PRECISION_INT)
    {
        m_edit_result.Format(_T("%d"), static_cast<int>(result));
    }
    else
    {
        CString format_string;
        format_string.Format(_T("%%.%dlf"), m_slider_precision.GetPos());
        m_edit_result.Format(format_string, result);
    }

    if(m_check_log)
    {
        // Construct string that will be written to log file
        CString str_to_write_to_file;
        // Selected_item_text == precision
        CreateStringForLog(str_to_write_to_file, selected_item_text, result);

        WriteToLog(str_to_write_to_file);
    }

    // Stage 4.2
    static int imageCount = 1;
    const cv::String name("result");
    const cv::String extension(".jpg");
    const cv::String imageName = name + std::to_string(imageCount) + extension;
    imageCount++;
    cv::Mat image = cv::Mat::zeros(640, 1280, CV_8UC3); // 8-bit, 3-channel (color)
    
    // read result from m_edit_result (it was filled already)
    std::string strResult = CT2A(LPCWSTR(m_edit_result));
    
    // TODO: fix scaling with number of chars in result string
    cv::Size imageSize = image.size();
    COLORREF colorBackground = c_colorButtonBackground.GetColor();
    COLORREF colorText = c_colorButtonText.GetColor();
    image.setTo(cv::Scalar(GetBValue(colorBackground), GetGValue(colorBackground), GetRValue(colorBackground)));
    cv::putText(image, strResult, cv::Point(imageSize.width/4, imageSize.height/2), cv::FONT_HERSHEY_SIMPLEX, 3.0,
        cv::Scalar(GetBValue(colorText), GetGValue(colorText), GetRValue(colorText)), 20);

    cv::imshow(imageName, image);
    
    // Create binary image
    cv::Mat imageBinary;
    //cv::Scalar lowerBound(0, 0, 0);
    //cv::Scalar upperBound(200, 200, 200);
    cv::Scalar lowerBound(100, 100, 100);
    cv::Scalar upperBound(255, 255, 255);
    cv::inRange(image, lowerBound, upperBound, imageBinary);
    cv::imshow("imageBinary", imageBinary);
    
    
    // save image to a file
    cv::imwrite(imageName, image);

    // Stage 7 (read image pixel color)
    if (v_editPosX >= imageSize.width || v_editPosY >= imageSize.height)
    {
        AfxMessageBox(_T("Invalid coordinate !"));
    }
    else
    {
        uchar pixelColorBinary = imageBinary.at<uchar>(cv::Point(v_editPosX, v_editPosY));
        cv::Vec3b pixelColorColored = image.at<cv::Vec3b>(cv::Point(v_editPosX, v_editPosY));

        TRACE("BIN: %d, RGB: %d,%d,%d\n", pixelColorBinary, pixelColorColored[2],
                                                            pixelColorColored[1],
                                                            pixelColorColored[0]);
    }
    // Cleanup
    UpdateData(0);
    m_input1 = 0;
    m_input2 = 0;
    m_mathematical_operation = globals::MATHEMATICAL_OPERATION::NONE;
    m_edit_input.SetWindowTextW(_T(""));
}

bool CCalculatorDlg::GetInput(double& input)
{
    CString str;
    m_edit_input.GetWindowTextW(str);

    input = CStringToDouble(str);

    // This handles all other cases, that EditInput's OnChar method does not
    // e.g. ".85" or "-."
    // EDIT: appears, that e.g. .85 is actually succesfully converted to 0.85
    if (std::isnan(input))
    {
        return false;
    }
    return true;
}

void CCalculatorDlg::CreateStringForLog(CString& out_string
                                        , const CString& in_precision
                                        , double in_result) const
{
    if (in_precision == globals::STRING_PRECISION_INT)
    {
        out_string.Format(_T("%.0lf"), m_input1);

        switch (m_mathematical_operation)
        {
        case globals::MATHEMATICAL_OPERATION::PLUS:
            out_string += " + ";
            break;
        case globals::MATHEMATICAL_OPERATION::MINUS:
            out_string += " - ";
            break;
        case globals::MATHEMATICAL_OPERATION::MULTIPLY:
            out_string += " * ";
            break;
        case globals::MATHEMATICAL_OPERATION::DIVIDE:
            out_string += " / ";
            break;
        }

        CString tmp_string;
        tmp_string.Format(_T("%.0lf = "), m_input2);
        out_string += tmp_string;

        tmp_string.Format(_T("%d"), static_cast<int>(in_result));
        out_string += tmp_string;
    }
    else // if (in_precision == "double")
    {
        // string to help format, e.g. %.0lf, %.1lf, %.7lf ...
        CString format_string;
        format_string.Format(_T("%%.%dlf"), m_slider_precision.GetPos());

        out_string.Format(format_string, m_input1);

        switch (m_mathematical_operation)
        {
        case globals::MATHEMATICAL_OPERATION::PLUS:
            out_string += " + ";
            break;
        case globals::MATHEMATICAL_OPERATION::MINUS:
            out_string += " - ";
            break;
        case globals::MATHEMATICAL_OPERATION::MULTIPLY:
            out_string += " * ";
            break;
        case globals::MATHEMATICAL_OPERATION::DIVIDE:
            out_string += " / ";
            break;
        }

        CString tmp_string;
        tmp_string.Format(format_string, m_input2);
        out_string += tmp_string;
        out_string += " = ";

        tmp_string.Format(format_string, in_result);

        out_string += tmp_string;
    }

    out_string += " | ";

    // Add timestamp to log
    DATE date;

    if (m_radio_group_log_time == globals::RADIO_GROUP_LOG_TIME_ENABLE)
    {
        // Use date time picker
        date = m_dt_picker_log.GetDate();
        COleDateTime oleDateTime(date);
        out_string += oleDateTime.Format(_T("%Y-%m-%d %H:%M:%S\n"));
    }
    else
    {
        // Use system date/time
        SYSTEMTIME system_time;
        GetLocalTime(&system_time);
        
        CString tmp_string;
        tmp_string.Format(_T("%04d-%02d-%02d %02d:%02d:%02d\n"),
            system_time.wYear,
            system_time.wMonth,
            system_time.wDay,
            system_time.wHour,
            system_time.wMinute,
            system_time.wSecond);

        out_string += tmp_string;
    }
}

void CCalculatorDlg::WriteToLog(const CString& in_string)
{
    m_file_log.SeekToEnd();
    m_file_log.Write(in_string, in_string.GetLength() * sizeof(wchar_t));
}

void CCalculatorDlg::OnDestroy()
{
    CBCGPDialog::OnDestroy();

    if (m_file_log != CFile::hFileNull)
        m_file_log.Close();

    m_flagCameraClose = true;

    if (m_cameraThread.joinable())
        m_cameraThread.join();

    cv::destroyAllWindows();
}

void CCalculatorDlg::OnBnClickedButtonLogFileChange()
{
    LogDlg dlg(this);

    INT_PTR response = dlg.DoModal();

    if (response == IDOK)
    {
        CString filepath;
        dlg.GetFullFilepath(filepath);
        ChangeLogFile(filepath);
    }
}

//Gets filepath, without the actual filename
void CCalculatorDlg::GetFilepath(CString& out_string) const
{
    CString full_filepath = m_file_log.GetFilePath();
    int last_backslash_pos = full_filepath.ReverseFind('\\');
    if (last_backslash_pos != -1)
        out_string = full_filepath.Left(last_backslash_pos);
}

//Gets full filepath, including filename
void CCalculatorDlg::GetFullFilepath(CString& out_string) const
{
    out_string = m_file_log.GetFilePath();
}

void CCalculatorDlg::GetFileName(CString& out_string) const
{
    out_string = m_file_log.GetFileName();
}

bool CCalculatorDlg::OpenLogFile(const CString& in_string_filepath)
{
    CFileException file_exception;
    if (!m_file_log.Open(in_string_filepath,
        CFile::modeWrite | CFile::modeNoTruncate | CFile::modeCreate,
        &file_exception))
    {
        CString error_msg;
        const int error_buffer_max_len = 255;
        TCHAR error_buffer[error_buffer_max_len];

        file_exception.GetErrorMessage(error_buffer, error_buffer_max_len);

        error_msg = _T("Error opening log file: ");
        error_msg += error_buffer;

        AfxMessageBox(error_msg);

        EndDialog(IDCANCEL);
        return false;
    }
    return true;
}

void CCalculatorDlg::ChangeLogFile(const CString& in_string_filepath)
{
    // Log file was changed, close previous one and open new
    if (m_file_log != CFile::hFileNull)
        m_file_log.Close();

    if (!OpenLogFile(in_string_filepath))
    {
        // TODO: If a new file could not be opened for some reason,
        // we should probably open the previous one, or open default

        // Skip for now
    }
}

void CCalculatorDlg::OnBnClickedRadioLogTimeDisable()
{
    m_dt_picker_log.EnableWindow(false);
}

void CCalculatorDlg::OnBnClickedRadioLogTimeEnable()
{
    m_dt_picker_log.EnableWindow(true);
}


void CCalculatorDlg::OnCbnSelchangeComboPrecision()
{
    // Read m_combobox_precision
    CString selected_item_text;
    int selected_item_index = m_combobox_precision.GetCurSel();
    if (selected_item_index != CB_ERR)
    {
        m_combobox_precision.GetLBText(selected_item_index, selected_item_text);
        
       // m_combobox_precision.SetItemData(0, E_INT)
    }

    // if (m_combobox_precision.GetItemData(selected_item_index == E_INT);
    if (selected_item_text == globals::STRING_PRECISION_INT)
    {
        m_slider_precision.EnableWindow(false);
    }
    else
    {
        m_slider_precision.EnableWindow(true);
    }
}

CCalculatorDlg::CameraCapture::CameraCapture(int* ptr) 
    : p_cameraFrameNum(ptr)
{
}

void CCalculatorDlg::CameraCapture::operator()() const
{
    cv::VideoCapture camera(0);
    if (camera.isOpened())
    {
        cv::Mat frame;
        int counter = 0;
        int imageCount = 1;
        while (!m_flagCameraClose)
        {
            camera >> frame;
            if (frame.empty())
            {
                //break;
            }
            else
            {
                cv::imshow("Camera Capture", frame);
                if (*p_cameraFrameNum > 0)
                {
                    if( ( counter = ( counter % *p_cameraFrameNum ) ) == 0 )
                    {
                        const cv::String name("CameraImages\\CameraImage");
                        const cv::String extension(".jpg");
                        const cv::String imageName = name + std::to_string(imageCount) + extension;
                        cv::imwrite(imageName, frame);
                        imageCount++;
                    }   
                    counter++;
                }
            }

            if (cv::waitKey(10) >= 0)
                m_flagCameraClose = true;
        }
    }
    camera.release();
}


void CCalculatorDlg::OnBnClickedButtonSaveImages()
{
    UpdateData();
    cameraFrameNum = v_cameraFrameNum;
}
