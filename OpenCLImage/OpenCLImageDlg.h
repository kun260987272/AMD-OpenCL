
// OpenCLImageDlg.h : 头文件
//

#pragma once

struct DrawPara
{
	CImage* pImgSrc;
	CDC* pDC;
	int oriX;
	int oriY;
	int width;
	int height;
};


// COpenCLImageDlg 对话框
class COpenCLImageDlg : public CDialogEx
{
// 构造
public:
	COpenCLImageDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OPENCLIMAGE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持



// 实现
protected:
	HICON m_hIcon;

	CImage* m_pImgSrc;//图像
	CImage* m_pImgDst;//目标图像
	float scaleNum;//缩放倍数

	void ThreadDraw(DrawPara *p);//绘制
	static UINT Update(void* p);
	void ImageCopy(CImage* pImgSrc, CImage* pImgDrt);
	CImage* getImage() { return m_pImgSrc; }

	
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOpen();
	afx_msg void OnBnClickedProcess();
};
