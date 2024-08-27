// EditInput.cpp : implementation file
//

#include "pch.h"
#include "Calculator.h"
#include "EditInput.h"


// EditInput

IMPLEMENT_DYNAMIC(EditInput, CEdit)

EditInput::EditInput()
//    : CEdit::CEdit()  //Called automatically
    : m_mathematical_operation(globals::MATHEMATICAL_OPERATION::NONE)
{
}

EditInput::~EditInput()
{
}


BEGIN_MESSAGE_MAP(EditInput, CEdit)
    ON_WM_CHAR()
END_MESSAGE_MAP()


// EditInput message handlers


void EditInput::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    //TODO: needs a better implementation, it accepts e.g. ".85" or "-."
    if (nChar == VK_TAB)
        return;

    //Max 21 characters
    if (GetWindowTextLength() > 21 &&
        (nChar != VK_BACK && nChar != VK_DELETE) )
        return;

    if ((nChar >= '0' && nChar <= '9')
        || nChar == '.' || nChar == '-'
        || nChar == VK_BACK
        || nChar == VK_DELETE)
    {
        CString str_text;
        GetWindowText(str_text);
        //Check for multiple decimal points
        if (nChar == '.' && str_text.Find('.') != -1)
        {
            //Ignore the second decimal point
            AfxMessageBox(_T("Invalid Input: Only 1 decimal point !"));
        }
        //Check for negative sign
        else if (nChar == '-' &&
            (LOWORD(GetSel()) != 0 || str_text.Find('-') >= 0))
        {
            //Ignore, if cursor position is not 0
            //Ignore multiple negative signs
            AfxMessageBox(_T("Invalid Input: Negative sign !"));
        }
        else
            CEdit::OnChar(nChar, nRepCnt, nFlags);
    }
    else
    {
        //Ignore other characters
        AfxMessageBox(_T("Invalid Input: Not acceptable character !"));
    }
}
