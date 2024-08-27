#pragma once
#include "afxdialogex.h"

#include "CalculatorDlg.h"

// LogDlg dialog

class LogDlg : public CDialogEx
{
	DECLARE_DYNAMIC(LogDlg)

public:
	LogDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~LogDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_LOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

private:
    CCalculatorDlg* m_p_parent_dlg;

	DECLARE_MESSAGE_MAP()

public:
    virtual BOOL OnInitDialog();

private:
    afx_msg void OnBnClickedButtonFile();

private:
    CString m_edit_filepath;
    CString m_edit_filename;

public:
    void GetFullFilepath(CString& out_string) const;
};
