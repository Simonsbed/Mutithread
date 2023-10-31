
// MutithreadView.cpp: CMutithreadView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Mutithread.h"
#endif

#include "MutithreadDoc.h"
#include "MutithreadView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMutithreadView

IMPLEMENT_DYNCREATE(CMutithreadView, CView)

BEGIN_MESSAGE_MAP(CMutithreadView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

// CMutithreadView 생성/소멸

CMutithreadView::CMutithreadView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CMutithreadView::~CMutithreadView()
{
}

BOOL CMutithreadView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CMutithreadView 그리기

void CMutithreadView::OnDraw(CDC* /*pDC*/)
{
	CMutithreadDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CMutithreadView 인쇄

BOOL CMutithreadView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CMutithreadView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CMutithreadView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CMutithreadView 진단

#ifdef _DEBUG
void CMutithreadView::AssertValid() const
{
	CView::AssertValid();
}

void CMutithreadView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMutithreadDoc* CMutithreadView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMutithreadDoc)));
	return (CMutithreadDoc*)m_pDocument;
}
#endif //_DEBUG


// CMutithreadView 메시지 처리기

unsigned int ThreadProcess(LPVOID pParam)       // 쓰레드로 처리할 일반함수
{
	CDC dc;
	HDC hDC = ::GetDC(HWND(pParam));   //pParam을 통해 this->m_hWnd 매개변수가 전달됨
	//GetDC함수는 매개변수의 dc를 가져옮
	//원형 CDC* GetDC(LPCRECT lprcRect = NULL, DWORD dwFlags = OLEDC_PAINTBKGND);
	// lprcRect : 창없는 작업영역의 클라이언트 좌표에서 사각형에 대한 포인터 NULL은 전체객체의 범위
	// dwFlags : OLEDC_PAINTBKGND, OLEDC_NODRAW, LOEDC_OFFSCREEN
	dc.Attach(hDC);   // 새로만든 핸들, 래퍼객체와 Windows 객체에 대해 핸들을 지정하며 dc객체를 뷰클래스의 작업영역 핸들임

	CPen Mypen, * pOldpen;
	int i, j;
	dc.SelectStockObject(NULL_BRUSH);
	for (j = 0; j < 255; j++)      // 255값을 증가시키면 원을 더 오래 그린다.
	{
		for (i = 0; i < 255; i++)
		{
			Mypen.CreatePen(PS_SOLID, 2, i << (j % 3 * 8));
			pOldpen = dc.SelectObject(&Mypen);
			dc.Ellipse(300 - i, 300 - i, 300 + i, 300 + i);
			dc.SelectObject(&pOldpen);
			Mypen.DeleteObject();
		}
	}
	dc.Detach();     // dc를 해제함. 함수가 종료되기 전에 dc를 m_hWnd에서 분리해야 함.
	::ReleaseDC(HWND(pParam), hDC); // int Release(CDC* pDC); 원형으로 DC를 해제해 줌.
	return 0;

}



void CMutithreadView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	AfxBeginThread(ThreadProcess, this->m_hWnd);


	CView::OnLButtonDown(nFlags, point);
}


void CMutithreadView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CClientDC dc(this);
	dc.TextOutW(point.x, point.y, L"User Message");

	CView::OnRButtonDown(nFlags, point);
}
