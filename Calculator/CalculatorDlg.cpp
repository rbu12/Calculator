
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


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCalculatorDlg dialog



CCalculatorDlg::CCalculatorDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CALCULATOR_DIALOG, pParent)
    , m_check_log(globals::CHECKBOX_LOG_FALSE)
    , m_radio_group_log_time(globals::RADIO_GROUP_LOG_TIME_DISABLE)
{
    // Stage 4.1 (show picture)
    cv::Mat img = cv::imread(globals::STRING_IMAGE_NAME_JOB);
    cv::namedWindow(globals::STRING_WINDOW_NAME, cv::WINDOW_AUTOSIZE);
    cv::imshow(globals::STRING_WINDOW_NAME, img);
    cv::moveWindow(globals::STRING_WINDOW_NAME, 30, 100);
}

CCalculatorDlg::~CCalculatorDlg()
{
    cv::destroyAllWindows();
}

void CCalculatorDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT_INPUT, m_edit_input);
    DDX_Text(pDX, IDC_EDIT_RESULT, m_edit_result);
    DDX_Control(pDX, IDC_COMBO_PRECISION, m_combobox_precision);
    DDX_Check(pDX, IDC_CHECK_LOG, m_check_log);
    DDX_Radio(pDX, IDC_RADIO_LOG_TIME_DISABLE, m_radio_group_log_time);
    DDX_Control(pDX, IDC_DATETIMEPICKER_LOG, m_dt_picker_log);
    DDX_Control(pDX, IDC_SLIDER_PRECISION, m_slider_precision);
}

BEGIN_MESSAGE_MAP(CCalculatorDlg, CDialogEx)
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
END_MESSAGE_MAP()


// CCalculatorDlg message handlers

BOOL CCalculatorDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

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

    // Add timestamp to log
    CTime date_time_ctrl;
    if (m_radio_group_log_time == globals::RADIO_GROUP_LOG_TIME_ENABLE)
    {
        // Use date time picker
        m_dt_picker_log.GetTime(date_time_ctrl);
    }
    else
    {
        // Use system time
        date_time_ctrl = CTime::GetCurrentTime();
    }

    out_string += " / ";
    out_string += date_time_ctrl.Format(_T("%Y-%m-%d %H:%M:%S\n"));
}

void CCalculatorDlg::WriteToLog(const CString& in_string)
{
    m_file_log.SeekToEnd();
    m_file_log.Write(in_string, in_string.GetLength() * sizeof(wchar_t));
}

void CCalculatorDlg::OnDestroy()
{
    CDialogEx::OnDestroy();

    if (m_file_log != CFile::hFileNull)
        m_file_log.Close();
}

void CCalculatorDlg::OnBnClickedButtonLogFileChange()
{
    LogDlg dlg(this);

    int response = dlg.DoModal();

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
    }

    if (selected_item_text == globals::STRING_PRECISION_INT)
    {
        m_slider_precision.EnableWindow(false);
    }
    else
    {
        m_slider_precision.EnableWindow(true);
    }
}
