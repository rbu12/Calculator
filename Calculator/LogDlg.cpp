// LogDlg.cpp : implementation file
//

#include "pch.h"
#include "Calculator.h"
#include "afxdialogex.h"
#include "LogDlg.h"


// LogDlg dialog

IMPLEMENT_DYNAMIC(LogDlg, CDialogEx)

LogDlg::LogDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_LOG, pParent)
    , m_edit_filepath(_T(""))
    , m_edit_filename(_T(""))
{
    m_p_parent_dlg = dynamic_cast<CCalculatorDlg*>(pParent);
}

LogDlg::~LogDlg()
{
}

void LogDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_FILEPATH, m_edit_filepath);
    DDX_Text(pDX, IDC_EDIT_FILENAME, m_edit_filename);
}


BEGIN_MESSAGE_MAP(LogDlg, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON_FILE, &LogDlg::OnBnClickedButtonFile)
END_MESSAGE_MAP()


// LogDlg message handlers


BOOL LogDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    m_p_parent_dlg->GetFilepath(m_edit_filepath);
    m_p_parent_dlg->GetFileName(m_edit_filename);
    UpdateData(false);

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}


void LogDlg::OnBnClickedButtonFile()
{
    CFileDialog file_dlg(TRUE, _T("txt"), nullptr, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY,
        _T("Text Files (*.txt)|*.txt|All Files (*.*)|*.*||"));

    if (file_dlg.DoModal() == IDOK)
    {
        CString full_filepath = file_dlg.GetPathName();
        int last_backslash_pos = full_filepath.ReverseFind('\\');
        if (last_backslash_pos != -1)
        {
            m_edit_filepath = full_filepath.Left(last_backslash_pos);
            m_edit_filename = full_filepath.Right(full_filepath.GetLength() - last_backslash_pos - 1);
            UpdateData(0);
        }
    }
}

void LogDlg::GetFullFilepath(CString& out_string) const
{
    out_string = m_edit_filepath + "\\" + m_edit_filename;
}