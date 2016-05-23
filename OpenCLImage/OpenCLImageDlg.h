
// OpenCLImageDlg.h : ͷ�ļ�
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


// COpenCLImageDlg �Ի���
class COpenCLImageDlg : public CDialogEx
{
// ����
public:
	COpenCLImageDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OPENCLIMAGE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��



// ʵ��
protected:
	HICON m_hIcon;

	CImage* m_pImgSrc;//ͼ��
	CImage* m_pImgDst;//Ŀ��ͼ��
	float scaleNum;//���ű���

	void ThreadDraw(DrawPara *p);//����
	static UINT Update(void* p);
	void ImageCopy(CImage* pImgSrc, CImage* pImgDrt);
	CImage* getImage() { return m_pImgSrc; }

	
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOpen();
	afx_msg void OnBnClickedProcess();
};
