// LogDlg.cpp : implementation file
//

#include "pch.h"
#include "Calculator.h"
#include "afxdialogex.h"
#include "LogDlg.h"

//************************************************************
// Custom axis to handle label formatting
//************************************************************
class CCustomYAxisBase : public CBCGPChartAxisY
{
    DECLARE_DYNCREATE(CCustomYAxisBase)

public:
    ////no need
    //CCustomYAxisBase()
    //{
    //}
    //
    //CCustomYAxisBase(int nAxisID, CBCGPChartAxis::AxisDefaultPosition position, CBCGPChartVisualObject* pChartCtrl) :
    //    CBCGPChartAxisY(nAxisID, position, pChartCtrl)
    //{
    //}

    virtual void GetDisplayedLabel(double dblValue, CString& strLabel);
};

IMPLEMENT_DYNCREATE(CCustomYAxisBase, CBCGPChartAxisY)

//------------------------------------------------------
// Displays label in format n,nnn.nn
//------------------------------------------------------
void CCustomYAxisBase::GetDisplayedLabel(double dblValue, CString& strLabel)
{
    strLabel.Format(_T("%.2f%%"), dblValue);
        
    // This is just inserting commas when number has more than 3 digits
    //int nPointIndex = strLabel.Find('.');
    //
    //for (int nIndex = nPointIndex - 3; nIndex > 0; nIndex -= 3)
    //{
    //    if (nPointIndex <= 0)
    //    {
    //        break;
    //    }
    //
    //    strLabel.Insert(nIndex, _T(","));
    //}
}




inline double Rand(double dblStart, double dblFinish)
{
    double minVal = min(dblStart, dblFinish);
    double maxVal = max(dblStart, dblFinish);

    return (maxVal - minVal) * (double)rand() / (RAND_MAX + 1) + minVal;
}


// LogDlg dialog

IMPLEMENT_DYNAMIC(LogDlg, CBCGPDialog)

LogDlg::LogDlg(CWnd* pParent /*=nullptr*/)
	: CBCGPDialog(IDD_DIALOG_LOG, pParent)
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
    CBCGPDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_FILEPATH, m_edit_filepath);
    DDX_Text(pDX, IDC_EDIT_FILENAME, m_edit_filename);
    DDX_Control(pDX, IDC_CONTROL_CHART, c_chartCTRL);
}


BEGIN_MESSAGE_MAP(LogDlg, CBCGPDialog)
    ON_BN_CLICKED(IDC_BUTTON_FILE, &LogDlg::OnBnClickedButtonFile)
END_MESSAGE_MAP()


// LogDlg message handlers


BOOL LogDlg::OnInitDialog()
{
    CBCGPDialog::OnInitDialog();

    m_p_parent_dlg->GetFilepath(m_edit_filepath);
    m_p_parent_dlg->GetFileName(m_edit_filename);
    UpdateData(false);



     //CharCtrl stuff

    CBCGPChartVisualObject* pChart = c_chartCTRL.GetChart();
    ASSERT_VALID(pChart);

    pChart->SetChartTitle(_T("Paretov diagram"));

    CBCGPChartSeries* pSeries1 = pChart->CreateSeries(_T("Importance weight"), CBCGPColor(1, 0.6, 0), BCGP_CT_DEFAULT, BCGPChartColumn);
    CBCGPChartSeries* pSeries2 = pChart->CreateSeries(_T("Cumulative percent"), CBCGPColor(0.6, 0.1, 0.9), BCGP_CT_DEFAULT, BCGPChartLine);

    CBCGPChartAxis* pPrimaryXAxis = pChart->GetChartAxis(BCGP_CHART_X_PRIMARY_AXIS);
    CBCGPChartAxis* pPrimaryYAxis = pChart->GetChartAxis(BCGP_CHART_Y_PRIMARY_AXIS);

    CBCGPChartAxis* pSecondaryYAxis = pChart->ReplaceDefaultAxis(BCGP_CHART_Y_SECONDARY_AXIS, RUNTIME_CLASS(CCustomYAxisBase));

    // Rotate X axis labels
    pPrimaryXAxis->m_axisLabelsFormat.m_textFormat.SetDrawingAngle(90);

    // Show only Y axis grid
    pChart->ShowAxisGridLines(BCGP_CHART_X_PRIMARY_AXIS, FALSE, FALSE);

    // Adjust axis's width
    pPrimaryXAxis->m_axisLineFormat.m_dblWidth   = 0.1;
    pPrimaryYAxis->m_axisLineFormat.m_dblWidth   = 0.1;
    pSecondaryYAxis->m_axisLineFormat.m_dblWidth = 0.1;





    // ADD POINTS

    //pChart->SetThemeOpacity(20);

    COleDateTime today = COleDateTime::GetCurrentTime();

    double arTemp[2][12] = {
        { 162., 132., 129, 80, 60, 55, 34, 28, 22, 18, 6, 3 },
        { 5., 12, 32, 39, 48, 55, 62, 78, 89, 91, 94, 100 } 
    };

    for (int i = 0; i < 12; i++)
    {
        COleDateTime date(today.GetYear(), i+1, 1, 0, 0, 0);

        pSeries1->AddDataPoint(date.Format(_T("%b")), arTemp[0][i]);
        pSeries2->AddDataPoint(arTemp[1][i]);
    }

    BCGPChartFormatSeries style = pSeries2->GetSeriesFormat();
    style.m_curveType = BCGPChartFormatSeries::CCT_SPLINE_HERMITE;
    style.SetSeriesLineWidth(2);
    pSeries2->SetSeriesFormat(style);
    


    // ADJUST AXIS RANGES
    // display second series on the secondary Y axis
    pSeries2->SetRelatedAxis(pSecondaryYAxis, CBCGPChartSeries::AI_Y);

    // set fixed display ranges with fixed major unit, because automatic ranges 
    // will look a bit different
    pPrimaryYAxis->SetFixedDisplayRange(0., 180., 20.);
    pSecondaryYAxis->SetFixedDisplayRange(0., 120., 20);

    // Show data markers
    //pChart->ShowDataMarkers();
    //only for series2
    pSeries2->ShowMarker(1);













    //OnUpdateChart();l

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}


void LogDlg::OnBnClickedButtonFile()
{
    //CBCGPFileDialog file_dlg(_T("XXXXXXX"), TRUE, _T("txt"), nullptr, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY,
    //    _T("Text Files (*.txt)|*.txt|All Files (*.*)|*.*||"));
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


