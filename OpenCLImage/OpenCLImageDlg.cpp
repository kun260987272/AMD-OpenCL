
// OpenCLImageDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "OpenCLImage.h"
#include "OpenCLImageDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COpenCLImageDlg 对话框



COpenCLImageDlg::COpenCLImageDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_OPENCLIMAGE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void COpenCLImageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(COpenCLImageDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_OPEN, &COpenCLImageDlg::OnBnClickedOpen)
	ON_BN_CLICKED(ID_PROCESS, &COpenCLImageDlg::OnBnClickedProcess)
END_MESSAGE_MAP()


// COpenCLImageDlg 消息处理程序

BOOL COpenCLImageDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_pImgSrc = NULL;
	m_pImgDst = NULL;
	scaleNum = 0.5;

	AfxBeginThread((AFX_THREADPROC)&COpenCLImageDlg::Update, this);



	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void COpenCLImageDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
		if (m_pImgSrc != NULL)
		{
			DrawPara p;
			p.height = m_pImgSrc->GetHeight();
			p.width = m_pImgSrc->GetWidth();
			p.oriX = 50;
			p.oriY = 100;
			p.pDC = GetDC();
			p.pImgSrc = m_pImgSrc;
			ThreadDraw(&p);
		}
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR COpenCLImageDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


UINT COpenCLImageDlg::Update(void* p)
{
	while (1)
	{
		Sleep(200);
		COpenCLImageDlg* dlg = (COpenCLImageDlg*)p;
		if (dlg->m_pImgSrc != NULL)
		{
			DrawPara p;
			p.height = dlg->m_pImgSrc->GetHeight();
			p.width = dlg->m_pImgSrc->GetWidth();
			p.oriX = 50;
			p.oriY = 100;
			p.pDC = dlg->GetDC();
			p.pImgSrc = dlg->getImage();
			dlg->ThreadDraw(&p);
		}
	}
	return 0;
}

void COpenCLImageDlg::ThreadDraw(DrawPara *p)
{
	CRect rect;
	GetClientRect(&rect);
	CDC memDC;             // 用于缓冲绘图的内存画笔  
	CBitmap memBitmap;         // 用于缓冲绘图的内存画布
	memDC.CreateCompatibleDC(p->pDC);  // 创建与原画笔兼容的画笔
	memBitmap.CreateCompatibleBitmap(p->pDC, p->width, p->height);  // 创建与原位图兼容的内存画布
	memDC.SelectObject(&memBitmap);      // 创建画笔与画布的关联
	memDC.FillSolidRect(rect, p->pDC->GetBkColor());

	// 将pImgSrc的内容缩放画到内存画布中
	p->pImgSrc->StretchBlt(memDC.m_hDC, 0, 0, p->width, p->height);

	// 将已画好的画布复制到真正的缓冲区中
	p->pDC->BitBlt(p->oriX, p->oriY, p->width, p->height, &memDC, 0, 0, SRCCOPY);
	memBitmap.DeleteObject();
	memDC.DeleteDC();
}

void COpenCLImageDlg::ImageCopy(CImage* pImgSrc, CImage* pImgDrt)
{
	int MaxColors = pImgSrc->GetMaxColorTableEntries();
	RGBQUAD* ColorTab;
	ColorTab = new RGBQUAD[MaxColors];

	CDC *pDCsrc, *pDCdrc;
	if (!pImgDrt->IsNull())
	{
		pImgDrt->Destroy();
	}
	pImgDrt->Create(pImgSrc->GetWidth(), pImgSrc->GetHeight(), pImgSrc->GetBPP(), 0);

	if (pImgSrc->IsIndexed())
	{
		pImgSrc->GetColorTable(0, MaxColors, ColorTab);
		pImgDrt->SetColorTable(0, MaxColors, ColorTab);
	}

	pDCsrc = CDC::FromHandle(pImgSrc->GetDC());
	pDCdrc = CDC::FromHandle(pImgDrt->GetDC());
	pDCdrc->BitBlt(0, 0, pImgSrc->GetWidth(), pImgSrc->GetHeight(), pDCsrc, 0, 0, SRCCOPY);
	pImgSrc->ReleaseDC();
	pImgDrt->ReleaseDC();
	delete ColorTab;

}


