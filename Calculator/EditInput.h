#pragma once


// EditInput

#include "globals.h"

class EditInput : public CBCGPEdit
{
	DECLARE_DYNAMIC(EditInput)

public:
	EditInput();
	virtual ~EditInput();

protected:
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);

private:
    globals::MATHEMATICAL_OPERATION m_mathematical_operation;
};


