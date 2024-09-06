
// CalculatorDlg.h : header file
//

#pragma once

#include "Helpers.h"
#include "EditInput.h"

#include <KEL2017/IP.h>

#include <thread>



// CCalculatorDlg dialog
class CCalculatorDlg : public CBCGPDialog
{
// Construction
public:
	CCalculatorDlg(CWnd* pParent = nullptr);	// standard constructor
	virtual ~CCalculatorDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CALCULATOR_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;
	DECLARE_MESSAGE_MAP()

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnDestroy();

    // private control variables
private:
    EditInput m_edit_input;
    CString m_edit_result;
    CBCGPComboBox m_combobox_precision;
    CBCGPDateTimeCtrl m_dt_picker_log;
    CBCGPSliderCtrl m_slider_precision;
    CBCGPColorButton c_colorButtonBackground;
    CBCGPColorButton c_colorButtonText;


    
    // KEL ImageViewer
    CBCGPKImageViewer c_imageViewerJob;


    
    // private value variables
    BOOL m_check_log;
    int m_radio_group_log_time;
    int v_editPosX;
    int v_editPosY;
    int v_cameraFrameNum;
    int cameraFrameNum;

    // private member variables
private:
    double m_input1 = 0.0;
    double m_input2 = 0.0;

    CFile m_file_log;

    globals::MATHEMATICAL_OPERATION m_mathematical_operation;

    static std::atomic<bool> m_flagCameraClose;

    //private handlers
private:
    afx_msg void OnBnClickedButtonOperationPlus();
    afx_msg void OnBnClickedButtonOperationMinus();
    afx_msg void OnBnClickedButtonOperationMultiply();
    afx_msg void OnBnClickedButtonOperationDivide();
    afx_msg void OnBnClickedButtonEquals();
    afx_msg void OnBnClickedButtonLogFileChange();
    afx_msg void OnBnClickedRadioLogTimeDisable();
    afx_msg void OnBnClickedRadioLogTimeEnable();
    afx_msg void OnCbnSelchangeComboPrecision();

    //private custom methods
private:
    bool GetInput(double& input);
    void CreateStringForLog(CString & out_string
                            , const CString& in_precision
                            , double in_result) const;

    void WriteToLog(const CString& in_string);

    //public custom methods
public:
    void GetFullFilepath(CString& out_string) const;
    void GetFilepath(CString& out_string) const;
    void GetFileName(CString& out_string) const;
    void ChangeLogFile(const CString& in_string_filepath);
    bool OpenLogFile(const CString& in_string_filepath);

    // camera stuff
private:
    struct CameraCapture
    {
        int* p_cameraFrameNum;
        
        CameraCapture(int* ptr);
        
        void operator()() const;
    };
    std::thread m_cameraThread;

public:
    afx_msg void OnBnClickedButtonSaveImages();
};