void COpenCLImageDlg::OnBnClickedOpen()
{
	// TODO: 在此添加控件通知处理程序代码
		//选择文件对话框
	TCHAR  szFilter[] = _T("JPEG(*.jpg)|*.jpg|BMP(*.bmp)|*.bmp|TIFF (*.tif)|*.tif|All Files (*.*)|*.*|");
	CFileDialog fileOpenDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, szFilter);
	if (fileOpenDlg.DoModal() == IDOK)
	{
		CString imFilePath;
		VERIFY(imFilePath = fileOpenDlg.GetPathName());
		if (m_pImgSrc != NULL)
		{
			m_pImgSrc->Destroy();
			delete m_pImgSrc;
		}
		m_pImgSrc = new CImage();
		m_pImgSrc->Load(imFilePath);
		this->Invalidate();
	}
}


void COpenCLImageDlg::OnBnClickedProcess()
{
	// TODO: 在此添加控件通知处理程序代码

	//两个平台、每个平台两个设备
	for (int i = 0;i < 2;++i)
	{
		for (int j = 0;j < 2;++j)
		{
			int start = clock();

			//OpenCL变量
			cl_platform_id platform[2];
			cl_device_id device[2];
			cl_context context;
			cl_command_queue queue;
			cl_program program;
			cl_kernel scale;
			cl_int err;

			//1 platform
			err = clGetPlatformIDs(2, platform, NULL);

			if (err != CL_SUCCESS)
			{
				AfxMessageBox("clGetPlatformIDs error");
			}

			//2 device
			clGetDeviceIDs(platform[i], CL_DEVICE_TYPE_ALL, 2, device, NULL);
			if (err != CL_SUCCESS)
			{
				AfxMessageBox("clGetDeviceIDs error");
			}

			//3 context
			context = clCreateContext(NULL, 2, device, NULL, NULL, &err);
			if (err != CL_SUCCESS)
			{
				AfxMessageBox("clCreateContext error");
			}

			//4 queue
			queue = clCreateCommandQueue(context, device[j], 0, &err);
			if (err != CL_SUCCESS)
			{
				AfxMessageBox("clCreateCommandQueue error");
			}

			//5 program
			//读文件
			FILE* fp;
			err = fopen_s(&fp, "OpenCLImage.cl", "rb");
			if (err)
			{
				AfxMessageBox("fopen_s error");
			}

			fseek(fp, 0L, SEEK_END);
			size_t size = ftell(fp);
			fseek(fp, 0L, SEEK_SET);
			char* buf = new char[size + 1];
			fread(buf, 1, size, fp);
			buf[size] = NULL;
			fclose(fp);
			const char* source = buf;
			
			program = clCreateProgramWithSource(context, 1, &source, &size, &err);
			if (err != CL_SUCCESS)
			{
				AfxMessageBox("clCreateProgramWithSource error");
			}

			err = clBuildProgram(program, 2, device, NULL, NULL, NULL);
			if (err != CL_SUCCESS)
			{
				AfxMessageBox("clBuildProgram error");
			}

			//6 kernel
			scale = clCreateKernel(program, "scale", &err);
			if (err != CL_SUCCESS)
			{
				AfxMessageBox("clCreateKernel error");
			}

			//创建目标图像
			if (m_pImgDst != NULL)
			{
				m_pImgDst->Destroy();
				delete m_pImgDst;
			}
			m_pImgDst = new CImage();
			//缩放后大小改变
			m_pImgDst->Create(m_pImgSrc->GetWidth()*scaleNum, m_pImgSrc->GetHeight()*scaleNum, m_pImgSrc->GetBPP());

			//原
			byte* p_srcRealData = (byte*)m_pImgSrc->GetBits();
			int srcPit = m_pImgSrc->GetPitch();
			int srcBitCount = m_pImgSrc->GetBPP() / 8;
			int srcNum = m_pImgSrc->GetWidth()*m_pImgSrc->GetHeight();

			//目的
			byte* p_dstRealData = (byte*)m_pImgDst->GetBits();
			int dstPit = m_pImgDst->GetPitch();
			int dstBitCount = m_pImgDst->GetBPP() / 8;
			int dstNum = m_pImgDst->GetWidth()*m_pImgDst->GetHeight();
			//主机原像素
			int* host_srcR = new int[srcNum];
			int* host_srcG = new int[srcNum];
			int* host_srcB = new int[srcNum];
			//主机目的像素
			int* host_dstR = new int[dstNum];
			int* host_dstG = new int[dstNum];
			int* host_dstB = new int[dstNum];
			//获取像素
			for (int k = 0;k < srcNum;++k)
			{
				int x = k % m_pImgSrc->GetWidth();
				int y = k / m_pImgSrc->GetWidth();

				host_srcR[k] = *(p_srcRealData + y*srcPit + x*srcBitCount + 2);
				host_srcG[k] = *(p_srcRealData + y*srcPit + x*srcBitCount + 1);
				host_srcB[k] = *(p_srcRealData + y*srcPit + x*srcBitCount);
			}

			//设备内存大小
			size_t mem_srcSize = sizeof(int)*srcNum;
			size_t mem_dstSize = sizeof(int)*dstNum;
			//设备内存
			cl_mem dev_srcR = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, mem_srcSize, host_srcR, &err);
			if (err != CL_SUCCESS)
			{
				AfxMessageBox("clCreateBuffer dev_srcR error");
			}
			cl_mem dev_srcG = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, mem_srcSize, host_srcG, &err);
			if (err != CL_SUCCESS)
			{
				AfxMessageBox("clCreateBuffer dev_srcG error");
			}
			cl_mem dev_srcB = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, mem_srcSize, host_srcB, &err);
			if (err != CL_SUCCESS)
			{
				AfxMessageBox("clCreateBuffer dev_srcB error");
			}


			cl_mem dev_dstR = clCreateBuffer(context, CL_MEM_WRITE_ONLY, mem_dstSize, NULL, &err);
			if (err != CL_SUCCESS)
			{
				AfxMessageBox("clCreateBuffer dev_dstR error");
			}
			cl_mem dev_dstG = clCreateBuffer(context, CL_MEM_WRITE_ONLY, mem_dstSize, NULL, &err);
			if (err != CL_SUCCESS)
			{
				AfxMessageBox("clCreateBuffer dev_dstG error");
			}
			cl_mem dev_dstB = clCreateBuffer(context, CL_MEM_WRITE_ONLY, mem_dstSize, NULL, &err);
			if (err != CL_SUCCESS)
			{
				AfxMessageBox("clCreateBuffer dev_dstB error");
			}

			//设置kernel参数
			int srcWidth = m_pImgSrc->GetWidth();
			int srcHeight = m_pImgSrc->GetHeight();
			int dstWidth = m_pImgDst->GetWidth();

			err = clSetKernelArg(scale, 0, sizeof(cl_mem), &dev_srcR);
			err |= clSetKernelArg(scale, 1, sizeof(cl_mem), &dev_srcG);
			err |= clSetKernelArg(scale, 2, sizeof(cl_mem), &dev_srcB);
			err |= clSetKernelArg(scale, 3, sizeof(cl_mem), &dev_dstR);
			err |= clSetKernelArg(scale, 4, sizeof(cl_mem), &dev_dstG);
			err |= clSetKernelArg(scale, 5, sizeof(cl_mem), &dev_dstB);
			err |= clSetKernelArg(scale, 6, sizeof(int), &srcWidth);
			err |= clSetKernelArg(scale, 7, sizeof(int), &srcHeight);
			err |= clSetKernelArg(scale, 8, sizeof(int), &dstWidth);
			err |= clSetKernelArg(scale, 9, sizeof(float), &scaleNum);

			if (err != CL_SUCCESS)
			{
				AfxMessageBox("clSetKernelArg error");
			}

			//调用kernel
			const size_t global_work_size = dstNum;
			

			err = clEnqueueNDRangeKernel(queue, scale, 1, 0, &global_work_size, 0, NULL, NULL, NULL);

			
			if (err != CL_SUCCESS)
			{
				AfxMessageBox("clEnqueueNDRangeKernel error");
			}

			//7 result
			err = clEnqueueReadBuffer(queue, dev_dstR, CL_TRUE, 0, mem_dstSize, host_dstR, NULL, NULL, NULL);
			if (err != CL_SUCCESS)
			{
				AfxMessageBox("clEnqueueReadBuffer R error");
			}

			err = clEnqueueReadBuffer(queue, dev_dstG, CL_TRUE, 0, mem_dstSize, host_dstG, NULL, NULL, NULL);
			if (err != CL_SUCCESS)
			{
				AfxMessageBox("clEnqueueReadBuffer G error");
			}

			err = clEnqueueReadBuffer(queue, dev_dstB, CL_TRUE, 0, mem_dstSize, host_dstB, NULL, NULL, NULL);
			if (err != CL_SUCCESS)
			{
				AfxMessageBox("clEnqueueReadBuffer B error");
			}

			//赋值像素
			for (int k = 0;k < dstNum;++k)
			{
				int x = k % m_pImgDst->GetWidth();
				int y = k / m_pImgDst->GetWidth();

				*(p_dstRealData + y*dstPit + x*dstBitCount + 2) = host_dstR[k];
				*(p_dstRealData + y*dstPit + x*dstBitCount + 1) = host_dstG[k];
				*(p_dstRealData + y*dstPit + x*dstBitCount) = host_dstB[k];
			}

			//8 clean
			delete[] host_srcR;
			delete[] host_srcG;
			delete[] host_srcB;
			delete[] host_dstR;
			delete[] host_dstG;
			delete[] host_dstB;
			clReleaseMemObject(dev_srcR);
			clReleaseMemObject(dev_srcG);
			clReleaseMemObject(dev_srcB);
			clReleaseMemObject(dev_dstR);
			clReleaseMemObject(dev_dstG);
			clReleaseMemObject(dev_dstB);
			clReleaseKernel(scale);
			clReleaseProgram(program);
			clReleaseCommandQueue(queue);
			clReleaseContext(context);
			int end = clock();
			//显示时间
			if (i == 0 && j == 0)
			{
				CStatic* time = (CStatic*)GetDlgItem(IDC_TIME00);
				CString text;
				text.Format(_T("%dms"), end - start);
				time->SetWindowText(text);
			}
			else if (i == 0 && j == 1)
			{
				CStatic* time = (CStatic*)GetDlgItem(IDC_TIME01);
				CString text;
				text.Format(_T("%dms"), end - start);
				time->SetWindowText(text);
			}
			else if (i == 1 && j == 0)
			{
				CStatic* time = (CStatic*)GetDlgItem(IDC_TIME10);
				CString text;
				text.Format(_T("%dms"), end - start);
				time->SetWindowText(text);
			}
			else
			{
				CStatic* time = (CStatic*)GetDlgItem(IDC_TIME11);
				CString text;
				text.Format(_T("%dms"), end - start);
				time->SetWindowText(text);
			}
		}
	}
	ImageCopy(m_pImgDst, m_pImgSrc);
	this->Invalidate();
}
