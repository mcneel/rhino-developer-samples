/////////////////////////////////////////////////////////////////////////////
// SampleRenderer.cpp
//

#include "stdafx.h"
#include "SampleRenderer.h"


CSampleRenderer::CSampleRenderer(RhRdk::Realtime::ISignalUpdate* pSignalUpdateInterface)
: m_pRenderThread(0),
 m_pSignalUpdateInterface(pSignalUpdateInterface)
{
	// Initialize in idle state
	m_bRunning = false;
}

CSampleRenderer::~CSampleRenderer()
{
	// Make sure the render process is stopped
	StopRenderProcess();
}

bool CSampleRenderer::StartRenderProcess(const ON_2iSize& frameSize)
{
	if (nullptr == m_pRenderWnd)
	{
		m_pRenderWnd = IRhRdkRenderWindow::New();
		if (nullptr == m_pRenderWnd)
			return false;
	}

	m_pRenderWnd->SetSize(frameSize);

	if (!m_pRenderWnd->EnsureDib())
	{
		delete m_pRenderWnd;
		m_pRenderWnd = nullptr;
		return false;
	}

	// If the render thread doesn't exist then create one and start it
	if (0 == m_pRenderThread)
	{
		// Set flag that the renderer should keep on running
		m_bRunning = true;

		// Create the thread in suspended state, pass it a pointer to this object.
		m_pRenderThread = AfxBeginThread(RenderProcess, (void*)this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, 0);

		// We'll destroy the thread object ourselves
		m_pRenderThread->m_bAutoDelete = FALSE;

		// Start the thread
		m_pRenderThread->ResumeThread();
	}

	return true;
}

void CSampleRenderer::StopRenderProcess()
{
	// Stop the render if it's running
	if (0 != m_pRenderThread)
	{
		// Set the flag to tell renderer that it should stop
		m_bRunning = false;

		// Wait until renderer checks the flag and returns
		WaitForSingleObject(m_pRenderThread->m_hThread, INFINITE);

		// Delete the thread
		delete m_pRenderThread;

		// And set the pointer to NULL so we don't try stopping it again
		m_pRenderThread = 0;
	}

	delete m_pRenderWnd;
	m_pRenderWnd = nullptr;
}

bool CSampleRenderer::Running() const
{
	// Flag tells if the renderer should still run
	return m_bRunning;
}

// Renderer thread executes this static method. We pass in the pointer to our renderer object.
unsigned int CSampleRenderer::RenderProcess(void* pData)
{
	// Cast void pointer to CSampleRenderer* and check it's not null.
	CSampleRenderer* pRenderProcess = static_cast<CSampleRenderer*>(pData);
	if (0 == pRenderProcess)
		return 0;

	const ON_2iSize sz = pRenderProcess->m_pRenderWnd->Size();

	// Start rendering and remember to update the render image every now and then.
	int iScanline = -1;

	while (true)
	{
		// Check if we're supposed to stop
		if (!pRenderProcess->Running())
			break;

		// Lock the rendered image in order to update it. 
		// Check for null and render one scanline
		IRhRdkRenderWindow::IChannel* pChannel = pRenderProcess->m_pRenderWnd->OpenChannel(IRhRdkRenderWindow::chanRGBA);
		if (nullptr != pChannel)
		{
			if (iScanline == -1 || iScanline >= sz.cy)
				iScanline = sz.cy - 1;

			const int py = iScanline;
			for (int px = 0; px < sz.cx; px++)
			{
				const float red = (float)(rand() % 256) / 255.0f;
				const float green = (float)(rand() % 256) / 255.0f;
				const float blue = (float)(rand() % 256) / 255.0f;

				float fColor[4];
				fColor[0] = red;
				fColor[1] = green;
				fColor[2] = blue;
				fColor[3] = 1.0f;

				pChannel->SetValue(px, py, ComponentOrder::RGBA, fColor);
			}

			pChannel->Close();

			// Signal redraw
			if (nullptr != pRenderProcess->m_pSignalUpdateInterface)
				pRenderProcess->m_pSignalUpdateInterface->SignalUpdate();

			// Invalidate the drawn area so that the dib will be updated.
			ON_4iRect rect(0, py, sz.cx, py+1);
			pRenderProcess->m_pRenderWnd->InvalidateArea(rect);
		}

		iScanline--;

		// Rest for about 50 milliseconds
		Sleep(50);
	}

	return 0;
}
